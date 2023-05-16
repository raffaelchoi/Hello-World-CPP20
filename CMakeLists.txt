cmake_minimum_required(VERSION 3.20...3.26) # Specify the minimum and maximum required version of CMake.

project(HelloWorldInCpp20 VERSION 1.0 DESCRIPTION "Overcomplicated HelloWorld in C++20") # Set the project name, version, and description.

set(CMAKE_CXX_STANDARD 20) # Set the C++ standard to C++20.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Make the C++ standard required.
set(CMAKE_CXX_EXTENSIONS OFF) # Turn off compiler-specific extensions, ensuring maximum portability.

add_executable(HelloWorld main.cpp) # Create an executable target named "HelloWorld" from the source files "main.cpp".
