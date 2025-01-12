cmake_minimum_required(VERSION 2.16)

project(SkipList)

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置.o文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/obj)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/obj)

# 如果目录不存在则创建
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/obj)

# 收集所有源文件和头文件
file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB PROJECT_HEADERS "*.h")

# 从所有源文件中移除 stress.cpp
list(REMOVE_ITEM PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/stress.cpp)

# 生成主程序（包含所有头文件和除stress.cpp外的源文件）
add_executable(SkipList ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# 生成压力测试程序（包含所有头文件和stress.cpp）
add_executable(stress_test stress.cpp ${PROJECT_HEADERS})