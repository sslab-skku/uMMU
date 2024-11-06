set(VMI_SOURCES
  # vmi_page.c vmi_page_swap.c
  # tinyalloc.c
  rasm_dirjmp.asm
  rasm_indjmp.asm rasm_cmov.asm rasm_perm.asm

  rasm_sse2_support.asm
  vmi_zmm.asm vmi_zmm_fast.asm vmi_linear.asm
  #aes_zmm3.zsm
  aes.asm fake_aes.asm
  oram.c
  rdrand.asm
)

# CMI Library
function(add_cmi_library CMI_NAME)
  cmake_parse_arguments(CMI "" "" "SRCS;DEFS" ${ARGN} )
  if(NOT CMI_DEFS)
    set(CMI_DEFS "")
  endif()
  # Add Library
  add_library(${CMI_NAME} STATIC ${CMI_SRCS} ${VMI_SOURCES})
  add_library(${CMI_NAME}_nosse STATIC ${CMI_SRCS} ${VMI_SOURCES})
  add_library(${CMI_NAME}_sse STATIC ${CMI_SRCS} ${VMI_SOURCES})
  add_library(${CMI_NAME}_sse2 STATIC ${CMI_SRCS} ${VMI_SOURCES})


  target_compile_definitions(${CMI_NAME} PRIVATE ${CMI_DEFS} RASM_NOSSE_SUPPORT)
  target_compile_definitions(${CMI_NAME}_nosse PRIVATE ${CMI_DEFS} RASM_NOSSE_SUPPORT)
  target_compile_definitions(${CMI_NAME}_sse PRIVATE ${CMI_DEFS} RASM_SSE_SUPPORT)
  target_compile_definitions(${CMI_NAME}_sse2 PRIVATE ${CMI_DEFS} RASM_SSE2_SUPPORT)

  # Add Tests
  foreach(X ${CMI_NAME}; ${CMI_NAME}_sse2; ${CMI_NAME}_sse; ${CMI_NAME}_nosse;)
    add_executable("test_${X}" tests/test_cmi.c "${VMI_SOURCES}")
    target_link_libraries("test_${X}" ${X})
    add_executable("test_simple_${X}" tests/test_simple.c "${VMI_SOURCES}")
    target_link_libraries("test_simple_${X}" ${X})
  endforeach()
endfunction()


add_cmi_library(cmi_base SRCS cmi_base.c)
add_cmi_library(cmi_xor2 SRCS cmi_xor2.c)
add_cmi_library(cmi_aes SRCS cmi_aes.c)

add_cmi_library(cmi_rdrand SRCS cmi_rdrand.c)

add_executable(test_cmi_dummy tests/test_cmi.c)
target_compile_definitions(test_cmi_dummy PRIVATE TEST_CMI_USE_DUMMY)
target_link_libraries(test_cmi_dummy cmi_base)

add_cmi_library(cmi_rasm_perm_cmov
  SRCS cmi_aligend.c
  DEFS ALIGEND_LOAD=rasm_perm_load64 ALIGEND_STORE=rasm_cmov_store64)

add_cmi_library(cmi_rasm_perm_indjmp
  SRCS cmi_aligend.c
  DEFS ALIGEND_LOAD=rasm_perm_load64 ALIGEND_STORE=rasm_indjmp_store64)

add_cmi_library(cmi_rasm_indjmp
  SRCS cmi_aligend.c
  DEFS ALIGEND_LOAD=rasm_indjmp_load64 ALIGEND_STORE=rasm_indjmp_store64)

add_cmi_library(cmi_rasm_dirjmp
  SRCS cmi_aligend.c
  DEFS ALIGEND_LOAD=rasm_dirjmp_load64 ALIGEND_STORE=rasm_dirjmp_store64)

add_cmi_library(cmi_rasm_cmov
  SRCS cmi_aligend.c
  DEFS ALIGEND_LOAD=rasm_cmov_load64 ALIGEND_STORE=rasm_cmov_store64)


# add_cmi_library(cmi_zmm_vmov SRCS cmi_zmm.c DEFS CMI_ZMM_USE_VMOV)
# add_cmi_library(cmi_zmm_perm SRCS cmi_zmm.c DEFS CMI_ZMM_USE_PERM)
# add_cmi_library(cmi_zmm_perm_indjmp SRCS cmi_zmm.c DEFS CMI_ZMM_USE_PERM_INDJMP)
# add_cmi_library(cmi_zmm_indjmp SRCS cmi_zmm.c DEFS CMI_ZMM_USE_INDJMP)

# Page cmi library
foreach(X RANGE 6 8)
  foreach(Y rasm_cmov;rasm_indjmp;rasm_perm_cmov;rasm_perm_indjmp)
    string(TOUPPER "VMI_PAGE_USE_${Y}" DEF1)
    add_cmi_library(cmi_page${X}_${Y}_base
      SRCS cmi_page.c vmi_page.c vmi_page_swap_base.c
      DEFS ${DEF1} VA_OFF_LEN=${X} VA_VPN_LEN=16
    )
    add_cmi_library(cmi_page${X}_${Y}_dummy
      SRCS cmi_page.c vmi_page.c vmi_page_swap_dummy.c
      DEFS ${DEF1} VA_OFF_LEN=${X} VA_VPN_LEN=16
    )
  endforeach()
