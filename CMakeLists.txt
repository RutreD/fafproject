cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)

#kthook
set(KTHOOK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/kthook)
IF(NOT IS_DIRECTORY ${KTHOOK_DIR})
    execute_process(COMMAND git clone --depth 1 --recursive "https://github.com/kin4stat/kthook.git" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDIF()
add_subdirectory("kthook")
#kthook
#ImGui
IF(DEFINED ENV{IMGUI_DIR})
    set(IMGUI_DIR ENV{IMGUI_DIR})
ELSE()
    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
    IF(NOT IS_DIRECTORY ${IMGUI_DIR})
        execute_process(COMMAND git clone --depth 1 --recursive "https://github.com/ocornut/imgui.git" WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    ENDIF()
ENDIF()

add_library(imgui STATIC)

target_include_directories(imgui
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)
target_sources(imgui PRIVATE
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_dx9.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
)
#ImGui

add_library(${PROJECT_NAME} SHARED "main.cpp")

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")

target_link_libraries(${PROJECT_NAME} PUBLIC kthook imgui)
