# Собираем все .cpp файлы в папке src
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Создаем статическую библиотеку
add_library(rns_core STATIC ${SRC_FILES})

# Указываем, где искать наши публичные заголовки
target_include_directories(rns_core PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)