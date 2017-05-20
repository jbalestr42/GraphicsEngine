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

# Find and link a library for mac
function(link_library_mac lib_name lib_path)
	message("- Link lib : ${lib_name}")

	find_library(${lib_name}_LIB ${lib_name} ${lib_path})
	target_link_libraries(GraphicsEngine ${${lib_name}_LIB})
endfunction()

# Define the minimum cmake version
cmake_minimum_required(VERSION 3.0)

# Change the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set the project name
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
	includes/OpenGL.hpp
)

#if (WIN32 OR MSVC)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
#elseif (UNIX)
#    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#endif() 

# Set the executable name
add_executable(GraphicsEngine ${SRCS} ${HEADERS})

# Add the libraries includes
include_directories(lib/include)

# Don't need to use the full path when including a file
target_include_directories(GraphicsEngine PUBLIC includes)

# Link libraries
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("-- Windows detected")
	link_library(assimp "lib/assimp-3.3.1/x86")
	link_library(DevIL "lib/DevIL-1.8.0/x86")
	link_library(ILU "lib/DevIL-1.8.0/x86")
	link_library(ILUT "lib/DevIL-1.8.0/x86")
	link_library(glew32 "lib/glew-2.0.0/x86")
	link_library(glfw3 "lib/glfw-3.2.1/x86/lib-vc2015")
	add_definitions(-DWIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("-- Linux detected")
	add_definitions(-DLINUX)
	link_library_mac(libassimp.so "lib/assimp-3.3.1/Linux")
	link_library_mac(libIL.so "lib/DevIL-1.8.0/Linux")
	link_library_mac(libILU.so "lib/DevIL-1.8.0/Linux")
	link_library_mac(libILUT.so "lib/DevIL-1.8.0/Linux")
	link_library_mac(libGLEW.so "lib/glew-2.0.0/Linux")
	link_library_mac(libglfw.so "lib/glfw-3.2.1/Linux")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("-- Darwin detected")
	add_definitions(-DMACOS)
	link_library_mac(libassimp.dylib "lib/assimp-3.3.1/Mac")
	link_library_mac(libIL.dylib "lib/DevIL-1.8.0/Mac")
	link_library_mac(libILU.dylib "lib/DevIL-1.8.0/Mac")
	link_library_mac(libILUT.dylib "lib/DevIL-1.8.0/Mac")
	link_library_mac(libglfw3.dylib "lib/glfw-3.2.1/Mac")
endif()

# Link OpenGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	message("- Link OpenGL : ${OPENGL_gl_LIBRARY}")
	target_include_directories(GraphicsEngine PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(GraphicsEngine ${OPENGL_gl_LIBRARY})
else ()
	message(FATAL_ERROR " : OpenGL not found !")
endif ()

# Set the flags
if (CMAKE_COMPILER_IS_GNUCC)
	message("- GNU detected")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wextra")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror")
elseif (MSVC)
	message("- MSVC detected")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /WX")

	# Prevent link warning in mvc
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib")

	# Don't generate the ZERO_CHECK project
	set(CMAKE_SUPPRESS_REGENERATION true)

	# Remove false warnings about std::copy and std::move
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif ()
