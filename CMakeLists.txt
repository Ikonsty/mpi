cmake_minimum_required(VERSION 3.16)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(PROJECT_NAME lab5_mpi)

project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too much problem for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing.
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan)
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
set(ENABLE_TSan OFF)
#! MemorySanitizer detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)


#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

set(INCLUDES includes/parser.hpp
            includes/errors.h
            includes/grid.hpp
            includes/diff_scheme.hpp
            includes/helper.hpp)

set(SOURCES sources/parser.cpp
            sources/grid.cpp
            sources/diff_scheme.cpp
            sources/helper.cpp)

#! Project main executable source compilation
add_executable(${PROJECT_NAME} main.cpp ${INCLUDES} ${SOURCES})

#! Put path to your project headers
target_include_directories(${PROJECT_NAME} PRIVATE options_parser)

#! Add external packages
# options_parser requires boost::program_options library

set (MAGICKCORE_QUANTUM_DEPTH, 16)

find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(LibArchive REQUIRED)
set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set (Boost_USE_MULTITHREAD ON)  # enable multithreading
find_package(Boost 1.71.0 COMPONENTS program_options system locale mpi REQUIRED)
find_package(Threads REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)

include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES})
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR} ${LibArchive_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} archive)
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system Boost::locale Boost::mpi)

##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
                $<TARGET_FILE:${PROJECT_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
                DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)
