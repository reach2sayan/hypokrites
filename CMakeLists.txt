cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(hypokrites VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)
set(HEADERS include/actor.hpp
        include/mailbox.hpp
        include/message_handler.hpp
        include/monitors.hpp
        include/utility/meta.hpp)

add_library(hypokrites ${HEADERS} src/main.cpp
        src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/utility)

# INTERFACE for library consumers
target_include_directories(${PROJECT_NAME}
        INTERFACE ${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_NAME})
