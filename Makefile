# ============================================================================
# CSE 211 - Data Structures Term Project
# C++ Project Makefile
# ============================================================================
# Group ID:     [Your Group ID]
# Members:      [Member Names and IDs]
# Problem:      [Problem Number and Title]
# ============================================================================

# Compiler settings
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g
LDFLAGS :=

# Directories
SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
DOCS_DIR := docs
DATA_DIR := data
LIBS_DIR := libs
SCRIPTS_DIR := scripts

# Target executable
TARGET := $(BIN_DIR)/main
TEST_TARGET := $(BIN_DIR)/tests

# Source files
SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Test files (including subdirectories: unit/, integration/, edge_cases/, performance/)
TEST_SOURCES := $(shell find $(TEST_DIR) -name '*.cpp' 2>/dev/null)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SOURCES))

# Main object (exclude from test build)
MAIN_OBJ := $(OBJ_DIR)/main.o
LIB_OBJECTS := $(filter-out $(MAIN_OBJ),$(OBJECTS))

# ============================================================================
# Default target
# ============================================================================
.PHONY: all
all: directories $(TARGET)
	@echo "Build complete: $(TARGET)"

# ============================================================================
# Create directories
# ============================================================================
.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR)/data_structures
	@mkdir -p $(OBJ_DIR)/core
	@mkdir -p $(OBJ_DIR)/utils
	@mkdir -p $(OBJ_DIR)/frontend
	@mkdir -p $(OBJ_DIR)/tests/unit
	@mkdir -p $(OBJ_DIR)/tests/integration
	@mkdir -p $(OBJ_DIR)/tests/edge_cases
	@mkdir -p $(OBJ_DIR)/tests/performance
	@mkdir -p $(BIN_DIR)

# ============================================================================
# Link main executable
# ============================================================================
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# ============================================================================
# Compile source files
# ============================================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(LIBS_DIR) -c $< -o $@

# ============================================================================
# Run the program
# ============================================================================
.PHONY: run
run: all
	@echo "Running $(TARGET)..."
	@$(TARGET) $(DATA_DIR)/input_default.json

# ============================================================================
# Build and run tests
# ============================================================================
.PHONY: test
test: directories $(TEST_TARGET)
	@echo "Running tests..."
	@$(TEST_TARGET)

$(TEST_TARGET): $(LIB_OBJECTS) $(TEST_OBJECTS)
	@echo "Linking test executable..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling test $<..."
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -I$(LIBS_DIR) -c $< -o $@

# ============================================================================
# Memory check with Valgrind
# ============================================================================
.PHONY: memcheck
memcheck: all
	@echo "Running Valgrind memory check..."
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --verbose \
	         $(TARGET) $(DATA_DIR)/input_default.json

.PHONY: memcheck-test
memcheck-test: test
	@echo "Running Valgrind on tests..."
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         $(TEST_TARGET)

# ============================================================================
# Install dependencies
# ============================================================================
.PHONY: deps
deps:
	@echo "Installing dependencies..."
	@mkdir -p $(LIBS_DIR)
	# nlohmann/json (header-only)
	@if [ ! -f $(LIBS_DIR)/json.hpp ]; then \
		echo "Downloading nlohmann/json..."; \
		curl -sL -o $(LIBS_DIR)/json.hpp \
			https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp; \
	fi
	# Add more dependencies as needed
	@echo "Dependencies installed."

# ============================================================================
# Code formatting with clang-format
# ============================================================================
.PHONY: format
format:
	@echo "Formatting code with clang-format..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format -i -style=file
	@echo "Formatting complete."

.PHONY: format-check
format-check:
	@echo "Checking code format..."
	@find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.hpp' | \
		xargs clang-format --dry-run --Werror -style=file

# ============================================================================
# Generate documentation with Doxygen
# ============================================================================
.PHONY: docs
docs:
	@echo "Generating documentation..."
	@if [ -f $(DOCS_DIR)/Doxyfile ]; then \
		doxygen $(DOCS_DIR)/Doxyfile; \
		echo "Documentation generated in $(DOCS_DIR)/html/"; \
	else \
		echo "Error: $(DOCS_DIR)/Doxyfile not found"; \
		exit 1; \
	fi

# ============================================================================
# Debug build
# ============================================================================
.PHONY: debug
debug: CXXFLAGS += -DDEBUG -O0 -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: clean all

