#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libmemcached::libmemcachedutil" for configuration "Release"
set_property(TARGET libmemcached::libmemcachedutil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libmemcached::libmemcachedutil PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libmemcachedutil.so.2.0.0"
  IMPORTED_SONAME_RELEASE "libmemcachedutil.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS libmemcached::libmemcachedutil )
list(APPEND _IMPORT_CHECK_FILES_FOR_libmemcached::libmemcachedutil "${_IMPORT_PREFIX}/lib/libmemcachedutil.so.2.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
