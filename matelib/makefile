RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)

# Clean and compile .so
all: build/libmatelib.so

build/matelib:
	mkdir -p $@

build/libmatelib.so: build/matelib $(OBJS)
	$(CC) -shared -o "$@" $(OBJS)

build/%.o: %.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Print all sources
sources:
	@echo $(C_SRCS) $(H_SRCS)

# Clean build files
clean:
	$(RM) build

install: all
	sudo cp -u build/libmatelib.so /usr/lib
	sudo cp --parents -u $(H_SRCS) /usr/include

uninstall:
	sudo rm -f /usr/lib/libmatelib.so
	sudo rm -rf /usr/include/matelib

.PHONY: all debug sources clean install uninstall
