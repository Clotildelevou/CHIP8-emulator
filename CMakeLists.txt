# ==========================================
#            Various definitions
# ==========================================

cmake_minimum_required(VERSION 3.15)
project(42sh VERSION 1.0.0 LANGUAGES C)

#Verbose option
set(CMAKE_VERBOSE_MAKEFILE OFF)

#Test option
option(ENABLE_TESTS "Build tests along with the library" ON)

set(SOURCE_ROOT "${PROJECT_SOURCE_DIR}/src")
set(EXC_TST "${PROJECT_SOURCE_DIR}/build")
set(CFLAGS -g -Wall -Wextra -Werror -pedantic -std=c99 -I${SOURCE_ROOT})
set(LDFLAGS_TEST -fsanitize=address -lcriterion)

# ==========================================
#             Libraries
# ==========================================

# The name of all the libs in the project.
# If you want to add a lib, add its name to this variable
set(LIBS
    lib1
    lib2
    )

# lib1
file(GLOB_RECURSE LIB1_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/lib1/*.c")
add_library(lib1 STATIC ${UTILS_SOURCES})
target_compile_options(lib1 PRIVATE ${CFLAGS})

# lib2
file(GLOB_RECURSE LIB2_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/lib2/*.c")
add_library(lib2 STATIC ${LIB2_SOURCES})
target_compile_options(lib2 PRIVATE ${CFLAGS})
target_link_libraries(lib2 PRIVATE lib1)

# ==========================================
#             Test executables
# ==========================================

if (ENABLE_TESTS)

    include (CTest)

    # Lib1
    file(GLOB_RECURSE LIB1_TESTS RELATIVE ${CMAKE_SOURCE_DIR}
        "tests/unit_tests/lib1/*.c")
    add_executable(lib1_unit ${LIB1_TESTS})
    target_compile_options(lib1_unit PRIVATE ${CFLAGS})
    target_link_libraries(lib1_unit PRIVATE lib1)
    target_link_options(lib1_unit PRIVATE ${LDFLAGS_TEST})
    add_test(NAME lib1_unit COMMAND ${EXC_TST}/lib1_unit)

    # Lib2
    file(GLOB_RECURSE LIB2_TESTS RELATIVE ${CMAKE_SOURCE_DIR}
        "tests/unit_tests/lib2/*.c")
    add_executable(lib2_unit ${LIB2_TESTS})
    target_compile_options(lib2_unit PRIVATE ${CFLAGS})
    target_link_libraries(lib2_unit PRIVATE lib2 lib1)
    target_link_options(lib2_unit PRIVATE ${LDFLAGS_TEST})
    add_test(NAME lib2_unit COMMAND ${EXC_TST}/lib2_unit)

endif()

# ==========================================
#           Final executable
# ==========================================

add_executable(my_project src/main.c)
target_link_libraries(my_project PRIVATE ${LIBS})
target_compile_options(my_project PRIVATE ${CFLAGS})
