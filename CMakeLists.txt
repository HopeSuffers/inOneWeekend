cmake_minimum_required(VERSION 3.27)
project(inOneWeekend)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

# Executable
add_executable(inOneWeekend main.cpp Vec3.hpp Color.hpp
        Ray.hpp
        HitTable.hpp
        Sphere.hpp
        HitTableList.hpp
        RTweekend.hpp
        Interval.hpp
        Camera.hpp
        Material.hpp
)

# ImageMagick definitions
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

# Attempt to find ImageMagick automatically
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

if (NOT ImageMagick_FOUND)
    message(FATAL_ERROR "ImageMagick could not be found. Please ensure it is installed and the environment variables are set.")
endif ()

# Include directories
include_directories(${ImageMagick_INCLUDE_DIRS})

# Link libraries
target_link_libraries(inOneWeekend ${ImageMagick_LIBRARIES})




