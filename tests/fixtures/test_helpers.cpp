/**
 * @file test_helpers.cpp
 * @brief Test helper functions and fixtures
 */

#include "test_helpers.h"
#include <cstring>

namespace test_fixtures {

// Helper to create a simple test graph
Graph* createTestGraph() {
    Graph* graph = new Graph(4);
    graph->addBidirectionalEdge(0, 1, 5);
    graph->addBidirectionalEdge(1, 2, 3);
    graph->addBidirectionalEdge(2, 3, 4);
    return graph;
}

// Helper to create test facilities
Facilities* createTestFacilities() {
    Facilities* facilities = new Facilities();
    
    // Add depot
    Facility depot("Depot", "depot", 0, 0, 0);
    facilities->addFacility(depot);
    
    // Add disposal
    Facility disposal("Dump1", "disposal", 10, 10, 3);
    facilities->addFacility(disposal);
    
    // Add bins
    Bin bin1("B1", "Park", 100, 50, 10, 1);
    Bin bin2("B2", "Market", 100, 70, 15, 2);
    facilities->addBin(bin1);
    facilities->addBin(bin2);
    
    // Add truck
    Truck truck("T1", 500, 0, 0);
    facilities->setTruck(truck);
    
    return facilities;
}

// Cleanup helper
void cleanup(Graph* graph, Facilities* facilities) {
    if (graph) delete graph;
    if (facilities) delete facilities;
}

} // namespace test_fixtures