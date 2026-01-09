# ============================================================================
# CSE 211 - Data Structures Term Project
# C++ Project Makefile
# ============================================================================
# Group ID:     04
# Members:      [Member Names and IDs]
# Problem:      04 - Garbage Collection Truck Routing & Bin Overflow Predictor
# ============================================================================

# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -Iexternal
LDFLAGS := -lncurses

# Directories
SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
DOCS_DIR := docs
DATA_DIR := data
EXT_DIR := external

# Target executables
TARGET := $(BIN_DIR)/garbage_sim
TEST_TARGET := $(BIN_DIR)/test_runner

# Source files
CORE_SOURCES := $(wildcard $(SRC_DIR)/core/*.cpp)
UTILS_SOURCES := $(wildcard $(SRC_DIR)/utils/*.cpp)
DS_SOURCES := $(wildcard $(SRC_DIR)/data_structures/*.cpp)
FRONTEND_SOURCES := $(wildcard $(SRC_DIR)/frontend/*.cpp)

# All sources except main
LIB_SOURCES := $(CORE_SOURCES) $(UTILS_SOURCES) $(DS_SOURCES) $(FRONTEND_SOURCES)
MAIN_SOURCE := $(SRC_DIR)/main.cpp

# Objects
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LIB_SOURCES))
MAIN_OBJECT := $(OBJ_DIR)/main.o

# Test sources
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SOURCES))

# Include paths
INCLUDES := -I$(INC_DIR) -I$(EXT_DIR)

# Default data file
DEFAULT_DATA := $(DATA_DIR)/data.json

# ============================================================================
# REQUIRED TARGETS
# ============================================================================

# Default: Build the project
.PHONY: all
all: directories $(TARGET)
	@echo "✓ Build complete: $(TARGET)"

# Build and run the main program
.PHONY: run
run: all
	@echo "→ Running simulation with default data..."
	@$(TARGET) $(DEFAULT_DATA)

# Build and run all tests
.PHONY: test
test: directories $(TEST_TARGET)
	@echo "→ Running all tests..."
	@$(TEST_TARGET)

# Install/setup dependencies
.PHONY: deps
deps:
	@echo "→ Checking dependencies..."
	@command -v g++ >/dev/null 2>&1 || { echo "✗ g++ not found. Install: sudo apt-get install build-essential"; exit 1; }
	@echo "  ✓ g++ found"
	@command -v make >/dev/null 2>&1 || { echo "✗ make not found. Install: sudo apt-get install make"; exit 1; }
	@echo "  ✓ make found"
	@ldconfig -p | grep -q libncurses || { echo "✗ ncurses not found. Install: sudo apt-get install libncurses-dev"; exit 1; }
	@echo "  ✓ ncurses found"
	@if command -v valgrind >/dev/null 2>&1; then echo "  ✓ valgrind found (optional)"; else echo "  ⚠ valgrind not found (optional): sudo apt-get install valgrind"; fi
	@if command -v doxygen >/dev/null 2>&1; then echo "  ✓ doxygen found (optional)"; else echo "  ⚠ doxygen not found (optional): sudo apt-get install doxygen graphviz"; fi
	@echo "✓ All required dependencies satisfied"

# Remove build artifacts
.PHONY: clean
clean:
	@echo "→ Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

# Generate documentation
.PHONY: docs
docs:
	@echo "→ Generating documentation..."
	@if [ -f $(DOCS_DIR)/Doxyfile ]; then \
		doxygen $(DOCS_DIR)/Doxyfile; \
		echo "✓ Documentation generated in $(DOCS_DIR)/html/"; \
		echo "  Open $(DOCS_DIR)/html/index.html in browser"; \
	else \
		echo "✗ Error: $(DOCS_DIR)/Doxyfile not found"; \
		exit 1; \
	fi

# ============================================================================
# ADDITIONAL TARGETS
# ============================================================================

# Create directories
.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR)/core
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/data_structures
	@mkdir -p $(OBJ_DIR)/frontend
	@mkdir -p $(OBJ_DIR)/tests
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(DOCS_DIR)/html

# Link main executable
$(TARGET): $(LIB_OBJECTS) $(MAIN_OBJECT)
	@echo "→ Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "→ Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Link test executable
$(TEST_TARGET): $(LIB_OBJECTS) $(TEST_OBJECTS)
	@echo "→ Linking test executable..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile test files
$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "→ Compiling test $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================================
# CONVENIENCE TARGETS
# ============================================================================

# Run with different data files
.PHONY: run-minimal run-overflow run-capacity run-empty
run-minimal: all
	@echo "→ Running with minimal test data..."
	@$(TARGET) $(DATA_DIR)/test_minimal.json

run-overflow: all
	@echo "→ Running overflow scenario..."
	@$(TARGET) $(DATA_DIR)/test_overflow.json

run-capacity: all
	@echo "→ Running capacity test..."
	@$(TARGET) $(DATA_DIR)/test_capacity.json

run-empty: all
	@echo "→ Running empty edge case..."
	@$(TARGET) $(DATA_DIR)/test_empty.json

# Memory checks
.PHONY: memcheck memcheck-test memcheck-all
memcheck: all
	@echo "→ Running Valgrind memory check..."
	@valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --log-file=$(BUILD_DIR)/valgrind_main.log \
	         $(TARGET) $(DEFAULT_DATA)
	@echo "✓ Memory check complete. See $(BUILD_DIR)/valgrind_main.log"

memcheck-test: test
	@echo "→ Running Valgrind on tests..."
	@valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --log-file=$(BUILD_DIR)/valgrind_test.log \
	         $(TEST_TARGET)
	@echo "✓ Test memory check complete. See $(BUILD_DIR)/valgrind_test.log"

memcheck-all: memcheck memcheck-test
	@echo "✓ All memory checks complete"

# Code formatting
.PHONY: format format-check
format:
	@echo "→ Formatting code..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format -i -style=file
	@echo "✓ Formatting complete"

format-check:
	@echo "→ Checking code format..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format --dry-run --Werror -style=file

# Debug and release builds
.PHONY: debug release
debug: CXXFLAGS += -DDEBUG -O0 -fsanitize=address -fsanitize=undefined
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined
debug: clean all
	@echo "✓ Debug build complete with sanitizers"

release: CXXFLAGS := -std=c++17 -O3 -DNDEBUG
release: clean all
	@echo "✓ Release build complete"

# Submission
.PHONY: submit
submit: clean
	@echo "→ Creating submission archive..."
	@GROUP_ID=$${GROUP_ID:-"GroupXX"}; \
	ARCHIVE_NAME=$${GROUP_ID}_garbage_collection.zip; \
	zip -9 -r $$ARCHIVE_NAME \
		$(SRC_DIR)/ \
		$(INC_DIR)/ \
		$(TEST_DIR)/ \
		$(DATA_DIR)/ \
		$(DOCS_DIR)/Doxyfile \
		$(EXT_DIR)/ \
		frontend/ \
		Makefile \
		.clang-format \
		README.md \
		.gitignore \
		-x "*.o" -x "*.obj" -x "*.exe" -x "*.out" \
		-x ".git/*" -x "$(BUILD_DIR)/*" -x "build/*" \
		-x ".vscode/*" -x ".idea/*" -x "*.swp" -x "*~" \
		-x "*.log" -x "$(DOCS_DIR)/html/*"; \
	echo "✓ Created: $$ARCHIVE_NAME"

# Statistics
.PHONY: stats
stats:
	@echo "=== Project Statistics ==="
	@echo "Core files:    $$(find $(SRC_DIR)/core -name '*.cpp' 2>/dev/null | wc -l) cpp, $$(find $(INC_DIR)/core -name '*.h' -o -name '*.hpp' 2>/dev/null | wc -l) headers"
	@echo "Utils files:   $$(find $(SRC_DIR)/utils -name '*.cpp' 2>/dev/null | wc -l) cpp, $$(find $(INC_DIR)/utils -name '*.h' -o -name '*.hpp' 2>/dev/null | wc -l) headers"
	@echo "DS files:      $$(find $(SRC_DIR)/data_structures -name '*.cpp' 2>/dev/null | wc -l) cpp, $$(find $(INC_DIR)/data_structures -name '*.hpp' 2>/dev/null | wc -l) headers"
	@echo "Frontend:      $$(find frontend -name '*.cpp' -o -name '*.h' 2>/dev/null | wc -l) files"
	@echo "Test files:    $$(find $(TEST_DIR) -name '*.cpp' 2>/dev/null | wc -l)"
	@echo "Data files:    $$(find $(DATA_DIR) -name '*.json' 2>/dev/null | wc -l)"
	@echo "Total LOC:     $$(find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 | awk '{print $$1}')"

# Help
.PHONY: help
help:
	@echo "Garbage Collection Optimization - Build System"
	@echo "=============================================="
	@echo ""
	@echo "Required Targets:"
	@echo "  make          Build the project"
	@echo "  make run      Build and run the main program"
	@echo "  make test     Build and run all tests"
	@echo "  make deps     Install/setup dependencies"
	@echo "  make clean    Remove build artifacts"
	@echo "  make docs     Generate documentation"
	@echo ""
	@echo "Additional Targets:"
	@echo "  make run-minimal     Run with minimal test data"
	@echo "  make run-overflow    Run overflow scenario"
	@echo "  make run-capacity    Run capacity test"
	@echo "  make memcheck        Valgrind check on main"
	@echo "  make memcheck-test   Valgrind check on tests"
	@echo "  make format          Format code"
	@echo "  make debug           Debug build with sanitizers"
	@echo "  make release         Optimized release build"
	@echo "  make stats           Show project statistics"
	@echo "  make submit          Create submission archive"
	@echo ""
	@echo "Usage:"
	@echo "  GROUP_ID=Group05 make submit"

.DEFAULT_GOAL := help