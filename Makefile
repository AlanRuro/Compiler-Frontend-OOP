CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = include
TARGET = main

# List of source files (in src/)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# List of object files (in build/obj/)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Create necessary directories
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

all: $(BIN_DIR)/$(TARGET)

# Link object files to produce the executable in bin/
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp in src/ to .o in build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

test:
	$(BIN_DIR)/$(TARGET) scripts/test.py

.PHONY: all clean test
