# cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 添加对C++11的支持
# SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

# 项目信息
project(eventloop)

#include 路径
include_directories(${PROJECT_SOURCE_DIR}/base)


#第三方的.a文件和.h文件放到代码的lib文件夹
#link_directories(${PROJECT_SOURCE_DIR}/lib)

#指定生成目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
#查找src目录下的文件,并将名称保存到SRCS变量
aux_source_directory(. SRCS)

#添加base子目录
add_subdirectory(base)

# 指定生成目标
add_executable(eventloop ${SRCS})

#添加链接库
#target_link_libraries(discardserver muduo_net)
#target_link_libraries(discardserver muduo_base)
target_link_libraries(eventloop pthread rt)
target_link_libraries(eventloop libserver_base)
