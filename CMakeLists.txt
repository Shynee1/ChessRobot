cmake_minimum_required(VERSION 3.29.6)
project(ChessRobot)

set(CMAKE_CXX_STANDARD            20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS        True)

set(ASSETS_FOLDER ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_DESTINATION ${CMAKE_BINARY_DIR}/assets)

find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glaze REQUIRED)

# Get all cpp files from /src folder
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Create executable with cpp files
add_executable(${PROJECT_NAME} ${SOURCES})

# Link header files
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/external
)

# Copy assets folder to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSETS_FOLDER} ${ASSETS_DESTINATION}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glaze::glaze 
    SDL2::SDL2-static
    SDL2::SDL2main
    SDL2_image::SDL2_image-static
    SDL2_ttf::SDL2_ttf-static
    Boost::filesystem
)