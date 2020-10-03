

#####################################################
cmake_minimum_required(VERSION 3.1.0)
project(Octree)
set(CMAKE_CXX_STANDARD 11)

set(GLUT_INCLUDE_DIR ${GLUT_ROOT_PATH} /OpenGL/include/GL)
set(OPENGL_LIBRARY_DIR ${GLUT_ROOT_PATH} /OpenGL/lib)



set(MY_HEADER_FILES
	Octreemejorado.h
)

set(MY_SOURCE_FILES
	main.cpp
)



add_executable(Octree ${MY_HEADER_FILES}
	${MY_SOURCE_FILES})
target_link_libraries(Octree ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})


add_custom_target(openglbinaries
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/freeglut32.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/glew32.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/glew32mx.dll       ${CMAKE_BINARY_DIR}/Debug
  
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/freeglut32.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/glew32.dll       ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${GLUT_ROOT_PATH}/Opengl/bin/glew32mx.dll        ${CMAKE_BINARY_DIR}/Release
)
