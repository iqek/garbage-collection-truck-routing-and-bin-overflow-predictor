/**
 * @file RoutePlanner.h
 * @brief Determines optimal routes for garbage collection.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

#include "Graph.h"
#include "Facilities.h"
#include "PriorityQueue.h"
#include "OverflowPredictor.h"
#include "Route.h"

namespace project {

/**
 * @brief Class responsible for planning truck routing decisions.
 *
 * Routing decisions balance overflow risk and travel distance through
 * multi-objective optimization to minimize travel while preventing overflows.
 */
class RoutePlanner {
private:
    const Graph& graph;
    OverflowPredictor predictor;

    /**
     * @brief Calculates priority score for a bin.
     * 
     * Combines overflow risk and distance into single score.
     * Lower score = higher priority.
     * @param bin Bin to evaluate.
     * @param distance Distance from current location.
     * @return Priority score.
     */
    double calculatePriority(const Bin& bin, int distance) const;

public:
    /**
     * @brief Constructs a route planner instance.
     * @param graph Reference to the city graph.
     */
    explicit RoutePlanner(const Graph& graph);

    /**
     * @brief Plans a complete collection route for the truck.
     * 
     * Considers truck capacity, bin priorities, and disposal facility locations
     * to create an efficient multi-bin route.
     * @param facilities The system's physical facilities and assets.
     * @return Route object containing sequence of bins to visit.
     */
    Route planRoute(const Facilities& facilities);

    /**
     * @brief Selects the next single bin for the truck to visit.
     * 
     * Greedy selection based on priority score (overflow risk + distance).
     * @param facilities The system's physical facilities and assets.
     * @return The index of the selected bin. Returns -1 if no bin needs collection.
     */
    int selectNextBin(const Facilities& facilities);

    /**
     * @brief Finds the nearest disposal facility from current location.
     * @param currentNode Current truck location.
     * @param facilities System facilities.
     * @return Node ID of nearest disposal facility, or -1 if none found.
     */
    int findNearestDisposal(int currentNode, const Facilities& facilities) const;

    /**
     * @brief Computes the shortest path distance between two nodes.
     *
     * Uses Dijkstra's algorithm with custom PriorityQueue.
     * @param from The source node.
     * @param to The destination node.
     * @return The shortest distance value between the two nodes.
     */
    int computeDistance(int from, int to) const;

    /**
     * @brief Checks if any bins require immediate attention.
     * @param facilities System facilities.
     * @return true if any bin is critical (near overflow).
     */
    bool hasCriticalBins(const Facilities& facilities) const;
};

} // namespace project