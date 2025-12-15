/**
 * @file Truck.h
 * @brief Garbage collection truck and its basic operations.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

namespace project {

/**
 * @brief The physical garbage collection truck.
 *
 * Manages the truck's capacity, current load, and its current location (node)
 * within the city graph.
 */
class Truck {
private:
    int capacity;
    int load;
    int currentNode;

public:
    /**
     * @brief Constructs a truck instance.
     * @param capacity Maximum carrying capacity.
     * @param startNode The initial location (Depot node index).
     */
    Truck(int capacity, int startNode);

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

    // Getter methods...
    int getLoad() const;
    int getCurrentNode() const;
};

} // namespace project