endforeach()

# For stat
add_cmi_library(cmi_page6_base_base
  SRCS cmi_page.c vmi_page.c vmi_page_swap_base.c
  DEFS VMI_PAGE_USE_BASE VA_OFF_LEN=6 PPN_BASE=0 VA_VPN_LEN=16)

add_cmi_library(cmi_page6_base_base_stats
  SRCS cmi_page.c vmi_page.c vmi_page_swap_base.c
  DEFS VMI_PAGE_USE_BASE VA_OFF_LEN=6 PPN_BASE=3 VA_VPN_LEN=16 PUBLIC VMI_MEASURE_STATS)

# RDRAND
add_cmi_library(cmi_page6_rdrand
  SRCS cmi_page.c vmi_page.c vmi_page_swap_rdrand.c
  DEFS
  #VMI_PAGE_USE_RASM_INDJMP
  VMI_PAGE_USE_RASM_PERM_CMOV
  #VMI_OBLIV_PAGE_LOOKUP
  VA_OFF_LEN=6 PPN_BASE=0 VA_VPN_LEN=16)

# AES
# add_cmi_library(cmi_page6_rasm_perm_indjmp_aes
#   SRCS cmi_page.c vmi_page.c vmi_page_swap_aes.c
#   DEFS VMI_PAGE_USE_RASM_INDJMP  VA_OFF_LEN=6 PPN_BASE=3 VA_VPN_LEN=16)

add_cmi_library(cmi_page6_rasm_indjmp_aes
  SRCS cmi_page.c vmi_page.c vmi_page_swap_aes.c
  DEFS VMI_PAGE_USE_RASM_INDJMP VA_OFF_LEN=6 PPN_BASE=3 VA_VPN_LEN=16)

add_cmi_library(cmi_page6_rasm_perm_indjmp_aes
  SRCS cmi_page.c vmi_page.c vmi_page_swap_aes.c
  DEFS VMI_PAGE_USE_RASM_PERM_INDJMP VA_OFF_LEN=6 PPN_BASE=3 VA_VPN_LEN=20)


# add_cmi_library(cmi_page6_rasm_indjmp_aes_clock
#   SRCS cmi_page.c vmi_page.c vmi_page_swap_aes.c
#   DEFS VMI_PAGE_USE_RASM_INDJMP VA_OFF_LEN=6 PPN_BASE=3 VA_V VMI_PAGE_USE_CLOCK)


# ORAM
foreach(L RANGE 1 13) # ~ 2MB
  add_cmi_library(cmi_oramB64Z4L${L} SRCS cmi_oram.c oram.c
    DEFS ORAM_BLOCK_SIZE=64 ORAM_Z=4 ORAM_L=${L} ORAM_STASH_SIZE=2000
  )
endforeach()

# PAGE + ORAM
foreach(X 6;7)
  foreach(L RANGE 1 6) # ~ 2^16B
    add_cmi_library(cmi_page${X}_oramB64Z4L${L} SRCS cmi_page.c vmi_page.c vmi_page_swap_oram.c
      DEFS VA_OFF_LEN=${X} VMI_PAGE_USE_RASM_PERM_CMOV VMI_OBLIV_PAGE_LOOKUP VA_VPN_LEN=16
      ORAM_BLOCK_SIZE=64 ORAM_Z=4 ORAM_L=${L}
    )
  endforeach()
  foreach(L RANGE 7 13) # ~ 2^21B (2MB)
    add_cmi_library(cmi_page${X}_oramB64Z4L${L} SRCS cmi_page.c vmi_page.c vmi_page_swap_oram.c
      DEFS VA_OFF_LEN=${X} VMI_PAGE_USE_RASM_PERM_CMOV VMI_OBLIV_PAGE_LOOKUP VA_VPN_LEN=16
      ORAM_BLOCK_SIZE=64 ORAM_Z=4 ORAM_L=${L}
    )
  endforeach()
  # Measure Sats
  foreach(L RANGE 1 6) # ~ 2^16B
    add_cmi_library(cmi_page${X}_stats_oramB64Z4L${L} SRCS cmi_page.c vmi_page.c vmi_page_swap_oram.c
      DEFS VA_OFF_LEN=${X} VMI_PAGE_USE_RASM_PERM_CMOV VMI_OBLIV_PAGE_LOOKUP VA_VPN_LEN=16 PUBLIC VMI_MEASURE_STATS
      ORAM_BLOCK_SIZE=64 ORAM_Z=4 ORAM_L=${L}
    )
  endforeach()
  foreach(L RANGE 7 13) # ~ 2^21B (2MB)
    add_cmi_library(cmi_page${X}_stats_oramB64Z4L${L} SRCS cmi_page.c vmi_page.c vmi_page_swap_oram.c
      DEFS VA_OFF_LEN=${X} VMI_PAGE_USE_RASM_PERM_CMOV VMI_OBLIV_PAGE_LOOKUP VA_VPN_LEN=16 PUBLIC VMI_MEASURE_STATS
      ORAM_BLOCK_SIZE=64 ORAM_Z=4 ORAM_L=${L}
    )
  endforeach()
endforeach()

