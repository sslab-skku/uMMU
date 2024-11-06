# add_library(TaintAnalysis MODULE TaintAnalysis.cpp)

add_library(VMIPass MODULE
  AnnoCMITransform.cpp LibcTransform.cpp InstSecret.cpp PropSecret.cpp Stats.cpp ExtractAnnotation.cpp ${SVF_LIB})
target_link_libraries(VMIPass
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" ${SVF_LIB})

# add_library(ExtractAnnotation MODULE ExtractAnnotation.cpp ${SVF_LIB})
# target_link_libraries(ExtractAnnotation
#   "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" ${SVF_LIB})


# add_library(InstSecret MODULE InstSecret.cpp ${SVF_LIB})
# target_link_libraries(InstSecret
#   "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" ${SVF_LIB})

# add_library(PropSecret MODULE PropSecret.cpp ${SVF_LIB})
# target_link_libraries(PropSecret
#   "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" ${SVF_LIB})

# add_library(Stats MODULE Stats.cpp)
# target_link_libraries(Stats
#   "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>" ${SVF_LIB})

