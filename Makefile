CC=gcc
CFLAGS=$(shell pkg-config --cflags raylib)
LDFLAGS=$(shell pkg-config --libs raylib)

TARGET=cuboid

cuboid:
	$(CC) cuboid.c $(CFLAGS) $(LDFLAGS) -o cuboid

clean:
	rm -f $(TARGET)
