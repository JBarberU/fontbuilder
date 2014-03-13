cmake_minimum_required(VERSION 2.8)

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
${SRCROOT}/main.cpp
)

set(HEADERS

)

add_executable(df-generator ${SRC} ${HEADERS})
