cmake_minimum_required(VERSION 3.10)

project(WarehouseInventoryMonitor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(inventory_core
    src/Produs.cpp
    src/Furnizor.cpp
    src/Depozit.cpp
)

target_include_directories(inventory_core PUBLIC src)

add_executable(warehouse_inventory src/main.cpp)
target_link_libraries(warehouse_inventory PRIVATE inventory_core)

add_executable(test_inventory tests/test_inventory.cpp)
target_link_libraries(test_inventory PRIVATE inventory_core)

enable_testing()
add_test(NAME inventory_tests COMMAND test_inventory)
