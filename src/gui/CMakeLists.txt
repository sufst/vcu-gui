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
        appearance/LookAndFeel.cpp
        components/MainComponent.cpp 
        components/TorqueMapComponent.cpp
        components/TabbedComponent.cpp
        menubar/MenuBar.cpp
        windows/AboutWindow.cpp
        windows/MainWindow.cpp
)