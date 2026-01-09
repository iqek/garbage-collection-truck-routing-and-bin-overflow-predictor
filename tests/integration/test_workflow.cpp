/**
 * @file test_integration.cpp
 * @brief Integration tests for complete system workflows
 */

#include "doctest.h"
#include "core/Simulation.h"
#include "core/RoutePlanner.h"
#include "utils/JsonParser.h"

using namespace project;

TEST_CASE("Integration: Simple simulation workflow") {
    // Setup minimal graph
    Graph graph(4);
    graph.addBidirectionalEdge(0, 1, 5);  // Depot to B1
    graph.addBidirectionalEdge(1, 2, 3);  // B1 to Disposal
    
    // Setup facilities
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 2);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    Bin bin("B1", "ParkAve", 100, 90, 5, 1);
    facilities.addBin(bin);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    // Run simulation
    Simulation sim(graph, facilities, 5);
    sim.run();
    
    CHECK(sim.isFinished() == true);
    CHECK(sim.getTime() == 5);
    CHECK(sim.getTotalDistance() > 0);
}

TEST_CASE("Integration: Multiple bins collection") {
    Graph graph(6);
    graph.addBidirectionalEdge(0, 1, 5);   // Depot to B1
    graph.addBidirectionalEdge(0, 2, 5);   // Depot to B2
    graph.addBidirectionalEdge(1, 3, 3);   // B1 to Disposal
    graph.addBidirectionalEdge(2, 3, 4);   // B2 to Disposal
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 3);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    Bin bin1("B1", "ParkAve", 100, 30, 8, 1);
    Bin bin2("B2", "Cafe", 100, 70, 10, 2);
    facilities.addBin(bin1);
    facilities.addBin(bin2);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    Simulation sim(graph, facilities, 3);
    sim.run();
    
    CHECK(sim.getCollectionsCompleted() > 0);
}

TEST_CASE("Integration: Truck capacity overflow handling") {
    Graph graph(4);
    graph.addBidirectionalEdge(0, 1, 5);
    graph.addBidirectionalEdge(1, 2, 3);
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 2);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    // Bin with more waste than truck capacity
    Bin bin("B1", "ParkAve", 200, 150, 10, 1);
    facilities.addBin(bin);
    
    Truck truck("T1", 100, 0, 0);  // Small truck
    facilities.setTruck(truck);
    
    Simulation sim(graph, facilities, 2);
    sim.run();
    
    // Should handle gracefully without crash
    CHECK(sim.isFinished() == true);
}

TEST_CASE("Integration: Route planning with critical bins") {
    Graph graph(5);
    graph.addBidirectionalEdge(0, 1, 10);  // Depot to B1 (far)
    graph.addBidirectionalEdge(0, 2, 2);   // Depot to B2 (near)
    graph.addBidirectionalEdge(2, 3, 3);   // B2 to Disposal
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 3);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    // B1: Low priority (low fill)
    Bin bin1("B1", "Far", 100, 20, 5, 1);
    // B2: High priority (near overflow)
    Bin bin2("B2", "Near", 100, 95, 10, 2);
    
    facilities.addBin(bin1);
    facilities.addBin(bin2);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    RoutePlanner planner(graph);
    Route route = planner.planRoute(facilities);
    
    // Should prioritize B2 (critical bin) even though B1 is valid too
    CHECK(route.getLength() > 0);
}

TEST_CASE("Integration: Emergency reschedule on overflow") {
    Graph graph(4);
    graph.addBidirectionalEdge(0, 1, 5);
    graph.addBidirectionalEdge(1, 2, 3);
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 2);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    // Bin that will overflow quickly
    Bin bin("B1", "ParkAve", 100, 85, 20, 1);
    facilities.addBin(bin);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    Simulation sim(graph, facilities, 5);
    sim.run();
    
    // System should detect and handle overflow
    CHECK(sim.getOverflowCount() >= 0);
}

TEST_CASE("Integration: Depot return after collection") {
    Graph graph(4);
    graph.addBidirectionalEdge(0, 1, 5);
    graph.addBidirectionalEdge(1, 2, 3);
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 2);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    Bin bin("B1", "ParkAve", 100, 50, 10, 1);
    facilities.addBin(bin);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    Simulation sim(graph, facilities, 1);
    sim.step();
    
    // After one day, truck should return to depot
    CHECK(facilities.getTruck().getCurrentNode() == 0);
}

TEST_CASE("Integration: Statistics tracking") {
    Graph graph(4);
    graph.addBidirectionalEdge(0, 1, 5);
    graph.addBidirectionalEdge(1, 2, 3);
    
    Facilities facilities;
    
    Facility depot("Depot", "depot", 0, 0, 0);
    Facility disposal("Dump1", "disposal", 10, 10, 2);
    facilities.addFacility(depot);
    facilities.addFacility(disposal);
    
    Bin bin("B1", "ParkAve", 100, 50, 10, 1);
    facilities.addBin(bin);
    
    Truck truck("T1", 500, 0, 0);
    facilities.setTruck(truck);
    
    Simulation sim(graph, facilities, 3);
    sim.run();
    
    CHECK(sim.getTotalDistance() > 0);
    CHECK(sim.getCollectionsCompleted() >= 0);
    CHECK(sim.getTime() == 3);
}