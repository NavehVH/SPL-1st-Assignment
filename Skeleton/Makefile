CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

# Name of the executable
TARGET = $(BIN_DIR)/warehouse

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)