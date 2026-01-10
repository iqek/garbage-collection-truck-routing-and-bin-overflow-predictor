# Garbage Collection Optimization System

**CSE 211 - Data Structures Term Project**

A smart waste management system using predictive analytics and graph-based routing algorithms to optimize garbage collection schedules.

---

## Group Information

**Group Number:** [Your Group Number]

**Members:**
    İrem Irmak Ünlüer (20240702030)  
		İlber Eren Tüt (20240702017)  
		Miray Duygulu (20230702097)  
		Kerem Akdeniz (20240702047)  
		İpek Çelik (20240702019)

**Problem:** Smart Waste Management with Predictive Overflow Detection

---

## Project Description

This project implements an intelligent garbage collection routing system that optimizes truck routes while preventing bin overflows. The system uses:

- **Graph-based routing** with Dijkstra's algorithm for optimal path planning
- **Predictive analytics** to forecast bin overflow times using historical data
- **Multi-objective optimization** balancing travel distance and overflow risk
- **Interactive TUI** (Text User Interface) for real-time simulation monitoring

The system processes city maps as weighted graphs, tracks bin fill levels over time, and dynamically routes collection trucks to minimize both overflow events and total travel distance.

---

## Build Instructions

### Install Dependencies

```bash
make deps
```

This checks for:
- C++17 compiler (g++ or clang++)
- Make build system
- ncurses library (for TUI)
- Valgrind (optional, for memory checking)
- Doxygen + Graphviz (optional, for documentation)
- clang-format (optional, for code formatting)

### Build the Project

```bash
make
```

Compiles all source files and creates `build/bin/garbage_sim`.

### Run the Program

```bash
make run
```

Runs the simulation with `data/data.json` (the default dataset).

**Alternative:** Run directly with custom data:
```bash
./build/bin/garbage_sim data/test_overflow.json
./build/bin/garbage_sim data/test_capacity.json --no-ui
./build/bin/garbage_sim data/test_minimal.json --days 14
```

### Run Tests

```bash
make test
```

Builds and executes all tests with formatted output showing:
- Unit tests (data structures, algorithms)
- Integration tests (workflows)
- Edge cases (boundary conditions)
- Performance tests (stress tests)

### Clean Build Artifacts

```bash
make clean
```

Removes all compiled objects and executables from `build/`.

### Generate Documentation

```bash
make docs
```

Generates HTML documentation using Doxygen. Open `docs/html/index.html` in a browser.

---

## Usage Examples

### Basic Usage

```bash
# Run with default data (data.json - 5 bins, realistic scenario)
make run

# Run with specific test scenarios
./build/bin/garbage_sim data/test_minimal.json      # Simple: 1 bin
./build/bin/garbage_sim data/test_overflow.json     # Stress: High fill rates
./build/bin/garbage_sim data/test_capacity.json     # Challenge: Small truck, large bins
./build/bin/garbage_sim data/test_empty.json        # Edge case: No bins

# Run without UI (text output only)
./build/bin/garbage_sim data/data.json --no-ui
./build/bin/garbage_sim data/test_overflow.json --no-ui

# Set custom simulation duration
./build/bin/garbage_sim data/data.json --days 14
./build/bin/garbage_sim data/test_minimal.json --days 30

# Show help
./build/bin/garbage_sim --help
```

### Available Test Scenarios

| Data File | Description | Bins | Challenge |
|-----------|-------------|------|-----------|
| `data.json` | Default realistic scenario | 5 | Balanced routing |
| `test_minimal.json` | Minimal test case | 1 | Basic functionality |
| `test_overflow.json` | High fill rates | 3 | Overflow prevention |
| `test_capacity.json` | Large bins, small truck | 3 | Capacity management |
| `test_empty.json` | No bins | 0 | Edge case handling |

### Testing

```bash
# Run all tests
make test

# Run with Valgrind memory check
make memcheck-test
```

### Code Formatting

```bash
# Format all source files
clang-format -i src/*.cpp include/*.h

# Or use make target
make format
```

### Documentation

```bash
# Generate docs
doxygen docs/Doxyfile

# Or use make target
make docs
```

---

## Project Structure

