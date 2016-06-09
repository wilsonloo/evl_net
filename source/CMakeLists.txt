# CMakeLists for src directory
PROJECT(evlnet)

aux_source_directory(. EVLNET_SRCS)

# shared library
ADD_LIBRARY(evlnet SHARED ${EVLNET_SRCS})

# static library
ADD_LIBRARY(evlnet_static STATIC ${EVLNET_SRCS})
SET_TARGET_PROPERTIES(evlnet_static PROPERTIES OUTPUT_NAME "evlnet")
SET_TARGET_PROPERTIES(evlnet PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(evlnet_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
