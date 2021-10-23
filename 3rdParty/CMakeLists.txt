add_library(tinyxml2 tinyxml2/tinyxml2.cpp)

target_include_directories(tinyxml2 PUBLIC
    ${CMAKE_SOURCE_DIR}/3rdParty
)

set_target_properties(tinyxml2 PROPERTIES
    DEBUG_POSTFIX "d"
    EXCLUDE_FROM_ALL TRUE
    FOLDER "ExternalDependencies"
)
