cmake_minimum_required(VERSION 3.24)
project(HelloWorldJuce VERSION 1.0.0)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
endif()

set(CMAKE_CXX_STANDARD 20)

project(MyProjectName VERSION 1.0.0)

add_executable(HelloWorldJuce main.cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(JUCE_examples main.cpp)

# ==============================================================================
#
#  This file is part of the JUCE library.
#  Copyright (c) 2022 - Raw Material Software Limited
#
#  JUCE is an open source library subject to commercial or open-source
#  licensing.
#
#  By using JUCE, you agree to the terms of both the JUCE 7 End-User License
#  Agreement and JUCE Privacy Policy.
#
#  End User License Agreement: www.juce.com/juce-7-licence
#  Privacy Policy: www.juce.com/juce-privacy-policy
#
#  Or: You may also use this code under the terms of the GPL v3 (see
#  www.gnu.org/licenses).
#
#  JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
#  EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
#  DISCLAIMED.
#
# ==============================================================================

if (WIN32)
    add_subdirectory("C://JUCE" JUCE)
    add_subdirectory("C://JUCE//extras" extras)
else ()
    find_package(JUCE CONFIG REQUIRED)
endif ()

set(SUBDIRS
        AudioPlugin
        ConsoleApp
        GuiApp
        SystemLib
)

foreach (subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach ()