cmake_minimum_required(VERSION 3.14)
project(Terminal_2048 C)

set(CMAKE_C_STANDARD 11)

add_executable(Terminal_2048 2048.c 2048.h conio-linux.h)