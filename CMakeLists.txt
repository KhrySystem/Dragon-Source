cmake_minimum_required(VERSION 3.24)

set(C_FILES )
set(CC_FILES )
set(CXX_FILES dg/dgEngine.cpp dg/init.cpp dg/internal.cpp dg/gpu.cpp)
set(M_FILES )

set(SRC ${C_FILES} ${CC_FILES} ${CXX_FILES} ${M_FILES})

add_library(dragon-0 SHARED ${SRC})

target_include_directories(dragon-0 PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(dragon-0 PUBLIC ${Dragon_LINK_LIBRARIES} Dragon::Ironbreath Dragon::Streambreath)
target_link_libraries(dragon-0 INTERFACE ${Dragon_LINK_INTERFACES})

add_library(Dragon::Dragon ALIAS dragon-0)