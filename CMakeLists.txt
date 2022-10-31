cmake_minimum_required(VERSION 3.20)

set(Dragon_LIB_NAME dragon-${Dragon_VERSION_MAJOR})

set(CXX_FILES 
	dg/audio/outputDevice.cpp 
	dg/internal/gpu.cpp 
	dg/internal/init.cpp 
	dg/internal/option.cpp
	dg/message/callbacks.cpp 
)

if(DRAGON_BUILD_SHARED)
	add_library(${Dragon_LIB_NAME} SHARED ${CXX_FILES} ${HPP_FILES})
	set(BUILD_SHARED_LIBS ${OLD_BUILD_SHARED})
else() 
	add_library(${Dragon_LIB_NAME} STATIC ${CXX_FILES} ${HPP_FILES})
endif()
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw glm::glm openxr_loader OpenAL ${Vulkan_LIBRARIES})
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})
