/**
 * @file JsonParser.cpp
 * @brief Implementation of JsonParser class.
 * @author İpek Çelik
 * @date 2025-12-28
 */

#include "JsonParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace project {

// Constructor
JsonParser::JsonParser(const char* path) : dataPath(path) {
}

// Read entire file into string
std::string JsonParser::readFile() const {
    std::ifstream file(dataPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << dataPath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Extract string value from JSON
std::string JsonParser::extractString(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return "";
    }
    
    // Find opening quote
    size_t start = json.find("\"", pos + searchKey.length());
    if (start == std::string::npos) {
        return "";
    }
    start++; // Move past the quote
    
    // Find closing quote
    size_t end = json.find("\"", start);
    if (end == std::string::npos) {
        return "";
    }
    
    return json.substr(start, end - start);
}

// Extract integer value from JSON
int JsonParser::extractInt(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return 0;
    }
    
    // Skip to after the colon and any whitespace
    size_t start = pos + searchKey.length();
    while (start < json.length() && (json[start] == ' ' || json[start] == '\t')) {
        start++;
    }
    
    // Extract number
    std::string numStr;
    while (start < json.length() && (isdigit(json[start]) || json[start] == '-')) {
        numStr += json[start];
        start++;
    }
    
    if (numStr.empty()) {
        return 0;
    }
    
    return std::stoi(numStr);
}

// Find a JSON array
std::string JsonParser::findArray(const std::string& json, const std::string& arrayName) const {
    std::string searchKey = "\"" + arrayName + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return "";
    }
    
    // Find opening bracket
    size_t start = json.find("[", pos);
    if (start == std::string::npos) {
        return "";
    }
    
    // Find matching closing bracket
    int bracketCount = 1;
    size_t end = start + 1;
    
    while (end < json.length() && bracketCount > 0) {
        if (json[end] == '[') bracketCount++;
        else if (json[end] == ']') bracketCount--;
        end++;
    }
    
    if (bracketCount != 0) {
        return "";
    }
    
    return json.substr(start + 1, end - start - 2); // Exclude [ and ]
}

// Extract objects from array
std::string* JsonParser::extractObjects(const std::string& arrayContent, int& count) const {
    // Count objects (count opening braces at depth 0)
    count = 0;
    int depth = 0;
    
    for (size_t i = 0; i < arrayContent.length(); i++) {
        if (arrayContent[i] == '{') {
            if (depth == 0) count++;
            depth++;
        } else if (arrayContent[i] == '}') {
            depth--;
        }
    }
    
    if (count == 0) {
        return nullptr;
    }
    
    // Extract each object
    std::string* objects = new std::string[count];
    int objIndex = 0;
    depth = 0;
    size_t start = 0;
    
    for (size_t i = 0; i < arrayContent.length(); i++) {
        if (arrayContent[i] == '{') {
            if (depth == 0) {
                start = i;
            }
            depth++;
        } else if (arrayContent[i] == '}') {
            depth--;
            if (depth == 0) {
                objects[objIndex++] = arrayContent.substr(start, i - start + 1);
            }
        }
    }
    
    return objects;
}

// Load facilities
Facility* JsonParser::loadFacilities(int& count) {
    std::string content = readFile();
    std::string arrayContent = findArray(content, "facilities");
    
    std::string* objects = extractObjects(arrayContent, count);
    if (!objects) {
        count = 0;
        return nullptr;
    }
    
    Facility* facilities = new Facility[count];
    
    for (int i = 0; i < count; i++) {
        std::string id = extractString(objects[i], "id");
        std::string type = extractString(objects[i], "type");
        int x = extractInt(objects[i], "x");
        int y = extractInt(objects[i], "y");
        
        // Map facility ID to node
        int nodeId = mapper.getOrCreateNode(id);
        
        facilities[i] = Facility(id, type, x, y, nodeId);
    }
    
    delete[] objects;
    return facilities;
}

// Load bins
Bin* JsonParser::loadBins(int& count) {
    std::string content = readFile();
    std::string arrayContent = findArray(content, "bins");
    
    std::string* objects = extractObjects(arrayContent, count);
    if (!objects) {
        count = 0;
        return nullptr;
    }
    
    Bin* bins = new Bin[count];
    
    for (int i = 0; i < count; i++) {
        std::string id = extractString(objects[i], "id");
        std::string location = extractString(objects[i], "location");
        int capacity = extractInt(objects[i], "capacity");
        int currentFill = extractInt(objects[i], "current_fill");
        int fillRate = extractInt(objects[i], "fill_rate");
        
        // Map bin ID to node
        int nodeId = mapper.getOrCreateNode(id);
        
        bins[i] = Bin(id, location, capacity, currentFill, fillRate, nodeId);
    }
    
    delete[] objects;
    return bins;
}

// Load truck
Truck JsonParser::loadTruck() {
    std::string content = readFile();
    std::string arrayContent = findArray(content, "trucks");
    
    int count;
    std::string* objects = extractObjects(arrayContent, count);
    
    if (!objects || count == 0) {
        std::cerr << "Error: No trucks found in JSON" << std::endl;
        return Truck();
    }
    
    // Load first truck only (single truck system)
    std::string id = extractString(objects[0], "id");
    int capacity = extractInt(objects[0], "capacity");
    int currentLoad = extractInt(objects[0], "current_load");
    std::string position = extractString(objects[0], "position");
    
    // Map position to node
    int startNode = mapper.getNode(position);
    if (startNode == -1) {
        std::cerr << "Warning: Truck position '" << position 
                  << "' not found in mappings. Defaulting to node 0." << std::endl;
        startNode = 0;
    }
    
    delete[] objects;
    return Truck(id, capacity, currentLoad, startNode);
}

// Load graph
Graph JsonParser::loadGraph() {
    std::string content = readFile();
    
    // First, ensure all locations are mapped
    // (This happens when loading facilities and bins first)
    int totalNodes = mapper.getLocationCount();
    
    if (totalNodes == 0) {
        std::cerr << "Warning: No locations mapped. Load facilities and bins first." << std::endl;
        totalNodes = 10; // Default size
    }
    
    Graph graph(totalNodes);
    
    // Parse edges
    std::string arrayContent = findArray(content, "edges");
    int count;
    std::string* objects = extractObjects(arrayContent, count);
    
    if (!objects) {
        std::cerr << "Warning: No edges found in JSON" << std::endl;
        return graph;
    }
    
    for (int i = 0; i < count; i++) {
        std::string from = extractString(objects[i], "from");
        std::string to = extractString(objects[i], "to");
        int distance = extractInt(objects[i], "distance");
        
        // Look up node IDs
        int fromNode = mapper.getNode(from);
        int toNode = mapper.getNode(to);
        
        if (fromNode == -1) {
            std::cerr << "Warning: Edge 'from' location '" << from 
                      << "' not found in mappings." << std::endl;
            continue;
        }
        
        if (toNode == -1) {
            std::cerr << "Warning: Edge 'to' location '" << to 
                      << "' not found in mappings." << std::endl;
            continue;
        }
        
        graph.addEdge(fromNode, toNode, distance);
    }
    
    delete[] objects;
    return graph;
}

// Get mapper reference
LocationMapper& JsonParser::getMapper() {
    return mapper;
}

} // namespace project