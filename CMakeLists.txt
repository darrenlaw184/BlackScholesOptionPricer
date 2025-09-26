cmake_minimum_required(VERSION 3.16)
project(BlackScholesOptionPricer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
if (NOT TARGET glfw)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW3 REQUIRED glfw3)
    set(GLFW_LIBS ${GLFW3_LIBRARIES})
    set(GLFW_INCLUDE ${GLFW3_INCLUDE_DIRS})
else()
    set(GLFW_LIBS glfw)
    set(GLFW_INCLUDE "")
endif()

# Dear ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# ImPlot
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/implot)
set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/BlackScholesModel.cpp
    src/OptionPricerGUI.cpp
    ${IMGUI_SOURCES}
    ${IMPLOT_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${GLFW_INCLUDE}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    ${GLFW_LIBS}
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
