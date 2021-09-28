
CC=gcc

LIBS := -lreadline -lcommons -lcspecs -lpthread
OBJECTS_SHARED := $(patsubst %.c, %.o, $(wildcard ../../shared/*.c))

build-example-lib:
	cd examples/lib_implementation && \
	$(CC) -c -Wall -Werror -I"../../" -fpic matelib.c && \
	$(CC) -shared matelib.o -o libmate.so && \
	rm matelib.o

build-example: build-example-lib
	cd examples/lib_usage && \
	$(CC) -g3 -Wall -L"../lib_implementation" -I"../../" main.c -lmate -lpthread -lrt -lcommons -o example

run: build-example
	cd examples/lib_usage && \
	LD_LIBRARY_PATH="../lib_implementation/" ./example

clean:
	rm examples/lib_implementation/libmate.so examples/lib_usage/example