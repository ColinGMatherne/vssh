CC = gcc
CFLAGS = -Wall -Wpedantic -g -std=c89
LFLAGS = -lc
files = src/main.c
obj = src/obj/main.o
out = vssh

all: $(obj)
	$(CC) $(obj) $(CFLAGS) -o $(out) $(LFLAGS)

src/obj/main.o: src/main.c
	$(CC) -c src/main.c $(CFLAGS) $(LFLAGS) -o src/obj/main.o

clean:
	rm $(obj) $(out)
