/**
 * @file RoutePlanner.h
 * @brief Determines optimal routes and targets for garbage collection.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "Graph.h"
#include "Facilities.h"
#include "PriorityQueue.h"
#include "OverflowPredictor.h"

namespace project {

/**
 * @brief Class responsible for selecting the truck's routing decisions.
 *
 * Routing decisions are made by balancing two core metrics: Overflow Risk
 * (from `OverflowPredictor`) and Travel Distance (from `Graph`), aiming for
 * multi-objective optimization to minimize travel while preventing bins from overflowing.
 */
class RoutePlanner {
private:
    const Graph& graph;
    OverflowPredictor predictor;

public:
    /**
     * @brief Constructs a route planner instance.
     * @param graph Reference to the city graph.
     */
    explicit RoutePlanner(const Graph& graph);

    /**
     * @brief Selects the next bin for the truck to visit.
     * @param facilities The system's physical facilities and assets.
     * @return The index of the selected bin. Returns a special value (e.g., -1) if no bin is selected.
     */
    int selectNextBin(const Facilities& facilities);

    /**
     * @brief Computes the shortest path distance between two nodes.
     *
     * This method uses the `Graph` structure and typically employs
     * Dijkstra's algorithm or a similar shortest path algorithm, leveraging the
     * custom `PriorityQueue` for efficiency.
     * @param from The source node.
     * @param to The destination node.
     * @return The shortest distance value between the two nodes.
     */
    int computeDistance(int from, int to) const;
};

} // namespace project