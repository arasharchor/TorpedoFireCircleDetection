cmake_minimum_required(VERSION 2.8) 
project( algo )             #write folders name here instead of "project"
find_package( OpenCV REQUIRED ) 
add_executable( a algo.cpp )   # a is executable .  canny. cpp is ur cpp file.
target_link_libraries( a ${OpenCV_LIBS} )
