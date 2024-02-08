PROJECT=bubble
CC=g++
FLAGS=-pedantic
OpenGL_FLAGS=-lepoxy -lGLEW -lGL -lglfw -ldl
ASSIMP_FLAGS= -ldl -lGLESv2 -lassimp
OUTPUT=-o ${PROJECT}
CPP_FILES=*.cpp

compile:
	@${CC} ${FLAGS} ${OUTPUT} ${CPP_FILES} ${OpenGL_FLAGS} ${ASSIMP_FLAGS}

clean:
	@if [ -f ${PROJECT} ]; then rm -rf ${PROJECT}; fi