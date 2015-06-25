#include <stdio.h>
#include <math.h>
#include "renderer/renderer.h"
#include "meshes/ascII.h"

float ascII_TD[846];
float ascII_PT[7455];
float ascII_PTEX[7455];

void drawAscIIf(float *, int *, int *, int *, int, int, struct rendrInfo);
void drawAscIIaf(float *, int *, int *, int *, int, int, struct rendrInfo);

void drawAscII(char *ascIIStr, float xSz, float ySz,
               float xPos, float yPos, float zPos,
               float xRot, float yRot, float zRot,
               int color, int bgColor, int antialiasing, struct rendrInfo rI)
{
   int i;
   int chrPos = 0;
   
   // Color
   for (i=0; i < ascII_COLORDATSZ[ascII_MESHCNT-1]; i++)
   ascII_COLOR[i] = color;
   
   // X Size
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+6] = ascII_TRANS[i+6] * xSz;
   // Y Size
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+7] = ascII_TRANS[i+7] * ySz;
   
   // X Position
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+3] = xPos;
   // Y Position
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+4] = yPos;
   // Z Position
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+5] = zPos;
   
   // X Rotation
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+0] = xRot;
   // Y Rotation
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+1] = yRot;
   // Z Rotation
   for (i=0; i < ascII_MESHCNT*9; i=i+9)
   ascII_TD[i+2] = zRot;
   
   float xAscIIRot = 1, yAscIIRot = 0, zAscIIRot = 0;
   
   rotate(&xAscIIRot, &yAscIIRot, &zAscIIRot, 0, -ascII_TD[0], 0);
   rotate(&xAscIIRot, &yAscIIRot, &zAscIIRot, ascII_TD[1], 0, 0);
   rotate(&xAscIIRot, &yAscIIRot, &zAscIIRot, 0, 0, -ascII_TD[2]);
   
   addRotLocSz(ascII_TD, ascII_PT, ascII_POINTDATSZ,
               ascII_POINT, ascII_MESHCNT);
   
   int ascII_MESHNUM = (int)ascIIStr[chrPos]-32;
   chrPos++;
   
   if (!antialiasing)
   drawAscIIf(ascII_PT, ascII_LINE, ascII_COLOR,
              ascII_LINEDATSZ, ascII_MESHNUM, bgColor, rI);
   
   if (antialiasing)
   drawAscIIaf(ascII_PT, ascII_LINE, ascII_COLOR,
               ascII_LINEDATSZ, ascII_MESHNUM, bgColor, rI);
   
   
   while ((int)ascIIStr[chrPos])
   {
      ascII_MESHNUM = (int)ascIIStr[chrPos]-32;
      
      ascII_TD[(ascII_MESHNUM-1)*9+3] = xAscIIRot * (xSz*chrPos);
      ascII_TD[(ascII_MESHNUM-1)*9+4] = yAscIIRot * (xSz*chrPos);
      ascII_TD[(ascII_MESHNUM-1)*9+5] = zAscIIRot * (xSz*chrPos);
      
      chrPos++;
      
      for (i = ascII_POINTDATSZ[ascII_MESHNUM-2]; i < ascII_POINTDATSZ[ascII_MESHNUM-1]; i=i+3)
      {
         ascII_PTEX[i+0] = ascII_PT[i+0] + ascII_TD[(ascII_MESHNUM-1)*9+3];//x
         ascII_PTEX[i+1] = ascII_PT[i+1] + ascII_TD[(ascII_MESHNUM-1)*9+4];//y
         ascII_PTEX[i+2] = ascII_PT[i+2] + ascII_TD[(ascII_MESHNUM-1)*9+5];//z
      }
      
      if (!antialiasing)
      drawAscIIf(ascII_PTEX, ascII_LINE, ascII_COLOR,
                 ascII_LINEDATSZ, ascII_MESHNUM, bgColor, rI);
      
      if (antialiasing)
      drawAscIIaf(ascII_PTEX, ascII_LINE, ascII_COLOR,
                  ascII_LINEDATSZ, ascII_MESHNUM, bgColor, rI);
   }
}

// a = antialiasing, f = fog

void drawAscIIf(float *ascII_POINT, int *ascII_LINE, int *ascII_COLOR,
                int *ascII_LINEDATSZ, int ascII_MESHNUM, int bgColor, struct rendrInfo rI)
{
   int sidesNum;
   int pt1, pt2;
   int color;
   int colorCnt = 0;
   int lineCnt;
   
   lineCnt = ascII_LINEDATSZ[ascII_MESHNUM-2];
   
   while (lineCnt < ascII_LINEDATSZ[ascII_MESHNUM-1])
   {
      sidesNum = ascII_LINE[lineCnt];
      
      color = ascII_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = ascII_LINE[lineCnt+1] * 3;
         pt2 = ascII_LINE[lineCnt+2] * 3;
         
         fLine(ascII_POINT[pt1], ascII_POINT[pt1+1], ascII_POINT[pt1+2],
               ascII_POINT[pt2], ascII_POINT[pt2+1], ascII_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
   }
}

void drawAscIIaf(float *ascII_POINT, int *ascII_LINE, int *ascII_COLOR,
                 int *ascII_LINEDATSZ, int ascII_MESHNUM, int bgColor, struct rendrInfo rI)
{
   int sidesNum;
   int pt1, pt2;
   int color;
   int colorCnt = 0;
   int lineCnt;
   
   lineCnt = ascII_LINEDATSZ[ascII_MESHNUM-2];
   
   while (lineCnt < ascII_LINEDATSZ[ascII_MESHNUM-1])
   {
      sidesNum = ascII_LINE[lineCnt];
      
      color = ascII_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = ascII_LINE[lineCnt+1] * 3;
         pt2 = ascII_LINE[lineCnt+2] * 3;
         
         afLine(ascII_POINT[pt1], ascII_POINT[pt1+1], ascII_POINT[pt1+2],
                ascII_POINT[pt2], ascII_POINT[pt2+1], ascII_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
   }
}
