cmake_minimum_required ( VERSION 2.6 )
# -------------------------------------------------------------------
# Function 'add_ctags'
# -------------------------------------------------------------------
macro (
            ## Function name 
            add_ctags
            ## Argument list
            target         # Target to depend on
        )

  set_source_files_properties( tags PROPERTIES GENERATED true)

  add_custom_command (
      OUTPUT tags
      COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q ${CMAKE_SOURCE_DIR}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating ctags..."
  )

  add_custom_target ( ctags DEPENDS tags )

  add_dependencies ( ${target} ctags )

endmacro()
# -------------------------------------------------------------------
# Function 'add_test_suite'
# -------------------------------------------------------------------
function(
            ## Function name 
            add_test_suite 
            ## Argument list
            name           # Test name
            source_list    # List of sources
        )

    add_executable ( ${name} ${source_list} )

    add_test ( ${name} ${name} )

    target_link_libraries (  ${name} ${Boost_LIBRARIES} magritlib )

    # Cross-testing win32 tests in linux works if .exe is omitted and wine is present.
    if ( CMAKE_CROSSCOMPILING )
      set_target_properties ( ${name} PROPERTIES SUFFIX "" )
    endif()

endfunction()
# -------------------------------------------------------------------
# MAIN 
# -------------------------------------------------------------------
project ( magrit-client )
set ( magrit_VERSION_MAJOR 0 )
set ( magrit_VERSION_MINOR 1 )
set ( magrit_VERSION_PATCH 0 )
set ( magrit_FULL_VERSION
      "${magrit_VERSION_MAJOR}.${magrit_VERSION_MINOR}.${magrit_VERSION_PATCH}" )
# -------------------------------------------------------------------
# PACKAGING
# -------------------------------------------------------------------
include ( InstallRequiredSystemLibraries )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY
      "Magrit distributed continuous integration" )
set ( CPACK_PACKAGE_VENDOR "magrit.com" )
set ( CPACK_PACKAGE_VERSION_MAJOR "${magrit_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR "${magrit_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH "${magrit_VERSION_PATCH}" )
set ( CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md )
set ( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING" )
set ( CPACK_PACKAGE_FILE_NAME
      "${PROJECT_NAME}-${magrit_FULL_VERSION}" )
set ( CPACK_RPM_PACKAGE_SUMMARY "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}" )
set ( CPACK_RPM_PACKAGE_DESCRIPTION "Magrit distributed continuous integration" )
set ( CPACK_RPM_PACKAGE_LICENSE "Gnu Affero General Public License" )
set ( CPACK_RPM_PACKAGE_VENDOR "magrit.com" )
set ( CPACK_RPM_PACKAGE_GROUP "Development/Tools" )
set ( CPACK_RPM_PACKAGE_REQUIRES "boost >= 1.40.0, boost-devel >= 1.40.0" )
set ( CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog" )
set ( CPACK_GENERATOR "ZIP;RPM" ) 
# -------------------------------------------------------------------
# Common stuff
# -------------------------------------------------------------------
# CMake scripts path
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/conf/ )
set ( CMAKE_CXX_FLAGS
      "-std=c++0x -pedantic -Wall -Werror -Wconversion -Wextra -Wno-unused -I${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CXX_FLAGS}" )
add_definitions ( -DMAGRIT_VERSION=${MAGRIT_VERSION} )
set ( CMAKE_INSTALL_RPATH "\$ORIGIN" )
# -------------------------------------------------------------------
# Dependencies 
# -------------------------------------------------------------------
# * BOOST
find_package ( Boost 1.40.0 REQUIRED COMPONENTS program_options filesystem system unit_test_framework )
include_directories ( ${Boost_INCLUDE_DIRS} )
# -------------------------------------------------------------------
# Targets 
# -------------------------------------------------------------------
#  `---> libmagrit.so / magrit.dll
# -------------------------------------------------------------------
set ( lib_SOURCES
      generic_command.cpp utils.cpp
      build_log.cpp build_send.cpp build_cat.cpp
      wait.cpp
      config.cpp config_add.cpp config_use.cpp config_remove.cpp
      monitor.cpp
      status.cpp
      share.cpp
    )
add_library ( magritlib SHARED ${lib_SOURCES} )
target_link_libraries ( magritlib ${Boost_LIBRARIES} )
set_target_properties ( magritlib PROPERTIES 
                        OUTPUT_NAME magrit 
                        # VERSION ${magrit_FULL_VERSION} 
                      )

# Not working in ubuntu
#add_ctags ( magritlib )

set ( magrit_COMMANDS "magrit-build-log" "magrit-wait-for" "magrit-send-build" "magrit-cat-build" "magrit-monitor" "magrit-status" "magrit-share" "magrit-config" )

# We create a command-line app for each command.
foreach ( CMD ${magrit_COMMANDS} )
  string ( REPLACE "-" "_" CMD_PREPROC_SYMBOL ${CMD} )
  string ( TOUPPER ${CMD_PREPROC_SYMBOL} CMD_PREPROC_SYMBOL ) 

  add_executable ( ${CMD} main.cpp )
  target_link_libraries ( ${CMD} magritlib )
  set_target_properties ( ${CMD} PROPERTIES COMPILE_DEFINITIONS ${CMD_PREPROC_SYMBOL} )
endforeach ()

# -------------------------------------------------------------------
#  `---> tests 
# -------------------------------------------------------------------
enable_testing() 
if ( Boost_FOUND )

    add_test_suite ( MainTestSuite "tests/MainTestSuite.cpp" )

endif ()

set ( WINEPATH )

foreach ( wine_path ${CMAKE_FIND_ROOT_PATH} )
  set ( WINEPATH "${WINEPATH};${wine_path}/bin" )
endforeach ()

add_custom_target(
              check
              COMMAND env "WINEPATH=\"${WINEPATH}\"" ${CMAKE_CTEST_COMMAND} --output-on-failure
              DEPENDS MainTestSuite 
             )
# -------------------------------------------------------------------
# Packaging
# -------------------------------------------------------------------
# TODO: allow RPM/DEB installations
install ( TARGETS
          magritlib ${magrit_COMMANDS} 
          DESTINATION "." COMPONENT maven_install )
# (keep this last)
include ( CPack )


