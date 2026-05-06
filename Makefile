CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
SRC = src/Produs.cpp src/Furnizor.cpp src/Depozit.cpp
APP = build/warehouse_inventory
TEST_APP = build/test_inventory

.PHONY: all run test clean

all: $(APP)

$(APP): $(SRC) src/main.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) src/main.cpp -o $(APP)

run: $(APP)
	./$(APP)

test: $(TEST_APP)
	./$(TEST_APP)

$(TEST_APP): $(SRC) tests/test_inventory.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) tests/test_inventory.cpp -o $(TEST_APP)

clean:
	rm -rf build
