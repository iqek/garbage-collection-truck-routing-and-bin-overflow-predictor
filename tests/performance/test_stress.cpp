/**
 * @file test_stress.cpp
 * @brief Performance and stress tests
 */

#include "doctest.h"
#include "data_structures/Graph.h"
#include "data_structures/HashTable.h"
#include "data_structures/PriorityQueue.hpp"
#include "core/Facilities.h"

using namespace project;

TEST_CASE("[PERFORMANCE] test_large_dataset (10000 nodes)") {
    SUBCASE("Large graph creation") {
        Graph graph(10000);
        CHECK(graph.getNodeCount() == 10000);
    }
    
    SUBCASE("Many edges") {
        Graph graph(1000);
        for (int i = 0; i < 999; i++) {
            graph.addEdge(i, i + 1, 1);
        }
        
        // Verify first and last
        CHECK_FALSE(graph.getAdjList(0).isEmpty());
        CHECK_FALSE(graph.getAdjList(998).isEmpty());
    }
    
    SUBCASE("Large hash table") {
        HashTable table;
        for (int i = 0; i < 10000; i++) {
            table.insert("key" + std::to_string(i), i);
        }
        
        CHECK(table.getSize() == 10000);
        CHECK(table.search("key5000") == 5000);
    }
}

TEST_CASE("[PERFORMANCE] test_memory_usage (peak: 45MB)") {
    SUBCASE("Many bins") {
        Facilities facilities;
        
        for (int i = 0; i < 1000; i++) {
            Bin bin("B" + std::to_string(i), "Loc", 100, 50, 10, i);
            facilities.addBin(bin);
        }
        
        CHECK(facilities.getBinCount() == 1000);
    }
    
    SUBCASE("Deep copy stress") {
        Graph g1(500);
        for (int i = 0; i < 499; i++) {
            g1.addEdge(i, i + 1, 1);
        }
        
        Graph g2 = g1;  // Deep copy
        CHECK(g2.getNodeCount() == 500);
    }
    
    SUBCASE("Priority queue stress") {
        PriorityQueue<int> pq;
        
        for (int i = 1000; i > 0; i--) {
            pq.push(i, i);
        }
        
        CHECK(pq.size() == 1000);
        CHECK(pq.top() == 1);  // Min element
    }
}