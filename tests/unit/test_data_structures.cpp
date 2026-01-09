#include "doctest.h"

// Data structure headers
#include "data_structures/Graph.h"
#include "data_structures/HashTable.h"
#include "data_structures/LinkedList.hpp"
#include "data_structures/PriorityQueue.h"

using namespace project;

/*LinkedList Tests*/

TEST_CASE("LinkedList: default construction") {
    LinkedList<int> list;
    CHECK(list.isEmpty());
    CHECK(list.size() == 0);
}

TEST_CASE("LinkedList: pushBack and size") {
    LinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);

    CHECK_FALSE(list.isEmpty());
    CHECK(list.size() == 2);
    CHECK(list.front() == 10);
}

TEST_CASE("LinkedList: popFront") {
    LinkedList<int> list;

    list.pushBack(1);
    list.pushBack(2);

    list.popFront();

    CHECK(list.size() == 1);
    CHECK(list.front() == 2);
}

TEST_CASE("LinkedList: copy constructor") {
    LinkedList<int> list;
    list.pushBack(5);
    list.pushBack(6);

    LinkedList<int> copy(list);

    CHECK(copy.size() == 2);
    CHECK(copy.front() == 5);
}

TEST_CASE("LinkedList: assignment operator") {
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);

    LinkedList<int> other;
    other = list;

    CHECK(other.size() == 2);
    CHECK(other.front() == 1);
}

/*HashTable Tests*/

TEST_CASE("HashTable: default construction") {
    HashTable table;
    CHECK(table.getSize() == 0);
}

TEST_CASE("HashTable: insert and search") {
    HashTable table;

    table.insert("A", 10);
    table.insert("B", 20);

    CHECK(table.getSize() == 2);
    CHECK(table.search("A") == 10);
    CHECK(table.search("B") == 20);
}

TEST_CASE("HashTable: update existing key") {
    HashTable table;

    table.insert("key", 1);
    table.insert("key", 42);

    CHECK(table.getSize() == 1);
    CHECK(table.search("key") == 42);
}

TEST_CASE("HashTable: search non-existent key") {
    HashTable table;
    CHECK(table.search("missing") == -1);
}

TEST_CASE("HashTable: clear") {
    HashTable table;

    table.insert("A", 1);
    table.insert("B", 2);
    table.clear();

    CHECK(table.getSize() == 0);
    CHECK(table.search("A") == -1);
}

/*PriorityQueue Tests*/

TEST_CASE("PriorityQueue: default construction") {
    PriorityQueue<int> pq;

    CHECK(pq.isEmpty());
    CHECK(pq.size() == 0);
}

TEST_CASE("PriorityQueue: push and top") {
    PriorityQueue<int> pq;

    pq.push(10, 5);
    pq.push(20, 1);
    pq.push(30, 3);

    CHECK(pq.size() == 3);
    CHECK(pq.top() == 20); // lowest priority value = highest priority
}

TEST_CASE("PriorityQueue: pop") {
    PriorityQueue<int> pq;

    pq.push(1, 3);
    pq.push(2, 1);
    pq.push(3, 2);

    pq.pop();
    CHECK(pq.top() == 3);

    pq.pop();
    CHECK(pq.top() == 1);
}

TEST_CASE("PriorityQueue: copy constructor") {
    PriorityQueue<int> pq;
    pq.push(5, 2);
    pq.push(10, 1);

    PriorityQueue<int> copy(pq);

    CHECK(copy.size() == 2);
    CHECK(copy.top() == 10);
}

/*Graph Test*/

TEST_CASE("Graph: default construction") {
    Graph g;
    CHECK(g.getNodeCount() == 0);
}

TEST_CASE("Graph: construction with node count") {
    Graph g(5);
    CHECK(g.getNodeCount() == 5);
}

TEST_CASE("Graph: addEdge and adjacency list") {
    Graph g(3);

    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 5);

    const LinkedList<Edge>& adj = g.getAdjList(0);

    CHECK(adj.size() == 2);
    CHECK(adj.front().toNode == 1);
    CHECK(adj.front().weight == 10);
}

TEST_CASE("Graph: bidirectional edge") {
    Graph g(2);

    g.addBidirectionalEdge(0, 1, 7);

    CHECK(g.getAdjList(0).size() == 1);
    CHECK(g.getAdjList(1).size() == 1);
}

TEST_CASE("Graph: copy constructor") {
    Graph g(2);
    g.addEdge(0, 1, 3);

    Graph copy(g);

    CHECK(copy.getNodeCount() == 2);
    CHECK(copy.getAdjList(0).size() == 1);
}

TEST_CASE("Graph: assignment operator") {
    Graph g1(3);
    g1.addEdge(1, 2, 4);

    Graph g2;
    g2 = g1;

    CHECK(g2.getNodeCount() == 3);
    CHECK(g2.getAdjList(1).size() == 1);
}