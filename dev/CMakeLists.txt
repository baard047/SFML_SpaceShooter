set(TARGET_NAME ${PROJECT_NAME})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
file(GLOB_RECURSE SOURCE_FILES "*.cpp" )

add_executable( ${TARGET_NAME} ${SOURCE_FILES} )
target_sources(${TARGET_NAME} PRIVATE ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries( ${TARGET_NAME} PRIVATE sfml-graphics sfml-audio )

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
