/**
 * @file test_algorithms.cpp
 * @brief Unit tests for core algorithms
 *@author Miray Duygulu
 *@date 2026-01-10
 */

#include "doctest.h"
#include "core/RoutePlanner.h"
#include "core/OverflowPredictor.h"
#include "core/Simulation.h"

using namespace project;

TEST_CASE("[UNIT] test_pathfinding") {
    SUBCASE("Dijkstra shortest path") {
        Graph graph(3);
        graph.addEdge(0, 1, 5);
        graph.addEdge(1, 2, 7);
        
        RoutePlanner planner(graph);
        int dist = planner.computeDistance(0, 2);
        CHECK(dist == 12);
    }
    
    SUBCASE("Find nearest disposal") {
        Graph graph(4);
        graph.addEdge(0, 1, 5);
        graph.addEdge(0, 2, 10);
        
        Facilities facilities;
        Facility d1("D1", "disposal", 0, 0, 1);
        Facility d2("D2", "disposal", 0, 0, 2);
        facilities.addFacility(d1);
        facilities.addFacility(d2);
        
        RoutePlanner planner(graph);
        int nearest = planner.findNearestDisposal(0, facilities);
        CHECK(nearest == 1);  // Closer one
    }
}

TEST_CASE("[UNIT] test_sorting") {
    SUBCASE("Priority queue ordering") {
        PriorityQueue<int> pq;
        pq.push(30, 3);
        pq.push(10, 1);
        pq.push(20, 2);
        
        CHECK(pq.top() == 10);  // Lowest priority first
        pq.pop();
        CHECK(pq.top() == 20);
    }
    
    SUBCASE("Bin priority scoring") {
        Graph graph(3);
        graph.addBidirectionalEdge(0, 1, 5);
        graph.addBidirectionalEdge(0, 2, 5);
        
        Facilities facilities;
        Facility depot("Depot", "depot", 0, 0, 0);
        facilities.addFacility(depot);
        
        Bin safeBin("B1", "Safe", 100, 20, 5, 1);
        Bin criticalBin("B2", "Critical", 100, 95, 10, 2);
        facilities.addBin(safeBin);
        facilities.addBin(criticalBin);
        
        Truck truck("T1", 500, 0, 0);
        facilities.setTruck(truck);
        
        RoutePlanner planner(graph);
        int selected = planner.selectNextBin(facilities);
        CHECK(selected == 1);  // Critical bin prioritized
    }
}

TEST_CASE("[UNIT] test_optimization") {
    SUBCASE("Overflow prediction") {
        Bin bin("B1", "Park", 100, 90, 5, 0);
        OverflowPredictor predictor(2);
        
        int days = predictor.predictDaysToOverflow(bin);
        CHECK(days == 2);  // (100-90)/5 = 2
    }
    
    SUBCASE("Critical bin detection") {
        Bin criticalBin("B1", "Park", 100, 98, 10, 0);
        OverflowPredictor predictor(3);
        
        CHECK(predictor.isCritical(criticalBin) == true);
    }
    
    SUBCASE("Route optimization") {
        int bins[] = {1, 2, 3};
        Route route(bins, 3);
        route.setTotalDistance(100);
        
        CHECK(route.getLength() == 3);
        CHECK(route.getTotalDistance() == 100);
    }
}
