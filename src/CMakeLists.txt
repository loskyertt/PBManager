# src/CMakeLists.txt
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# utils 模块：编译为静态库
add_library(utils STATIC
  utils/impl/utils.cpp
)
target_include_directories(utils
  PUBLIC  utils/include
  PRIVATE utils/impl
)

# 主可执行文件
add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} PRIVATE utils)
