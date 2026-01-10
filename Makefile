# ============================================================================
# CSE 211 - Data Structures Term Project
# C++ Project Makefile
# ============================================================================
# Group ID:     04
# Members:      İpek Çelik         20240702019
#               Kerem Akdeniz      20240702047
#               İlber Eren Tüt     20240702017
#               İrem Irmak Ünlüer  20240702030
#               Miray Duygulu      20230702097
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
FRONTEND_DIR := frontend

# Default data file
DEFAULT_DATA := $(DATA_DIR)/data.json

# Target executables
TARGET := $(BIN_DIR)/garbage_sim
TEST_TARGET := $(BIN_DIR)/test_runner

# Source files
CORE_SOURCES := $(wildcard $(SRC_DIR)/core/*.cpp)
UTILS_SOURCES := $(wildcard $(SRC_DIR)/utils/*.cpp)
DS_SOURCES := $(wildcard $(SRC_DIR)/data_structure/*.cpp)
FRONTEND_SOURCES := $(wildcard $(FRONTEND_DIR)/*.cpp)

LIB_SOURCES := $(CORE_SOURCES) $(UTILS_SOURCES) $(DS_SOURCES)
MAIN_SOURCE := $(SRC_DIR)/main.cpp

# Objects
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LIB_SOURCES))
FRONTEND_OBJECTS := $(patsubst $(FRONTEND_DIR)/%.cpp,$(OBJ_DIR)/frontend/%.o,$(FRONTEND_SOURCES))
MAIN_OBJECT := $(OBJ_DIR)/main.o

# Test sources (organized by subdirectory)
UNIT_TESTS := $(wildcard $(TEST_DIR)/unit/*.cpp)
INTEGRATION_TESTS := $(wildcard $(TEST_DIR)/integration/*.cpp)
EDGE_TESTS := $(wildcard $(TEST_DIR)/edge_cases/*.cpp)
PERF_TESTS := $(wildcard $(TEST_DIR)/performance/*.cpp)
FIXTURE_SOURCES := $(wildcard $(TEST_DIR)/fixtures/*.cpp)
TEST_MAIN_SOURCE := $(TEST_DIR)/test_main.cpp

ALL_TESTS := $(UNIT_TESTS) $(INTEGRATION_TESTS) $(EDGE_TESTS) $(PERF_TESTS)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/tests/%.o,$(ALL_TESTS))
FIXTURE_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/tests/%.o,$(FIXTURE_SOURCES))
TEST_MAIN_OBJECT := $(OBJ_DIR)/tests/test_main.o

# Include paths
INCLUDES := -I$(INC_DIR) -I$(EXT_DIR) -I$(FRONTEND_DIR)

# ============================================================================
# REQUIRED TARGETS
# ============================================================================

.PHONY: all
all: directories $(TARGET)
	@echo "✓ Build complete: $(TARGET)"

.PHONY: run
run: all
	@echo "→ Running garbage collection simulation..."
	@$(TARGET) $(DEFAULT_DATA)

.PHONY: test
test: directories $(TEST_TARGET)
	@echo "================================================================"
	@echo "PROJECT TEST SUITE"
	@echo "================================================================"
	@echo ""
	@$(TEST_TARGET)

.PHONY: deps
deps:
	@echo "→ Checking dependencies..."
	@command -v g++ >/dev/null 2>&1 || { echo "✗ g++ not found. Install: sudo apt-get install build-essential"; exit 1; }
	@echo "  ✓ g++ found"
	@command -v make >/dev/null 2>&1 || { echo "✗ make not found"; exit 1; }
	@echo "  ✓ make found"
	@ldconfig -p 2>/dev/null | grep -q libncurses || { echo "✗ ncurses not found. Install: sudo apt-get install libncurses-dev"; exit 1; }
	@echo "  ✓ ncurses found"
	@if command -v valgrind >/dev/null 2>&1; then echo "  ✓ valgrind found (optional)"; else echo "  ⚠ valgrind not found (optional)"; fi
	@if command -v doxygen >/dev/null 2>&1; then echo "  ✓ doxygen found (optional)"; else echo "  ⚠ doxygen not found (optional)"; fi
	@if command -v clang-format >/dev/null 2>&1; then echo "  ✓ clang-format found (optional)"; else echo "  ⚠ clang-format not found (optional)"; fi
	@echo "✓ All required dependencies satisfied"

.PHONY: clean
clean:
	@echo "→ Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

.PHONY: docs
docs:
	@echo "→ Generating documentation..."
	@if [ -f $(DOCS_DIR)/Doxyfile ]; then \
		doxygen $(DOCS_DIR)/Doxyfile; \
		echo "✓ Documentation generated in $(DOCS_DIR)/html/"; \
	else \
		echo "✗ Error: $(DOCS_DIR)/Doxyfile not found"; \
		exit 1; \
	fi

# ============================================================================
# BUILD RULES
# ============================================================================

.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR)/core $(OBJ_DIR)/utils $(OBJ_DIR)/data_structure $(OBJ_DIR)/frontend
	@mkdir -p $(OBJ_DIR)/tests/unit $(OBJ_DIR)/tests/integration 
	@mkdir -p $(OBJ_DIR)/tests/edge_cases $(OBJ_DIR)/tests/performance $(OBJ_DIR)/tests/fixtures
	@mkdir -p $(BIN_DIR) $(DOCS_DIR)/html

$(TARGET): $(LIB_OBJECTS) $(FRONTEND_OBJECTS) $(MAIN_OBJECT)
	@echo "→ Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "→ Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/frontend/%.o: $(FRONTEND_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "→ Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_TARGET): $(LIB_OBJECTS) $(FRONTEND_OBJECTS) $(FIXTURE_OBJECTS) $(TEST_OBJECTS) $(TEST_MAIN_OBJECT)
	@echo "→ Linking test executable..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================================
# UTILITY TARGETS
# ============================================================================

.PHONY: format
format:
	@echo "→ Formatting code with clang-format..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format -i
	@echo "✓ Formatting complete"

.PHONY: format-check
format-check:
	@echo "→ Checking code format..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format --dry-run --Werror

.PHONY: memcheck
memcheck: all
	@echo "→ Running Valgrind memory check..."
	@valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --log-file=$(BUILD_DIR)/valgrind_main.log \
	         $(TARGET) $(DEFAULT_DATA)
	@echo "✓ See $(BUILD_DIR)/valgrind_main.log"

.PHONY: memcheck-test
memcheck-test: test
	@echo "→ Running Valgrind on tests..."
	@valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --log-file=$(BUILD_DIR)/valgrind_test.log \
	         $(TEST_TARGET)
	@echo "✓ See $(BUILD_DIR)/valgrind_test.log"

.PHONY: debug
debug: CXXFLAGS += -DDEBUG -O0 -fsanitize=address -fsanitize=undefined
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined
debug: clean all

.PHONY: release
release: CXXFLAGS := -std=c++17 -O3 -DNDEBUG
release: clean all

.PHONY: submit
submit: clean
	@echo "→ Creating submission archive..."
	@GROUP_ID=$${GROUP_ID:-"GroupXX"}; \
	ARCHIVE_NAME=$${GROUP_ID}_garbage_collection.zip; \
	zip -9 -r $$ARCHIVE_NAME \
		$(SRC_DIR) $(INC_DIR) $(TEST_DIR) $(DATA_DIR) \
		$(DOCS_DIR)/Doxyfile $(EXT_DIR) frontend \
		Makefile .clang-format README.md .gitignore \
		-x "*.o" -x ".git/*" -x "$(BUILD_DIR)/*" -x ".vscode/*" \
		-x "*.log" -x "$(DOCS_DIR)/html/*"; \
	echo "✓ Created: $$ARCHIVE_NAME"

.PHONY: stats
stats:
	@echo "=== Project Statistics ==="
	@echo "Source files:  $$(find $(SRC_DIR) -name '*.cpp' 2>/dev/null | wc -l)"
	@echo "Header files:  $$(find $(INC_DIR) -name '*.h' -o -name '*.hpp' 2>/dev/null | wc -l)"
	@echo "Test files:    $$(find $(TEST_DIR) -name '*.cpp' 2>/dev/null | wc -l)"
	@echo "Total LOC:     $$(find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 | awk '{print $$1}')"

.PHONY: help
help:
	@echo "Garbage Collection Optimization - Build System"
	@echo "=============================================="
	@echo ""
	@echo "Required Targets:"
	@echo "  make          Build the project"
	@echo "  make run      Build and run the main program"
	@echo "  make test     Build and run all tests"
	@echo "  make deps     Check dependencies"
	@echo "  make clean    Remove build artifacts"
	@echo "  make docs     Generate documentation"
	@echo ""
	@echo "Additional Targets:"
	@echo "  make format       Format code with clang-format"
	@echo "  make memcheck     Run Valgrind on main"
	@echo "  make memcheck-test Run Valgrind on tests"
	@echo "  make debug        Debug build with sanitizers"
	@echo "  make release      Optimized release build"
	@echo "  make submit       Create submission archive"
	@echo "  make stats        Show project statistics"

.DEFAULT_GOAL := help