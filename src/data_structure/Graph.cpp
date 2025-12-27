#include "graph.h"

Graph::Graph() {
    head = nullptr;
}

Node* Graph::addNode(int id, NodeType type) {
    Node* newNode = new Node;
    newNode->id = id;
    newNode->type = type;
    newNode->edges = nullptr;
    newNode->next = head;
    head = newNode;
    return newNode;
}

Node* Graph::findNode(int id) {
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->id == id)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

void Graph::addEdge(int fromId, int toId, double distance) {
    Node* from = findNode(fromId);
    Node* to = findNode(toId);
    if (!from || !to)
        return;
    Edge* e = new Edge;
    e->to = to;
    e->distance = distance;
    e->next = from->edges;
    from->edges = e;
}

Node* Graph::getHead() {
    return head;
}

Graph::~Graph() {
    Node* curr = head;
    while (curr) {
        Edge* e = curr->edges;
        while (e) {
            Edge* temp = e;
            e = e->next;
            delete temp;
        }
        Node* tempNode = curr;
        curr = curr->next;
        delete tempNode;
    }
}
