cmake_minimum_required(VERSION 3.14)
project(0xApiTest)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.10.4
)
set(CPR_BUILD_TESTS OFF)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(0xApiTest
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        modules/http_client.cpp
        modules/http_client.h
        modules/request_io.cpp
        modules/request_io.h
)

target_include_directories(0xApiTest PRIVATE
        ${json_SOURCE_DIR}/single_include
)

target_link_libraries(0xApiTest PRIVATE
        Qt6::Widgets
        cpr::cpr
)