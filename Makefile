# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -O2
LDFLAGS = -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Target executable
TARGET = $(BUILD_DIR)/graphpath
TEST_TARGET = $(BUILD_DIR)/test_graph

# Source files
SOURCES = $(SRC_DIR)/graph.c $(SRC_DIR)/bfs.c $(SRC_DIR)/dfs.c $(SRC_DIR)/dijkstra.c $(SRC_DIR)/astar.c $(SRC_DIR)/bellman_ford.c $(SRC_DIR)/main.c
LIB_SOURCES = $(SRC_DIR)/graph.c $(SRC_DIR)/bfs.c $(SRC_DIR)/dfs.c $(SRC_DIR)/dijkstra.c $(SRC_DIR)/astar.c $(SRC_DIR)/bellman_ford.c

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJECTS = $(LIB_OBJECTS) $(BUILD_DIR)/test_graph.o

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Build tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)
	@echo "Test build complete!"

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete!"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Build Qt GUI
gui:
	@echo "Building Qt GUI..."
	@if command -v qmake6 >/dev/null 2>&1; then \
		cd gui && qmake6 && make; \
	elif command -v qmake >/dev/null 2>&1; then \
		cd gui && qmake && make; \
	else \
		echo "Error: qmake or qmake6 not found. Please install Qt5 or Qt6."; \
		exit 1; \
	fi
	@echo "GUI build complete! Run with: ./build/graphpath-gui"

# Run Qt GUI
run-gui: gui
	./build/graphpath-gui

# Clean Qt build files
clean-gui:
	cd gui && make clean 2>/dev/null || true
	rm -f gui/Makefile gui/.qmake.stash
	@echo "GUI clean complete!"

# Clean all (CLI + GUI)
clean-all: clean clean-gui
	@echo "Full clean complete!"

# Show help
help:
	@echo "GraphPath Makefile"
	@echo "=================="
	@echo "CLI Targets:"
	@echo "  make          - Build the CLI project"
	@echo "  make run      - Build and run the CLI program"
	@echo "  make test     - Build and run tests"
	@echo "  make clean    - Remove CLI build files"
	@echo ""
	@echo "GUI Targets:"
	@echo "  make gui      - Build the Qt GUI"
	@echo "  make run-gui  - Build and run the GUI"
	@echo "  make clean-gui - Remove GUI build files"
	@echo ""
	@echo "Other Targets:"
	@echo "  make clean-all - Remove all build files (CLI + GUI)"
	@echo "  make help     - Show this help message"

.PHONY: all clean run test help gui run-gui clean-gui clean-all
