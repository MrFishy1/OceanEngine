cmake_minimum_required(VERSION 3.11...3.22)
cmake_policy(VERSION 3.11)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

option(CMAKE_VERBOSE_MAKEFILE "Use CMake verbose make file." ON)
option(Ocean_DOCS "Generate Ocean Engine documentation target." ON)

project("Ocean Engine")

# Finds the system it is on. for fun and debugging if needed
string(TIMESTAMP time)
message(STATUS "TIME OF BUILD: ${time}")
if(WIN32)
    message(STATUS "System: Windows")
elseif(APPLE)
    message(STATUS "System: MACOS")
elseif(UNIX)
    message(STATUS "System: Linux/Unix")
else()
    message(WARNING "OS Assessment: Unknown platform. Status: Disastrous.")
endif()

# List out found info.
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Tries to list the compiler, otherwise throw error.
if(CMAKE_CXX_COMPILER)
    message(STATUS "Compiler Located: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Compiler Reassessment: Compiler not found. Status: Calamitous.")
endif()

if (MSVC)
    message(STATUS)
    message(FATAL_ERROR "Ocean Only Supports GCC Compilers!")
endif ()

### OpenGL ###
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL NOT Found.")
else()
    message(STATUS "OpenGL Libraries Found.")
endif ()

### Vulkan ###
set(Vulkan_FOUND False)
find_package(Vulkan REQUIRED)

if (DEFINED ${VULKAN_SDK_PATH})
    set(Vulkan_FOUND True)
endif()

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan descent protocol: Active. Utter failure of Building: Imminent.")
else()
    message(STATUS "Vulkan Libraries Found.")
endif()

### Shaders ###
find_program(GLSL_VALIDATOR glslangValidator HINTS
    ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
    /usr/bin 
    /usr/local/bin 
    ${VULKAN_SDK_PATH}/Bin
    ${VULKAN_SDK_PATH}/Bin32
    $ENV{VULKAN_SDK}/Bin/ 
    $ENV{VULKAN_SDK}/Bin32/
)

file(GLOB Ocean_GLSL_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Shaders/*.vert"
)

foreach(GLSL ${Ocean_GLSL_SOURCE})
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

### Library Type ###
option(BUILD_SHARED_LIBS "Building with shared libraries..." OFF)

### Output ###
if (BUILD_SHARED_LIBS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
endif()

### Directories ###
set(OCEAN_SRC_DIRECTORY    "${PROJECT_SOURCE_DIR}/Ocean/src")

set(GLFW_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/glfw/include")
set(GLM_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/glm")
set(TLSF_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/tlsf")
set(STEAM_AUDIO_DIRECTORY  "${PROJECT_SOURCE_DIR}/Ocean/vendor/steamaudio/include")
set(GLAD_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/glad/include")
set(STB_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/stb")


### Projects ###
message(STATUS)
message(STATUS "Passing off to Bob the builder")
message(STATUS)

# GLFW
string(TIMESTAMP time "%M:%S")
message(STATUS "GLFW Starting (${time})...")
add_subdirectory(Ocean/vendor/glfw EXCLUDE_FROM_ALL)
message(STATUS)


# GLM
string(TIMESTAMP time "%M:%S")
message(STATUS "GLM Starting (${time})...")
if (BUILD_SHARED_LIBS)
    set(GLM_BUILD_LIBRARY ON)
else()
    set(GLM_BUILD_LIBRARY OFF)
endif()
set(GLM_ENABLE_CXX_17 ON)
set(GLM_BUILD_TESTS OFF)
add_subdirectory(Ocean/vendor/glm EXCLUDE_FROM_ALL)
message(STATUS)

# Steam Audio
string(TIMESTAMP time "%M:%S")
message(STATUS "Steam Audio Starting (${time})...")
add_subdirectory(Ocean/vendor/steamaudio EXCLUDE_FROM_ALL)
message(STATUS)

# Vulkan Memory Allocator
string(TIMESTAMP time "%M:%S")
message(STATUS "Vulkan Memory Allocator Starting (${time})...")
add_subdirectory(Ocean/vendor/vma EXCLUDE_FROM_ALL)
message(STATUS)

set(
    OCEAN_COMPILE_FLAGS

    -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    -Wall
    -Wextra
    -Wold-style-cast
    -Wsign-promo
    -Woverloaded-virtual
    -Wctor-dtor-privacy
    -Wnon-virtual-dtor
    -Weffc++
    -pedantic
)

string(TIMESTAMP time "%M:%S")
message(STATUS "Ocean Starting (${time})...")
message(STATUS)
add_subdirectory(Ocean)

if (Ocean_DOCS)
    
    string(TIMESTAMP time "%M:%S")
    add_subdirectory(docs)
    message(STATUS "Doxygen Setup Started (${time})...")
    message(STATUS)

endif (Ocean_DOCS)

string(TIMESTAMP time "%M:%S")
message(STATUS "Sandbox Starting (${time})...")
message(STATUS)
add_subdirectory(Sandbox)

string(TIMESTAMP time "%H:%M:%S")
message(STATUS "All required steps Completed, EOF for ${PROJECT_NAME} at time: ${time}\n")
message(STATUS "Clever little rats. I will not forget what you have done here today.")
