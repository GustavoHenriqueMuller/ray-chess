cmake_minimum_required(VERSION 3.25)
project(ray_chess)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Our Project

add_executable(${PROJECT_NAME} src/Main.cpp src/Game.cpp src/Game.h src/pieces/Piece.h src/Position.h src/pieces/Peon.cpp src/pieces/Peon.h src/Board.h src/Board.cpp src/pieces/Rook.cpp src/pieces/Rook.h src/pieces/Piece.cpp src/pieces/Knight.cpp src/pieces/Knight.h src/pieces/Bishop.cpp src/pieces/Bishop.h src/pieces/Queen.cpp src/pieces/Queen.h src/pieces/King.cpp src/pieces/King.h src/Move.h src/Renderer.cpp src/Renderer.h src/pieces/PieceEnums.h src/screen/Screen.h src/screen/GameScreen.cpp src/screen/GameScreen.h)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()