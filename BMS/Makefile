CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror

PROGS = bms1A bms1B

all: $(PROGS)

bms1A: coder.c rscode/libecc.a io.o
	$(CC) $(CFLAGS) $^ -o $@

bms1B: decoder.c rscode/libecc.a io.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

rscode/libecc.a:
	$(MAKE) -C rscode

clean:
	rm -rf *.o $(PROGS)
	$(MAKE) -C rscode clean

zip:
	zip xcoufa09.zip Makefile rscode/{*.c,*.h,Makefile,README,LICENSE,*.doc,*.txt} *.c *.h
