cmake_minimum_required(VERSION 2.8.0)
project(PiDash C CXX)

# Compiler Variables ###########################################################

# Set Cpp Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable In-Source Builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

# Makefile Settings
set(CMAKE_VERBOSE_MAKEFILE OF)
set(CMAKE_COLOR_MAKEFILE   OFF)

# Find Sources #################################################################

file(GLOB_RECURSE SRC_FILES	"src/*.h"        "src/*.cpp")

# GLAD
set(GLAD_INCLUDE_DIR   "${PROJECT_SOURCE_DIR}/deps/glad/include")
set(GLAD_SRC           "${PROJECT_SOURCE_DIR}/deps/glad/src/glad.c")

# GLM
set(GLM_INCLUDE_DIR    "${PROJECT_SOURCE_DIR}/deps/glm/")

# SOIL
file(GLOB_RECURSE SOIL_SRC_FILES       "${PRJECT_SOURCE_DIR}/deps/soil/src/*.c")
set(SOIL_INCLUDE_DIR   "${PRJECT_SOURCE_DIR}/deps/soil/include")

# SPDLOG
set(SPDLOG_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/deps/spdlog/include")

# Find Dependencies ############################################################

find_package(glfw3    REQUIRED)
find_package(OpenGL   REQUIRED)

# Targets ######################################################################

add_executable(
	${PROJECT_NAME}
	${SRC_FILES}
	${GLAD_SRC}
	${SOIL_SRC_FILES}
)

include_directories(
	${glfw3_INCLUDE_DIR}
	${OpenGL_INCLUDE_DIR}
	${GLAD_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${SPDLOG_INCLUDE_DIR}
	${SOIL_INCLUDE_DIR}
)


if (WIN32)
    target_link_libraries(
		${PROJECT_NAME}
		${GLFW3_LIBRARY}
	)
# Linux
elseif (UNIX AND NOT APPLE)
    target_link_libraries(
		${PROJECT_NAME}
		-lpthread
		-ldl
		glfw
		${OPENGL_LIBRARIES}
	)
elseif (APPLE)
	    target_link_libraries(
		${PROJECT_NAME}
		-lpthread
		-ldl
		"-framework IOKit"
		"-framework AppKit"
		"-framework OpenGL"
		"-framework CoreFoundation"
		"-framework Carbon"
		glfw
	)
endif()
