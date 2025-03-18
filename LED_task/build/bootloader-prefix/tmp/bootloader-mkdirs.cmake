# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/tmp"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/src/bootloader-stamp"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/src"
  "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/software/Espressif/frameworks/esp-idf-v5.3.1/Workspace/202503_Mar/LED_task/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
