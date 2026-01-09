/**
 * @file test_io.cpp
 * @brief Tests for JSON parsing and file I/O operations
 */

#include "doctest.h"
#include "utils/JsonParser.h"
#include "utils/LocationMapper.h" 
#include <fstream>
#include <cstdio>

using namespace project;

/*=== JSON PARSING TESTS ===*/

TEST_CASE("IO: Parse valid JSON file") {
    JsonParser parser("data.json");
    
    int binCount = 0;
    Bin* bins = parser.loadBins(binCount);
    
    CHECK(binCount > 0);
    CHECK(bins != nullptr);
    
    delete[] bins;
}

TEST_CASE("IO: Load bins from JSON") {
    JsonParser parser("data.json");
    
    int count = 0;
    Bin* bins = parser.loadBins(count);
    
    REQUIRE(bins != nullptr);
    REQUIRE(count >= 1);
    
    // Verify first bin has expected structure
    CHECK(bins[0].getId() != "");
    CHECK(bins[0].getCapacity() > 0);
    CHECK(bins[0].getFillRate() >= 0);
    
    delete[] bins;
}

TEST_CASE("IO: Load facilities from JSON") {
    JsonParser parser("data.json");
    
    int count = 0;
    Facility* facilities = parser.loadFacilities(count);
    
    REQUIRE(facilities != nullptr);
    REQUIRE(count >= 1);
    
    // Should have at least a depot
    bool hasDepot = false;
    for (int i = 0; i < count; i++) {
        if (facilities[i].isDepot()) {
            hasDepot = true;
            break;
        }
    }
    CHECK(hasDepot == true);
    
    delete[] facilities;
}

TEST_CASE("IO: Load truck from JSON") {
    JsonParser parser("data.json");
    
    Truck truck = parser.loadTruck();
    
    CHECK(truck.getId() != "");
    CHECK(truck.getCapacity() > 0);
    CHECK(truck.getLoad() >= 0);
}

TEST_CASE("IO: Load graph from JSON") {
    JsonParser parser("data.json");
    
    // Load facilities and bins first to populate mapper
    int binCount, facilityCount;
    Bin* bins = parser.loadBins(binCount);
    Facility* facilities = parser.loadFacilities(facilityCount);
    
    Graph graph = parser.loadGraph();
    
    CHECK(graph.getNodeCount() > 0);
    
    delete[] bins;
    delete[] facilities;
}

TEST_CASE("IO: Parse minimal JSON") {
    JsonParser parser("test_minimal.json");
    
    int count = 0;
    Bin* bins = parser.loadBins(count);
    
    CHECK(count == 1);
    REQUIRE(bins != nullptr);
    CHECK(bins[0].getId() == "B1");
    
    delete[] bins;
}

TEST_CASE("IO: Parse empty bins array") {
    JsonParser parser("test_empty.json");
    
    int count = 0;
    Bin* bins = parser.loadBins(count);
    
    CHECK(count == 0);
    CHECK(bins == nullptr);
}

TEST_CASE("IO: Parse overflow scenario JSON") {
    JsonParser parser("test_overflow.json");
    
    int count = 0;
    Bin* bins = parser.loadBins(count);
    
    REQUIRE(bins != nullptr);
    CHECK(count >= 3);
    
    // Check for high fill rates
    bool hasHighFill = false;
    for (int i = 0; i < count; i++) {
        if (bins[i].getFillRate() >= 20) {
            hasHighFill = true;
            break;
        }
    }
    CHECK(hasHighFill == true);
    
    delete[] bins;
}

TEST_CASE("IO: Parse capacity test JSON") {
    JsonParser parser("test_capacity.json");
    
    int facilityCount = 0;
    Facility* facilities = parser.loadFacilities(facilityCount);
    
    REQUIRE(facilities != nullptr);
    
    // Should have multiple disposal sites
    int disposalCount = 0;
    for (int i = 0; i < facilityCount; i++) {
        if (facilities[i].isDisposal()) {
            disposalCount++;
        }
    }
    CHECK(disposalCount >= 2);
    
    delete[] facilities;
}

TEST_CASE("IO: Parse stress test JSON") {
    JsonParser parser("test_stress.json");
    
    int binCount = 0;
    Bin* bins = parser.loadBins(binCount);
    
    CHECK(binCount >= 10); // Should have many bins
    
    if (bins) {
        delete[] bins;
    }
}

/*=== LOCATION MAPPER TESTS ===*/

TEST_CASE("IO: LocationMapper creates unique nodes") {
    LocationMapper mapper;
    
    int node1 = mapper.getOrCreateNode("Depot");
    int node2 = mapper.getOrCreateNode("B1");
    int node3 = mapper.getOrCreateNode("Depot");
    
    CHECK(node1 != node2);
    CHECK(node1 == node3); // Same ID returns same node
}

TEST_CASE("IO: LocationMapper tracks count") {
    LocationMapper mapper;
    
    mapper.getOrCreateNode("A");
    mapper.getOrCreateNode("B");
    mapper.getOrCreateNode("C");
    
    CHECK(mapper.getLocationCount() == 3);
}

