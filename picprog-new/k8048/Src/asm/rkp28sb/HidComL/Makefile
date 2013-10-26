CC=gcc
CFLAGS=
CLIB=

TARGET=hidcom
SOURCE=hidcom.c
OBJECT=$(SOURCE:.c=.o)
HEADER=

default:$(TARGET)

$(TARGET):$(OBJECT)
	@echo -n "[LINK] "
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET) $(CLIB)

$(OBJECT):$(HEADER) Makefile

clean:
	rm -f $(TARGET) *.o *~ .*~

%.o:%.c
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@
