set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)
qt_standard_project_setup()

set(TS_FILES i18n/DeskBabel_de_DE.ts)

set(PROJECT_SOURCES
    main.cpp
    dictionary.hpp
    dictionary.cpp
    app_model.hpp
    app_model.cpp
    translations_list_model.hpp
    translations_list_model.cpp
    qml/main.qml
    ${TS_FILES}
    )

configure_file(version.hpp.in version.hpp @ONLY)

qt_add_executable(DeskBabel ${PROJECT_SOURCES})

qt_add_translations(DeskBabel TS_FILES ${TS_FILES})

target_compile_definitions(DeskBabel
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(DeskBabel PRIVATE Qt::Core Qt::Quick Qt::Concurrent)

set_target_properties(DeskBabel PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER DeskBabel
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )

qt_add_qml_module(DeskBabel
    URI DeskBabelQuick
    VERSION 1.0
    QML_FILES qml/main.qml
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
