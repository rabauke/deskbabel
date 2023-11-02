configure_file(version.hpp.in version.hpp @ONLY)

add_subdirectory(DeskBabelQuick)

qt_add_executable(DeskBabel WIN32 main.cpp)

target_compile_definitions(DeskBabel
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(DeskBabel PRIVATE Qt::Core Qt::Quick)
target_include_directories(DeskBabel PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

qt_add_qml_module(DeskBabel
    URI DeskBabelApp
    VERSION 1.0
    QML_FILES qml/main.qml
    RESOURCES qtquickcontrols2.conf
    NO_RESOURCE_TARGET_PATH
    )

qt_import_plugins(DeskBabel INCLUDE DeskBabelPlugin)

set_target_properties(DeskBabel PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER DeskBabel
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )

qt_add_translations(DeskBabel TS_FILES i18n/DeskBabel_de_DE.ts)

install(TARGETS DeskBabel
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_property(INSTALL "bin/$<TARGET_FILE_NAME:DeskBabel>" PROPERTY CPACK_START_MENU_SHORTCUTS "DeskBabel")

qt_generate_deploy_qml_app_script(
    TARGET DeskBabel
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
  if(RPMBUILD_FOUND)
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
  else()
    set(CPACK_GENERATOR "TGZ;DEB")
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
  set(CPACK_GENERATOR "ZIP;WIX")
else()
  set(CPACK_GENERATOR "ZIP")
endif()

configure_file(CPackProjectConfig.txt.in CPackProjectConfig.txt)
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackProjectConfig.txt")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Heiko Bauke")

set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_VERBATIM_VARIABLES true)

include(CPack)
