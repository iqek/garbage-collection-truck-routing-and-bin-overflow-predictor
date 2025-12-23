/**
 * @file Facility.h
 * @brief Represents a facility (depot or disposal site) in the city.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once
#include <string>

namespace project {

/**
 * @brief Represents a facility such as depot or disposal site.
 */
class Facility {
private:
    std::string id;
    std::string type;  // "depot" or "disposal"
    int x;
    int y;
    int nodeId;

public:
    /**
     * @brief Constructs a facility.
     * @param id Unique facility identifier (e.g., "Depot", "Dump").
     * @param type Type of facility ("depot" or "disposal").
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param nodeId Graph node index where facility is located.
     */
    Facility(const std::string& id, const std::string& type, 
             int x, int y, int nodeId);

    /**
     * @brief Checks if this is a depot facility.
     * @return true if type is "depot".
     */
    bool isDepot() const;

    /**
     * @brief Checks if this is a disposal facility.
     * @return true if type is "disposal".
     */
    bool isDisposal() const;

    // Getter methods
    std::string getId() const;
    std::string getType() const;
    int getX() const;
    int getY() const;
    int getNodeId() const;
};

} // namespace project