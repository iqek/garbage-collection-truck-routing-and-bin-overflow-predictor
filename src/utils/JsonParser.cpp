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
    std::ifstream file(dataPath);  //dataPath'teki dosyayı aç
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << dataPath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();  //dosyayı oku buffer'da sakla
    return buffer.str();  //buffer'daki içeriği string olarak al
}

// Extract string value from JSON
std::string JsonParser::extractString(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";  //"key"
    size_t pos = json.find(searchKey); //ilk " indexi. ör:"id"nin ilk tırnağına geldik
    
    if (pos == std::string::npos) {  //bulunamadı anlamına geliyor
        return "";
    }
    
    // Find opening quote
    size_t start = json.find("\"", pos + searchKey.length());  //"key"den sonraki ilk tırnak ex:"B1"in ilk tırnağına geldik
    if (start == std::string::npos) {
        return "";
    }
    start++; // Move past the quote. ex:B'ye geldik
    
    // Find closing quote
    size_t end = json.find("\"", start);  //ex: B1"in kapanış tırnağını bulduk
    if (end == std::string::npos) {
        return "";
    }
    
    return json.substr(start, end - start); //ex: B1 döndürür
}

// Extract integer value from JSON
int JsonParser::extractInt(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return 0;
    }
    
    // Skip to after the colon and any whitespace
    size_t start = pos + searchKey.length(); //
    while (start < json.length() && (json[start] == ' ' || json[start] == '\t')) {
        start++;  //sayının ilk basamağına geldik
    }
    
    // Extract number
    std::string numStr;
    while (start < json.length() && (isdigit(json[start]) || json[start] == '-')) {  //rakamlar bitene kadar
        numStr += json[start];  //sayıyı string olarak kaydet
        start++;
    }
    
    if (numStr.empty()) {
        return 0;
    }
    
    return std::stoi(numStr);  //string to integer
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
    int depth = 0; //iç içe obje seviyesi
    
    for (size_t i = 0; i < arrayContent.length(); i++) {
        if (arrayContent[i] == '{') {
            if (depth == 0) count++;  //yeni object başladı
            depth++;   //bir object içine girdik
        } else if (arrayContent[i] == '}') {
            depth--;  //objectten çıktık
        }
    } 
    
    if (count == 0) { 
        return nullptr;
    }
    
    // Extract each object
    std::string* objects = new std::string[count];  //her nesne için string oluştur
    int objIndex = 0;
    depth = 0;
    size_t start = 0;
    
    for (size_t i = 0; i < arrayContent.length(); i++) {
        if (arrayContent[i] == '{') {
            if (depth == 0) {  //yeni object başladı
                start = i;  //pozisyonunu kaydet
            }
            depth++; 
        } else if (arrayContent[i] == '}') {  //object kapandı
            depth--;  //(depth = 0)
            if (depth == 0) {
                objects[objIndex++] = arrayContent.substr(start, i - start + 1);  //string'i array'e kaydet, sonraki nesne için index artır
            }
        }
    }  
    
    return objects;
}

// Load facilities
Facility* JsonParser::loadFacilities(int& count) {
    std::string content = readFile();  //tüm dosyayı oku
    std::string arrayContent = findArray(content, "facilities");  //facilities array'ini bul
    
    std::string* objects = extractObjects(arrayContent, count);  //array'i nesnelere böl
    if (!objects) {
        count = 0;
        return nullptr;
    }
    
    Facility* facilities = new Facility[count];
    
    for (int i = 0; i < count; i++) {  //objectin tüm değerlerini aldık
        std::string id = extractString(objects[i], "id");
        std::string type = extractString(objects[i], "type");
        int x = extractInt(objects[i], "x");
        int y = extractInt(objects[i], "y");
        
        // Map facility ID to node
        int nodeId = mapper.getOrCreateNode(id);  //facility'e nodeId verdik
        
        facilities[i] = Facility(id, type, x, y, nodeId);  //yeni facility nesnesi oluştur
    }
    
    delete[] objects;  //memory leak olmaması için array'e ayrılan belleği serbest bırak
    return facilities;  //facilities array'i
}

// Load bins
Bin* JsonParser::loadBins(int& count) {  //same with facilities
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
Truck JsonParser::loadTruck() {  //tek truck olduğu için pointer değil
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
    int startNode = mapper.getNode(position);  //position = depot'un zaten nodeId'si var
    if (startNode == -1) {  //position bulunamadıysa uyarı
        std::cerr << "Warning: Truck position '" << position 
                  << "' not found in mappings. Defaulting to node 0." << std::endl;
        startNode = 0;  //varsayılan
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
    
    Graph graph(totalNodes);  //bin + facilities graphı oluştur
    
    // Parse edges
    std::string arrayContent = findArray(content, "edges");
    int count;
    std::string* objects = extractObjects(arrayContent, count);  //edge nesnelerini aldık
    
    if (!objects) {
        std::cerr << "Warning: No edges found in JSON" << std::endl;
        return graph;  //edge yoksa boş graph
    }
    
    for (int i = 0; i < count; i++) {
        std::string from = extractString(objects[i], "from");
        std::string to = extractString(objects[i], "to");
        int distance = extractInt(objects[i], "distance");
        
        // Look up node IDs
        int fromNode = mapper.getNode(from);
        int toNode = mapper.getNode(to);  //string Id'leri node numaralarına çevir
        
        if (fromNode == -1) {
            std::cerr << "Warning: Edge 'from' location '" << from 
                      << "' not found in mappings." << std::endl;
            continue;  //node bulunamadıysa edge'i atla sonrakine geç
        }
        
        if (toNode == -1) {
            std::cerr << "Warning: Edge 'to' location '" << to 
                      << "' not found in mappings." << std::endl;
            continue;
        }
        
        graph.addEdge(fromNode, toNode, distance);  //yeni edge'i ekle
    }
    
    delete[] objects;
    return graph;
}

// Get mapper reference
LocationMapper& JsonParser::getMapper() { 
    return mapper; //parser dışından mapper'a ulaşmak için
}  

} // namespace project