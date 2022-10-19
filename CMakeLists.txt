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
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(VCU-GUI VERSION 1.1.0)
set(TARGET_NAME ${PROJECT_NAME})

# Must define '_USE_MATH_DEFINES' before #include <cmath>.
# See https://learn.microsoft.com/en-us/cpp/c-runtime-library/math-constants?view=msvc-170
# We define it globally here.
add_compile_definitions(_USE_MATH_DEFINES=1)

# OS / architecture specific
if(APPLE)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        # build natively for debug
        set(CMAKE_OSX_ARCHITECTURES "${CMAKE_HOST_SYSTEM_PROCESSOR}" CACHE INTERNAL "" FORCE)
    ELSE()
        # universal binary for release
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE INTERNAL "" FORCE)
    ENDIF()
ENDIF()

# JUCE application setup
add_subdirectory(lib/juce)

juce_add_gui_app(${TARGET_NAME}
    PRODUCT_NAME "VCU GUI"
    COMPANY_NAME "SUFST"
    BUNDLE_ID "com.sufst.vcu-gui"
    ICON_SMALL res/images/AppIcon_512.png
    ICON_LARGE res/images/AppIcon_1024.png
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
        res/images/AppIcon_512.png
        res/images/AppIcon_1024.png
        res/images/Delete.png
        res/fonts/karla/fonts/ttf/static/Karla-Light.ttf
)

# get the commit hash
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)



# compilation
target_compile_definitions(${TARGET_NAME}
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>"
        -DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"
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