# the whole CMakeLists.txt for project evl_net
PROJECT(evlnet)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")                                                           

set (CMAKE_CXX_STANDARD 11)
                                                                                                                
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")                                                                           
    MESSAGE(STATUS "current platform: Linux ")                                                                   
    ADD_DEFINITIONS(-D_PF_LINUX_)                                                                               
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")                                                                     
    MESSAGE(STATUS "current platform: Windows")        
    ADD_DEFINITIONS(-D_PF_WINDOWS_)            
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
	ADD_DEFINITIONS(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	
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
    
	${OTHER_INCLUDE_DIRS}
)


# lib directories
LINK_DIRECTORIES(
    ${PROJECT_BINARY_DIR}/lib

	${OTHER_LIBRARY_DIRS}
)

SET(EXECUTABLE_OTUPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
INSTALL(DIRECTORY include/ DESTINATION include) 
INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/libevlnet.a
    ${LIBRARY_OUTPUT_PATH}/libevlnet.so DESTINATION lib)                                   

ADD_SUBDIRECTORY(source)
