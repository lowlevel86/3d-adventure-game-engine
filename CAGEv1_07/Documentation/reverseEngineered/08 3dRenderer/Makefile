CC = i586-mingw32msvc-gcc
WINDRES = i586-mingw32msvc-windres
RES =
OBJ = main.o renderer/line.o renderer/rotate.o renderer/transformation.o $(RES)
LIBS = -mwindows
BIN = main.exe
CFLAGS = -Wall
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o $(CFLAGS)
