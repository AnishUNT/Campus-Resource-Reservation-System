# Campus Resource Reservation System - Milestone 1
#
#   make          build ./reservation_system
#   make run      build then run
#   make clean    remove build artifacts
#
# Equivalent single command without make:
#   g++ -std=c++17 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o reservation_system

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude

SRC_DIR  := src
OBJ_DIR  := build
TARGET   := reservation_system

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
HEADERS := $(wildcard include/*.h)

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	$(CXX) $(CXXFLAGS) tests/unit_tests.cpp $(filter-out build/main.o,$(OBJECTS)) -o build/unit_tests
	./build/unit_tests
	python3 tests/integration_tests.py

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
