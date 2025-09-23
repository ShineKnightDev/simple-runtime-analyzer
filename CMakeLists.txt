# simple-runtime-analyzer CMake Configuration
# Usage:
#   cmake -B build -S .                                                                 # Configure project
#   cmake --build build --target custom_program SRC=../my_program.cpp NAME=my_tool      # Compile custom program
#   cmake --build build --target my_tool_run                                            # Run custom program
#   cmake --build build --target example_run                                            # Run example program
#   cmake --build build --target clear                                                  # Clean generated files
#   cmake --build build --target docs                                                   # Generate documentation

#-----------------------------------------------------------------------------#
#                           simple-runtime-analyzer                           #
#-----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.28)
project(simple-runtime-analyzer VERSION 1.0.0 LANGUAGES CXX)

#-----------------------------------------------------------------------------#
#                             Output directories                              #
#-----------------------------------------------------------------------------#

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(DATA_DIR "${PROJECT_SOURCE_DIR}/data")
file(MAKE_DIRECTORY "${BIN_DIR}" "${DATA_DIR}")

#-----------------------------------------------------------------------------#
#                                Main Library                                 #
#-----------------------------------------------------------------------------#

add_library(runtime_lib INTERFACE)
target_compile_features(runtime_lib INTERFACE cxx_std_23)

if(MSVC)
    target_compile_options(runtime_lib INTERFACE /W4 /WX /permissive- /EHsc)
else()
    target_compile_options(runtime_lib INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(runtime_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set(PLOT_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/scripts/plot.py")
add_definitions(-DPLOT_SCRIPT_PATH="${PLOT_SCRIPT_PATH}")

#-----------------------------------------------------------------------------#
#                      Future migration to C ++ modules                       #
#-----------------------------------------------------------------------------#
# When clang and GCC fully support the standard modules:
# - Replace INTERFACE with STATIC or MODULE_LIBRARY
# - Use FILE_SET CXX_MODULES on target_sources()
# - Replace #include with import <modulestd>
#-----------------------------------------------------------------------------#

#-----------------------------------------------------------------------------#
#                           Python Plot Generation                            #
#-----------------------------------------------------------------------------#

find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    message(STATUS "Python found: ${Python3_EXECUTABLE}")
else()
    message(WARNING " Python not found. Plot generation will be disabled")
endif()

#-----------------------------------------------------------------------------#
#                         Documentation with Doxygen                          #
#-----------------------------------------------------------------------------#

# Option to enable/disable documentation
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)

if(BUILD_DOCUMENTATION)
    find_package(Doxygen)
    
    if(Doxygen_FOUND)
        message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")
        message(STATUS "Documentation build: ENABLED")
        
        # Verify that docs directories exist
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/docs")
            message(WARNING "Docs directory not found: ${PROJECT_SOURCE_DIR}/docs")
        endif()
        
        # Verify that .md files exist
        file(GLOB DOCS_MD_FILES "${PROJECT_SOURCE_DIR}/docs/*.md")
        
        if(NOT DOCS_MD_FILES)
            message(WARNING "No .md files found in docs/ directory")
        else()
            message(STATUS "Found .md files: ${DOCS_MD_FILES}")
        endif()
        
        # Verify Doxygen Awesome submodule
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/docs/doxygen-awesome/doxygen-awesome.css")
            message(WARNING "Doxygen Awesome CSS not found. Run: git submodule update --init --recursive")
        else()
            message(STATUS "Doxygen Awesome CSS: FOUND")
        endif()
        
        # Create documentation target
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating documentation from .md files + code"
            VERBATIM
        )
        
        add_custom_command(TARGET docs POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Documentation generated at: ${PROJECT_BINARY_DIR}/docs/html/index.html"
            COMMENT "Documentation ready at: docs/html/index.html"
        )
        
    else()
        message(WARNING " Doxygen not found - documentation target 'docs' will not be available")
        message(STATUS "Install Doxygen to enable documentation generation")
    endif()
else()
    message(STATUS "Documentation: DISABLED (enable with -DBUILD_DOCUMENTATION=ON)")
endif()

#-----------------------------------------------------------------------------#
#                                Main Targets                                 #
#-----------------------------------------------------------------------------#

# Official Example Program
set(EXAMPLE_SRC "${PROJECT_SOURCE_DIR}/example/src/example.cpp")
add_executable(example ${EXAMPLE_SRC})
target_link_libraries(example PRIVATE runtime_lib)
set_target_properties(example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    OUTPUT_NAME "example"
)

# Custom program system
if(DEFINED SRC)
    # Get the desired output name or use default
    if(DEFINED NAME)
        set(CUSTOM_OUTPUT_NAME "${NAME}")
    else()
        # Generate name from source file name
        get_filename_component(CUSTOM_BASE_NAME ${SRC} NAME_WE)
        set(CUSTOM_OUTPUT_NAME "custom_${CUSTOM_BASE_NAME}")
    endif()
    
    add_executable(${CUSTOM_OUTPUT_NAME} ${SRC})
    target_link_libraries(${CUSTOM_OUTPUT_NAME} PRIVATE runtime_lib)
    set_target_properties(${CUSTOM_OUTPUT_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
        OUTPUT_NAME "${CUSTOM_OUTPUT_NAME}"
    )

    # Create run target for custom program
    add_custom_target(${CUSTOM_OUTPUT_NAME}_run
        COMMAND ${BIN_DIR}/${CUSTOM_OUTPUT_NAME}
        COMMENT "Running custom program ${CUSTOM_OUTPUT_NAME}..."
        DEPENDS ${CUSTOM_OUTPUT_NAME}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
    
    # Main target alias for easier discovery
    add_custom_target(custom_program
        COMMENT "Build custom program ${CUSTOM_OUTPUT_NAME}"
        DEPENDS ${CUSTOM_OUTPUT_NAME}
    )
endif()

#-----------------------------------------------------------------------------#
#                               Custom Targets                                #
#-----------------------------------------------------------------------------#

add_custom_target(example_run
    COMMAND ${BIN_DIR}/example
    COMMENT "Running the example program..."
    DEPENDS example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} -E remove -f ${BIN_DIR}/example
    COMMAND ${CMAKE_COMMAND} -E remove -f ${BIN_DIR}/custom_*
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DATA_DIR}/*.csv
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DATA_DIR}/*.json
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DATA_DIR}/*.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${DATA_DIR}/*.png
    COMMENT "Cleaning binaries and data files..."
)

#-----------------------------------------------------------------------------#