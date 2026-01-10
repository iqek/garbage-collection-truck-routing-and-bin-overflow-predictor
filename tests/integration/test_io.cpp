/**
 * @file test_io.cpp
 * @brief Integration tests for io
 * @author Miray Duygulu
 * @date 2026-01-10
 */

#pragma once

#include "Bin.h"
#include "Facility.h"
#include "Graph.h"
#include "LocationMapper.h"
#include "Truck.h"

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

namespace project {

class JsonParser {
private:
    const char* dataPath;
    LocationMapper mapper;

public:
    explicit JsonParser(const char* path);

    Graph loadGraph();
    Bin* loadBins(int& count);
    Truck loadTruck();
    Facility* loadFacilities(int& count);

    LocationMapper& getMapper();
};


JsonParser::JsonParser(const char* path) : dataPath(path) {}


Facility* JsonParser::loadFacilities(int& count) {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            count = 0;
            return nullptr;
        }

        json data = json::parse(file);
        auto facilitiesJson = data["facilities"];
        count = facilitiesJson.size();

        if (count == 0) {
            return nullptr;
        }

        Facility* facilities = new Facility[count];

        for (int i = 0; i < count; ++i) {
            auto& f = facilitiesJson[i];

            std::string id = f["id"];
            std::string type = f["type"];
            int x = f["x"];
            int y = f["y"];

            int nodeId = mapper.getOrCreateNode(id);
            facilities[i] = Facility(id, type, x, y, nodeId);
        }

        return facilities;

    } catch (json::exception& e) {
        std::cerr << "JSON parse error in loadFacilities: " << e.what()
                  << std::endl;
        count = 0;
        return nullptr;
    }
}


Bin* JsonParser::loadBins(int& count) {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            count = 0;
            return nullptr;
        }

        json data = json::parse(file);
        auto binsJson = data["bins"];
        count = binsJson.size();

        if (count == 0) {
            return nullptr;
        }

        Bin* bins = new Bin[count];

        for (int i = 0; i < count; ++i) {
            auto& b = binsJson[i];

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


Truck JsonParser::loadTruck() {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            return Truck();
        }

        json data = json::parse(file);
        auto trucksJson = data["trucks"];

        if (trucksJson.empty()) {
            std::cerr << "Error: No trucks found in JSON" << std::endl;
            return Truck();
        }

        auto& t = trucksJson[0];

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


Graph JsonParser::loadGraph() {
    try {
        std::ifstream file(dataPath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << dataPath << std::endl;
            return Graph(10);
        }

        json data = json::parse(file);

        int totalNodes = mapper.getLocationCount();
        if (totalNodes == 0) {
            std::cerr
                << "Warning: No locations mapped. Load bins and facilities first."
                << std::endl;
            totalNodes = 10;
        }

        Graph graph(totalNodes);

        auto edgesJson = data["edges"];
        for (auto& e : edgesJson) {
            std::string from = e["from"];
            std::string to = e["to"];
            int distance = e["distance"];

            int fromNode = mapper.getNode(from);
            int toNode = mapper.getNode(to);

            if (fromNode == -1 || toNode == -1) {
                std::cerr << "Warning: Invalid edge (" << from << " -> " << to
                          << ")" << std::endl;
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
