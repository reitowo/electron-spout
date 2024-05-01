cmake_minimum_required(VERSION 3.25)
project(electron_spout)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_JS_INC})

add_library(electron_spout SHARED main.cpp spout_output.cpp spout_output.h ${CMAKE_JS_SRC})
set_target_properties(electron_spout PROPERTIES PREFIX "" SUFFIX ".node" OUTPUT_NAME "electron-spout")
target_link_libraries(electron_spout PRIVATE ${CMAKE_JS_LIB} delayimp.lib dxgi.lib dxguid.lib)

find_package(Spout2 CONFIG REQUIRED)
target_link_libraries(electron_spout PRIVATE Spout2::Spout_static Spout2::SpoutDX_static)

add_custom_command(TARGET electron_spout POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/electron-spout.node.d.ts" "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/electron-spout.node.d.ts")