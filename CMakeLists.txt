cmake_minimum_required(VERSION 3.20)
project(GameProject VERSION 1.0.0 LANGUAGES CXX)

# Compiler Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Looking for SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/world.cpp
    src/tile.cpp
)

# Header files
set(HEADERS
    src/game.h
    src/player.h
    src/world.h
    src/tile.h
)

# Creating an executable file
add_executable(game ${SOURCES} ${HEADERS})

# Enabling SFML
target_link_libraries(game
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

# Windows Settings
if(WIN32)
    # Adding a flag to disable the console
    set_target_properties(game PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
endif()

# Copying the SFML DLL to Windows (if necessary)
if(WIN32 AND NOT SFML_STATIC)
    # We find the SFML DLL files
    get_target_property(SFML_LIB_TYPE SFML::Graphics TYPE)
    if(SFML_LIB_TYPE STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics> $<TARGET_FILE_DIR:game>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Window> $<TARGET_FILE_DIR:game>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::System> $<TARGET_FILE_DIR:game>
        )
    endif()
endif()
