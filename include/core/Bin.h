/**
 * @file Bin.h
 * @brief Represents a garbage bin in the city.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

namespace project {

/**
 * @brief Garbage bin with capacity, current fill level, and a daily fill rate.
 */
class Bin {
private:
    int id;
    int capacity;
    int currentFill;
    int fillRate;
    int nodeId;

public:
    /**
     * @brief Constructs a garbage bin.
     * @param id Unique bin identifier.
     * @param capacity Maximum capacity of the bin.
     * @param fillRate Daily fill rate (units/day).
     * @param nodeId Graph node index where the bin is located.
     */
    Bin(int id, int capacity, int fillRate, int nodeId);

    /**
     * @brief Updates the fill level for one time step (day).
     * @post The `currentFill` amount is increased by `fillRate`, capped at `capacity`.
     */
    void updateFill();

    /**
     * @brief Empties the bin by a specified amount upon collection.
     * @param amount The collected amount.
     * @post `currentFill` amount is reduced by `amount`.
     */
    void collect(int amount);

    // Getter methods...
    int getCurrentFill() const;
    int getCapacity() const;
    int getFillRate() const;
    int getNodeId() const;
};

} // namespace project