set(EXTRA_COMPONENT_DIRS
    "D:/ProgramFiles/PercepioView/View for FreeRTOS/ESP-IDF_FreeRTOS/TraceRecorder/kernelports/ESP-IDF_FreeRTOS"
)

# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(frtos_20260921_taskPriority)
