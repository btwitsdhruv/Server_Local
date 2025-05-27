cmake_minimum_required(VERSION 3.10)
project(tcpudp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

# nlohmann/json (can use system package or header-only)
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_executable(tcpudp
    src/main.cpp
    src/UdpSender.cpp
    src/TcpReceiver.cpp
    src/DataStructure.cpp
    src/DataDecoder.cpp

)

target_link_libraries(tcpudp PRIVATE nlohmann_json::nlohmann_json pthread)
