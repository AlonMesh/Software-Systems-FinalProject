CXX = g++
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_SRCS := $(filter-out $(TEST_DIR)/TestRunner.cpp, $(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

CXXFLAGS = -std=c++11 -Wall -Wextra -I$(SRC_DIR)

# Main program
MAIN_SRC := $(SRC_DIR)/main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o
MAIN_TARGET := $(BUILD_DIR)/main

# Test program
TEST_TARGET := $(BUILD_DIR)/test
TEST_RUNNER := $(TEST_DIR)/TestRunner.cpp

all: $(MAIN_TARGET) $(TEST_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(MAIN_TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_RUNNER) $(filter-out $(MAIN_OBJ), $^)

run: $(MAIN_TARGET)
	./$(MAIN_TARGET) $(ARGS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run test clean
