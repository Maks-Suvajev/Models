# QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(ModelsLib
    source/TextureModel.cpp
    source/TextureModel.h
    ${CORE_MOC_FILES}
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

target_link_libraries(ModelsLib
    PUBLIC 
        Qt6::Widgets
        ResourceManagersLib
        DisplaysLib
)

target_include_directories(ModelsLib 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source
)