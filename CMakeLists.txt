cmake_minimum_required(VERSION 3.10)

project(textRPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/entities/Istota.cpp
    src/entities/Gracz.cpp
    src/core/Game.cpp
)

add_executable(RPG_App ${SOURCES})