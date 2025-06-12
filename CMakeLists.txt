cmake_minimum_required(VERSION 3.25)
project(unicast-based-multicast-2Dmeshes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Simulator
        main.cpp
        Event.cpp
        Hypercube.cpp
        HypercubeNode.cpp
        HypercubeRouting.cpp
        testfuc.cpp
        )