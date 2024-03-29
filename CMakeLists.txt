cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("SOM")
find_package(OpenGL REQUIRED)
add_subdirectory(extern)
add_executable(SOM)

set_target_properties(SOM
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_sources(SOM PRIVATE
    "src/main.cpp"
    "src/world.cpp"
    "src/matrixStack.cpp"
    "src/SOM.cpp"
)

target_include_directories(SOM PRIVATE
    "include/"
)

target_link_libraries(SOM PRIVATE
    glad # include(BuildGlad.cmake)
    imgui # include(BuildImgui.cmake)
    glm::glm # add_subdirectory(ext/glm)
    glfw  # find_package(glfw3 REQUIRED)
    OpenGL::GL # find_package(OpenGL REQUIRED)
)

add_custom_command(TARGET SOM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:SOM>/shader"
    VERBATIM
)

