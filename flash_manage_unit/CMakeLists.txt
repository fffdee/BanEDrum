cmake_minimum_required(VERSION 3.10)
project(BG_FMU)

# 指定C语言编译器
set(CMAKE_C_COMPILER gcc)

# 设置源文件
file(GLOB_RECURSE SRC_FILES
   
    "./src/main.c"
    "./download/*.c"
    "./read/*.c"
)


# 生成可执行文件
add_executable(BG_FMU ${SRC_FILES})

# 设置头文件目录
target_include_directories(BG_FMU PUBLIC
    ./download/
    ./read/
)

# 链接所需的库（如果有的话）
target_link_libraries(BG_FMU ev m)
