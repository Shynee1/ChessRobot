cmake_minimum_required(VERSION 3.29.6)
project(ChessRobot)

set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS        True)

find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glaze REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/external
)

set(link_src "${CMAKE_SOURCE_DIR}/assets")
if(UNIX AND NOT APPLE) 
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/assets") 
else() 
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets") 
endif() 
add_custom_command( 
    TARGET ${PROJECT_NAME} 
    POST_BUILD COMMAND ${CMAKE_COMMAND} 
        -E create_symlink ${link_src} ${link_dst} 
    DEPENDS ${link_dst} 
    COMMENT "symbolic link ${link_src} => ${link_dst}" 
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glaze::glaze 
    SDL2::SDL2-static
    SDL2::SDL2main
    SDL2_image::SDL2_image-static
    Boost::filesystem
)