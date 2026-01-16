/**
 * @file LocationMapper.h
 * @brief Maps string location IDs to integer graph node indices using a Hash Table.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-26
 */

#pragma once
#include "../data_structures/HashTable.h"

#include <string>

namespace project {

/**
 * @class LocationMapper
 * @brief Mapper for managing location-to-node-ID conversions.
 */
class LocationMapper {
private:
    HashTable hashTable;  ///< Internal hash table for storage
    int nextNodeId;       ///< Counter for assigning unique IDs

public:
    /**
     * @brief Constructs an empty location mapper.
     */
    LocationMapper();

    /**
     * @brief Destructor to release memory.
     */
    ~LocationMapper();

    /**
     * @brief Gets or creates a node ID for a location.
     * @param locationId String identifier (e.g., "Depot", "B1").
     * @return The assigned integer node ID.
     */
    int getOrCreateNode(const std::string& locationId);

    /**
     * @brief Gets the node ID for an existing location.
     * @param locationId String identifier.
     * @return The node ID, or -1 if not found.
     */
    int getNode(const std::string& locationId) const;

    /**
     * @brief Checks if a location has been mapped.
     */
    bool hasLocation(const std::string& locationId) const;

    /**
     * @brief Gets total number of unique mapped locations.
     */
    int getLocationCount() const;

    /**
     * @brief Clears all mappings.
     */
    void clear();
};

}  // namespace project