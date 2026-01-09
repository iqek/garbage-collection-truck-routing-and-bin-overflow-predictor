#include "doctest.h"

// Data structures
#include "data_structures/Graph.h"
#include "data_structures/PriorityQueue.hpp"
#include "data_structures/LinkedList.hpp"

using namespace project;

/*
 * Utility / Edge Case Tests
 * These tests ensure that basic data structures behave safely
 * under invalid or empty-state conditions.
 */

/* INVALID GRAPH ACCESS */

TEST_CASE("Graph: invalid node access returns empty adjacency list") {
    Graph g(2);

    const LinkedList<Edge>& adj = g.getAdjList(100); // invalid index

    CHECK(adj.size() == 0);
    CHECK(adj.isEmpty());
}

/* EMPTY PRIORITY QUEUE */

TEST_CASE("PriorityQueue: empty queue safety") {
    PriorityQueue<int> pq;

    CHECK(pq.isEmpty());
    CHECK(pq.size() == 0);

    // pop on empty queue should not crash
    CHECK_NOTHROW(pq.pop());
}

/* LINKED LIST EMPTY POP */

TEST_CASE("LinkedList: popFront on empty list does not crash") {
    LinkedList<int> list;

    CHECK(list.isEmpty());
    CHECK(list.size() == 0);

    // popping from empty list should be safe
    CHECK_NOTHROW(list.popFront());

    CHECK(list.isEmpty());
    CHECK(list.size() == 0);
}
