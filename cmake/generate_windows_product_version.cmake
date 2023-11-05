# Converts version_info.h.in and version.rc.in template files into WIN32
# resource files with version information and general resource strings.
#
# Usage:
#   generate_windows_product_version(product_VERSION_FILES
#     NAME ProjectName
#     ICON icon.ico
#     VERSION_MAJOR 1
#     VERSION_MINOR 2
#     VERSION_PATCH 3
#     VERSION_REVISION 4
#   )
#
#   add_executable(target_name ${target_files} ${product_VERSION_FILES})
#
# Parameters:
#   NAME               - name of executable or dll
#   BUNDLE             - bundle, ${NAME} is default
#   ICON               - path to application icon, ${CMAKE_SOURCE_DIR}/product.ico is default
#   VERSION_MAJOR      - product major version, ${CMAKE_PROJECT_VERSION_MAJOR} is default
#   VERSION_MINOR      - product minor version, ${CMAKE_PROJECT_VERSION_MINOR} is default
#   VERSION_PATCH      - product patch version, ${CMAKE_PROJECT_VERSION_PATCH} is default
#   VERSION_REVISION   - product revision or build number, ${CMAKE_PROJECT_VERSION_TWEAK} is default
#   COMPANY_NAME       - company name, no default
#   COMPANY_COPYRIGHT  - copyright statement, Copyright (C) ${CURRENT_YEAR} ${COMPANY_NAME} is default
#   COMMENTS           - comment, ${NAME} v${VERSION_MAJOR}.${VERSION_MINOR} is default
#   ORIGINAL_FILENAME  - original filename, ${NAME} is default
#   INTERNAL_NAME      - internal name, ${NAME} is default
#   FILE_DESCRIPTION   - file description, ${NAME} is default
function(generate_windows_product_version outfiles)
  set(options)
  set(one_value_args
      NAME
      BUNDLE
      ICON
      VERSION_MAJOR
      VERSION_MINOR
      VERSION_PATCH
      VERSION_REVISION
      COMPANY_NAME
      COMPANY_COPYRIGHT
      COMMENTS
      ORIGINAL_FILENAME
      INTERNAL_NAME
      FILE_DESCRIPTION)
  set(multi_value_args)
  cmake_parse_arguments(PRODUCT "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT DEFINED PRODUCT_BUNDLE OR "${PRODUCT_BUNDLE}" STREQUAL "")
    set(PRODUCT_BUNDLE "${PRODUCT_NAME}")
  endif()
  if(NOT DEFINED PRODUCT_ICON OR "${PRODUCT_ICON}" STREQUAL "")
    set(PRODUCT_ICON "${CMAKE_SOURCE_DIR}/product.ico")
  endif()

  if(NOT DEFINED PRODUCT_VERSION_MAJOR OR "${PRODUCT_VERSION_MAJOR}" STREQUAL "")
    set(PRODUCT_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
  endif()
  if(NOT DEFINED PRODUCT_VERSION_MINOR OR "${PRODUCT_VERSION_MINOR}" STREQUAL "")
    if(NOT DEFINED CMAKE_PROJECT_VERSION_MINOR OR "${CMAKE_PROJECT_VERSION_MINOR}" STREQUAL "")
      set(PRODUCT_VERSION_MINOR 0)
    else()
      set(PRODUCT_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
    endif()
  endif()
  if(NOT DEFINED PRODUCT_VERSION_PATCH OR "${PRODUCT_VERSION_PATCH}" STREQUAL "")
    if(NOT DEFINED CMAKE_PROJECT_VERSION_PATCH OR "${CMAKE_PROJECT_VERSION_PATCH}" STREQUAL "")
      set(PRODUCT_VERSION_PATCH 0)
    else()
      set(PRODUCT_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
    endif()
  endif()
  if(NOT DEFINED PRODUCT_VERSION_REVISION OR "${PRODUCT_VERSION_REVISION}" STREQUAL "")
    if(NOT DEFINED CMAKE_PROJECT_VERSION_TWEAK OR "${CMAKE_PROJECT_VERSION_TWEAK}" STREQUAL "")
      set(PRODUCT_VERSION_REVISION 0)
    else()
      set(PRODUCT_VERSION_REVISION ${CMAKE_PROJECT_VERSION_TWEAK})
    endif()
  endif()

  if(NOT DEFINED PRODUCT_COMPANY_COPYRIGHT OR "${PRODUCT_COMPANY_COPYRIGHT}" STREQUAL "")
    string(TIMESTAMP current_year "%Y")
    set(PRODUCT_COMPANY_COPYRIGHT "Copyright (C) ${current_year} ${PRODUCT_COMPANY_NAME}")
  endif()
  if(NOT DEFINED PRODUCT_COMMENTS OR "${PRODUCT_COMMENTS}" STREQUAL "")
    set(PRODUCT_COMMENTS "${PRODUCT_NAME} v${PRODUCT_VERSION_MAJOR}.${PRODUCT_VERSION_MINOR}")
  endif()
  if(NOT DEFINED PRODUCT_ORIGINAL_FILENAME OR "${PRODUCT_ORIGINAL_FILENAME}" STREQUAL "")
    set(PRODUCT_ORIGINAL_FILENAME "${PRODUCT_NAME}")
  endif()
  if(NOT DEFINED PRODUCT_INTERNAL_NAME OR "${PRODUCT_INTERNAL_NAME}" STREQUAL "")
    set(PRODUCT_INTERNAL_NAME "${PRODUCT_NAME}")
  endif()
  if(NOT DEFINED PRODUCT_FILE_DESCRIPTION OR "${PRODUCT_FILE_DESCRIPTION}" STREQUAL "")
    set(PRODUCT_FILE_DESCRIPTION "${PRODUCT_NAME}")
  endif()

  set(out_version_info_file ${CMAKE_CURRENT_BINARY_DIR}/version_info.h)
  set(out_version_resource_file ${CMAKE_CURRENT_BINARY_DIR}/version_info.rc)
  configure_file(
      ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/version_info.h.in
      ${out_version_info_file}
      @ONLY)
  configure_file(
      ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/version_info.rc.in
      ${out_version_resource_file}
      COPYONLY)
  list(APPEND ${outfiles} ${out_version_info_file} ${out_version_resource_file})
  set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()
