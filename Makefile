IDIR=src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm

_DEPS = src/hash_table.h src/prime.h
DEPS = $(patsubst src/%,$(IDIR)/%,$(_DEPS))

_OBJ = main.o hash_table.o prime.o
OBJ = $(patsubst src/%,$(ODIR)/%,$(_OBJ))
VPATH:= src

$(ODIR)%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: main
	./main

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ main $(INCDIR)/*~
