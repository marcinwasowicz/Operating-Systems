CC=gcc
CFLAGS= -Wall

z1library:
	$(CC) -fPIC -shared zadanie1Library.c -o z1library.so
z1program:
	$(CC) $(CFLAGS) zadanie1.c  -L. -Wl,-rpath=. z1library.so -o zadanie1
clean:
	rm *.txt
	rm *.so
	rm zadanie1
all:
	make z1library
	make z1program