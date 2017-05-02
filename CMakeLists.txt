cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_C_COMPILER /usr/local/bin/gcc-6)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++-6)

project( VR_Compressor )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenMP REQUIRED)
message("OpenMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

find_package(OpenCV REQUIRED)

set(Boost_USE_STATIC_LIB OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.55.0 COMPONENTS system filesystem program_options)

include_directories(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(BasicPlayer basic_player.cpp raw_video.cpp)
set_property(TARGET BasicPlayer PROPERTY CXX_STANDARD 11)
set_property(TARGET BasicPlayer PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(encoder encoder.cpp raw_video.cpp utils.cpp)
set_property(TARGET encoder PROPERTY CXX_STANDARD 11)
set_property(TARGET encoder PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(vr_player vr_player.cpp decoder.cpp utils.cpp)
set_property(TARGET vr_player PROPERTY CXX_STANDARD 11)
set_property(TARGET vr_player PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(BasicPlayer ${Boost_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(encoder ${Boost_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(vr_player ${Boost_LIBRARIES} ${OpenCV_LIBS})
