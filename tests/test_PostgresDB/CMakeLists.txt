cmake_minimum_required(VERSION 3.10)

project(test_PostgresDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE debug)
# set(SANITIZE_COMPILE_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")
# set(SANITIZE_LINK_FLAGS "-fsanitize=address,undefined")
set(SANITIZE_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")

add_executable(${PROJECT_NAME}
    ../../src/PostgresDB.cpp
    test_PostgresDB.cpp
    ../../src/good_function.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/../../include
)

target_compile_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS})

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBRARIES})

