cmake_minimum_required(VERSION 3.22)
add_subdirectory(body)

# Bot unit tests - shared source files
set(SRC_FILES
        ../src/Core/Logger.cpp
)

set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${SRC_FILES} PARENT_SCOPE)