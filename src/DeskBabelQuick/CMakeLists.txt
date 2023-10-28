
qt_policy(SET QTP0001 NEW)
qt_add_qml_module(DeskBabelPlugin
    URI DeskBabelQuick
    PLUGIN_TARGET DeskBabelPlugin
    DEPENDENCIES QtQuick
)

target_sources(DeskBabelPlugin PRIVATE
    dictionary.hpp
    dictionary.cpp
    app_model.hpp
    app_model.cpp
    translations_list_model.hpp
    translations_list_model.cpp
)

target_compile_definitions(DeskBabelPlugin
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(DeskBabelPlugin PRIVATE Qt::Core Qt::Quick Qt::Concurrent)
target_include_directories(DeskBabelPlugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
