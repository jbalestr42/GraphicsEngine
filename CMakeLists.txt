# Find and link a library
function(link_library lib_name lib_path)
	message("- Link lib and copy dlls : ${lib_name}")

	find_library(${lib_name}_LIB ${lib_name} ${lib_path})
	target_link_libraries(GraphicsEngine ${${lib_name}_LIB})
	get_filename_component(${lib_name}_DIR ${${lib_name}_LIB} PATH)

	# Copy all dlls into the executable directory
	file(GLOB files ${${lib_name}_DIR}/*.dll)
	foreach (file ${files})
		add_custom_command(TARGET GraphicsEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${file} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
	endforeach()
endfunction()

# Find and link a library
function(link_library_mac lib_name lib_path)
	message("- Link lib : ${lib_name}")

	find_library(${lib_name}_LIB ${lib_name} ${lib_path})
	target_link_libraries(GraphicsEngine ${${lib_name}_LIB})
	get_filename_component(${lib_name}_DIR ${${lib_name}_LIB} PATH)

	# Copy all dlls into the executable directory
	file(GLOB files ${${lib_name}_DIR}/*.dll)
	foreach (file ${files})
		add_custom_command(TARGET GraphicsEngine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${file} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
	endforeach()
endfunction()

# Define the minimum cmake version
cmake_minimum_required(VERSION 3.0)

# Don't generate the ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(GraphicsEngine CXX)

add_compile_options(-std=c++11)

set(SRCS
	sources/Camera.cpp
	sources/Color.cpp
	sources/DirectionalLight.cpp
	sources/Keyboard.cpp
	sources/Light.cpp
	sources/LightManager.cpp
	sources/main.cpp
	sources/Material.cpp
	sources/Matrix.cpp
	sources/Mesh.cpp
	sources/Model.cpp
	sources/Mouse.cpp
	sources/PointLight.cpp
	sources/Quaternion.cpp
	sources/ResourceManager.cpp
	sources/Shader.cpp
	sources/SpotLight.cpp
	sources/Texture.cpp
	sources/Transformable.cpp
	sources/Vector2.cpp
	sources/Vector3.cpp
	sources/Vector4.cpp
	sources/Vertex.cpp
	sources/Windows.cpp
)

set(HEADERS
	includes/Camera.hpp
	includes/Color.hpp
	includes/DirectionalLight.hpp
	includes/Keyboard.hpp
	includes/Light.hpp
	includes/LightManager.hpp
	includes/Material.hpp
	includes/Matrix.hpp
	includes/Mesh.hpp
	includes/Model.hpp
	includes/Mouse.hpp
	includes/PointLight.hpp
	includes/Quaternion.hpp
	includes/ResourceManager.hpp
	includes/Shader.hpp
	includes/SpotLight.hpp
	includes/Texture.hpp
	includes/Transformable.hpp
	includes/Vector2.hpp
	includes/Vector3.hpp
	includes/Vector4.hpp
	includes/Vertex.hpp
	includes/Windows.hpp
)

#if (WIN32 OR MSVC)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
#elseif (UNIX)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#endif() 

# determine, whether we want a static binary
#SET(STATIC_LINKING FALSE CACHE BOOL "Build a static binary?")

find_package(OpenGL REQUIRED)

add_executable(GraphicsEngine ${SRCS} ${HEADERS})

if (OPENGL_FOUND)
	message("OPENGL FOUND ${OPENGL_gl_LIBRARY}")
	target_include_directories(GraphicsEngine PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(GraphicsEngine ${OPENGL_gl_LIBRARY})
else ()
	message(FATAL_ERROR " OPENGL NOT FOUND")
endif ()

# Add the libraries includes
include_directories(lib/include)

# Link libraries
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("-- Windows detected")
	link_library(assimp "lib/assimp-3.3.1/x86")
	link_library(DevIL "lib/DevIL-1.8.0/x86")
	link_library(ILU "lib/DevIL-1.8.0/x86")
	link_library(ILUT "lib/DevIL-1.8.0/x86")
	link_library(glew32 "lib/glew-2.0.0/x86")
	link_library(glfw3 "lib/glfw-3.2.1/x86/lib-vc2015")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("-- Linux detected")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("-- Darwin detected")
	link_library_mac(libassimp.dylib "lib/assimp-3.3.1/Mac")
	link_library_mac(libIL.dylib "lib/DevIL-1.8.0/Mac")
	link_library_mac(libILU.dylib "lib/DevIL-1.8.0/Mac")
	link_library_mac(libILUT.dylib "lib/DevIL-1.8.0/Mac")
	#link_library_mac(libGLEW.dylib "lib/glew-2.0.0/Mac")
	link_library_mac(libglfw3.dylib "lib/glfw-3.2.1/Mac")
endif()


# static
#link_library(glew32s "lib/glew-2.0.0/Win32")
#link_library(glfw3dll "lib/glfw-3.2.1/x86-vc2015")
#add_definitions(-DGLEW_STATIC)
#add_definitions(-DGLFW_DLL)

# Set the flags
if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wextra")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror")
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
	#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /WX")
endif (MSVC)

# Don't need to use the full path when including a file
target_include_directories(GraphicsEngine PUBLIC includes)
