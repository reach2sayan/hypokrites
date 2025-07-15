cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(hypokrites VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wfatal-errors)
find_package(Boost REQUIRED)

set(UTILITY_HEADERS
        include/utility/meta.hpp
        include/utility/callable.hpp
        include/utility/thread_pool.hpp
        include/utility/threadsafe_list.hpp
        include/utility/threadsafe_queue.hpp
)

set(HEADERS include/actor.hpp
        include/messages.hpp
        include/message_handler.hpp
        include/actor_base.hpp
        include/actor_system.hpp
        include/scheduler.hpp
        actor_state.hpp
)

add_library(hypokrites ${HEADERS} ${UTILITY_HEADERS}
        src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/utility)

# INTERFACE for library consumers
target_include_directories(${PROJECT_NAME}
        INTERFACE ${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_NAME})
