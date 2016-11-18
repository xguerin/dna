file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
add_library(dna_hal_soclib_arm_v6 OBJECT ${SOURCES})
