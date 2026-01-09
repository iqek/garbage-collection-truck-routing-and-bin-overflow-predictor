#include "doctest.h"

// Core / Algorithms
#include "core/RoutePlanner.h"
#include "core/OverflowPredictor.h"
#include "core/Simulation.h"

// Core / Data
#include "core/Bin.h"
#include "core/Facility.h"
#include "core/Facilities.h"
#include "core/Truck.h"

// Graph
#include "data_structures/Graph.h"

using namespace project;

/*OVERFLOW PREDICTOR TESTS*/

TEST_CASE("OverflowPredictor - already overflowing bin") {
    Bin bin("B1", "loc", 100, 100, 10, 0);
    OverflowPredictor predictor(2);

    CHECK(bin.isOverflowing() == true);
    CHECK(predictor.predictDaysToOverflow(bin) == -1);
    CHECK(predictor.isCritical(bin) == true);
}

TEST_CASE("OverflowPredictor - zero fill rate safety") {
    Bin bin("B2", "loc", 100, 10, 0, 0);
    OverflowPredictor predictor(3);

    int days = predictor.predictDaysToOverflow(bin);
    CHECK(days > 0);
}

TEST_CASE("OverflowPredictor - critical threshold logic") {
    Bin bin("B3", "loc", 100, 90, 5, 0);
    bin.recordFillLevel(90);
    bin.recordFillLevel(95);
    bin.recordFillLevel(100);

    OverflowPredictor predictor(2);
    CHECK(predictor.isCritical(bin) == true);
}

/*ROUTE CLASS TESTS*/

TEST_CASE("Route - deep copy correctness") {
    int bins[3] = {1, 2, 3};
    Route r1(bins, 3);
    Route r2 = r1;

    CHECK(r2.getLength() == 3);
    CHECK(r2.getBinAt(0) == 1);
    CHECK(r2.getBinAt(2) == 3);
}

TEST_CASE("Route - assignment operator self safety") {
    int bins[2] = {5, 6};
    Route r(bins, 2);

    r = r;
    CHECK(r.getLength() == 2);
    CHECK(r.getBinAt(1) == 6);
}

TEST_CASE("Route - out of bounds access") {
    Route r;
    CHECK(r.getBinAt(0) == -1);
}

/*ROUTE PLANNER & GRAPH*/

TEST_CASE("RoutePlanner - computeDistance simple graph") {
    Graph graph(3);
    graph.addEdge(0, 1, 5);
    graph.addEdge(1, 2, 7);

    RoutePlanner planner(graph);
    CHECK(planner.computeDistance(0, 2) == 12);
}

TEST_CASE("RoutePlanner - no disposal facilities") {
    Facilities facilities;
    Graph graph(1);
    RoutePlanner planner(graph);

    CHECK(planner.findNearestDisposal(0, facilities) == -1);
}

/*SIMULATION*/

TEST_CASE("Simulation - empty setup does not crash") {
    Graph graph(1);
    Facilities facilities;

    Simulation sim(graph, facilities, 1);
    CHECK_NOTHROW(sim.run());
}