project(Test)

# Compile gtest
add_subdirectory(gtest)
include_directories( ${gtest_INCLUDE_DIR} ${fs_SOURCE_DIR})

message(${gtest_INCLUDE_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test 
        BitmapTest.cpp
        BlockManagerTest.cpp
)
        
target_link_libraries(test gtest_main fslib)

add_custom_command(TARGET test POSTBUILD COMMAND test)