TEST_CASE("IO: LocationMapper hasLocation check") {
    LocationMapper mapper;
    
    mapper.getOrCreateNode("Depot");
    
    CHECK(mapper.hasLocation("Depot") == true);
    CHECK(mapper.hasLocation("NotExist") == false);
}

TEST_CASE("IO: LocationMapper getNode for non-existent") {
    LocationMapper mapper;
    
    CHECK(mapper.getNode("DoesNotExist") == -1);
}

TEST_CASE("IO: LocationMapper clear") {
    LocationMapper mapper;
    
    mapper.getOrCreateNode("A");
    mapper.getOrCreateNode("B");
    
    mapper.clear();
    
    CHECK(mapper.getLocationCount() == 0);
    CHECK(mapper.hasLocation("A") == false);
}

/*=== STRING EXTRACTION TESTS ===*/

TEST_CASE("IO: Extract string from JSON") {
    JsonParser parser("data.json");
    std::string json = R"({"id":"B1","location":"ParkAve"})";
    
    std::string id = parser.extractString(json, "id");
    std::string location = parser.extractString(json, "location");
    
    CHECK(id == "B1");
    CHECK(location == "ParkAve");
}

TEST_CASE("IO: Extract integer from JSON") {
    JsonParser parser("data.json");
    std::string json = R"({"capacity":100,"fill_rate":8})";
    
    int capacity = parser.extractInt(json, "capacity");
    int fillRate = parser.extractInt(json, "fill_rate");
    
    CHECK(capacity == 100);
    CHECK(fillRate == 8);
}

TEST_CASE("IO: Extract non-existent key") {
    JsonParser parser("data.json");
    std::string json = R"({"id":"B1"})";
    
    std::string result = parser.extractString(json, "nonexistent");
    CHECK(result == "");
    
    int intResult = parser.extractInt(json, "nonexistent");
    CHECK(intResult == 0);
}

TEST_CASE("IO: Find array in JSON") {
    JsonParser parser("data.json");
    std::string json = R"({"bins":[{"id":"B1"},{"id":"B2"}]})";
    
    std::string array = parser.findArray(json, "bins");
    CHECK(array.find("B1") != std::string::npos);
    CHECK(array.find("B2") != std::string::npos);
}

TEST_CASE("IO: Extract objects from array") {
    JsonParser parser("data.json");
    std::string arrayContent = R"({"id":"B1"},{"id":"B2"},{"id":"B3"})";
    
    int count = 0;
    std::string* objects = parser.extractObjects(arrayContent, count);
    
    REQUIRE(objects != nullptr);
    CHECK(count == 3);
    
    delete[] objects;
}

/*=== ERROR HANDLING TESTS ===*/

TEST_CASE("IO: Handle missing file gracefully") {
    JsonParser parser("nonexistent_file.json");
    
    std::string content = parser.readFile();
    CHECK(content == "");
}

TEST_CASE("IO: Handle malformed JSON") {
    // Create temporary malformed JSON file
    std::ofstream file("test_malformed.json");
    file << "{invalid json}";
    file.close();
    
    JsonParser parser("test_malformed.json");
    
    int count = 0;
    Bin* bins = parser.loadBins(count);
    
    // Should handle gracefully (return null or empty)
    // Exact behavior depends on implementation
    
    if (bins) {
        delete[] bins;
    }
    
    std::remove("test_malformed.json");
}

TEST_CASE("IO: Handle empty JSON file") {
    std::ofstream file("test_empty_file.json");
    file << "";
    file.close();
    
    JsonParser parser("test_empty_file.json");
    std::string content = parser.readFile();
    
    CHECK(content == "");
    
    std::remove("test_empty_file.json");
}

/*=== INTEGRATION WITH MAPPER ===*/

TEST_CASE("IO: Parser uses mapper correctly") {
    JsonParser parser("test_minimal.json");
    
    int binCount = 0;
    Bin* bins = parser.loadBins(binCount);
    
    int facilityCount = 0;
    Facility* facilities = parser.loadFacilities(facilityCount);
    
    LocationMapper& mapper = parser.getMapper();
    
    // All locations should be mapped
    CHECK(mapper.getLocationCount() >= binCount + facilityCount);
    
    delete[] bins;
    delete[] facilities;
}

TEST_CASE("IO: Consistent node IDs across loads") {
    JsonParser parser("test_minimal.json");
    
    int binCount = 0;
    Bin* bins = parser.loadBins(binCount);
    
    int facilityCount = 0;
    Facility* facilities = parser.loadFacilities(facilityCount);
    
    Graph graph = parser.loadGraph();
    
    // Node IDs should be consistent and valid
    if (binCount > 0) {
        int binNode = bins[0].getNodeId();
        CHECK(binNode >= 0);
        CHECK(binNode < graph.getNodeCount());
    }
    
    delete[] bins;
    delete[] facilities;
}
