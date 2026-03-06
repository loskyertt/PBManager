# for each "lib/x.cpp", generate shared library or static library
message("CMakeLists.txt for lib directory")

# 在这里添加动态库，库名称：math_utils
add_library(utils SHARED
  ./utils.cpp
)

target_include_directories(utils PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
  set_target_properties(utils PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

message("\n--------------------------------------------")