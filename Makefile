CXX = g++
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_SRCS := $(filter-out $(TEST_DIR)/TestRunner.cpp, $(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

CXXFLAGS = -std=c++11 -Wall -Wextra -I$(SRC_DIR)

# Sieve program (previously Main program)
SIEVE_SRC := $(SRC_DIR)/sieve.cpp
SIEVE_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SIEVE_SRC))
SIEVE_TARGET := $(BUILD_DIR)/sieve

# Client program
CLIENT_SRC := $(SRC_DIR)/client.cpp
CLIENT_OBJ := $(BUILD_DIR)/client.o
CLIENT_TARGET := $(BUILD_DIR)/client

# Server program
SERVER_SRC := $(SRC_DIR)/server.cpp
SERVER_OBJ := $(BUILD_DIR)/server.o
SERVER_TARGET := $(BUILD_DIR)/server

# Launcher program
LAUNCHER_SRC := $(SRC_DIR)/launcher.cpp
LAUNCHER_OBJ := $(BUILD_DIR)/launcher.o
LAUNCHER_TARGET := $(BUILD_DIR)/launcher

# Test program
TEST_TARGET := $(BUILD_DIR)/test
TEST_RUNNER := $(TEST_DIR)/TestRunner.cpp

# Updating all target to build sieve, client, server, launcher, and test
all: $(SIEVE_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET) $(LAUNCHER_TARGET) $(TEST_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SIEVE_TARGET): $(filter-out $(BUILD_DIR)/client.o $(BUILD_DIR)/server.o $(BUILD_DIR)/launcher.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(filter-out $(BUILD_DIR)/server.o $(BUILD_DIR)/sieve.o $(BUILD_DIR)/launcher.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_TARGET): $(filter-out $(BUILD_DIR)/client.o $(BUILD_DIR)/sieve.o $(BUILD_DIR)/launcher.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LAUNCHER_TARGET): $(filter-out $(BUILD_DIR)/client.o $(BUILD_DIR)/sieve.o $(BUILD_DIR)/server.o, $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

# TODO: Make the test target to build the test executable
$(TEST_TARGET): $(TEST_RUNNER) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_RUNNER) $(TEST_OBJS) $(filter-out $(BUILD_DIR)/sieve.o $(BUILD_DIR)/launcher.o $(BUILD_DIR)/client.o $(BUILD_DIR)/server.o, $(OBJS)) 

sieve: $(SIEVE_TARGET)
	./$(SIEVE_TARGET)

client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

launcher: $(LAUNCHER_TARGET)
	./$(LAUNCHER_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all sieve client server launcher test clean
