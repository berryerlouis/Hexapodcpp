add_subdirectory(status)
add_subdirectory(logger)

set(SRC_FILES
    ../src/Core/Logger.cpp
)

set(EXECUTABLES_SRC ${EXECUTABLES_SRC} ${SRC_FILES} PARENT_SCOPE)
