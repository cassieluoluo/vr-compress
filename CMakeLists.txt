cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project( VR_Compressor )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED)

set(Boost_USE_STATIC_LIB OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55.0 COMPONENTS system filesystem program_options)

include_directories(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
add_executable(BasicPlayer basic_player.cpp raw_video.cpp)
add_executable(Encoder encoder.cpp raw_video.cpp)
set_property(TARGET BasicPlayer PROPERTY CXX_STANDARD 11)
set_property(TARGET Encoder PROPERTY CXX_STANDARD 11)
set_property(TARGET BasicPlayer PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET Encoder PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(BasicPlayer ${Boost_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(Encoder ${Boost_LIBRARIES} ${OpenCV_LIBS})

