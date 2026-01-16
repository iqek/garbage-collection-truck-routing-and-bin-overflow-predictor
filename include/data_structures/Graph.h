/**
 * @file Graph.h
 * @brief Represents the city map as a weighted graph using adjacency lists.
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#pragma once

#include "LinkedList.hpp"

namespace project {

/**
 * @brief Structure for a weighted edge.
 *
 * Stores the destination node and the edge weight (distance) required
 * for routing algorithms like Dijkstra's.
 */
struct Edge {
    int toNode;  ///< Index of the destination node the edge connects to.
    int weight;  ///< The weight or distance of the edge.

    Edge() : toNode(0), weight(0) {}
    Edge(int to, int w) : toNode(to), weight(w) {}
};

/**
 * @brief Node in the pointer-based graph structure.
 */
struct GraphNode {
    int nodeId;              ///< Node identifier
    LinkedList<Edge> edges;  ///< Adjacency list of edges
    GraphNode* next;         ///< Pointer to next node in the list

    GraphNode() : nodeId(0), next(nullptr) {}
    GraphNode(int id) : nodeId(id), next(nullptr) {}
};

/**
 * @brief Graph data structure using pointer-based adjacency lists.
 *
 * Models the city with nodes (locations) and weighted edges (roads)
 * to support shortest path calculations for the RoutePlanner.
 */
class Graph {
private:
    GraphNode* head;  ///< Head of the linked list of nodes
    int nodeCount;    ///< Total number of nodes

    /**
     * @brief Finds a node by its ID.
     * @param nodeId The node ID to search for.
     * @return Pointer to the node, or nullptr if not found.
     */
    GraphNode* findNode(int nodeId) const;

public:
    /**
     * @brief Constructs an empty graph.
     */
    Graph();

    /**
     * @brief Constructs a graph with a specified number of nodes.
     * @param nodeCount The total number of nodes to create.
     */
    explicit Graph(int nodeCount);

    /**
     * @brief Destroys the graph and releases memory.
     * @post Dynamically allocated memory for all nodes is deallocated.
     */
    ~Graph();

    /**
     * @brief Copy constructor.
     */
    Graph(const Graph& other);

    /**
     * @brief Assignment operator.
     */
    Graph& operator=(const Graph& other);

    /**
     * @brief Adds a weighted edge between two nodes.
     * @param from Source node index.
     * @param to Destination node index.
     * @param weight The weight/distance of the edge.
     * @pre `from` and `to` indices must be valid (0 <= index < nodeCount).
     * @post A new `Edge` structure is added to the adjacency list of the `from` node.
     */
    void addEdge(int from, int to, int weight);

    /**
     * @brief Adds a bidirectional edge (adds edge in both directions).
     * @param node1 First node.
     * @param node2 Second node.
     * @param weight The weight/distance of the edge.
     */
    void addBidirectionalEdge(int node1, int node2, int weight);

    /**
     * @brief Returns the adjacency list for a specific node.
     * @param node The index of the node whose list is requested.
     * @pre `node` index must be valid (0 <= node < nodeCount).
     * @return Reference to the linked list containing `Edge` structures.
     */
    LinkedList<Edge>& getAdjList(int node);

    /**
     * @brief Returns the adjacency list for a specific node (const version).
     * @param node The index of the node.
     * @return Const reference to the adjacency list.
     */
    const LinkedList<Edge>& getAdjList(int node) const;

    /**
     * @brief Returns the total number of nodes in the graph.
     * @return The node count.
     */
    int getNodeCount() const;
};

}  // namespace project
