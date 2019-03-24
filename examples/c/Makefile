CC		?= gcc
C_FLAGS 	:= -ansi -pedantic -Werror -Wall -O3 -ffast-math -flto -lffts -lpitch_detection -std=c11

all: example

example:
	$(CC) ./example.c -o $@ $(C_FLAGS)

clean:
	-rm example

.PHONY: clean
