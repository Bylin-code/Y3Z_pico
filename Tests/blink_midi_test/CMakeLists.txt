# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(main C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(main
  main.c
  usb_descriptors.c
  midi_helper.c
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(main)

# Linked libraries
target_link_libraries(main 
    PUBLIC 
        pico_stdlib 
        pico_stdio_usb
        tinyusb_device 
        tinyusb_board
)
 