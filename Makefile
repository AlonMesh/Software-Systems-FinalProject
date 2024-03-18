CXX = g++
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_SRCS := $(filter-out $(TEST_DIR)/TestRunner.cpp, $(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Exclude main, client, and server objects from OBJS for specific targets
MAIN_EXCLUDES := $(BUILD_DIR)/main.o $(BUILD_DIR)/client.o $(BUILD_DIR)/server.o
COMMON_OBJS := $(filter-out $(MAIN_EXCLUDES), $(OBJS))

CXXFLAGS = -std=c++11 -Wall -Wextra -I$(SRC_DIR)

# Main program
MAIN_SRC := $(SRC_DIR)/main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o
MAIN_TARGET := $(BUILD_DIR)/main

# Client program
CLIENT_SRC := $(SRC_DIR)/client.cpp
CLIENT_OBJ := $(BUILD_DIR)/client.o
CLIENT_TARGET := $(BUILD_DIR)/client

# Server program
SERVER_SRC := $(SRC_DIR)/server.cpp
SERVER_OBJ := $(BUILD_DIR)/server.o
SERVER_TARGET := $(BUILD_DIR)/server

# Test program
TEST_TARGET := $(BUILD_DIR)/test
TEST_RUNNER := $(TEST_DIR)/TestRunner.cpp

all: $(MAIN_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET) $(TEST_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(MAIN_TARGET): $(COMMON_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJ) $(filter-out $(MAIN_OBJ), $(COMMON_OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJ) $(filter-out $(MAIN_OBJ), $(COMMON_OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(COMMON_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_RUNNER) $(filter-out $(MAIN_OBJ), $^)

run: $(MAIN_TARGET)
	./$(MAIN_TARGET) $(ARGS)

client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run client server test clean
