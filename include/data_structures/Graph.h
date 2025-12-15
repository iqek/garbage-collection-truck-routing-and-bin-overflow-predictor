/**
 * @file Graph.h
 * @brief Represents the city map as a weighted graph using adjacency lists.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "LinkedList.h"

namespace project {

/**
 * @brief Structure for a weighted edge.
 *
 * Stores the destination node and the edge weight (distance) required
 * for routing algorithms like Dijkstra's.
 */
struct Edge {
    int toNode;    ///< Index of the destination node the edge connects to.
    int weight;    ///< The weight or distance of the edge.
};

/**
 * @brief Graph data structure using adjacency lists.
 *
 * Models the city with nodes (locations) and weighted edges (roads)
 * to support shortest path calculations for the RoutePlanner.
 */
class Graph {
private:
    int nodes;
    // Array of linked lists, where each list stores Edge structs.
    LinkedList<Edge>* adjacency; 

public:
    /**
     * @brief Constructs a graph with a specified number of nodes.
     * @param nodeCount The total number of nodes in the graph.
     */
    explicit Graph(int nodeCount);
    
    /**
     * @brief Destroys the graph and releases memory for adjacency lists.
     * @post Dynamically allocated memory for the `adjacency` array is deallocated.
     */
    ~Graph();

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
     * @brief Returns the adjacency list for a specific node.
     * @param node The index of the node whose list is requested.
     * @pre `node` index must be valid (0 <= node < nodeCount).
     * @return Reference to the linked list containing `Edge` structures.
     */
    LinkedList<Edge>& getAdjList(int node); 

    /**
     * @brief Returns the total number of nodes in the graph.
     * @return The node count.
     */
    int getNodeCount() const;
};

} // namespace project