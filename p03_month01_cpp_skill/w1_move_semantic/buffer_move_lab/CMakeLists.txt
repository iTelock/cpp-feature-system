# CMakeList.txt: buffer_move_lab 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project ("buffer_move_lab")

# 将源代码添加到此项目的可执行文件。
add_executable (buffer_move_lab "buffer_move_lab.cpp" "buffer_move_lab.h")

# TODO: 如有需要，请添加测试并安装目标。
