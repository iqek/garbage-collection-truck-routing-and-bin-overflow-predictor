/**
 * @file Truck.h
 * @brief Garbage collection truck and its basic operations.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once
#include <string>

namespace project {

/**
 * @brief The physical garbage collection truck.
 *
 * Manages the truck's capacity, current load, and its current location (node)
 * within the city graph.
 */
class Truck {
private:
    std::string id;
    int capacity;
    int load;
    int currentNode;

public:
    /**
     * @brief Constructs a truck instance.
     * @param id Unique truck identifier (e.g., "T1").
     * @param capacity Maximum carrying capacity.
     * @param initialLoad Initial load amount.
     * @param startNode The initial location (node index).
     */
    Truck(const std::string& id, int capacity, int initialLoad, int startNode);

    /**
     * @brief Default constructor for initialization.
     */
    Truck();

    /**
     * @brief Moves the truck to a specified node.
     * @param node The destination node.
     * @post The truck's `currentNode` is updated.
     */
    void moveTo(int node);

    /**
     * @brief Adds collected garbage to the truck's load.
     * @param amount The collected amount.
     * @pre The collected amount must not exceed the remaining capacity.
     * @post The truck's `load` amount is increased by `amount`.
     */
    void collect(int amount);

    /**
     * @brief Empties the truck's load at a disposal facility.
     * @post The truck's `load` is set to 0.
     */
    void unload();

    /**
     * @brief Gets remaining capacity in the truck.
     * @return Available capacity (capacity - load).
     */
    int getRemainingCapacity() const;

    /**
     * @brief Checks if truck is full.
     * @return true if load >= capacity.
     */
    bool isFull() const;

    // Getter methods
    std::string getId() const;
    int getLoad() const;
    int getCapacity() const;
    int getCurrentNode() const;
};

} // namespace project