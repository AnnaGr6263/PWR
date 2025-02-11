cmake_minimum_required(VERSION 3.15)

set(EXECUTABLE_OUTPUT_PATH bin)

# Pliki projektu
file(GLOB SOURCE cpp/src/*.cpp cpp/main/main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)

add_executable(simulation ${SOURCE})

# Ustawienie katalogu wyjściowego na wykresy
set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/plots/)
target_compile_definitions(simulation PRIVATE OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")

# Wyłączenie ostrzeżeń w czasie kompilacji
target_compile_options(simulation PRIVATE "-w")

# Matplot++ (klonuj do folderu "matplot")
add_subdirectory(matplot)

# Wyłączenie ostrzeżeń dla Matplot++
target_compile_options(matplot PRIVATE "-w")

# Linkowanie biblioteki Matplot++ do projektu
target_link_libraries(simulation PUBLIC matplot)
