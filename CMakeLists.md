set(GLFW_PATH "/opt/homebrew/Cellar/glfw/3.3.8")
set(GLM_PATH "/opt/homebrew/Cellar/glm/0.9.9.8/include/glm")
set(VULKAN_SDK_PATH "/Users/mv-mac/VulkanSDK/1.3.243.0/macOS")
 
cmake_minimum_required(VERSION 3.11.0)
 
set(NAME LveEngine)
 
message(STATUS "using ${CMAKE_GENERATOR}")

#IF(APPLE)
#  SET(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)
#ENDIF(APPLE)

project(${NAME} VERSION 0.23.0)

#set(CMAKE_OSX_ARCHITECTURES "arm64"
  #  CACHE STRING "The list of target architectures to build")

# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# 2. Set GLFW_PATH in .env.cmake to target specific glfw

set(GLFW_INCLUDE_DIRS "/opt/homebrew/Cellar/glfw/3.3.8/include/GLFW")
set(GLFW_LIB "/opt/homebrew/Cellar/glfw/3.3.8/lib/cmake/glfw3")
find_package(glfw3 3.3 REQUIRED) #3.3
set(GLFW_LIB "glfw")
message(STATUS "Found GLFW")
if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find glfw library!")
else()
    message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()
 
#include_directories(external)
 
# If TINYOBJ_PATH not specified in .env.cmake, try fetching from git repo
#if (NOT TINYOBJ_PATH)
#  message(STATUS "TINYOBJ_PATH not specified in .env.cmake, using external/tinyobjloader")
#  set(TINYOBJ_PATH external/tinyobjloader)
#endif()

file(GLOB_RECURSE CPP_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HPP_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)

# Build project, give it a name and includes list of file to be compiled
add_executable(${PROJECT_NAME} ${CPP_FILES} ${HPP_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
 
if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()
 
  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}
    )
 
  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )
 
  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
    )
    target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
endif() 

############## Build SHADERS #######################
 
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/bin
  ${VULKAN_SDK_PATH}/bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)
 
# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)
 
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)
 
add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)