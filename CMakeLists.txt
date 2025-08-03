cmake_minimum_required(VERSION 4.0)

project(unix_shell VERSION 0.1.0 LANGUAGES C)

set(C_STANDARD 23)
set(C_EXTENSIONS off)
set(C_STANDARD_REQUIRED on)

set(EXE wish)
add_executable(${EXE})

add_subdirectory(src)
