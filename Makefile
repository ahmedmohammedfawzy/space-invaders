CFLAGS = -g -pedantic -Wall -Wunused-parameter -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes 

run: build	
	bin/main

build:
	gcc -c src/*.c ${CFLAGS} -I include 
	gcc *.o -o bin/main -lSDL2main -lSDL2 -lSDL2_image

clean:
	rm -rf bin/* 
	rm *.o
