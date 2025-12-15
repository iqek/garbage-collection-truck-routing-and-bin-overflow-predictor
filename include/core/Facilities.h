/**
 * @file Facilities.h
 * @brief Manages all physical facilities (Bins and Truck) in the system.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "Bin.h"
#include "Truck.h"

namespace project {

/**
 * @brief Central container for all physical entities.
 *
 * Owns the garbage bins (managed via dynamic array) and the garbage truck,
 * providing centralized resource management and access for the simulation.
 */
class Facilities {
private:
    Bin* bins;
    int binCount;
    Truck truck;

public:
    /**
     * @brief Constructs an empty facilities container.
     */
    Facilities();
    
    /**
     * @brief Destroys the facilities and frees dynamic memory for bins.
     * @post The memory allocated for the `bins` array is deallocated.
     */
    ~Facilities();

    /**
     * @brief Adds a garbage bin to the collection.
     * @param bin The bin instance to add.
     * @post The internal `bins` array is resized (or reallocated) to include the new bin.
     */
    void addBin(const Bin& bin);

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
    Bin& getBin(int index);

    /**
     * @brief Returns the total number of bins.
     * @return The bin count.
     */
    int getBinCount() const;

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

} // namespace project