```
.
├── data/                          # JSON input files
│   ├── data.json                 # Main dataset (5 bins, 1 truck)
│   ├── test_minimal.json         # Minimal test (1 bin)
│   ├── test_overflow.json        # High fill rates scenario
│   ├── test_capacity.json        # Capacity stress test
│   └── test_empty.json           # Empty bins edge case
│
├── src/                          # Implementation files
│   ├── main.cpp                  # Program entry point
│   ├── core/                     # Core business logic
│   │   ├── Bin.cpp
│   │   ├── Truck.cpp
│   │   ├── Facility.cpp
│   │   ├── Facilities.cpp
│   │   ├── Route.cpp
│   │   ├── RoutePlanner.cpp
│   │   ├── OverflowPredictor.cpp
│   │   └── Simulation.cpp
│   ├── data_structures/          # Data structure implementations
│   │   ├── Graph.cpp
│   │   ├── HashTable.cpp
│   ├── utils/                    # Utilities
│   │   ├── JsonParser.cpp
│       └── LocationMapper.cpp
│
├── frontend/                 # UI components
│   ├── UIManager.cpp
│   ├── UIManager.h
│   ├── WindowManager.cpp
│   ├── WindowManager.h
│   ├── ColorScheme.h
│   └── ColorScheme.cpp
│
├── include/                      # Header files
│   ├── core/                     # Core headers
│   ├── data_structures/          # Data structure headers (templates)
│   │   ├── Graph.h
│   │   ├── HashTable.h
│   │   ├── LinkedList.hpp
│   │   └── PriorityQueue.hpp
│   └── utils/                    # Utility headers
│
├── tests/                        # Test suite
│   ├── test_main.cpp             # Custom test runner
│   ├── unit/                     # Unit tests
│   │   ├── test_data_structures.cpp
│   │   └── test_algorithms.cpp
│   ├── integration/              # Integration tests
│   │   ├── test_io.cpp
│   │   └── test_workflow.cpp
│   ├── edge_cases/               # Boundary tests
│   │   └── test_boundaries.cpp
│   ├── performance/              # Stress tests
│   │   ├── test_memory.cpp
│   │   └── test_stress.cpp
│   └── fixtures/                 # Test helpers
│       ├── test_helpers.h
│       └── test_helpers.cpp
│
├── docs/                         # Documentation
│   ├── Doxyfile                  # Doxygen configuration
│   └── uml/                     # Generated docs
│
├── external/                     # External dependencies
│   ├── nlohmann/
│   └── doctest.h                 # Testing framework
│
├── build/                        # Build output (created by make)
│   ├── obj/                      # Object files
│   └── bin/                      # Executables
│
├── Makefile                      # Build configuration
├── .clang-format                 # Code formatting rules
├── .gitignore                    # Git ignore patterns
└── README.md                     # This file
```

---

## Data Structures Used

### 1. **Graph (Pointer-Based Adjacency Lists)**
- **Files:** `Graph.h`, `Graph.cpp`
- **Why:** Efficiently represents sparse city road networks where not all locations are directly connected
- **Operations:**
  - Add edge: O(1)
  - Get adjacency list: O(1)
  - Traversal: O(V + E)
- **Usage:** Foundation for Dijkstra's shortest path algorithm in route planning

### 2. **HashTable (Chaining with DJB2 Hash)**
- **Files:** `HashTable.h`, `HashTable.cpp`
- **Why:** Fast O(1) average lookup for mapping string location IDs to integer graph node indices
- **Operations:**
  - Insert/Search: O(1) average
  - Automatic resizing at 0.7 load factor
- **Usage:** LocationMapper converts JSON string IDs like "Depot" and "B1" to graph node numbers

### 3. **LinkedList (Template-Based Singly Linked)**
- **Files:** `LinkedList.hpp`
- **Why:** Dynamic size, no need for random access, efficient for adjacency lists
- **Operations:**
  - Push/Pop front: O(1)
  - Iteration: O(n)
- **Usage:** 
  - Stores edges in graph adjacency lists
  - Generic container for dynamic collections

### 4. **PriorityQueue (Binary Min-Heap)**
- **Files:** `PriorityQueue.hpp`
- **Why:** Efficiently retrieves minimum-priority element needed for greedy algorithms
- **Operations:**
  - Insert: O(log n)
  - Extract-min: O(log n)
  - Get-min: O(1)
- **Usage:**
  - Dijkstra's algorithm: Select nearest unvisited node
  - Route planning: Prioritize bins by urgency score

### Why These Structures?

1. **Graph with Adjacency Lists** - Sparse graphs (cities) waste memory with adjacency matrices. Lists only store existing edges.

2. **HashTable** - JSON parsing requires many string-to-int lookups. Hash tables provide O(1) average time vs. O(log n) for trees.

3. **LinkedList** - Edge counts per node are unknown beforehand. Linked lists grow dynamically without reallocation.

4. **PriorityQueue** - Dijkstra's algorithm repeatedly needs "find minimum distance node", which heaps do in O(log n) vs. O(n) for arrays.

---

## Algorithms

### Dijkstra's Shortest Path
- **Location:** `RoutePlanner::computeDistance()`
- **Complexity:** O((V + E) log V) with binary heap
- **Purpose:** Find optimal routes between any two city locations
- **Why:** Guaranteed to find shortest path in weighted graphs with non-negative edges

### Overflow Prediction
- **Location:** `OverflowPredictor::predictDaysToOverflow()`
- **Complexity:** O(1)
- **Purpose:** Forecast when bins will overflow using historical fill data
- **Formula:** `days = (capacity - currentFill) / averageFillRate`

