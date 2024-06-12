cmake_minimum_required(VERSION 3.27)
project(hardened-queue C)

set(CMAKE_C_STANDARD 17)
set(INCLUDE_DIR "./include")

include_directories(${INCLUDE_DIR})

add_executable(hardened-queue
        src/main.c
        include/main.h
        src/file_helpers.c
        include/file_helpers.h
        src/linkedlist.c
        include/linkedlist.h
        src/security.c
        include/security.h
        src/command_callbacks.c
        include/command_callbacks.h
        src/commands.c
        include/commands.h
        src/person.c
        include/person.h
        include/colors.h
        src/threading.c
        include/threading.h
)