# simple-runtime-analyzer CMake Configuration
# Usage:
#   cmake --build . --target user_program -- -DSRC=src/my_code.cpp   # compile and run your own file and plot results
#   cmake --build . --target example_run                             # compile and run the official example and plot its results
#   cmake --build . --target example_clear                           # clean binaries and data files from the example
#   cmake --build . --target clear                                   # clean binaries and data files from your project
#   cmake --build . --target format_code                             # format code
#   cmake --build . --target tidy_code                               # analyze code
#   cmake --build . --target docs                                    # generate documentation

#-----------------------------------------------------------------------------#
#                           simple-runtime-analyzer                           #
#-----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.28)
project(simple-runtime-analyzer VERSION 1.0.0 LANGUAGES CXX)

#-----------------------------------------------------------------------------#
#                             Output directories                              #
#-----------------------------------------------------------------------------#

set(BIN_DIR "${PROJECT_BINARY_DIR}/bin")
set(DATA_DIR "${PROJECT_BINARY_DIR}/data")
file(MAKE_DIRECTORY "${BIN_DIR}" "${DATA_DIR}")

#-----------------------------------------------------------------------------#
#                                Main Library                                 #
#-----------------------------------------------------------------------------#

# Header only library. Prepared to migrate to modules in the future.
add_library(runtime_lib INTERFACE)

target_compile_features(runtime_lib INTERFACE cxx_std_23)

if(MSVC)
    target_compile_options(runtime_lib INTERFACE /W4 /WX /permissive- /EHsc)
else()
    target_compile_options(runtime_lib INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(runtime_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

#-----------------------------------------------------------------------------#
#                      Future migration to C ++ modules                       #
#-----------------------------------------------------------------------------#
# When clang and GCC fully support the standard modules:
# - Replace INTERFACE with STATIC or MODULE_LIBRARY
# - Use FILE_SET CXX_MODULES on target_sources()
# - Replace #include with import <modulestd>
#-----------------------------------------------------------------------------#

#-----------------------------------------------------------------------------#
#                           Analysis and Formatting                           #
#-----------------------------------------------------------------------------#

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_HEADERS "${PROJECT_SOURCE_DIR}/include/**/*.hpp")
    file(GLOB_RECURSE ALL_SOURCES "${PROJECT_SOURCE_DIR}/src/**/*.cpp")
    add_custom_target(format_code
        COMMAND ${CLANG_FORMAT} -style=file -i --fallback-style=none ${ALL_HEADERS} ${ALL_SOURCES}
        COMMENT "📝 Formatting code with clang-format"
    )
else()
    message(⚠️ WARNING "clang-format not found. Skipping format_code target.")
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set(EXAMPLE_SRC "${PROJECT_SOURCE_DIR}/example/src/example.cpp")
    add_custom_target(tidy_code
        COMMAND ${CLANG_TIDY} ${EXAMPLE_SRC} -- =${CMAKE_BINARY_DIR}
        COMMENT "🔍 Analyzing code with clang-tidy"
    )
else()
    message(⚠️ WARNING "clang-tidy not found. Skipping tidy_code target.")
endif()

find_program(IWYU include-what-you-use)
if(IWYU)
    add_custom_target(iwyu_code
        COMMAND ${IWYU} -p=${CMAKE_BINARY_DIR} ${EXAMPLE_SRC}
        COMMENT "🔍 Analyzing includes with include-what-you-use"
    )
else()
    message(⚠️ WARNING "IWYU not found. Skipping iwyu_code target.")
endif()

#-----------------------------------------------------------------------------#
#                         Documentation with Doxygen                          #
#-----------------------------------------------------------------------------#

find_package(Doxygen)
if(Doxygen_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "📚 Generating documentation with Doxygen"
    )
else()
    message(⚠️ WARNING "Doxygen not found. Skipping documentation generation.")
endif()

#-----------------------------------------------------------------------------#
#                                Main Targets                                 #
#-----------------------------------------------------------------------------#

# Official Example Program
set(EXAMPLE_SRC "${PROJECT_SOURCE_DIR}/example/src/example.cpp")
add_executable(example ${EXAMPLE_SRC})
target_link_libraries(example PRIVATE runtime_lib)
set_target_properties(example PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# User program
if(DEFINED SRC)
    add_executable(user_program ${SRC})
    target_link_libraries(user_program PRIVATE runtime_lib)
    set_target_properties(user_program PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
endif()

#-----------------------------------------------------------------------------#
#                               Custom Targets                                #
#-----------------------------------------------------------------------------#

add_custom_target(example_run
    COMMAND ${BIN_DIR}/example
    COMMENT "🚀 Running the example program..."
)
add_dependencies(example_run example)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E rm -rf 
        "${BIN_DIR}" "${DATA_DIR}" "${PROJECT_BINARY_DIR}/lib"
        "${PROJECT_BINARY_DIR}/CMakeFiles"
        "${PROJECT_BINARY_DIR}/cmake_install.cmake"
        "${PROJECT_BINARY_DIR}/Makefile"
    COMMENT "🧹 Full cleanup of build artifacts..."
)

#-----------------------------------------------------------------------------#