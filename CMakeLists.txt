cmake_minimum_required(VERSION 3.30)
project(unicast_based_multicast_hypercubes)

set(CMAKE_CXX_STANDARD 20)

add_executable(unicast_based_multicast_hypercubes main.cpp Buffer.cpp common.cpp Event.cpp Hypercube.cpp HypercubeNode.cpp Message.cpp RoutingHypercube.cpp)
