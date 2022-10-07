# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.1)

project (hello)

# Add source to this project's executable.
set(NURBS_DIR "src")
set(WRITE_IGES 
		"${NURBS_DIR}/write_iges.h" "${NURBS_DIR}/write_iges.cpp")
add_executable (hello "CMakeProject1.cpp" "CMakeProject1.h"  "main.cpp" ${WRITE_IGES})
target_include_directories(hello PUBLIC ${NURBS_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hello PROPERTY CXX_STANDARD 17)
endif()

# add 3rdParty
# add sisl  https://github.com/SINTEF-Geometry/SISL.git
set(SISL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SISL)
add_subdirectory(SISL)   
target_include_directories(hello PUBLIC "${SISL_SOURCE_DIR}/include")
if(CMAKE_BUILD_TYPE MATCHES Release)
	target_link_libraries(hello PUBLIC "${SISL_SOURCE_DIR}/out/build/x64-release/sisl.lib")
else()
	target_link_libraries(hello PUBLIC "${SISL_SOURCE_DIR}/out/build/x64-debug/sisl.lib")
endif()

# add libIGES  https://github.com/cbernardo/libIGES.git
add_subdirectory(libIGES)  
set(IGES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libIGES")
set(IGES_BUILD_DIR "${IGES_SOURCE_DIR}/out/build")
set(IGES_INSTALL_DIR "${IGES_SOURCE_DIR}/out/install")

if(CMAKE_BUILD_TYPE MATCHES Release)
	link_directories("${IGES_BUILD_DIR}/x64-release/src")
	target_include_directories(hello PUBLIC "${IGES_INSTALL_DIR}/x64-release/include/iges")
	target_link_libraries(hello PUBLIC iges_static)

else()
	link_directories("${IGES_BUILD_DIR}/x64-build/src")
	target_include_directories(hello PUBLIC "${IGES_INSTALL_DIR}/x64-debug/include/iges")
	target_link_libraries(hello PUBLIC iges_static)
endif()
# TODO: Add tests and install targets if needed.
#aaa