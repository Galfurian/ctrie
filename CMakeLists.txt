# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------
# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(ctrie CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# ENABLE FETCH CONTENT
# -----------------------------------------------------------------------------

# We need this in order to import external projects.
include(FetchContent)
# Hide fetchcontent variables.
mark_as_advanced(FORCE
    FETCHCONTENT_QUIET
    FETCHCONTENT_BASE_DIR
    FETCHCONTENT_FULLY_DISCONNECTED
    FETCHCONTENT_UPDATES_DISCONNECTED
)

# -----------------------------------------------------------------------------
# Add options.
# -----------------------------------------------------------------------------

option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)

# -----------------------------------------------------------------------------
# DEPENDENCIES
# -----------------------------------------------------------------------------

# We want doxygen for the documentation.
find_package(Doxygen)

find_package(Threads REQUIRED)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ library.
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
# Inlcude header directories.
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set the library to use c++-11
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

# -----------------------------------------------------------------------------
# Set the compilation flags.
# -----------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings for MSVC-specific "safe" functions like strcpy_s, etc.,
    # which are not portable and may clutter warning logs.
    target_compile_definitions(${PROJECT_NAME} INTERFACE _CRT_SECURE_NO_WARNINGS)

    # Disable warning C4702: unreachable code.
    add_compile_options(/wd4702)

    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(${PROJECT_NAME} INTERFACE /WX)
    endif()

    if(STRICT_WARNINGS)
        # Enable external header management to suppress warnings in system and
        # external headers, making it easier to focus on project-specific issues.
        target_compile_options(${PROJECT_NAME} INTERFACE /experimental:external)
        target_compile_options(${PROJECT_NAME} INTERFACE /external:I ${CMAKE_BINARY_DIR})
        target_compile_options(${PROJECT_NAME} INTERFACE /external:anglebrackets)
        target_compile_options(${PROJECT_NAME} INTERFACE /external:W0)

        # Use a high warning level to catch as many potential issues as possible.
        target_compile_options(${PROJECT_NAME} INTERFACE /W4)

        # Enforce standards-compliant behavior to avoid relying on MSVC-specific extensions.
        target_compile_options(${PROJECT_NAME} INTERFACE /permissive-)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(${PROJECT_NAME} INTERFACE -Werror)
    endif()

    if(STRICT_WARNINGS)
        # Enable a broad set of warnings to catch common and subtle issues:
        target_compile_options(${PROJECT_NAME} INTERFACE
            -Wall                # Enable most general-purpose warnings.
            -Wextra              # Enable extra warnings not included in -Wall.
            -Wconversion         # Warn about implicit type conversions that may lose data.
            -pedantic            # Enforce strict compliance with the C++ standard.
            -Wshadow             # Warn about variable shadowing, which can cause subtle bugs.
            -Wnon-virtual-dtor   # Warn when a class with virtual functions lacks a virtual destructor.
            -Wnull-dereference   # Warn about potential null pointer dereferences.
            -Wformat=2           # Enable strict checks for printf/scanf format strings.
            -Woverloaded-virtual # Warn when a derived class function hides a base class virtual function.
            -Wfloat-equal        # Warn about direct comparisons of floating-point values, which can be imprecise.
        )

    endif()
endif()

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(BUILD_EXAMPLES)

    add_executable(${PROJECT_NAME}_example_insert ${PROJECT_SOURCE_DIR}/examples/example_insert.cpp)
    target_link_libraries(${PROJECT_NAME}_example_insert ${PROJECT_NAME})

    add_executable(${PROJECT_NAME}_example_find ${PROJECT_SOURCE_DIR}/examples/example_find.cpp)
    target_link_libraries(${PROJECT_NAME}_example_find ${PROJECT_NAME})

    add_executable(${PROJECT_NAME}_example_remove ${PROJECT_SOURCE_DIR}/examples/example_remove.cpp)
    target_link_libraries(${PROJECT_NAME}_example_remove ${PROJECT_NAME})
    
    add_executable(${PROJECT_NAME}_example_thread_safety ${PROJECT_SOURCE_DIR}/examples/example_thread_safety.cpp)
    target_link_libraries(${PROJECT_NAME}_example_thread_safety ${PROJECT_NAME} Threads::Threads)
    
    add_executable(${PROJECT_NAME}_example_serialize ${PROJECT_SOURCE_DIR}/examples/example_serialize.cpp)
    target_link_libraries(${PROJECT_NAME}_example_serialize ${PROJECT_NAME})
        
    add_executable(${PROJECT_NAME}_example_complex ${PROJECT_SOURCE_DIR}/examples/example_complex.cpp)
    target_link_libraries(${PROJECT_NAME}_example_complex ${PROJECT_NAME})

