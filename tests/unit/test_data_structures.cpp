/**
 * @file test_data_structures.cpp
 * @brief Unit tests for custom data structures
 *@author Miray Duygulu
 *@date 2026-01-10
 */

#include "doctest.h"
#include "data_structures/Graph.h"
#include "data_structures/HashTable.h"
#include "data_structures/LinkedList.hpp"
#include "data_structures/PriorityQueue.hpp"

using namespace project;

TEST_CASE("[UNIT] test_constructor") {
    SUBCASE("Graph") {
        Graph g(5);
        CHECK(g.getNodeCount() == 5);
    }
    
    SUBCASE("HashTable") {
        HashTable table;
        CHECK(table.getSize() == 0);
    }
    
    SUBCASE("LinkedList") {
        LinkedList<int> list;
        CHECK(list.isEmpty());
    }
    
    SUBCASE("PriorityQueue") {
        PriorityQueue<int> pq;
        CHECK(pq.isEmpty());
    }
}

TEST_CASE("[UNIT] test_insert") {
    SUBCASE("Graph add edge") {
        Graph g(3);
        g.addEdge(0, 1, 5);
        CHECK(g.getAdjList(0).size() == 1);
    }
    
    SUBCASE("HashTable insert") {
        HashTable table;
        table.insert("key1", 42);
        CHECK(table.search("key1") == 42);
    }
    
    SUBCASE("LinkedList push") {
        LinkedList<int> list;
        list.pushBack(10);
        CHECK(list.front() == 10);
    }
    
    SUBCASE("PriorityQueue push") {
        PriorityQueue<int> pq;
        pq.push(42, 1);
        CHECK(pq.top() == 42);
    }
}

TEST_CASE("[UNIT] test_delete") {
    SUBCASE("LinkedList pop") {
        LinkedList<int> list;
        list.pushBack(10);
        list.popFront();
        CHECK(list.isEmpty());
    }
    
    SUBCASE("PriorityQueue pop") {
        PriorityQueue<int> pq;
        pq.push(42, 1);
        pq.pop();
        CHECK(pq.isEmpty());
    }
    
    SUBCASE("HashTable clear") {
        HashTable table;
        table.insert("key", 10);
        table.clear();
        CHECK(table.getSize() == 0);
    }
}

TEST_CASE("[UNIT] test_search") {
    SUBCASE("HashTable search existing") {
        HashTable table;
        table.insert("key1", 100);
        table.insert("key2", 200);
        CHECK(table.search("key1") == 100);
        CHECK(table.search("key2") == 200);
    }
    
    SUBCASE("HashTable search missing") {
        HashTable table;
        CHECK(table.search("missing") == -1);
    }
    
    SUBCASE("Graph adjacency lookup") {
        Graph g(3);
        g.addEdge(0, 1, 5);
        const LinkedList<Edge>& adj = g.getAdjList(0);
        CHECK_FALSE(adj.isEmpty());
    }
}

TEST_CASE("[UNIT] test_traversal") {
    SUBCASE("LinkedList iteration") {
        LinkedList<int> list;
        list.pushBack(1);
        list.pushBack(2);
        list.pushBack(3);
        
        int sum = 0;
        for (int val : list) {
            sum += val;
        }
        CHECK(sum == 6);
    }
    
    SUBCASE("Graph edge traversal") {
        Graph g(4);
        g.addEdge(0, 1, 5);
        g.addEdge(0, 2, 3);
        
        const LinkedList<Edge>& adj = g.getAdjList(0);
        int count = 0;
        for (const Edge& e : adj) {
            (void)e;
            count++;
        }
        CHECK(count == 2);
    }
}
