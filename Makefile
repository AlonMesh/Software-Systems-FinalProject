CXX = g++
SRC_DIR = src
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

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

# Pipeline program
PIPELINE_SRCS := $(SRC_DIR)/Pipeline.cpp $(SRC_DIR)/TSQueue.hpp $(SRC_DIR)/ActiveObject.hpp
PIPELINE_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(PIPELINE_SRCS))
PIPELINE_TARGET := $(BUILD_DIR)/pipeline

# Producer program
PRODUCER_SRCS := $(SRC_DIR)/ProducerConsumer.cpp $(SRC_DIR)/ProducerConsumer.hpp
PRODUCER_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(PRODUCER_SRCS))
PRODUCER_TARGET := $(BUILD_DIR)/producer


# Updating all target to build sieve, client, server, launcher, pipeline and producer
all: $(SIEVE_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET) $(LAUNCHER_TARGET) $(PIPELINE_TARGET) $(PRODUCER_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(SIEVE_TARGET): $(SIEVE_OBJ) $(filter-out $(CLIENT_OBJ) $(SERVER_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS) $(PRODUCER_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJ) $(filter-out $(SIEVE_OBJ) $(SERVER_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS) $(PRODUCER_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJ) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(LAUNCHER_OBJ) $(PIPELINE_OBJS) $(PRODUCER_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LAUNCHER_TARGET): $(LAUNCHER_OBJ) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ) $(PIPELINE_OBJS) $(PRODUCER_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PIPELINE_TARGET): $(PIPELINE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PRODUCER_TARGET): $(PRODUCER_OBJS) $(filter-out $(SIEVE_OBJ) $(CLIENT_OBJ) $(LAUNCHER_OBJ) $(SERVER_OBJ) $(PIPELINE_OBJS) $(PIPELINE_OBJS), $(OBJS))
	$(CXX) $(CXXFLAGS) -o $@ $^


# Individual targets for running the programs
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

producer: $(PRODUCER_TARGET)
	./$(PRODUCER_TARGET)

# Clean target to remove the build directory
clean:
	rm -rf $(BUILD_DIR)

# Ensure 'all' is up-to-date
.PHONY: all sieve client server launcher pipeline producer clean
