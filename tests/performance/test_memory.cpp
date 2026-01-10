/**
 * @file test_memory.cpp
 * @brief Memory & lifetime stress tests 
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#include "doctest.h"

#include "data_structures/HashTable.h"
#include "core/Facilities.h"
#include "core/Simulation.h"
#include "data_structures/Graph.h"
#include "core/Bin.h"
#include "core/Facility.h"
#include "core/Truck.h"

using namespace project;


TEST_CASE("[MEMORY][HashTable] massive insert + resize + destruction") {
    CHECK_NOTHROW({
        HashTable table(3);  // very small to force resize

        for (int i = 0; i < 1000; i++) {
            table.insert("key_" + std::to_string(i), i);
        }

        CHECK(table.getSize() == 1000);
    });
    // Destructor must clean:
    // - all HashNodes
    // - bucket array
}

TEST_CASE("[MEMORY][HashTable] clear called multiple times") {
    HashTable table;

    table.insert("A", 1);
    table.insert("B", 2);

    CHECK_NOTHROW(table.clear());
    CHECK(table.getSize() == 0);

    // clear twice → should NOT double free
    CHECK_NOTHROW(table.clear());
}


TEST_CASE("[MEMORY][Facilities] repeated addBin / addFacility stress") {
    Facilities facilities;

    for (int i = 0; i < 200; i++) {
        Bin bin("B" + std::to_string(i), "L" + std::to_string(i),
                100, 50, 5, i);
        facilities.addBin(bin);
    }

    for (int i = 0; i < 50; i++) {
        Facility f("F" + std::to_string(i),
                   (i == 0 ? "depot" : "disposal"),
                   i, i, i);
        facilities.addFacility(f);
    }

    CHECK(facilities.getBinCount() == 200);
    CHECK(facilities.getFacilityCount() == 50);
    // Destructor must clean:
    // - old arrays created during resizing
    // - final bins[] and facilities[]
}

TEST_CASE("[MEMORY][Facilities] getDisposalNodes ownership") {
    Facilities facilities;

    facilities.addFacility(Facility("Depot", "depot", 0, 0, 0));
    facilities.addFacility(Facility("D1", "disposal", 1, 1, 1));
    facilities.addFacility(Facility("D2", "disposal", 2, 2, 2));

    int count = 0;
    int* nodes = facilities.getDisposalNodes(count);

    REQUIRE(nodes != nullptr);
    CHECK(count == 2);

    // Caller is responsible → MUST be deletable
    CHECK_NOTHROW(delete[] nodes);
}



TEST_CASE("[MEMORY][Graph] destructor frees adjacency lists") {
    CHECK_NOTHROW({
        Graph graph(20);

        for (int i = 0; i < 19; i++) {
            graph.addBidirectionalEdge(i, i + 1, 10);
        }
    });
    // Graph destructor must free:
    // - GraphNode list
    // - each LinkedList<Edge>
}

TEST_CASE("[MEMORY][Graph] copy constructor deep copy") {
    Graph g1(10);
    g1.addBidirectionalEdge(0, 1, 5);
    g1.addBidirectionalEdge(1, 2, 7);

    CHECK_NOTHROW({
        Graph g2 = g1;
    });
    // Both graphs destroyed independently → no double free
}



TEST_CASE("[MEMORY][Simulation] full lifecycle stress") {
    Graph graph(10);

    for (int i = 0; i < 9; i++) {
        graph.addBidirectionalEdge(i, i + 1, 1);
    }

    Facilities facilities;

    facilities.addFacility(Facility("Depot", "depot", 0, 0, 0));
    facilities.addFacility(Facility("Dump", "disposal", 5, 5, 5));

    for (int i = 0; i < 5; i++) {
        facilities.addBin(
            Bin("B" + std::to_string(i), "L" + std::to_string(i),
                50, 45, 10, i)
        );
    }

    Truck truck("T1", 60, 0, 0);
    facilities.setTruck(truck);

    CHECK_NOTHROW({
        Simulation sim(graph, facilities, 10);
        sim.run();
    });
    // Exiting scope must clean:
    // - planner internals
    // - no dangling references
}



TEST_CASE("[MEMORY][WorstCase] early scope exit") {
    CHECK_NOTHROW({
        Facilities facilities;
        facilities.addBin(Bin("B1", "L1", 10, 5, 1, 1));
    });
    // Destructor called immediately → must not leak
}
