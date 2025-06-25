cmake_minimum_required(VERSION 3.12)

project(ParserSales)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE debug)
set(SANITIZE_COMPILE_FLAGS "-fsanitize=address,undefined" "-fno-omit-frame-pointer")
set(SANITIZE_LINK_FLAGS "-fsanitize=address,undefined")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    src/TelegramSender.cpp
    src/PostgresDB.cpp
    src/JsonReader.cpp
    src/PyLoader.cpp
    # src/main.cpp
    tests/test_PostgresDB/test_PostgresDB_2.cpp
    # src/TelegramUser.cpp
    # src/BotTelegram.cpp
    # src/Interface.cpp
    src/good_function.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZE_FLAGS})

find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(ParserSales PRIVATE ${PostgreSQL_LIBRARIES})

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(ParserSales PRIVATE ${CURL_LIBRARIES})