SRC_PATH=./Source
OUT_PATH=./Build
VPATH= ${SRC_PATH}
OBJ = $(wildcard ${SRC_PATH}/*.cpp)
CC = g++
CFLAGS=-Wall

${OUT_PATH}/util: $(OBJ)
	${CC} ${CFLAGS} -o $@ $^



