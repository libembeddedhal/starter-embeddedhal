# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.21)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain_arm_picolibc.cmake)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(embedded-starter VERSION 0.0.1 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(EXECUTABLE ${PROJECT_NAME}.elf)

# Find packages go here.
find_package(libembeddedhal REQUIRED)
find_package(libxbitset REQUIRED)
find_package(libarmcortex REQUIRED)
find_package(liblpc40xx REQUIRED)

# Adding something we can run - Output name matches target name
add_executable(${EXECUTABLE} embeddedhal-starter/main.cpp)

# Link each target with other targets or add options, etc.
target_compile_features(${EXECUTABLE} PRIVATE cxx_std_20)
set_target_properties(${EXECUTABLE} PROPERTIES CXX_EXTENSIONS OFF)

# Make sure you link your targets with this command. It can also link libraries
# and even flags, so linking a target that does not exist will not give a
# configure-time error.
target_link_libraries(${EXECUTABLE}
                      PRIVATE
                      libembeddedhal::libembeddedhal
                      libxbitset::libxbitset
                      libarmcortex::libarmcortex
                      liblpc40xx::liblpc40xx)

target_compile_definitions(${EXECUTABLE} PRIVATE -DPLATFORM="lpc40xx")

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

$<$<CONFIG:Debug>:-Og>
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
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
POST_BUILD COMMAND ${CMAKE_SIZE_UTIL} bin/${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
COMMAND ${CMAKE_OBJCOPY} -O ihex bin/${EXECUTABLE} bin/${PROJECT_NAME}.hex
COMMAND ${CMAKE_OBJCOPY} -O binary bin/${EXECUTABLE} ${PROJECT_NAME}.bin)

# Create dissembly files
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
COMMAND ${CMAKE_OBJDUMP} --disassemble --demangle bin/${EXECUTABLE}
        > bin/${PROJECT_NAME}.S
COMMAND ${CMAKE_OBJDUMP}
        --all-headers --source --disassemble --demangle bin/${EXECUTABLE}
        > bin/${PROJECT_NAME}.lst)
