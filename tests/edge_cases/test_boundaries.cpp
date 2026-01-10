/**
 * @file test_boundaries.cpp
 * @brief Boundary condition and edge case tests
 */

#include "doctest.h"
#include "core/Bin.h"
#include "core/Truck.h"
#include "core/Facilities.h"
#include "data_structures/Graph.h"
#include "utils/JsonParser.h"

using namespace project;

TEST_CASE("[EDGE CASES] test_empty_input") {
    SUBCASE("Empty bins array") {
        JsonParser parser("data/test_empty.json");
        
        int count = 0;
        Bin* bins = parser.loadBins(count);
        
        CHECK(count == 0);
        CHECK(bins == nullptr);
    }
    
    SUBCASE("Empty graph") {
        Graph graph(0);
        CHECK(graph.getNodeCount() == 0);
    }
    
    SUBCASE("Empty facilities") {
        Facilities facilities;
        CHECK(facilities.getBinCount() == 0);
        CHECK(facilities.getDepotNode() == -1);
    }
}

TEST_CASE("[EDGE CASES] test_single_element") {
    SUBCASE("Single bin") {
        Bin bin("B1", "Park", 100, 50, 10, 0);
        CHECK(bin.getCurrentFill() == 50);
        
        bin.updateFill();
        CHECK(bin.getCurrentFill() == 60);
    }
    
    SUBCASE("Single node graph") {
        Graph graph(1);
        CHECK(graph.getNodeCount() == 1);
        CHECK(graph.getAdjList(0).isEmpty());
    }
    
    SUBCASE("Single facility") {
        Facilities facilities;
        Facility depot("Depot", "depot", 0, 0, 0);
        facilities.addFacility(depot);
        
        CHECK(facilities.getFacilityCount() == 1);
    }
}

TEST_CASE("[EDGE CASES] test_maximum_values") {
    SUBCASE("Bin at capacity") {
        Bin bin("B1", "Park", 100, 100, 10, 0);
        CHECK(bin.isOverflowing() == true);
        
        bin.updateFill();
        CHECK(bin.getCurrentFill() == 100);  // Capped
    }
    
    SUBCASE("Truck full") {
        Truck truck("T1", 500, 500, 0);
        CHECK(truck.isFull() == true);
        CHECK(truck.getRemainingCapacity() == 0);
    }
    
    SUBCASE("High fill rate") {
        Bin bin("B1", "Park", 100, 50, 1000, 0);
        bin.updateFill();
        CHECK(bin.getCurrentFill() == 100);  // Capped at capacity
    }
}

TEST_CASE("[EDGE CASES] test_invalid_input (handled gracefully)") {
    SUBCASE("Negative collection") {
        Bin bin("B1", "Park", 100, 50, 10, 0);
        bin.collect(-10);
        CHECK(bin.getCurrentFill() == 50);  // Unchanged
    }
    
    SUBCASE("Overcollection") {
        Bin bin("B1", "Park", 100, 30, 10, 0);
        bin.collect(100);
        CHECK(bin.getCurrentFill() == 0);  // Floor at 0
    }
    
    SUBCASE("Invalid graph node") {
        Graph graph(3);
        const LinkedList<Edge>& adj = graph.getAdjList(100);
        CHECK(adj.isEmpty());
    }
    
    SUBCASE("Missing file") {
        JsonParser parser("data/nonexistent.json");
        
        int count = 0;
        Bin* bins = parser.loadBins(count);
        
        // Should handle gracefully
        CHECK(count == 0);
        if (bins) delete[] bins;
    }
}