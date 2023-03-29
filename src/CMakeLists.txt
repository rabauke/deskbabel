set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)

set(TS_FILES i18n/DeskBabel_de_DE.ts)

set(PROJECT_SOURCES
    main.cc
    dictionary.hpp
    dictionary.cc
    app_model.hpp
    app_model.cc
    translations_list_model.hpp
    translations_list_model.cc
    qml/main.qml
    application.qrc
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
    NO_RESOURCE_TARGET_PATH
    )

install(TARGETS DeskBabel DESTINATION ${CMAKE_INSTALL_BINDIR})
# install(IMPORTED_RUNTIME_ARTIFACTS DeskBabel DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(CPACK_PACKAGE_NAME "DeskBabel")
# set(CPACK_PACKAGE_VENDOR "vendor")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_CONTACT Heiko Bauke)

include(CPack)
