/**
 * @file Graph.cpp
 * @brief Pointer-based graph implementation using adjacency lists.
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#include "data_structures/Graph.h"

namespace project {

// Default constructor
Graph::Graph() : head(nullptr), nodeCount(0) {}

// Constructor with node count
Graph::Graph(int count) : head(nullptr), nodeCount(count) {
    for (int i = count - 1; i >= 0; i--) {  // create nodes in reverse order
        GraphNode* newNode = new GraphNode(i);
        newNode->next = head;  // insert at front
        head = newNode;
    }
}

// Destructor
Graph::~Graph() {
    GraphNode* current = head;
    while (current != nullptr) {
        GraphNode* temp = current;
        current = current->next;
        delete temp;  // free each node
    }
}

// Copy constructor
Graph::Graph(const Graph& other) : head(nullptr), nodeCount(other.nodeCount) {
    if (other.head == nullptr) {
        return;
    }

    GraphNode* otherCurrent = other.head;
    GraphNode* prevNode = nullptr;

    while (otherCurrent != nullptr) {
        GraphNode* newNode = new GraphNode(otherCurrent->nodeId);
        newNode->edges = otherCurrent->edges;  // copy adjacency list

        if (prevNode == nullptr) {
            head = newNode;  // first node
        } else {
            prevNode->next = newNode;  // link to previous
        }

        prevNode = newNode;
        otherCurrent = otherCurrent->next;
    }
}

// Assignment operator
Graph& Graph::operator=(const Graph& other) {
    if (this == &other) {
        return *this;  // self-assignment check
    }

    GraphNode* current = head;  // clean up existing nodes
    while (current != nullptr) {
        GraphNode* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    nodeCount = other.nodeCount;

    if (other.head == nullptr) {
        return *this;
    }

    GraphNode* otherCurrent = other.head;
    GraphNode* prevNode = nullptr;

    while (otherCurrent != nullptr) {
        GraphNode* newNode = new GraphNode(otherCurrent->nodeId);
        newNode->edges = otherCurrent->edges;  // copy adjacency list

        if (prevNode == nullptr) {
            head = newNode;  // first node
        } else {
            prevNode->next = newNode;  // link to previous
        }

        prevNode = newNode;
        otherCurrent = otherCurrent->next;
    }

    return *this;
}

// Finds a node by its ID
GraphNode* Graph::findNode(int nodeId) const {
    GraphNode* current = head;
    while (current != nullptr) {
        if (current->nodeId == nodeId) {
            return current;  // node found
        }
        current = current->next;
    }
    return nullptr;  // not found
}

// Adds a weighted edge between two nodes
void Graph::addEdge(int from, int to, int weight) {
    GraphNode* fromNode = findNode(from);
    if (fromNode != nullptr) {
        Edge edge(to, weight);
        fromNode->edges.pushBack(edge);  // add edge to adjacency list
    }
}

// Adds a bidirectional edge
void Graph::addBidirectionalEdge(int node1, int node2, int weight) {
    addEdge(node1, node2, weight);  // add edge in both directions
    addEdge(node2, node1, weight);
}

// Returns the adjacency list for a specific node
LinkedList<Edge>& Graph::getAdjList(int node) {
    GraphNode* graphNode = findNode(node);
    if (graphNode != nullptr) {
        return graphNode->edges;
    }

    static LinkedList<Edge> emptyList;  // fallback to prevent crash
    return emptyList;
}

// Returns the adjacency list for a specific node (const version)
const LinkedList<Edge>& Graph::getAdjList(int node) const {
    GraphNode* graphNode = findNode(node);
    if (graphNode != nullptr) {
        return graphNode->edges;
    }

    static const LinkedList<Edge> emptyList;  // fallback
    return emptyList;
}

// Returns the total number of nodes
int Graph::getNodeCount() const {
    return nodeCount;
}

}  // namespace project
