cmake_minimum_required(VERSION 2.8)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(HEADERS
${INCROOT}/Image.h
${INCROOT}/DistanceField.h
)

add_custom_target(include SOURCES ${HEADERS})
add_subdirectory(stb)

