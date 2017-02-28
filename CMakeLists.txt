cmake_minimum_required (VERSION 3.0.2) 

include (GNUInstallDirs)
include (FindPkgConfig)
 
set (PROJECT graph-generator)
set (VERSION 1.0.0)

 
project (${PROJECT} C)

file (GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.c")
file (GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "src/*.h")

# pkg_search_module (LIBFCGI REQUIRED libfcgi)

set (LIBRARIES fcgi pthread)


# set (INCLUDES)


#BUILD
	
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
# set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -export-dynamic")

add_definitions (-DPACKAGE_VERSION="${VERSION}")
add_definitions (-DPACKAGE_NAME="${PROJECT}")

include_directories (${INCLUDES})
add_executable (${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBRARIES})


#INSTALL

install (TARGETS ${PROJECT} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install (FILES res/graph-generator-cpu.service DESTINATION /${CMAKE_INSTALL_SYSCONFDIR}/systemd/system/)
install (FILES res/graph-generator-temp.service DESTINATION /${CMAKE_INSTALL_SYSCONFDIR}/systemd/system/)
install (FILES res/index.html DESTINATION /${CMAKE_INSTALL_LOCALSTATEDIR}/www/statistics/)
install (FILES res/grid.svg DESTINATION /${CMAKE_INSTALL_LOCALSTATEDIR}/www/statistics/)
