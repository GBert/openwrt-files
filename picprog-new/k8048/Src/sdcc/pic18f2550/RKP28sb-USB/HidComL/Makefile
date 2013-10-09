CC=gcc
CFLAGS=
CLIB=

TARGET=hidcom
SOURCE=hidcom.c
OBJECT=hidcom.o
HEADER=

default:$(TARGET)

$(TARGET):$(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET) $(CLIB)

$(OBJECT):$(HEADER)

clean:
	rm -f $(TARGET) *.o *~ .*~

c.o:$(CC) -c $< -o $@
