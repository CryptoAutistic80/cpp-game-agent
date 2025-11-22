cmake_minimum_required(VERSION 3.15)
project(cpp_game_agent LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(CURL REQUIRED)

add_library(cpp_game_agent
    src/GroqBackend.cpp
    src/Agent.cpp
)

target_include_directories(cpp_game_agent PUBLIC include)

target_link_libraries(cpp_game_agent
    PUBLIC
        nlohmann_json::nlohmann_json
        CURL::libcurl
)

add_executable(agent_cli examples/main.cpp)
target_link_libraries(agent_cli PRIVATE cpp_game_agent)

target_compile_definitions(agent_cli PRIVATE CPP_GAME_AGENT_CLI)

install(TARGETS cpp_game_agent DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
