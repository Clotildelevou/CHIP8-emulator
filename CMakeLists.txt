# ==========================================
#            Various definitions
# ==========================================

cmake_minimum_required(VERSION 3.15)
project(CHIP8-emulator VERSION 1.0.0 LANGUAGES C)

#Verbose option
set(CMAKE_VERBOSE_MAKEFILE OFF)

#Test option
option(ENABLE_TESTS "Build tests along with the library" ON)


set(SOURCE_ROOT "${PROJECT_SOURCE_DIR}/src")
set(EXC_TST "${PROJECT_SOURCE_DIR}/build")
set(CFLAGS -Wextra -Werror -pedantic -std=c99 -I${SOURCE_ROOT})
set(LDFLAGS_TEST -fsanitize=address -lcriterion)

# ==========================================
#             Libraries
# ==========================================

# The name of all the libs in the project.
# If you want to add a lib, add its name to this variable
set(LIBS
    chip8
    display
    input
    )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# chip8
file(GLOB_RECURSE CHIP8_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/chip8/*.c")
add_library(chip8 STATIC ${CHIP8_SOURCES})
target_compile_options(chip8 PRIVATE ${CFLAGS})

# display
file(GLOB_RECURSE DISPLAY_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/display/*.c")
add_library(display STATIC ${DISPLAY_SOURCES})
target_compile_options(display PRIVATE ${CFLAGS})
target_link_libraries(display PRIVATE chip8)

file(GLOB_RECURSE INPUT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/input/*.c")
add_library(input STATIC ${INPUT_SOURCES})
target_compile_options(input PRIVATE ${CFLAGS})
target_link_libraries(input PRIVATE chip8)

# ==========================================
#             Test executables
# ==========================================

if (ENABLE_TESTS)

    include (CTest)

    #chip8
    file(GLOB_RECURSE CHIP8_TESTS RELATIVE ${CMAKE_SOURCE_DIR}
        "tests/unit_tests/chip8/*.c")
    add_executable(chip8_unit ${CHIP8_TESTS})
    target_compile_options(chip8_unit PRIVATE ${CFLAGS})
    target_link_libraries(chip8_unit PRIVATE chip8)
    target_link_options(chip8_unit PRIVATE ${LDFLAGS_TEST})
    add_test(NAME chip8_unit COMMAND ${EXC_TST}/chip8_unit)

    ######Lib2
    ####file(GLOB_RECURSE#LIB2_TESTS#RELATIVE#${CMAKE_SOURCE_DIR}
    ########"tests/unit_tests/lib2/*.c")
    ####add_executable(lib2_unit#${LIB2_TESTS})
    ####target_compile_options(lib2_unit#PRIVATE#${CFLAGS})
    ####target_link_libraries(lib2_unit#PRIVATE#lib2#lib1)
    ####target_link_options(lib2_unit#PRIVATE#${LDFLAGS_TEST})
    ####add_test(NAME#lib2_unit#COMMAND#${EXC_TST}/lib2_unit)

endif()

# ==========================================
#           Final executable
# ==========================================

add_executable(chip8_emulator src/main.c)
target_link_libraries(chip8_emulator PRIVATE ${LIBS} ${SDL2_LIBRARIES})
target_compile_options(chip8_emulator PRIVATE ${CFLAGS})
