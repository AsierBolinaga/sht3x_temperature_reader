project(tempreader)
cmake_minimum_required(VERSION 3.0)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SH3TX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sht3x_reader")
set(TEMPREADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sht3x_reader/tempreader")
set(COMM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sht3x_reader/comm")
set(BOARD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../sht3x_reader/board")

set(PL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../portability_layer")
set(BBB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../platform/beaglebone")


# add source files
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.c
                          ${SH3TX_DIR}/*.c
                          ${TEMPREADER_DIR}/*.c
                          ${COMM_DIR}/*.c
                          ${BOARD_DIR}/*.c
                          ${BBB_DIR}/*.c
                          ${PL_DIR}/*.c)

include_directories(${SH3TX_DIR} ${SOURCE_DIR} ${TEMPREADER_DIR} 
                    ${COMM_DIR} ${BOARD_DIR} ${PL_DIR} ${BBB_DIR})

add_executable(tempreader ${SOURCES})

target_link_libraries(tempreader i2c)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

