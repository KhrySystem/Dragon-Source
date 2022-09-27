cmake_minimum_required(VERSION 3.24)

project(Dragon-Source VERSION 0.0.0.1)

set(C_FILES )
set(CC_FILES )
set(CXX_FILES dg/dgEngine.cpp dg/init.cpp)
set(M_FILES )

set(SRC ${C_FILES} ${CC_FILES} ${CXX_FILES} ${M_FILES})

add_library(Dragon-Source SHARED ${SRC})

target_include_directories(Dragon-Source PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(Dragon-Source PUBLIC ${Dragon_LINK_LIBRARIES})
target_link_libraries(Dragon-Source INTERFACE ${Dragon_LINK_INTERFACES})

add_library(Dragon::Dragon ALIAS Dragon-Source)