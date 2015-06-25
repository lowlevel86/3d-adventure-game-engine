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

void rotate(float *, float *, float *, float, float, float);

void getObjLgths(float, float, float, float *, float *, float *);

void getRelObjPt(float, float, float,
                 float, float, float,
                 float, float, float,
                 float, float, float,
                 float, float, float,
                 float *, float *, float *);

void line(float, float, float,
          float, float, float, int, struct rendrInfo);

void fLine(float, float, float,
           float, float, float, int, int, struct rendrInfo);
