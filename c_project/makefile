CC=gcc
CFLAGS= -Wall -Wextra -std=c11 -g -O3 -march=native `pkg-config --cflags libxml-2.0`

LIBS = `pkg-config --libs libxml-2.0`

DEPS = interface.h xmlFunctions.h simpLList.h stringFunctions.h growingArray.h userLList.h articleLList.h snapshotStats.h

OBJ = simpLList.o xmlFunctions.o stringFunctions.o growingArray.o interface.o program.o userLList.o articleLList.o snapshotStats.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

program: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	@echo compile exited
clean:
	rm -f *o
	@echo clean exited
