CC = i586-mingw32msvc-gcc
WINDRES = i586-mingw32msvc-windres
RES =
OBJ = main.o mirrorGirl.o $(RES)
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

mirrorGirl.o: mirrorGirl.c
	$(CC) -c mirrorGirl.c -o mirrorGirl.o $(CFLAGS)

