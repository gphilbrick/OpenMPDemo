cmake_minimum_required( VERSION 3.9 )

project( OpenMPDemo )

add_executable( ${CMAKE_PROJECT_NAME}
	convolve.h
	main.cpp 
	greyscale.h
	savepgm.h
)

find_package( OpenMP REQUIRED )
target_link_libraries( ${CMAKE_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX )

target_include_directories( ${CMAKE_PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)
