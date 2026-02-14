add_subdirectory(Body)
add_subdirectory(Leg)
add_subdirectory(Legs)

add_library(Bot 
    ${SRC_FILES_BODY}
    ${SRC_FILES_LEG} 
    ${SRC_FILES_LEGS}
)

target_include_directories(Bot
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/Body
        ${CMAKE_CURRENT_SOURCE_DIR}/Leg
        ${CMAKE_CURRENT_SOURCE_DIR}/Legs
)