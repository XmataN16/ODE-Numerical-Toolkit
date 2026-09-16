cmake_minimum_required(VERSION 3.20)
project(ODENumericalToolkit VERSION 1.0.0 LANGUAGES CXX)

# Требуем современный C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем наши поддиректории
add subdirectory(src)
add_subdirectory(app)

# Включаем тесты, если это не релизная сборка
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()