# the whole CMakeLists.txt for project evl_net
PROJECT(evlnet)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)


MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")                                                           
                                                                                                                
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")                                                                           
    MESSAGE(STATUS "current platform: Linux ")                                                                   
    ADD_DEFINITIONS(-D_PF_LINUX_)                                                                               
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")                                                                     
    MESSAGE(STATUS "current platform: Windows")                                                                 
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")                                                                     
    MESSAGE(STATUS "current platform: FreeBSD")                                                                 
ELSE ()                                                                                                         
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")                                                       
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")                                                                       
                                                                                                                
MESSAGE(STSTUS "###################################")
ADD_DEFINITIONS(-DEVL_NET_LIBRARY_SOURCE)

# include directories
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../ # for third_party
    ${PROJECT_SOURCE_DIR}/include    
    ${PROJECT_SOURCE_DIR}/include/detail
    
    /usr/local/boost_1_61_0
    /usr/local/include
    /usr/include
)


# lib directories
LINK_DIRECTORIES(
    ${PROJECT_BINARY_DIR}/lib

    /usr/local/lib
    /usr/lib
)

SET(EXECUTABLE_OTUPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/libevlnet.a
    ${LIBRARY_OUTPUT_PATH}/libevlnet.so DESTINATION lib/evlnet)                                   
INSTALL(DIRECTORY include/ DESTINATION include) 

ADD_SUBDIRECTORY(source)
