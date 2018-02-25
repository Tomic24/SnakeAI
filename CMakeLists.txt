cmake_minimum_required (VERSION 2.6)

project (SnakeAI)

aux_source_directory(Engine Engine_Files)
add_library(Engine STATIC ${Engine_Files})

aux_source_directory(Displays Display_Files)
add_library(Displays STATIC ${Display_Files})

aux_source_directory(Snakes Snake_Files)
add_library(Snakes STATIC ${Snake_Files})

aux_source_directory(Utils Util_Files)
add_library(Utils STATIC ${Util_Files})

add_executable(SnakeAI Main.cpp)
target_link_libraries(SnakeAI Engine Displays Snakes Utils)

include_directories("${PROJECT_SOURCE_DIR}")