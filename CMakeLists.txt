cmake_minimum_required(VERSION 3.20)

project(Dragon-Source VERSION 0.0.0.1)

if(NOT Dragon::Headers)
	add_subdirectory(../Dragon-Headers headers)
endif()

set(C_FILES dg/dgEngine.c dg/init.c)
set(CC_FILES )
set(CXX_FILES )
set(M_FILES )

set(SRC ${C_FILES} ${CC_FILES} ${CXX_FILES} ${M_FILES})

add_library(Dragon-Source SHARED ${SRC})

message(WARNING ${Dragon_INCLUDE_DIRS})

target_include_directories(Dragon-Source PUBLIC ${Dragon_INCLUDE_DIRS})

target_link_libraries(Dragon-Source INTERFACE Dragon::Headers)

add_library(Dragon::Dragon ALIAS Dragon-Source)