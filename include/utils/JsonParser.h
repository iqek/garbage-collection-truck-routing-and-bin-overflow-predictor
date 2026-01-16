/**
 * @file JsonParser.h
 * @brief Parses input files in JSON format.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-26
 */

#pragma once

#include "core/Bin.h"
#include "core/Facility.h"
#include "core/Truck.h"
#include "data_structures/Graph.h"
#include "utils/LocationMapper.h"

#include <string>

namespace project {

/**
 * @brief JSON file parser for simulation configuration and initial data.
 *
 * Responsible for reading the initial state from JSON, including the city graph,
 * garbage bins, truck configuration, and facilities. Handles string ID to
 * integer node ID conversion using LocationMapper.
 */
class JsonParser {
private:
    const char* dataPath;
    LocationMapper mapper;

public:
    /**
     * @brief Constructs the parser with the path to the data file.
     * @param path The file path to the input JSON data.
     */
    explicit JsonParser(const char* path);

    /**
     * @brief Loads and constructs the city graph from the JSON file.
     *
     * First pass: reads bins and facilities to build location mappings.
     * Second pass: reads edges and converts string IDs to node indices.
     * @post Graph is populated with nodes and weighted edges.
     * @return The loaded `Graph` object.
     */
    Graph loadGraph();

    /**
     * @brief Loads and constructs the array of garbage bins from JSON.
     *
     * Bins are assigned node IDs based on their location strings.
     * @param count Reference to store the total number of bins loaded.
     * @post The `count` variable is updated with the number of bins.
     * @return Dynamically allocated array of `Bin` objects.
     */
    Bin* loadBins(int& count);

    /**
     * @brief Loads and constructs the truck object from JSON.
     *
     * Truck position string is converted to node ID using mapper.
     * @return The loaded `Truck` object.
     */
    Truck loadTruck();

    /**
     * @brief Loads and constructs facilities array from JSON.
     *
     * Facilities include depots and disposal sites with coordinates.
     * @param count Reference to store the total number of facilities loaded.
     * @post The `count` variable is updated with facility count.
     * @return Dynamically allocated array of `Facility` objects.
     */
    Facility* loadFacilities(int& count);

    /**
     * @brief Gets reference to the location mapper.
     *
     * Useful for debugging or querying location-to-node mappings after parsing.
     * @return Reference to internal LocationMapper.
     */
    LocationMapper& getMapper();
};

}  // namespace project