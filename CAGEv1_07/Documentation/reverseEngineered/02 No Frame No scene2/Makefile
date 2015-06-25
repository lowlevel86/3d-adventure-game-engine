CC = i586-mingw32msvc-gcc
WINDRES = i586-mingw32msvc-windres
RES = 
OBJ = main.o drawMesh.o transform.o meshes/character.o meshes/shadow.o meshes/run.o meshes/course1.o meshes/dome1.o renderer/line.o renderer/line2.o renderer/rotate.o renderer/rotate2.o renderer/addRotLocSz.o renderer/addRotLocSz2.o $(RES)
LIBS = -mwindows
BIN = game.exe
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

drawMesh.o: drawMesh.c
	$(CC) -c drawMesh.c -o drawMesh.o $(CFLAGS)

transform.o: transform.c
	$(CC) -c transform.c -o transform.o $(CFLAGS)


meshes/dome1.o: meshes/dome1.c
	$(CC) -c meshes/dome1.c -o meshes/dome1.o $(CFLAGS)

meshes/course1.o: meshes/course1.c
	$(CC) -c meshes/course1.c -o meshes/course1.o $(CFLAGS)

meshes/character.o: meshes/character.c
	$(CC) -c meshes/character.c -o meshes/character.o $(CFLAGS)

meshes/shadow.o: meshes/shadow.c
	$(CC) -c meshes/shadow.c -o meshes/shadow.o $(CFLAGS)

meshes/run.o: meshes/run.c
	$(CC) -c meshes/run.c -o meshes/run.o $(CFLAGS)


renderer/line.o: renderer/line.c
	$(CC) -c renderer/line.c -o renderer/line.o $(CFLAGS)

renderer/line2.o: renderer/line2.c
	$(CC) -c renderer/line2.c -o renderer/line2.o $(CFLAGS)

renderer/rotate.o: renderer/rotate.c
	$(CC) -c renderer/rotate.c -o renderer/rotate.o $(CFLAGS)

renderer/rotate2.o: renderer/rotate2.c
	$(CC) -c renderer/rotate2.c -o renderer/rotate2.o $(CFLAGS)

renderer/addRotLocSz.o: renderer/addRotLocSz.c
	$(CC) -c renderer/addRotLocSz.c -o renderer/addRotLocSz.o $(CFLAGS)

renderer/addRotLocSz2.o: renderer/addRotLocSz2.c
	$(CC) -c renderer/addRotLocSz2.c -o renderer/addRotLocSz2.o $(CFLAGS)
