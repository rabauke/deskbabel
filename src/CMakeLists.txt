set(TS_FILES i18n/DeskBabel_de_DE.ts)

set(QML_FILES qml/main.qml)

set(PROJECT_SOURCES
    main.cpp
    dictionary.hpp
    dictionary.cpp
    app_model.hpp
    app_model.cpp
    translations_list_model.hpp
    translations_list_model.cpp
    )

configure_file(version.hpp.in version.hpp @ONLY)

qt_add_executable(DeskBabel ${PROJECT_SOURCES})

qt_add_translations(DeskBabel TS_FILES ${TS_FILES})

target_compile_definitions(DeskBabel
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(DeskBabel PRIVATE Qt::Core Qt::Quick Qt::Concurrent)
target_include_directories(DeskBabel PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(DeskBabel PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER DeskBabel
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )

qt_add_qml_module(DeskBabel
    URI DeskBabelQuick
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCES qtquickcontrols2.conf
    NO_RESOURCE_TARGET_PATH
    )

install(TARGETS DeskBabel
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET DeskBabel
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME "DeskBabel")
# set(CPACK_PACKAGE_VENDOR "vendor")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_CONTACT Heiko Bauke)

include(CPack)