endif()

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

if(BUILD_TESTS)

    # CMake has support for adding tests to a project.
    enable_testing()
    
    add_executable(${PROJECT_NAME}_test_ctrie ${PROJECT_SOURCE_DIR}/tests/test_ctrie.cpp)
    target_link_libraries(${PROJECT_NAME}_test_ctrie ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_ctrie_run ${PROJECT_NAME}_test_ctrie)

    add_executable(${PROJECT_NAME}_test_insert_find ${PROJECT_SOURCE_DIR}/tests/test_insert_find.cpp)
    target_link_libraries(${PROJECT_NAME}_test_insert_find ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_insert_find_run ${PROJECT_NAME}_test_insert_find)

    add_executable(${PROJECT_NAME}_test_insert_duplicate ${PROJECT_SOURCE_DIR}/tests/test_insert_duplicate.cpp)
    target_link_libraries(${PROJECT_NAME}_test_insert_duplicate ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_insert_duplicate_run ${PROJECT_NAME}_test_insert_duplicate)

    add_executable(${PROJECT_NAME}_test_remove_key ${PROJECT_SOURCE_DIR}/tests/test_remove_key.cpp)
    target_link_libraries(${PROJECT_NAME}_test_remove_key ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_remove_key_run ${PROJECT_NAME}_test_remove_key)

    add_executable(${PROJECT_NAME}_test_insert_common_prefix ${PROJECT_SOURCE_DIR}/tests/test_insert_common_prefix.cpp)
    target_link_libraries(${PROJECT_NAME}_test_insert_common_prefix ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_insert_common_prefix_run ${PROJECT_NAME}_test_insert_common_prefix)

    add_executable(${PROJECT_NAME}_test_remove_common_prefix ${PROJECT_SOURCE_DIR}/tests/test_remove_common_prefix.cpp)
    target_link_libraries(${PROJECT_NAME}_test_remove_common_prefix ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_remove_common_prefix_run ${PROJECT_NAME}_test_remove_common_prefix)

    add_executable(${PROJECT_NAME}_test_find_nonexistent ${PROJECT_SOURCE_DIR}/tests/test_find_nonexistent.cpp)
    target_link_libraries(${PROJECT_NAME}_test_find_nonexistent ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_find_nonexistent_run ${PROJECT_NAME}_test_find_nonexistent)

    add_executable(${PROJECT_NAME}_test_remove_nonexistent ${PROJECT_SOURCE_DIR}/tests/test_remove_nonexistent.cpp)
    target_link_libraries(${PROJECT_NAME}_test_remove_nonexistent ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_remove_nonexistent_run ${PROJECT_NAME}_test_remove_nonexistent)

    add_executable(${PROJECT_NAME}_test_insert_empty_key ${PROJECT_SOURCE_DIR}/tests/test_insert_empty_key.cpp)
    target_link_libraries(${PROJECT_NAME}_test_insert_empty_key ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_insert_empty_key_run ${PROJECT_NAME}_test_insert_empty_key)

    add_executable(${PROJECT_NAME}_test_find_after_reinsertion ${PROJECT_SOURCE_DIR}/tests/test_find_after_reinsertion.cpp)
    target_link_libraries(${PROJECT_NAME}_test_find_after_reinsertion ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_find_after_reinsertion_run ${PROJECT_NAME}_test_find_after_reinsertion)

    add_executable(${PROJECT_NAME}_test_long_key ${PROJECT_SOURCE_DIR}/tests/test_long_key.cpp)
    target_link_libraries(${PROJECT_NAME}_test_long_key ${PROJECT_NAME})
    add_test(${PROJECT_NAME}_test_long_key_run ${PROJECT_NAME}_test_long_key)

    if(Threads_FOUND)
        add_executable(${PROJECT_NAME}_test_concurrency ${PROJECT_SOURCE_DIR}/tests/test_concurrency.cpp)
        target_link_libraries(${PROJECT_NAME}_test_concurrency ${PROJECT_NAME} Threads::Threads)
        add_test(${PROJECT_NAME}_test_concurrency_run ${PROJECT_NAME}_test_concurrency)
    endif()
