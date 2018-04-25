cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) #Use c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) #require the above (otherwise allows fallback to earlier version)
set(CMAKE_CXX_EXTENSIONS OFF) #Don't use non-standard compiler extensions

project (obstacleSat)
set(SOURCES
Clause.cpp
Clause.hpp
Graph.cpp
Graph.hpp
RangeUtilities.hpp
SATInstance.cpp
SATInstance.hpp
SATInstanceGenerator.cpp
SATInstanceGenerator.hpp
SolutionAnalyzer.cpp
SolutionAnalyzer.hpp
Variable.cpp
Variable.hpp
main.cpp
)

#turn on all warnings, minus some that are not useful
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Weverything \
    -Wno-padded -Wno-c++98-compat -Wno-weak-vtables -Wno-missing-prototypes \
    -Wno-global-constructors -Wno-exit-time-destructors -Wno-deprecated-declarations \
    ")

#explicitly set/clear some warnings that Xcode has flags for
#(otherwise these are set to Xcode defaults, despite -Weverything, etc.)
#This list is probably actually longer, these are just the ones I have caught by
#using Unix Makefiles as well as Xcode
set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wconversion -Wcomma \
")

find_package(cryptominisat5 REQUIRED)
include_directories( ${CRYPTOMINISAT5_INCLUDE_DIRS})

add_executable(obstacleSat ${SOURCES})

target_link_libraries(obstacleSat ${CRYPTOMINISAT5_LIBRARIES})

