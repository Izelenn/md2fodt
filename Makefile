all: md2fodt

md2fodt: md2fodt.c fodt.h
	gcc -O3 -std=c99 md2fodt.c -o md2fodt

clean:
	rm md2fodt

