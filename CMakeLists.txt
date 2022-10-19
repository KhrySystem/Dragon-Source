cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE C_FILES   ./*.c)
file(GLOB_RECURSE CXX_FILES ./*.cpp)
file(GLOB_RECURSE M_FILES   ./*.m)


set(SRC ${C_FILES} ${CXX_FILES} ${M_FILES})

add_library(dragon-0 SHARED ${SRC})

target_include_directories(dragon-0 PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(dragon-0 PUBLIC ${Dragon_LINK_LIBRARIES})
target_link_libraries(dragon-0 INTERFACE ${Dragon_LINK_INTERFACES})

add_library(Dragon::Dragon ALIAS dragon-0)
list(APPEND Dragon_LINK_LIBRARIES Dragon::Dragon)