endif()

# -----------------------------------------------------------------------------
# CODE ANALYSIS
# -----------------------------------------------------------------------------

if(CLANG_TIDY_EXE)
    file(GLOB_RECURSE PROJECT_HEADERS_AND_SOURCES
        "${PROJECT_SOURCE_DIR}/include/**/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/**/*.hh"
        "${PROJECT_SOURCE_DIR}/include/**/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*.hh"
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/**/*.cc"
        "${PROJECT_SOURCE_DIR}/src/**/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/src/*.c"
    )
    add_custom_target(
        ${PROJECT_NAME}_clang_tidy
        COMMAND ${CLANG_TIDY_EXE}
        --p=${CMAKE_BINARY_DIR}
        ${PROJECT_HEADERS_AND_SOURCES}
        COMMENT "Running clang-tidy"
        VERBATIM
    )
    add_custom_target(
        ${PROJECT_NAME}_clang_tidy_fix
        COMMAND ${CLANG_TIDY_EXE}
        --fix --fix-errors
        --p=${CMAKE_BINARY_DIR}
        ${PROJECT_HEADERS_AND_SOURCES}
        COMMENT "Running clang-tidy-fix"
        VERBATIM
    )
endif()

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

if(DOXYGEN_FOUND)

    # FetchContent: Doxygen Awesome CSS
    FetchContent_Declare(doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG main
    )
    FetchContent_MakeAvailable(doxygenawesome)

    # Hide FetchContent variables to avoid clutter in ccmake.
    mark_as_advanced(FORCE
        FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME
        FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
    )

    # Read the file with the version.
    file(READ ${PROJECT_SOURCE_DIR}/include/ctrie/ctrie.hpp version_file)
    # Extract the version.
    string(REGEX MATCH "CTRIE_MAJOR_VERSION ([0-9]*)" _ ${version_file})
    set(CTRIE_MAJOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "CTRIE_MINOR_VERSION ([0-9]*)" _ ${version_file})
    set(CTRIE_MINOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "CTRIE_MICRO_VERSION ([0-9]*)" _ ${version_file})
    set(CTRIE_MICRO_VERSION ${CMAKE_MATCH_1})

    # Customization: Doxygen Configuration
    set(DOXYGEN_WARN_FORMAT "$file:$line:1: $text")
    set(DOXYGEN_PROJECT_NAME ctrie)
    set(DOXYGEN_PROJECT_BRIEF "A simple CTRIE library for C++")
    set(DOXYGEN_PROJECT_NUMBER "${CTRIE_MAJOR_VERSION}.${CTRIE_MINOR_VERSION}.${CTRIE_MICRO_VERSION}")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_GENERATE_LATEX NO) # Disable LaTeX if not needed
    set(DOXYGEN_GENERATE_MAN NO)   # Disable man pages if not needed

    # Styling and UX enhancements using Doxygen Awesome
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    # Set stricter warnings for better documentation quality
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR NO)

    # Exclude certain files or directories from documentation (if needed)
    set(DOXYGEN_EXCLUDE_PATTERNS "${PROJECT_SOURCE_DIR}/tests/*" "${PROJECT_SOURCE_DIR}/examples/*")

    file(GLOB_RECURSE PROJECT_HEADERS_AND_SOURCES
        "${PROJECT_SOURCE_DIR}/include/**/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/**/*.hh"
        "${PROJECT_SOURCE_DIR}/include/**/*.h"
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/include/*.hh"
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/**/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/**/*.cc"
        "${PROJECT_SOURCE_DIR}/src/**/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/src/*.c"
    )

    # Add Doxygen documentation target.
    doxygen_add_docs(
        ${PROJECT_NAME}_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_HEADERS_AND_SOURCES}
        COMMENT "Generating Doxygen documentation for the library"
    )

endif()
