cmake_minimum_required(VERSION 3.27)
project(MyGraphingCalculator)

set(CMAKE_CXX_STANDARD 17)
set(UNIX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_BUILD_TYPE "Release")

add_executable(MyGraphingCalculator
        src/variables.h
        src/DataGenerator.h
        src/main_loop.h
        src/FunctionParser/Node.h
        src/FunctionParser/Parser.h
        src/FunctionParser/Utility/UtilityVariables.h
        src/main.cpp
        src/FunctionParser/Lexer.cpp
        src/FunctionParser/Utility/UtilityVariables.cpp
        src/FunctionParser/Utility/UtilityFunctions.h
        "src/FunctionParser/main 2.cpp"
        src/FunctionParser/Parser.cpp
        src/FunctionParser/Utility/UtilityFunctions.cpp
        src/FunctionParser/Lexer.h
        src/FunctionParser/Node.cpp
        src/Math.h
        src/Draw.h)

find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(MyGraphingCalculator sfml-system sfml-window sfml-graphics)
