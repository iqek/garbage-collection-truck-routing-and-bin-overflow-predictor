/**
 * @file JsonParser.h
 * @brief Parses input files in JSON format.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "Graph.h"
#include "Bin.h"
#include "Truck.h"

namespace project {

/**
 * @brief JSON file parser for simulation configuration and initial data.
 *
 * Responsible for reading the initial state of the city graph, garbage bins,
 * and truck configuration from a JSON file path.
 */
class JsonParser {
private:
    const char* dataPath;

public:
    /**
     * @brief Constructs the parser with the path to the data file.
     * @param path The file path to the input JSON data.
     */
    explicit JsonParser(const char* path);

    /**
     * @brief Loads and constructs the city graph from the JSON file.
     * @post A new `Graph` object is created and populated with nodes and edges.
     * @return The loaded `Graph` object.
     */
    Graph loadGraph();

    /**
     * @brief Loads and constructs the array of garbage bins from the JSON file.
     * @param count Reference to store the total number of bins loaded.
     * @post The `count` variable is updated with the number of bins loaded.
     * @return A dynamically allocated array (`Bin*`) of loaded `Bin` objects.
     */
    Bin* loadBins(int& count);
    
    /**
     * @brief Loads and constructs the truck object from the JSON file.
     * @return The loaded `Truck` object.
     */
    Truck loadTruck();
};

} // namespace project