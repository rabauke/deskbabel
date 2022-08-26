cmake_minimum_required(VERSION 3.21)

project(DeskBabel VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)

set(TS_FILES i18n/DeskBabel_de_DE.ts)

set(PROJECT_SOURCES
        src/main.cc
        src/dictionary.hpp
        src/dictionary.cc
        src/app_model.hpp
        src/app_model.cc
        src/translations_list_model.hpp
        src/translations_list_model.cc
        qml/main.qml
        qml.qrc
        ${TS_FILES}
        )

configure_file(version.hpp.in version.hpp @ONLY)

qt_add_executable(DeskBabel
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )

qt_add_translations(DeskBabel TS_FILES ${TS_FILES})

target_compile_definitions(DeskBabel
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(DeskBabel
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent)

set_target_properties(DeskBabel PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER DeskBabel
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

qt_import_qml_plugins(DeskBabel)
qt_finalize_executable(DeskBabel)

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
