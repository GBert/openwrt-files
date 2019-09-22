BINFILE=gpiod-test

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -Wall
CLIB=

CSOURCE=gpiod-test.c
COBJECT=gpiod-test.o
CTARGET=gpiod-test

build:$(COBJECT) $(CTARGET)

$(CTARGET):$(COBJECT)
	$(CC) $(COBJECT) -o $(CTARGET) $(CLIB)

$(COBJECT):$(CHEADER) Makefile

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ $(CTARGET)
