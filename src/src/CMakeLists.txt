project(src)

add_library(fs File.cpp)

add_executable(exe main.cpp)
target_link_libraries(exe ${GLOG_LIBRARIES})





