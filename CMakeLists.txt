cmake_minimum_required(VERSION 3.12)

project(ParserSales)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE debug)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ParserSales
    src/TelegramSender.cpp
    src/PostgresDB
    src/JsonReader.cpp
    src/PyLoader.cpp
    src/main.cpp
    src/TelegramUser.cpp
    src/BotTelegram.cpp
    src/good_function.cpp
)

find_package(Boost REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(ParserSales PRIVATE ${PostgreSQL_LIBRARIES})

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(ParserSales PRIVATE ${CURL_LIBRARIES})