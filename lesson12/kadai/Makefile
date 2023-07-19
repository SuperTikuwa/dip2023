INCLUDE_DIR=`pkg-config --cflags --libs opencv4`

.PHONY: all
all: 	build run

.PHONY: build
build:
	@g++ main.cpp ${INCLUDE_DIR}

.PHONY: run
run:
	./a.out

