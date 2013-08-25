
EXTRA_CFLAGS := -std=c99 -c -I.
PICPROG_OBJS := lpicp_icsp.o k8048.o inhx32.o io.o pic16.o pic.o util.o

all: k8048

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $^

k8048: $(PICPROG_OBJS)
	$(CC) -o $@ $^

clean:
	rm -f *.o *~ .*~ k8048
