project(src)


#	Google-glog
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})

add_executable(fs main.cpp)





