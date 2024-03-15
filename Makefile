# Makefile for compiling C++ files, running main and test, and cleaning

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall

# Directories
SRC_DIR := src
OBJ_DIR := obj
TEST_DIR := tests
BIN_DIR := bin

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Test source file
TEST_SRC := $(TEST_DIR)/test.cpp

# Executables
MAIN := $(BIN_DIR)/main
TEST := $(BIN_DIR)/test

# Targets
all: $(MAIN) $(TEST)

$(MAIN): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST): $(OBJ_FILES) $(TEST_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run-main: $(MAIN)
	./$(MAIN) $(ARGS)
	
run-test: $(TEST)
	./$(TEST) 10

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all run-main run-test clean
