cmake_minimum_required(VERSION 3.10)
project(udptcp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")

# nlohmann/json (can use system package or header-only)
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_executable(udptcp
    src/main.cpp
    src/UdpReciver.cpp
    src/TcpServer.cpp
    src/Bridge.cpp
)

target_link_libraries(udptcp PRIVATE nlohmann_json::nlohmann_json pthread)
