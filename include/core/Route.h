/**
 * @file Route.h
 * @brief Represents a collection route with sequence of bins to visit.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

namespace project {

/**
 * @brief Represents a planned collection route.
 * 
 * A route is a sequence of bin indices that the truck should visit,
 * along with metadata about the route's efficiency.
 */
class Route {
private:
    int* binIndices;     // Array of bin indices to visit
    int length;          // Number of bins in route
    int totalDistance;   // Total travel distance
    bool needsDisposal;  // Whether truck needs to visit disposal during route

public:
    /**
     * @brief Constructs an empty route.
     */
    Route();

    /**
     * @brief Constructs a route with specified bins.
     * @param bins Array of bin indices.
     * @param count Number of bins in the array.
     */
    Route(int* bins, int count);

    /**
     * @brief Destructor to free allocated memory.
     */
    ~Route();

    /**
     * @brief Copy constructor.
     */
    Route(const Route& other);

    /**
     * @brief Assignment operator.
     */
    Route& operator=(const Route& other);

    /**
     * @brief Adds a bin to the end of the route.
     * @param binIndex Index of bin to add.
     */
    void addBin(int binIndex);

    /**
     * @brief Gets the bin index at a specific position in route.
     * @param position Position in route (0-based).
     * @return Bin index at that position.
     */
    int getBinAt(int position) const;

    /**
     * @brief Gets the number of bins in this route.
     * @return Route length.
     */
    int getLength() const;

    /**
     * @brief Sets the total distance for this route.
     * @param distance Total distance value.
     */
    void setTotalDistance(int distance);

    /**
     * @brief Gets the total distance of this route.
     * @return Total distance.
     */
    int getTotalDistance() const;

    /**
     * @brief Marks whether this route requires disposal visit.
     * @param needs true if disposal visit needed.
     */
    void setNeedsDisposal(bool needs);

    /**
     * @brief Checks if route requires disposal visit.
     * @return true if disposal needed.
     */
    bool requiresDisposal() const;

    /**
     * @brief Checks if route is empty.
     * @return true if no bins in route.
     */
    bool isEmpty() const;

    /**
     * @brief Clears all bins from route.
     */
    void clear();
};

} // namespace project