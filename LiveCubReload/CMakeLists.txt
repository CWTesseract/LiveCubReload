cmake_minimum_required (VERSION 3.8)
project(LiveCubReload)

add_subdirectory(efsw)

# Create our library.
add_library (LiveCubReload SHARED "src/main.cpp")

# Link CWTCore.
option(STATIC_LIB "" ON)
target_link_libraries (LiveCubReload PRIVATE efsw)
target_link_libraries (LiveCubReload LINK_PUBLIC CWTCore)

# Add the CWTCore includes.
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../CWTCore/include)
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/efsw/include)



