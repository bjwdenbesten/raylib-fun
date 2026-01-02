CC=gcc
CFLAGS=$(shell pkg-config --cflags raylib)
LDFLAGS=$(shell pkg-config --libs raylib)

TARGETS=cuboid halfpong

.PHONY: all clean	

all: $(TARGETS)

%: %.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGETS)
