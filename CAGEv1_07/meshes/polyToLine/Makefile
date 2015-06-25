CC = i586-mingw32msvc-gcc
WINDRES = i586-mingw32msvc-windres
RES = rsrc.res
OBJ = main.o editMode/editMode.o dataView/asciiSM.o dataView/dataView.o helpWnd/helpWnd.o nameWnd/nameWnd.o prtCCode.o polyToLine.o rmCComments.o readModelData.o findStrInStr.o renderer/line.o renderer/line2.o renderer/rotate.o renderer/rotate2.o renderer/addRotLocSz.o $(RES)
LIBS = -mwindows
BIN = "polyToLinev1.exe"
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

rmCComments.o: rmCComments.c
	$(CC) -c rmCComments.c -o rmCComments.o $(CFLAGS)

readModelData.o: readModelData.c
	$(CC) -c readModelData.c -o readModelData.o $(CFLAGS)

findStrInStr.o: findStrInStr.c
	$(CC) -c findStrInStr.c -o findStrInStr.o $(CFLAGS)

polyToLine.o: polyToLine.c
	$(CC) -c polyToLine.c -o polyToLine.o $(CFLAGS)

prtCCode.o: prtCCode.c
	$(CC) -c prtCCode.c -o prtCCode.o $(CFLAGS)

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

nameWnd/nameWnd.o: nameWnd/nameWnd.c
	$(CC) -c nameWnd/nameWnd.c -o nameWnd/nameWnd.o $(CFLAGS)

helpWnd/helpWnd.o: helpWnd/helpWnd.c
	$(CC) -c helpWnd/helpWnd.c -o helpWnd/helpWnd.o $(CFLAGS)

dataView/dataView.o: dataView/dataView.c
	$(CC) -c dataView/dataView.c -o dataView/dataView.o $(CFLAGS)

dataView/asciiSM.o: dataView/asciiSM.c
	$(CC) -c dataView/asciiSM.c -o dataView/asciiSM.o $(CFLAGS)

editMode/editMode.o: editMode/editMode.c
	$(CC) -c editMode/editMode.c -o editMode/editMode.o $(CFLAGS)

rsrc.res: rsrc.rc 
	$(WINDRES) -i rsrc.rc --input-format=rc -o rsrc.res -O coff
