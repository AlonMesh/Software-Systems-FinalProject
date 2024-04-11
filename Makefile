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

# Add the new source files to your definitions
PIPELINE_SRCS := $(SRC_DIR)/Pipeline.cpp $(SRC_DIR)/TSQueue.cpp $(SRC_DIR)/ActiveObject.cpp
PIPELINE_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(PIPELINE_SRCS))
PIPELINE_TARGET := $(BUILD_DIR)/pipeline

# Test program
TEST_TARGET := $(BUILD_DIR)/test
TEST_RUNNER := $(TEST_DIR)/TestRunner.cpp

# Updating all target to build sieve, client, server, launcher, and test
all: $(SIEVE_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET) $(LAUNCHER_TARGET) $(TEST_TARGET) $(PIPELINE_OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(SIEVE_TARGET): $(SIEVE_OBJ) $(filter-out $(CLIENT_OBJ) $(SERVER_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJ) $(filter-out $(SIEVE_OBJ) $(SERVER_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJ) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LAUNCHER_TARGET): $(LAUNCHER_OBJ) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(PIPELINE_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PIPELINE_TARGET): $(PIPELINE_OBJS) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(LAUNCHER_OBJ), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

# TODO: Make the test target to build the test executable
$(TEST_TARGET): $(TEST_RUNNER) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(filter-out $(SIEVE_OBJ) $(LAUNCHER_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(PIPELINE_OBJS), $(OBJS))

sieve: $(SIEVE_TARGET)
	./$(SIEVE_TARGET)

client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)

server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

launcher: $(LAUNCHER_TARGET)
	./$(LAUNCHER_TARGET)

pipeline: $(PIPELINE_TARGET)
	./$(PIPELINE_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

all: $(SIEVE_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET) $(LAUNCHER_TARGET) $(TEST_TARGET) $(PIPELINE_TARGET)

.PHONY: all sieve client server launcher test pipeline clean