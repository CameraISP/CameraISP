cmake_minimum_required(VERSION 3.10)
project(CameraISP CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path to OpenCV .cmake first, then the cmake can find OpenCV
find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(CameraISP ${SRC_FILES})

# Link OpenCV
target_link_libraries(CameraISP PRIVATE ${OpenCV_LIBS})

# Debug message
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
