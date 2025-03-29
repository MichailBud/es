#================================================================================
cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE cmake/arm-none-eabi-gcc.cmake)


#================================================================================
project(es LANGUAGES CXX C ASM)
set(DEVICE stm32f407vg)


#================================================================================

add_subdirectory(src)

add_subdirectory(libs)

add_subdirectory(flash_n_debug)

add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND cp -u ${CMAKE_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_BINARY_DIR}/firmware
)


#================================================================================
set(PROG stlink-v2)
string(CONCAT PROG_TARGET ${LOCM3_DEVICE_SERIE} x)

include(oocd_flash_script)

install(CODE ${OOCD_FLASH_SCRIPT})

