###############################################################################
#                   :
#   File            :   CMakeLists.txt
#                   :
#   Author(s)       :   Tim Brewis (@t-bre)
#                   :
#   Description     :   CMakeLists for GUI
#                   :
###############################################################################

target_sources(${TARGET_NAME}
    PRIVATE 
        components/MainComponent.cpp 
        components/TorqueMapComponent.cpp
        menubar/MenuBar.cpp
        windows/AboutWindow.cpp
        windows/MainWindow.cpp
)