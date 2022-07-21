###############################################################################
#                   :
#   File            :   CMakeLists.txt
#                   :
#   Author(s)       :   Tim Brewis (@t-bre)
#                   :
#   Description     :   Top level CMakeLists for project
#                   :
###############################################################################

cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VCU-GUI VERSION 1.1.0)
set(TARGET_NAME ${PROJECT_NAME})

# JUCE application setup
add_subdirectory(lib/JUCE)

juce_add_gui_app(${TARGET_NAME}
    PRODUCT_NAME "${PROJECT_NAME}"
    COMPANY_NAME "SUFST"
    BUNDLE_ID "com.sufst.${PROJECT_NAME}"
    ICON_SMALL res/AppIcon_512.png
    ICON_LARGE res/AppIcon_1024.png
)

juce_generate_juce_header(${TARGET_NAME})

# source files
target_sources(${TARGET_NAME}
    PRIVATE 
        src/Application.cpp 
        src/Main.cpp
        src/ConfigurationValueTree.cpp
)

target_include_directories(${TARGET_NAME} 
    PRIVATE lib/spline/src
)

add_subdirectory(src/gui)

# binary data
juce_add_binary_data(${TARGET_NAME}Data
    SOURCES 
        res/AppIcon_512.png
        res/AppIcon_1024.png
        res/Delete.png
)

# compilation
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
)

# linking
target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${TARGET_NAME}Data
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_utils
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# symlink compile commands to repo root
execute_process(COMMAND ln -sf ${CMAKE_SOURCE_DIR}/build/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)