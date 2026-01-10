/**
 * @file test_workflow.cpp
 * @brief Integration tests for complete workflows
 *@author Miray Duygulu
 *@date 2026-01-10
 */

#include "doctest.h"
#include "utils/JsonParser.h"
#include "core/Simulation.h"

using namespace project;

TEST_CASE("[INTEGRATION] test_load_process_save") {
    SUBCASE("Load from JSON") {
        JsonParser parser("data/test_minimal.json");
        
        int binCount = 0;
        Bin* bins = parser.loadBins(binCount);
        
        REQUIRE(bins != nullptr);
        CHECK(binCount == 1);
        CHECK(bins[0].getId() == "B1");
        
        delete[] bins;
    }
    
    SUBCASE("Process data") {
        JsonParser parser("data/test_minimal.json");
        
        int binCount, facilityCount;
        Bin* bins = parser.loadBins(binCount);
        Facility* facilities = parser.loadFacilities(facilityCount);
        Truck truck = parser.loadTruck();
        Graph graph = parser.loadGraph();
        
        Facilities facilityMgr;
        for (int i = 0; i < binCount; i++) {
            facilityMgr.addBin(bins[i]);
        }
        for (int i = 0; i < facilityCount; i++) {
            facilityMgr.addFacility(facilities[i]);
        }
        facilityMgr.setTruck(truck);
        
        CHECK(facilityMgr.getBinCount() == binCount);
        CHECK(facilityMgr.getFacilityCount() == facilityCount);
        
        delete[] bins;
        delete[] facilities;
    }
}

TEST_CASE("[INTEGRATION] test_full_pipeline") {
    SUBCASE("End-to-end simulation") {
        JsonParser parser("data/test_minimal.json");
        
        int binCount, facilityCount;
        Bin* bins = parser.loadBins(binCount);
        Facility* facilities = parser.loadFacilities(facilityCount);
        Truck truck = parser.loadTruck();
        Graph graph = parser.loadGraph();
        
        Facilities facilityMgr;
        for (int i = 0; i < binCount; i++) {
            facilityMgr.addBin(bins[i]);
        }
        for (int i = 0; i < facilityCount; i++) {
            facilityMgr.addFacility(facilities[i]);
        }
        facilityMgr.setTruck(truck);
        
        Simulation sim(graph, facilityMgr, 3);
        sim.run();
        
        CHECK(sim.isFinished() == true);
        CHECK(sim.getTime() == 3);
        CHECK(sim.getTotalDistance() >= 0);
        
        delete[] bins;
        delete[] facilities;
    }
    
    SUBCASE("Overflow scenario handling") {
        JsonParser parser("data/test_overflow.json");
        
        int binCount, facilityCount;
        Bin* bins = parser.loadBins(binCount);
        Facility* facilities = parser.loadFacilities(facilityCount);
        Truck truck = parser.loadTruck();
        Graph graph = parser.loadGraph();
        
        Facilities facilityMgr;
        for (int i = 0; i < binCount; i++) {
            facilityMgr.addBin(bins[i]);
        }
        for (int i = 0; i < facilityCount; i++) {
            facilityMgr.addFacility(facilities[i]);
        }
        facilityMgr.setTruck(truck);
        
        Simulation sim(graph, facilityMgr, 5);
        
        CHECK_NOTHROW(sim.run());
        CHECK(sim.isFinished() == true);
        
        delete[] bins;
        delete[] facilities;
    }
}
