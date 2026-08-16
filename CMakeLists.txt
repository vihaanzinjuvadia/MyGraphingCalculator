cmake_minimum_required(VERSION 3.16)
project(WorkingGraphingCalculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED) # Add other components like audio, network as needed
find_package(Threads REQUIRED)

add_executable(WorkingGraphingCalculator
        src/DataGenerator.h
        src/Draw.h
        src/main.cpp
        src/main_loop.h
        src/Math.h
        src/variables.h
        src/FunctionParser/Utility/UtilityFunctions.cpp
        src/FunctionParser/Utility/UtilityVariables.cpp
        src/FunctionParser/Lexer.cpp
        src/FunctionParser/Node.cpp
        src/FunctionParser/Parser.cpp)

target_link_libraries(WorkingGraphingCalculator sfml-graphics sfml-window sfml-system Threads::Threads) # Add other components as linked above

# Tell the program where the font lives, and drop a copy next to the executable so a
# moved build still finds one.
set(GRAPH_FONT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Font/JetBrainsMonoNerdFont-Regular.ttf")
target_compile_definitions(WorkingGraphingCalculator PRIVATE GRAPH_FONT_PATH="${GRAPH_FONT_FILE}")

add_custom_command(TARGET WorkingGraphingCalculator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${GRAPH_FONT_FILE}"
        "$<TARGET_FILE_DIR:WorkingGraphingCalculator>/JetBrainsMonoNerdFont-Regular.ttf")
