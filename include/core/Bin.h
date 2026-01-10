/**
 * @file Bin.h
 * @brief Represents a garbage bin in the city.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once
#include <string>

namespace project {

/**
 * @brief Garbage bin with capacity, current fill level, and a daily fill rate.
 */
class Bin {
private:
    std::string id;
    std::string location;
    int capacity;
    int currentFill;
    int fillRate;
    int nodeId;
    int fillHistory[7];  // Last 7 days of fill data
    int historyIndex;

public:
    /**
     * @brief Constructs a garbage bin.
     * @param id Unique bin identifier (e.g., "B1").
     * @param location Location name (e.g., "Main St").
     * @param capacity Maximum capacity of the bin.
     * @param currentFill Initial fill level.
     * @param fillRate Daily fill rate (units/day).
     * @param nodeId Graph node index where the bin is located.
     */
    Bin(const std::string& id, const std::string& location, int capacity, 
        int currentFill, int fillRate, int nodeId);

    /**
     * @brief Default constructor (needed for array allocation).
     */
    Bin();

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

    /**
     * @brief Records current fill level in history for prediction.
     * @param fillLevel The fill level to record.
     */
    void recordFillLevel(int fillLevel);

    /**
     * @brief Calculates average fill rate from historical data.
     * @return Average fill rate based on history.
     */
    double getAverageFillRate() const;

    /**
     * @brief Checks if bin has overflowed.
     * @return true if currentFill >= capacity.
     */
    bool isOverflowing() const;

    // Getter methods
    std::string getId() const;
    std::string getLocation() const;
    int getCurrentFill() const;
    int getCapacity() const;
    int getFillRate() const;
    int getNodeId() const;
    // Set
    void setCurrentFill(int fill);
};

} // namespace project