/**
 * @file Simulation.h
 * @brief Main loop controlling the garbage collection simulation.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "Graph.h"
#include "Facilities.h"
#include "RoutePlanner.h"

namespace project {

/**
 * @brief Runs the time-based simulation for garbage collection management.
 *
 * Responsible for advancing the simulation time, updating bin states,
 * delegating routing decisions to the `RoutePlanner`, and coordinating
 * the truck's movement.
 */
class Simulation {
private:
    Graph& graph;
    Facilities& facilities;
    RoutePlanner planner;
    int currentTime;

public:
    /**
     * @brief Constructs a simulation instance.
     * @param graph Reference to the city graph.
     * @param facilities Reference to the physical facilities.
     */
    Simulation(Graph& graph, Facilities& facilities);

    /**
     * @brief Advances the simulation by one time step (one day).
     *
     * In this step, bins fill up, the route is planned, and the truck moves.
     * @post `currentTime` is incremented, and the states of all entities are updated.
     */
    void step();

    /**
     * @brief Checks if the simulation has reached its termination condition.
     * @return `true` if the simulation is complete (e.g., reached target time).
     */
    bool isFinished() const;

    /**
     * @brief Returns the current simulation time.
     * @return The day/time value.
     */
    int getTime() const;
};

} // namespace project