cmake_minimum_required(VERSION 2.6.2)
project(app_rkfb_test)

set(RKFB_TEST_APP_SRC_FILES
#	bmp/bmp_operate.c
	rgb/rgb565_operate.c
	rgb/rgb888_operate.c
	rgb/argb8888_operate.c
	font/font.c
	rkfb_test.c
)

include_directories(
	${app_rkfb_test_SOURCE_DIR}/bmp
	${app_rkfb_test_SOURCE_DIR}/rgb
	${app_rkfb_test_SOURCE_DIR}/font
	${app_rkfb_test_SOURCE_DIR}/../freetype-2.9/include
)

#SET(CMAKE_BUILE_TYPE DEBUG)       #指定编译类型 Release  Debug
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99")

add_executable(app_rkfb_test ${RKFB_TEST_APP_SRC_FILES})
target_link_libraries(app_rkfb_test rkfb ion freetype)
install(TARGETS app_rkfb_test DESTINATION bin)


#set(BMP_OPT_APP_SRC_FILES
#    bmp_operate.c
#)
 
#add_executable(bmp_operate ${BMP_OPT_APP_SRC_FILES})
#target_link_libraries(bmp_operate rkfb ion)
#install(TARGETS bmp_operate DESTINATION bin)

