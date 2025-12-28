/**
 * @file Graph.cpp
 * @brief Pointer-based graph implementation using adjacency lists
 * @author Ýrem Irmak Ünlüer
 * @date 2025-12-29
 */

#include "graph.h"

// Constructor
Graph::Graph() {
    head = nullptr;
}

// Adds a new node to the graph
Node* Graph::addNode(int id, NodeType type) {
    Node* newNode = new Node;          // allocate node
    newNode->id = id;                  // set node ID
    newNode->type = type;              // set node type
    newNode->edges = nullptr;          // no edges yet
    newNode->next = head;              // insert at front
    head = newNode;
    return newNode;
}

// Finds a node by its ID
Node* Graph::findNode(int id) {
    Node* curr = head;                 // start from head
    while (curr != nullptr) {
        if (curr->id == id)
            return curr;               // node found
        curr = curr->next;             // move to next
    }
    return nullptr;                    // not found
}

// Adds an edge between two nodes
void Graph::addEdge(int fromId, int toId, double distance) {
    Node* from = findNode(fromId);     // source node
    Node* to = findNode(toId);         // destination node
    if (!from || !to)
        return;                        // invalid IDs
    Edge* e = new Edge;                // allocate edge
    e->to = to;                        // set destination
    e->distance = distance;            // set weight
    e->next = from->edges;             // insert at front
    from->edges = e;
}

// Returns the head of the node list
Node* Graph::getHead() {
    return head;
}

// Destructor: frees all nodes and edges
Graph::~Graph() {
    Node* curr = head;
    while (curr) {
        Edge* e = curr->edges;
        while (e) {
            Edge* temp = e;
            e = e->next;
            delete temp;               // delete edge
        }
        Node* tempNode = curr;
        curr = curr->next;
        delete tempNode;               // delete node
    }
}