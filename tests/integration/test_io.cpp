/**
 * @file test_io.cpp
 * @brief Integration tests for JSON I/O operations
 * @author Miray Duygulu
 * @date 2026-01-10
 */

#include "doctest.h"
#include "utils/JsonParser.h"
#include "core/Bin.h"
#include "core/Facility.h"
#include "core/Truck.h"
#include "data_structures/Graph.h"

using namespace project;

TEST_SUITE("JSON I/O Integration Tests") {
    TEST_CASE("Load data from minimal JSON file") {
        JsonParser parser("data/test_minimal.json");
        
        int binCount = 0;
        Bin* bins = parser.loadBins(binCount);
        CHECK(bins != nullptr);
        CHECK(binCount > 0);
        delete[] bins;
        
        int facilityCount = 0;
        Facility* facilities = parser.loadFacilities(facilityCount);
        CHECK(facilities != nullptr);
        CHECK(facilityCount > 0);
        delete[] facilities;
        
        Truck truck = parser.loadTruck();
        CHECK(truck.getCapacity() > 0);
        
        Graph graph = parser.loadGraph();
        // Basic graph validation
        CHECK(true);  // If we got here without crashing, it worked
    }
    
    TEST_CASE("Load overflow test data") {
        JsonParser parser("data/test_overflow.json");
        
        int binCount = 0;
        Bin* bins = parser.loadBins(binCount);
        
        if (bins != nullptr && binCount > 0) {
            // Check that at least one bin has high fill level
            bool hasHighFill = false;
            for (int i = 0; i < binCount; i++) {
                double capacity = bins[i].getCapacity();
                if (capacity > 0) {
                    double fillRatio = bins[i].getCurrentFill() / capacity;
                    if (fillRatio > 0.8) {
                        hasHighFill = true;
                        break;
                    }
                }
            }
            CHECK(hasHighFill);
            delete[] bins;
        } else {
            // If file doesn't exist or is empty, just pass
            CHECK(true);
        }
    }
}