# ============================================================================
# Release build
# ============================================================================
.PHONY: release
release: CXXFLAGS := -std=c++17 -O2 -DNDEBUG
release: clean all

# ============================================================================
# Clean build artifacts
# ============================================================================
.PHONY: clean
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

.PHONY: distclean
distclean: clean
	@echo "Removing all generated files..."
	@rm -rf $(DOCS_DIR)/html $(DOCS_DIR)/latex
	@echo "Distclean complete."

# ============================================================================
# Create submission archive (SOURCE CODE ONLY - NO BUILDS)
# ============================================================================
.PHONY: submit
submit: clean
	@echo "Creating compressed submission archive (source code only)..."
	@GROUP_ID=$${GROUP_ID:-"GroupXX"}; \
	PROB_NUM=$${PROB_NUM:-"XX"}; \
	ARCHIVE_NAME=$${GROUP_ID}_$${PROB_NUM}.zip; \
	zip -9 -r $$ARCHIVE_NAME \
		$(SRC_DIR) $(INC_DIR) $(TEST_DIR) $(DATA_DIR) $(DOCS_DIR) $(SCRIPTS_DIR) \
		frontend/ Makefile CMakeLists.txt .clang-format README.md \
		-x "*.o" \
		-x "*.obj" \
		-x "*.exe" \
		-x "*.out" \
		-x "*.a" \
		-x "*.so" \
		-x "*.dll" \
		-x "*.dylib" \
		-x ".git/*" \
		-x ".gitignore" \
		-x "$(BUILD_DIR)/*" \
		-x "build/*" \
		-x "cmake-build-*/*" \
		-x "*.dSYM/*" \
		-x ".vscode/*" \
		-x ".idea/*" \
		-x "*.swp" \
		-x "*~" \
		-x "*.log" \
		-x "__pycache__/*" \
		-x "*.pyc" \
		-x "node_modules/*" \
		-x "libs/*.a" \
		-x "libs/*.so"; \
	echo ""; \
	echo "Created: $$ARCHIVE_NAME"; \
	echo "Size: $$(du -h $$ARCHIVE_NAME | cut -f1)"; \
	echo ""; \
	echo "Contents (source code only - no builds):"; \
	unzip -l $$ARCHIVE_NAME | tail -1

# ============================================================================
# Show statistics
# ============================================================================
.PHONY: stats
stats:
	@echo "=== Project Statistics ==="
	@echo "Source files:  $$(find $(SRC_DIR) -name '*.cpp' | wc -l)"
	@echo "Header files:  $$(find $(INC_DIR) -name '*.h' -o -name '*.hpp' | wc -l)"
	@echo "Test files:    $$(find $(TEST_DIR) -name '*.cpp' 2>/dev/null | wc -l)"
	@echo "Data files:    $$(find $(DATA_DIR) -type f 2>/dev/null | wc -l)"
	@echo "Script files:  $$(find $(SCRIPTS_DIR) -type f 2>/dev/null | wc -l)"
	@echo "Total lines:   $$(find $(SRC_DIR) $(INC_DIR) -name '*.cpp' -o -name '*.h' | xargs wc -l | tail -1)"

# ============================================================================
# Help
# ============================================================================
.PHONY: help
help:
	@echo "CSE 211 - C++ Project Build System"
	@echo "==================================="
	@echo ""
	@echo "Build Targets:"
	@echo "  make           Build main executable"
	@echo "  make run       Build and run with default input"
	@echo "  make test      Build and run tests"
	@echo "  make debug     Build with debug flags and sanitizers"
	@echo "  make release   Build optimized release version"
	@echo ""
	@echo "Quality Targets:"
	@echo "  make memcheck  Run Valgrind memory check"
	@echo "  make format    Format code with clang-format"
	@echo "  make docs      Generate Doxygen documentation"
	@echo ""
	@echo "Setup Targets:"
	@echo "  make deps      Install/download dependencies"
	@echo ""
	@echo "Utility Targets:"
	@echo "  make clean     Remove build artifacts"
	@echo "  make distclean Remove all generated files"
	@echo "  make submit    Create submission archive"
	@echo "  make stats     Show project statistics"
	@echo "  make help      Show this help"
	@echo ""
	@echo "Submission:"
	@echo "  GROUP_ID=Group05 PROB_NUM=01 make submit"
