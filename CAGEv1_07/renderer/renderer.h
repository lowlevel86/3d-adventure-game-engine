struct rendrInfo
{
   int xWin;
   int yWin;
   float xCenter;
   float yCenter;
   float camLenZ;
   float camEndZ;
   float fogBgnZ;
   float fogEndZ;
   float perspctv;
   int ortho;
   float **zIndex;
   int **scrnBuff;
};
typedef struct rendrInfo RENDRINFO;

// in rotate.c
void rotate(float *, float *, float *, float, float, float);

// in rotate2.c
void rotatePoint(float *, float *, float *,
                 float, float, float, float, float, float);
void addRotAngsToAngs(float, float, float, float *, float *, float *);
void addParallelRotAngsToAngs(float, float, float, float *, float *, float *);

// in addRotLocSz.c
void addRotLocSz(float *, float *, int *, float *, int);

// in addRotLocSz2.c
void addScreenRotLocSz(float, float, float,
                       float, float, float, 
                       float, float, float, float *, int *, int);

// a = antialiasing, f = fog
// in line.c
void line(float, float, float,
          float, float, float, int, struct rendrInfo);
void fLine(float, float, float,
           float, float, float, int, int, struct rendrInfo);

// in line2.c
void aLine(float, float, float,
           float, float, float, int, struct rendrInfo);
void afLine(float, float, float,
            float, float, float, int, int, struct rendrInfo);

// in drawMesh.c
void drawMesh(float *, int *, int *, int *, int, int, int, struct rendrInfo);

// in drawAscII.c
void drawAscII(char *, float, float,
               float, float, float,
               float, float, float, int, int, int, struct rendrInfo);

// in transform.c
void globalRot(float *, float *, int, float, float, float, float, float, float);
void localRot(float *, int, float, float, float, float, float, float);
void globalRotRelToScrn(float *, float *, int, float, float, float, float, float, float);
void localRotRelToScrn(float *, int, float, float, float, float, float, float);
void globalMove(float *, float *, int, float, float, float);
void localMove(float *, int, float, float, float);
void globalResize(float *, float *, int, float, float, float, float, float, float);
void localResize(float *, int, float, float, float, float, float, float);
