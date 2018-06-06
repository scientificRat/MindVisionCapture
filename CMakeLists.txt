cmake_minimum_required(VERSION 2.8)
project(MindVisionCapture)
set(OpenCV_DIR "/usr/local/opencv3/share/OpenCV")
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)



add_executable(MindVisionCapture main.cpp MindVisionCapture.cpp MindVisionCapture.h)
target_link_libraries(MindVisionCapture ${OpenCV_LIBS} -lpthread -lMVSDK )