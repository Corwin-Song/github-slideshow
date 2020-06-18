#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HddlUnite::HddlUnite" for configuration "Release"
set_property(TARGET HddlUnite::HddlUnite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(HddlUnite::HddlUnite PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libHddlUnite.so.1.1.0"
  IMPORTED_SONAME_RELEASE "libHddlUnite.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS HddlUnite::HddlUnite )
list(APPEND _IMPORT_CHECK_FILES_FOR_HddlUnite::HddlUnite "${_IMPORT_PREFIX}/lib/libHddlUnite.so.1.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
