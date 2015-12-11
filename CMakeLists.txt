CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

PROJECT(IsaacGrimoire)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FIND_PACKAGE(FLTK REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_LIBRARY(SECURITY_FRAMEWORK Security)

ADD_SUBDIRECTORY(src bin)

INSTALL(
    FILES
        resources/collectibles.js
        resources/trinkets.js
        resources/cards.js
        resources/pills.js
        resources/pillspic.json
        resources/special.json
    DESTINATION
        .        
)

INSTALL(
    DIRECTORY
        resources/gfx
    DESTINATION
        .
    FILES_MATCHING
        PATTERN "*.png"
)

INSTALL(
    FILES
        resources/Info.plist
    DESTINATION
        IsaacGrimoire.app/Contents
)
