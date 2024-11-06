#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcachedprotocol" for configuration "Release"
set_property(TARGET libmemcached::libmemcachedprotocol APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcachedprotocol PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmemcachedprotocol.so.0.0.0"
  IMPORTED_SONAME_RELEASE "libmemcachedprotocol.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS libmemcached::libmemcachedprotocol )
list(APPEND _IMPORT_CHECK_FILES_FOR_libmemcached::libmemcachedprotocol "${_IMPORT_PREFIX}/lib/libmemcachedprotocol.so.0.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