### Multi-Objective Route Planning
- **Location:** `RoutePlanner::planRoute()`
- **Complexity:** O(B × V log V) where B = bins, V = nodes
- **Purpose:** Select bins balancing overflow urgency and travel distance
- **Method:** Greedy selection with priority scoring: `score = overflowRisk × 1000 + distance`

---

## Known Limitations

### Design Choices (Intentional Trade-offs)

1. **Deterministic Fill Rates** - Bins fill at constant daily rates
   - **Why:** Enables predictable overflow forecasting and testability
   - **Real-world:** Fill rates vary with weather, events, seasons
   - **Could add:** Stochastic fill model with weather parameters

2. **Greedy Routing** - Uses greedy bin selection rather than optimal TSP solution
   - **Why:** Trade-off between speed O(B × V log V) vs. TSP's NP-hard complexity
   - **Real-world:** Optimal routes exist but require exponential time
   - **Benefit:** Scales to hundreds of bins; TSP becomes impractical beyond ~20 bins

3. **Instant Disposal** - Unloading time not modeled (assumes zero disposal duration)
   - **Why:** Simplifies simulation time steps
   - **Real-world:** Disposal takes 10-15 minutes
   - **Easy fix:** Add `DISPOSAL_TIME` constant in Simulation.cpp (single file change)

### Technical Limitations

4. **Single Truck Only** - System supports one truck
   - **Impact:** Cannot model fleet coordination
   - **Fix complexity:** Would require multi-agent scheduling algorithms

5. **Static Road Network** - Graph cannot change during simulation
   - **Impact:** No traffic, road closures, or detours
   - **Fix complexity:** Would need dynamic graph updates and rerouting

6. **No Mid-Route Replanning** - Route fixed at start of day
   - **Impact:** Cannot adapt to real-time sensor data
   - **Fix complexity:** Requires online algorithm redesign

7. **Scale Limitations** - Performance degrades with 1000+ bins
   - **Cause:** O(B × V log V) route planning per day
   - **Mitigation:** Geographic clustering or hierarchical planning

8. **JSON-Only Input** - No binary format support
   - **Impact:** Slower parsing for large datasets
   - **Fix complexity:** Add Protocol Buffers or MessagePack parser

9. **Terminal-Based UI** - Requires ncurses
   - **Impact:** No graphical map visualization
   - **Alternative:** Run with `--no-ui` for headless operation

10. **No Path Blockages** - Cannot handle impassable roads
    - **Impact:** Assumes all roads always available
    - **Fix complexity:** Requires graph edge disabling and dynamic rerouting

---

## Testing

The test suite includes:

- **Unit Tests:** Data structures and algorithms
- **Integration Tests:** End-to-end workflows with real JSON files
- **Edge Cases:** Boundary conditions (empty inputs, maximums, invalid data)
- **Performance Tests:** Stress testing with large datasets

### Run All Tests

```bash
make test
```

### Example Output

```
================================================================
PROJECT TEST SUITE
================================================================

[UNIT] Data Structures
test_constructor ............................ PASSED
test_insert ................................. PASSED
test_delete ................................. PASSED
test_search ................................. PASSED
test_traversal .............................. PASSED

[UNIT] Algorithms
test_pathfinding ............................ PASSED
test_sorting ................................ PASSED
test_optimization ........................... PASSED

[INTEGRATION] Workflow
test_load_process_save ...................... PASSED
test_full_pipeline .......................... PASSED

[EDGE CASES] Boundary Conditions
test_empty_input ............................ PASSED
test_single_element ......................... PASSED
test_maximum_values ......................... PASSED
test_invalid_input .......................... PASSED (handled gracefully)

[PERFORMANCE] Stress Tests
test_large_dataset (10000 nodes) ............ PASSED (1.23s)
test_memory_usage ........................... PASSED (peak: 45MB)

================================================================
SUMMARY: 16/16 tests passed | 0 failed | 0 skipped
TIME: 2.45 seconds
MEMORY: No leaks detected
================================================================
```

### Memory Safety

```bash
make memcheck-test
```

Runs Valgrind to verify no memory leaks. View detailed log at `build/valgrind_test.log`.

---

## Dependencies

- **C++17** (g++ 7+ or clang++ 5+)
- **Make**
- **ncurses** (for TUI)
- **nlohmann/json** (header-only, included)
- **doctest** (header-only, included)
- **Valgrind** (optional)
- **Doxygen + Graphviz** (optional)
- **clang-format** (optional)

---

## Code Formatting

All code formatted using clang-format:

```bash
clang-format -i src/*.cpp include/*.h
```

Or:

```bash
make format
```

Configuration in `.clang-format` (Google style, 4-space indents).

---

## Documentation Generation

```bash
doxygen docs/Doxyfile
```

Or:

```bash
make docs
```

View at `docs/html/index.html`.

---
