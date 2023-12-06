project(LearnOpengl)
cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB IMGUI_SOURCE "lib/imgui-1.90/*.cpp")

add_executable(
        main 
        main.cpp
        ./lib/glad/src/glad.c 
        ${IMGUI_SOURCE} lib/imgui-1.90/backends/imgui_impl_glfw.cpp lib/imgui-1.90/backends/imgui_impl_opengl3.cpp 
        ${SOURCE_FILES} 
    )
include_directories(main lib include lib/glad/include/ lib/imgui-1.90)
target_link_libraries(main PRIVATE glfw PRIVATE OpenGL)

add_custom_target(
    run
    COMMAND ./main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
