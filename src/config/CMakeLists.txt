###############################################################################
#                   :
#   File            :   CMakeLists.txt
#                   :
#   Author(s)       :   Tim Brewis (@t-bre)
#                   :
#   Description     :   CMakeLists for config  model
#                   :
###############################################################################

target_sources(${TARGET_NAME}
    PRIVATE 
        TorqueMap.cpp
        DataModel.cpp
        Metadata.cpp
)