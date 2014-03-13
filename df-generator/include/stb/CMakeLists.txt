cmake_minimum_required(VERSION 2.8)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/stb)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/stb)

set(HEADERS
${INCROOT}/stb_image.h
${INCROOT}/stb_image_write.h
)

add_custom_target(stb_image SOURCES ${HEADERS})

