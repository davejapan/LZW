IDIR = header
ODIR = src
BDIR = build

CC=gcc
LD=gcc

CFLAGS=-Wall -I $(IDIR)

_DEPS = lzw.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJS = main.o dictionary.o encode.o decode.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

all: lzw

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lzw: $(OBJS)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS)

clean:
	rm -rf build/*
	rm -rf src/*.o
rebuild: clean all

.PHONY : clean
.SILENT : clean
