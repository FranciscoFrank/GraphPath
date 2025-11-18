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
SOURCES = $(SRC_DIR)/graph.c $(SRC_DIR)/bfs.c $(SRC_DIR)/dfs.c $(SRC_DIR)/dijkstra.c $(SRC_DIR)/main.c
LIB_SOURCES = $(SRC_DIR)/graph.c $(SRC_DIR)/bfs.c $(SRC_DIR)/dfs.c $(SRC_DIR)/dijkstra.c

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

# Show help
help:
	@echo "GraphPath Makefile"
	@echo "=================="
	@echo "Targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run the program"
	@echo "  make test     - Build and run tests"
	@echo "  make clean    - Remove build files"
	@echo "  make help     - Show this help message"

.PHONY: all clean run test help
