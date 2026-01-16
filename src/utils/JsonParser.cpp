/**
 * @file JsonParser.cpp
 * @brief Implementation using nlohmann/json library
 * @author İpek Çelik
 * @date 2025-12-28
 */

#include "utils/JsonParser.h"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace project {

JsonParser::JsonParser(const char* path) : dataPath(path) {}

// Load facilities
Facility* JsonParser::loadFacilities(int& count) {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            count = 0;
            return nullptr;
        }

        json data = json::parse(file);
        auto facilities_json = data["facilities"];
        count = facilities_json.size();

        if (count == 0) {
            return nullptr;
        }

        Facility* facilities = new Facility[count];

        for (int i = 0; i < count; i++) {
            auto& f = facilities_json[i];

            std::string id = f["id"];
            std::string type = f["type"];
            int x = f["x"];
            int y = f["y"];

            int nodeId = mapper.getOrCreateNode(id);

            facilities[i] = Facility(id, type, x, y, nodeId);
        }

        return facilities;

    } catch (json::exception& e) {
        std::cerr << "JSON parse error in loadFacilities: " << e.what() << std::endl;
        count = 0;
        return nullptr;
    }
}

// Load bins
Bin* JsonParser::loadBins(int& count) {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            count = 0;
            return nullptr;
        }

        json data = json::parse(file);
        auto bins_json = data["bins"];
        count = bins_json.size();

        if (count == 0) {
            return nullptr;
        }

        Bin* bins = new Bin[count];

        for (int i = 0; i < count; i++) {
            auto& b = bins_json[i];

            std::string id = b["id"];
            std::string location = b["location"];
            int capacity = b["capacity"];
            int currentFill = b["current_fill"];
            int fillRate = b["fill_rate"];

            int nodeId = mapper.getOrCreateNode(id);

            bins[i] = Bin(id, location, capacity, currentFill, fillRate, nodeId);
        }

        return bins;

    } catch (json::exception& e) {
        std::cerr << "JSON parse error in loadBins: " << e.what() << std::endl;
        count = 0;
        return nullptr;
    }
}

// Load truck
Truck JsonParser::loadTruck() {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            return Truck();
        }

        json data = json::parse(file);
        auto trucks_json = data["trucks"];

        if (trucks_json.empty()) {
            std::cerr << "Error: No trucks found in JSON" << std::endl;
            return Truck();
        }

        auto& t = trucks_json[0];  // Load first truck

        std::string id = t["id"];
        int capacity = t["capacity"];
        int currentLoad = t["current_load"];
        std::string position = t["position"];

        int startNode = mapper.getNode(position);
        if (startNode == -1) {
            std::cerr << "Warning: Truck position '" << position
                      << "' not found. Defaulting to node 0." << std::endl;
            startNode = 0;
        }

        return Truck(id, capacity, currentLoad, startNode);

    } catch (json::exception& e) {
        std::cerr << "JSON parse error in loadTruck: " << e.what() << std::endl;
        return Truck();
    }
}

// Load graph
Graph JsonParser::loadGraph() {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            return Graph(10);  // Default
        }

        json data = json::parse(file);

        int totalNodes = mapper.getLocationCount();
        if (totalNodes == 0) {
            std::cerr << "Warning: No locations mapped. Load facilities and bins first."
                      << std::endl;
            totalNodes = 10;
        }

        Graph graph(totalNodes);

        auto edges_json = data["edges"];

        for (auto& e : edges_json) {
            std::string from = e["from"];
            std::string to = e["to"];
            int distance = e["distance"];

            int fromNode = mapper.getNode(from);
            int toNode = mapper.getNode(to);

            if (fromNode == -1) {
                std::cerr << "Warning: Edge 'from' location '" << from << "' not found."
                          << std::endl;
                continue;
            }

            if (toNode == -1) {
                std::cerr << "Warning: Edge 'to' location '" << to << "' not found." << std::endl;
                continue;
            }

            graph.addEdge(fromNode, toNode, distance);
        }

        return graph;

    } catch (json::exception& e) {
        std::cerr << "JSON parse error in loadGraph: " << e.what() << std::endl;
        return Graph(10);
    }
}

LocationMapper& JsonParser::getMapper() {
    return mapper;
}

}  // namespace project