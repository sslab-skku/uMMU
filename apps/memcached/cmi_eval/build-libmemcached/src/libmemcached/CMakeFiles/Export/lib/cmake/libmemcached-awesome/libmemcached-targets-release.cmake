#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcached" for configuration "Release"
set_property(TARGET libmemcached::libmemcached APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcached PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmemcached.so.11.0.0"
  IMPORTED_SONAME_RELEASE "libmemcached.so.11"
  )

list(APPEND _IMPORT_CHECK_TARGETS libmemcached::libmemcached )
list(APPEND _IMPORT_CHECK_FILES_FOR_libmemcached::libmemcached "${_IMPORT_PREFIX}/lib/libmemcached.so.11.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
