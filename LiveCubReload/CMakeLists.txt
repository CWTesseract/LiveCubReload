cmake_minimum_required (VERSION 3.8)
project(LiveCubReload)

option(STATIC_LIB "Build efsw as a static library" ON)
SET(STATIC_LIB ON CACHE BOOL "Build efsw as a static library" FORCE)
add_subdirectory(efsw)

# Create our library.
add_library (LiveCubReload SHARED "src/main.cpp")

# Link efsw and CWTCore.
target_link_libraries (LiveCubReload PRIVATE efsw)
target_link_libraries (LiveCubReload LINK_PUBLIC CWTCore)

# Add the includes dirs.
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../CWTCore/include)
target_include_directories (LiveCubReload PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/efsw/include)



