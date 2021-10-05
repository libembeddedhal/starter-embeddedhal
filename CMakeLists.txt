cmake_minimum_required(VERSION 3.1...3.21)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain_arm_picolibc.cmake)

project(embedded-starter VERSION 0.0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(EXECUTABLE ${PROJECT_NAME}.elf)

find_package(liblpc40xx CONFIG REQUIRED)
find_package(libxbitset CONFIG REQUIRED)

add_executable(${EXECUTABLE} embeddedhal-starter/main.cpp)

target_compile_features(${EXECUTABLE} PRIVATE cxx_std_20)
set_target_properties(${EXECUTABLE} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${EXECUTABLE} PRIVATE -DPLATFORM=lpc40xx)

target_link_libraries(${EXECUTABLE}
                      PRIVATE
                      libxbitset::libxbitset
                      liblpc40xx::liblpc40xx)

if ("${TOOLCHAIN_NAME}" STREQUAL "arm-picolibc")
target_compile_options(${EXECUTABLE} PRIVATE
-mcpu=cortex-m4
-mthumb
-mfloat-abi=hard
-mfpu=fpv4-sp-d16
-mtpcs-frame
-mtpcs-leaf-frame

-fdata-sections
-ffunction-sections

-fno-exceptions
-fno-rtti
-fno-unwind-tables
-fno-threadsafe-statics
-fno-omit-frame-pointer
-ffreestanding

-Wall
-g
)

target_link_options(${EXECUTABLE} PRIVATE
-T${CMAKE_SOURCE_DIR}/embeddedhal-starter/linker.ld

-mcpu=cortex-m4
-mthumb
-mfloat-abi=hard
-mfpu=fpv4-sp-d16
-mtpcs-frame
-mtpcs-leaf-frame

-fno-exceptions
-fno-rtti
-fno-unwind-tables
-fno-threadsafe-statics
-fno-omit-frame-pointer
-ffreestanding

--specs=nosys.specs

-Xlinker --defsym=__heap_start=0x20000000
-Xlinker --defsym=__heap_end=0x20008000
-Xlinker --defsym=__heap_size=0x8000

--specs=picolibcpp.specs
--oslib=semihost
--crt0=hosted

-Wl,-Map=${PROJECT_NAME}.map,--cref
-Wl,--gc-sections
-Wl,--print-memory-usage
-g
)
endif()

# Print executable size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)

# Create dissembly files
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
COMMAND ${CMAKE_OBJDUMP} --disassemble --demangle ${EXECUTABLE}
        > ${PROJECT_NAME}.S
COMMAND ${CMAKE_OBJDUMP}
        --all-headers --source --disassemble --demangle ${EXECUTABLE}
        > ${PROJECT_NAME}.lst)
