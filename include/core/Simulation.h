/**
 * @file Simulation.h
 * @brief Main loop controlling the garbage collection simulation.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

#include "data_structures/Graph.h"
#include "core/Facilities.h"
#include "core/RoutePlanner.h"

namespace project {

/**
 * @brief Runs the time-based simulation for garbage collection management.
 *
 * Responsible for advancing simulation time, updating bin states,
 * planning routes, coordinating truck movement, and tracking performance metrics.
 */
class Simulation {
private:
    Graph& graph;
    Facilities& facilities;
    RoutePlanner planner;
    int currentTime;
    int maxTime;  // Total simulation duration (e.g., 7 days)
    
    // Performance tracking
    int overflowCount;
    int totalDistance;
    int collectionsCompleted;

public:
    /**
     * @brief Constructs a simulation instance.
     * @param graph Reference to the city graph.
     * @param facilities Reference to the physical facilities.
     * @param duration Total simulation days (default 7 for one week).
     */
    Simulation(Graph& graph, Facilities& facilities, int duration = 7);

    /**
     * @brief Advances the simulation by one time step (one day).
     *
     * Each step:
     * 1. Updates all bin fill levels
     * 2. Records fill history for predictions
     * 3. Plans collection route
     * 4. Executes truck movements and collections
     * 5. Handles disposal trips when truck is full
     * 6. Tracks overflow events
     * @post `currentTime` is incremented, entity states updated.
     */
    void step();

    /**
     * @brief Runs the complete simulation until finished.
     * 
     * Repeatedly calls step() until isFinished() returns true.
     */
    void run();

    /**
     * @brief Checks if the simulation has reached its termination condition.
     * @return `true` if currentTime >= maxTime.
     */
    bool isFinished() const;

    /**
     * @brief Returns the current simulation time (day).
     * @return The current day value.
     */
    int getTime() const;

    /**
     * @brief Returns the maximum simulation time (total days).
     * @return The maximum day value.
     */
    int getMaxTime() const;

    /**
     * @brief Returns reference to the facilities object.
     * @return Reference to Facilities.
     */
    Facilities& getFacilities();

    /**
     * @brief Checks for overflow events and updates counter.
     * @post Increments overflowCount for each bin at or over capacity.
     */
    void checkOverflows();

    /**
     * @brief Handles dynamic rescheduling when critical bins detected.
     * 
     * Called when sensor data indicates unexpected rapid filling.
     * Adjusts current route to prioritize critical bins.
     */
    void handleEmergencyReschedule();

    // Performance metrics getters
    
    /**
     * @brief Gets total number of overflow events recorded.
     * @return Overflow count.
     */
    int getOverflowCount() const;

    /**
     * @brief Gets total distance traveled by truck.
     * @return Total distance.
     */
    int getTotalDistance() const;

    /**
     * @brief Gets total number of successful collections.
     * @return Collection count.
     */
    int getCollectionsCompleted() const;

    /**
     * @brief Prints simulation statistics and results.
     */
    void printStatistics() const;
};

} // namespace project