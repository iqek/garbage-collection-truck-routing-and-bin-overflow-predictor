/**
 * @file test_helpers.h
 * @brief Test helper function declarations
 */

#pragma once

#include "data_structures/Graph.h"
#include "core/Facilities.h"
#include "core/Bin.h"
#include "core/Truck.h"
#include "core/Facility.h"

using namespace project;

namespace test_fixtures {

/**
 * @brief Creates a simple test graph with 4 nodes
 * @return Pointer to Graph (caller must delete)
 */
Graph* createTestGraph();

/**
 * @brief Creates test facilities with depot, disposal, bins, and truck
 * @return Pointer to Facilities (caller must delete)
 */
Facilities* createTestFacilities();

/**
 * @brief Cleanup helper for test resources
 * @param graph Graph to delete (can be nullptr)
 * @param facilities Facilities to delete (can be nullptr)
 */
void cleanup(Graph* graph, Facilities* facilities);

} // namespace test_fixtures