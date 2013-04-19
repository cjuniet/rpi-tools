cmake_minimum_required (VERSION 2.6)

project (rpi-tools)

set (CMAKE_C_FLAGS "-O2 -g -Wall")

add_executable (tmux-stats tmux-stats.c)
