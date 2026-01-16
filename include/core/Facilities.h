/**
 * @file Facilities.h
 * @brief Manages all physical facilities (Bins, Truck, and Facilities) in the system.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

#include "core/Bin.h"
#include "core/Facility.h"
#include "core/Truck.h"

namespace project {

/**
 * @brief Central container for all physical entities.
 *
 * Owns the garbage bins, facilities (depot/disposal), and the garbage truck,
 * providing centralized resource management and access for the simulation.
 */
class Facilities {
private:
    Bin* bins;
    int binCount;
    Truck truck;
    Facility* facilities;
    int facilityCount;

public:
    /**
     * @brief Constructs an empty facilities container.
     */
    Facilities();

    /**
     * @brief Destroys the facilities and frees dynamic memory.
     * @post The memory allocated for bins and facilities arrays is deallocated.
     */
    ~Facilities();

    /**
     * @brief Adds a garbage bin to the collection.
     * @param bin The bin instance to add.
     * @post The internal `bins` array is resized to include the new bin.
     */
    void addBin(const Bin& bin);

    /**
     * @brief Adds a facility to the collection.
     * @param facility The facility instance to add.
     * @post The internal `facilities` array is resized to include the new facility.
     */
    void addFacility(const Facility& facility);

    /**
     * @brief Returns the raw pointer to the array of bins.
     * @return Pointer to the start of the `Bin` array.
     */
    Bin* getBins();

    /**
     * @brief Returns a reference to a specific bin by its index.
     * @param index The index of the bin.
     * @pre `index` must be a valid index (0 <= index < binCount).
     * @return Reference to the requested `Bin` object.
     */
    Bin& getBin(int index) const;

    /**
     * @brief Returns the total number of bins.
     * @return The bin count.
     */
    int getBinCount() const;

    /**
     * @brief Returns the raw pointer to the array of facilities.
     * @return Pointer to the start of the `Facility` array.
     */
    Facility* getFacilities();

    /**
     * @brief Returns the total number of facilities.
     * @return The facility count.
     */
    int getFacilityCount() const;

    /**
     * @brief Finds and returns the depot node ID.
     * @return Node ID of the depot, or -1 if not found.
     */
    int getDepotNode() const;

    /**
     * @brief Finds all disposal facility node IDs.
     * @param count Output parameter for number of disposal sites found.
     * @return Dynamically allocated array of disposal node IDs.
     */
    int* getDisposalNodes(int& count) const;

    /**
     * @brief Sets the garbage truck instance.
     * @param truck The truck instance.
     */
    void setTruck(const Truck& truck);

    /**
     * @brief Returns a reference to the garbage truck.
     * @return Reference to the `Truck` object.
     */
    Truck& getTruck();
};

}  // namespace project
