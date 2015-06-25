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

// rotate relative to screen
void rotateRelToScrn(float, float, float, float *, float *, float *);

void addRotLocSz(float *, float *, int *, float *, int);

// a = antialiasing, f = fog
void line(float, float, float,
          float, float, float, int, struct rendrInfo);
void fLine(float, float, float,
           float, float, float, int, int, struct rendrInfo);

void aLine(float, float, float,
           float, float, float, int, struct rendrInfo);
void afLine(float, float, float,
            float, float, float, int, int, struct rendrInfo);
