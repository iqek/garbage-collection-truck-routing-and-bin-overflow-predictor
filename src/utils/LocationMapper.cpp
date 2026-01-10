/**
 * @file LocationMapper.cpp
 * @brief Implementation of LocationMapper class.
 * @author İpek Çelik
 * @date 2025-12-26
 */

#include "utils/LocationMapper.h"

namespace project {

// Constructor
LocationMapper::LocationMapper() : nextNodeId(0) {
}

// Destructor
LocationMapper::~LocationMapper() {
}

// Get or create node
int LocationMapper::getOrCreateNode(const std::string& locationId) {
    // Check if already exists
    int existingNode = hashTable.search(locationId);
    if (existingNode != -1) {
        return existingNode;
    }
    
    // Create new mapping
    int nodeId = nextNodeId++;
    hashTable.insert(locationId, nodeId);
    
    return nodeId;
}

// Get existing node
int LocationMapper::getNode(const std::string& locationId) const {
    return hashTable.search(locationId);
}

// Check if location exists
bool LocationMapper::hasLocation(const std::string& locationId) const {
    return hashTable.search(locationId) != -1;
}

// Get location count
int LocationMapper::getLocationCount() const {
    return nextNodeId;
}

// Clear all mappings
void LocationMapper::clear() {
    hashTable.clear();
    nextNodeId = 0;
}

} // namespace project