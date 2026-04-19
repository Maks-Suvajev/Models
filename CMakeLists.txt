# QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(ModelsLib
    source/Texture/TextureModel.cpp
    source/Texture/TextureModel.h
    source/Shader/ShaderSourceModel.cpp
    source/Shader/ShaderSourceModel.h
    source/Shader/ShaderCompiledModel.cpp
    source/Shader/ShaderCompiledModel.h
    source/Material/MaterialModel.h
    source/Material/MaterialModel.cpp
    source/SceneModel/SceneModelModel.cpp
    source/SceneModel/SceneModelModel.h
    ${CORE_MOC_FILES}
)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

target_link_libraries(ModelsLib
    PUBLIC 
        Qt6::Widgets
        ResourceManagersLib
        UICommonLib
)

target_include_directories(ModelsLib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Texture
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Shader
        ${CMAKE_CURRENT_SOURCE_DIR}/source/Material
        ${CMAKE_CURRENT_SOURCE_DIR}/source/SceneModel
)