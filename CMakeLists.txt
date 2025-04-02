cmake_minimum_required(VERSION 3.7)
project(test_clay CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CLAY_INCLUDE_ALL_EXAMPLES OFF)
set(FETCHCONTENT_QUIET FALSE)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

include(FetchContent)

# Download SDL3
FetchContent_Declare(
	SDL
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.4
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
message(STATUS "Using SDL via FetchContent")
FetchContent_MakeAvailable(SDL)
set_property(DIRECTORY "${sdl_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# Download SDL_ttf
FetchContent_Declare(
	SDL_ttf
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
	GIT_TAG main # Slightly risky to use main branch, but it's the only one available
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
message(STATUS "Using SDL_ttf via FetchContent")
FetchContent_MakeAvailable(SDL_ttf)
set_property(DIRECTORY "${sdl_ttf_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# Download SDL_image
FetchContent_Declare(
	SDL_image
	GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image.git"
	GIT_TAG release-3.2.0 # Slightly risky to use main branch, but it's the only one available
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
message(STATUS "Using SDL_image via FetchContent")
FetchContent_MakeAvailable(SDL_image)
set_property(DIRECTORY "${SDL_image_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

set(SOURCES
    src/main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
	SDL3::SDL3
	SDL3_ttf::SDL3_ttf
	SDL3_image::SDL3_image
)
