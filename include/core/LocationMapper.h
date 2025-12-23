/**
 * @file LocationMapper.h
 * @brief Maps string location IDs to integer graph node indices.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once
#include <string>

namespace project {

/**
 * @brief Maps string location identifiers to graph node indices.
 * 
 * Used during JSON parsing to convert location names like "Depot", "B1",
 * "Main St" into integer node IDs for graph operations.
 */
class LocationMapper {
private:
    struct MapEntry {
        std::string locationId;
        int nodeId;
    };
    
    MapEntry* mappings;
    int capacity;
    int size;
    int nextNodeId;

    /**
     * @brief Resizes the internal array when capacity is reached.
     */
    void resize();

public:
    /**
     * @brief Constructs an empty location mapper.
     */
    LocationMapper();

    /**
     * @brief Destructor to free allocated memory.
     */
    ~LocationMapper();

    /**
     * @brief Gets or creates a node ID for a location.
     * 
     * If the location already exists, returns its node ID.
     * If not, assigns a new node ID and stores the mapping.
     * @param locationId String identifier (e.g., "Depot", "B1").
     * @return The node ID (integer) for this location.
     */
    int getOrCreateNode(const std::string& locationId);

    /**
     * @brief Gets the node ID for an existing location.
     * @param locationId String identifier.
     * @return The node ID, or -1 if location not found.
     */
    int getNode(const std::string& locationId) const;

    /**
     * @brief Checks if a location has been mapped.
     * @param locationId String identifier.
     * @return true if location exists in mappings.
     */
    bool hasLocation(const std::string& locationId) const;

    /**
     * @brief Gets the total number of mapped locations.
     * @return Number of unique locations mapped.
     */
    int getLocationCount() const;

    /**
     * @brief Clears all mappings.
     */
    void clear();
};

} // namespace project