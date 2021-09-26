set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(TOOLCHAIN_NAME "arm-picolibc")
set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(BINUTILS_PATH ${CMAKE_SOURCE_DIR}/tools/gcc-arm-none-eabi-picolibc/bin)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${BINUTILS_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${BINUTILS_PATH}/${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${BINUTILS_PATH}/${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${BINUTILS_PATH}/${TOOLCHAIN_PREFIX}objcopy
    CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP ${BINUTILS_PATH}/${TOOLCHAIN_PREFIX}objdump
    CACHE INTERNAL "objdump tool")
set(CMAKE_SIZE_UTIL ${BINUTILS_PATH}/${TOOLCHAIN_PREFIX}size
    CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
