/**
 * @file main.cpp
 * @brief Entry point for the Garbage Collection Optimization System
 * @author [Your Group Members]
 * @date 2025-12-30
 */

#include <iostream>
#include <string>

#include "core/Simulation.h"
#include "core/Facilities.h"
#include "utils/JsonParser.h"
#include "UIManager.h"

using namespace project;

/**
 * @brief Prints usage information
 * @param programName The name of the program executable
 */
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <data_file.json> [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --no-ui          Run without interactive UI (text output only)\n";
    std::cout << "  --days N         Set simulation duration (default: 7)\n";
    std::cout << "  --help           Show this help message\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " data/data.json\n";
    std::cout << "  " << programName << " data/test_overflow.json --no-ui\n";
    std::cout << "  " << programName << " data/test_minimal.json --days 3\n";
    std::cout << "\nAvailable data files:\n";
    std::cout << "  data/data.json              - Main dataset\n";
    std::cout << "  data/test_minimal.json      - Minimal test case\n";
    std::cout << "  data/test_overflow.json     - Overflow scenario\n";
    std::cout << "  data/test_capacity.json     - Capacity stress test\n";
    std::cout << "  data/test_empty.json        - Empty bins edge case\n";
}

/**
 * @brief Runs simulation without UI (text output only)
 */
void runTextMode(const char* dataFile, int days) {
    std::cout << "=== Garbage Collection Optimization System ===\n";
    std::cout << "Loading data from: " << dataFile << "\n\n";

    // Parse JSON data
    JsonParser parser(dataFile);
    
    int binCount = 0, facilityCount = 0;
    Bin* bins = parser.loadBins(binCount);
    Facility* facilities = parser.loadFacilities(facilityCount);
    Truck truck = parser.loadTruck();
    Graph graph = parser.loadGraph();

    if (binCount == 0 && facilityCount == 0) {
        std::cerr << "Error: Failed to load data from " << dataFile << "\n";
        std::cerr << "Please check that the file exists and is valid JSON.\n";
        return;
    }

    // Setup facilities
    Facilities facilityMgr;
    for (int i = 0; i < binCount; i++) {
        facilityMgr.addBin(bins[i]);
    }
    for (int i = 0; i < facilityCount; i++) {
        facilityMgr.addFacility(facilities[i]);
    }
    facilityMgr.setTruck(truck);

    std::cout << "System Configuration:\n";
    std::cout << "  Bins:       " << binCount << "\n";
    std::cout << "  Facilities: " << facilityCount << "\n";
    std::cout << "  Truck:      " << truck.getId() 
              << " (capacity: " << truck.getCapacity() << ")\n";
    std::cout << "  Duration:   " << days << " days\n";
    std::cout << "\nRunning simulation...\n\n";

    // Run simulation
    Simulation sim(graph, facilityMgr, days);
    sim.run();

    // Print results
    std::cout << "\n";
    sim.printStatistics();

    // Cleanup
    delete[] bins;
    delete[] facilities;
}

/**
 * @brief Runs simulation with interactive TUI
 */
void runUIMode(const char* dataFile, int days) {
    // Parse JSON data
    JsonParser parser(dataFile);
    
    int binCount = 0, facilityCount = 0;
    Bin* bins = parser.loadBins(binCount);
    Facility* facilities = parser.loadFacilities(facilityCount);
    Truck truck = parser.loadTruck();
    Graph graph = parser.loadGraph();

    if (binCount == 0 && facilityCount == 0) {
        std::cerr << "Error: Failed to load data from " << dataFile << "\n";
        std::cerr << "Please check that the file exists and is valid JSON.\n";
        return;
    }

    // Setup facilities
    Facilities facilityMgr;
    for (int i = 0; i < binCount; i++) {
        facilityMgr.addBin(bins[i]);
    }
    for (int i = 0; i < facilityCount; i++) {
        facilityMgr.addFacility(facilities[i]);
    }
    facilityMgr.setTruck(truck);

    // Create simulation
    Simulation sim(graph, facilityMgr, days);

    // Run with UI
    UIManager ui(sim);
    ui.initialize();
    ui.run();
    ui.cleanup();

    // Print final statistics
    std::cout << "\n";
    sim.printStatistics();

    // Cleanup
    delete[] bins;
    delete[] facilities;
}

/**
 * @brief Main entry point
 */
int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    const char* dataFile = argv[1];
    bool useUI = true;
    int days = 7;  // Default simulation duration

    // Process options
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--no-ui") {
            useUI = false;
        } else if (arg == "--days") {
            if (i + 1 < argc) {
                days = std::stoi(argv[++i]);
                if (days <= 0) {
                    std::cerr << "Error: Days must be positive\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: --days requires an argument\n";
                return 1;
            }
        } else {
            std::cerr << "Warning: Unknown option '" << arg << "'\n";
        }
    }

    // Run simulation
    try {
        if (useUI) {
            runUIMode(dataFile, days);
        } else {
            runTextMode(dataFile, days);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }

    return 0;
}