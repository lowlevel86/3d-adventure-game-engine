#include <math.h>
#include <stdio.h>
#include "../renderer/renderer.h"
#include "asciiSM.h"

void printASCII(char *, int, int, float, struct rendrInfo);

void viewObjCenterLocs(float *mesh_TRANS, int mesh_MESHCNT, struct rendrInfo rI)
{
   int i;
   float xPt, yPt, zPt, perspctv;
   char asciiStr[1024];
   int asciiLocX, asciiLocY;
   float asciiLocZ;
   
   for (i = 0; i < mesh_MESHCNT; i++)
   {
      sprintf(asciiStr, "%i", i);
      
      xPt = mesh_TRANS[i*9+3];
      yPt = mesh_TRANS[i*9+4];
      zPt = mesh_TRANS[i*9+5];
      perspctv = rI.perspctv / (rI.perspctv - zPt);
      
      if (!rI.ortho)
      {
         xPt = xPt * perspctv;
         yPt = yPt * perspctv;
         zPt = zPt * perspctv;
      }
      
      asciiLocX = round(xPt + rI.xCenter);
      asciiLocY = round(yPt + rI.yCenter);
      asciiLocZ = zPt;
      
      printASCII(asciiStr, asciiLocX, asciiLocY, asciiLocZ, rI);
      
      if (asciiLocY+1 < rI.yWin)
      if (asciiLocX+1 < rI.xWin)
      if (asciiLocY >= 0)
      if (asciiLocX >= 0)
      {
         rI.scrnBuff[asciiLocX][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX][asciiLocY+1] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY+1] = 0x444444;
      }
   }
}

void viewPtLocs(float *mesh_POINT, int *mesh_POINTDATSZ, int mesh_MESHCNT, struct rendrInfo rI)
{
   float xPt, yPt, zPt, perspctv;
   int i;
   char asciiStr[1024];
   int asciiLocX, asciiLocY;
   float asciiLocZ;
   
   for (i = 0; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i=i+3)
   {
      sprintf(asciiStr, "%i", i);
      
      xPt = mesh_POINT[i];
      yPt = mesh_POINT[i+1];
      zPt = mesh_POINT[i+2];
      perspctv = rI.perspctv / (rI.perspctv - zPt);
      
      if (!rI.ortho)
      {
         xPt = xPt * perspctv;
         yPt = yPt * perspctv;
         zPt = zPt * perspctv;
      }
      
      asciiLocX = round(xPt + rI.xCenter);
      asciiLocY = round(yPt + rI.yCenter);
      asciiLocZ = zPt;
      
      printASCII(asciiStr, asciiLocX, asciiLocY, asciiLocZ, rI);
      
      if (asciiLocY+1 < rI.yWin)
      if (asciiLocX+1 < rI.xWin)
      if (asciiLocY >= 0)
      if (asciiLocX >= 0)
      {
         rI.scrnBuff[asciiLocX][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX][asciiLocY+1] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY+1] = 0x444444;
      }
   }
}

void viewLineLocs(int *mesh_LINE, int *mesh_LINEDATSZ, float *mesh_POINT, int mesh_MESHCNT, struct rendrInfo rI)
{
   float xPt, yPt, zPt, perspctv;
   int lineCnt;
   int pt1, pt2;
   char asciiStr[1024];
   int asciiLocX, asciiLocY;
   float asciiLocZ;
   
   lineCnt = 0;
   while (lineCnt < mesh_LINEDATSZ[mesh_MESHCNT-1])
   {
      pt1 = mesh_LINE[lineCnt+1] * 3;
      pt2 = mesh_LINE[lineCnt+2] * 3;
      
      sprintf(asciiStr, "%i", lineCnt);
      
      xPt = (mesh_POINT[pt2] - mesh_POINT[pt1]) / 2 + mesh_POINT[pt1];
      yPt = (mesh_POINT[pt2+1] - mesh_POINT[pt1+1]) / 2 + mesh_POINT[pt1+1];
      zPt = (mesh_POINT[pt2+2] - mesh_POINT[pt1+2]) / 2 + mesh_POINT[pt1+2];
      perspctv = rI.perspctv / (rI.perspctv - zPt);
      
      if (!rI.ortho)
      {
         xPt = xPt * perspctv;
         yPt = yPt * perspctv;
         zPt = zPt * perspctv;
      }
      
      asciiLocX = round(xPt + rI.xCenter);
      asciiLocY = round(yPt + rI.yCenter);
      asciiLocZ = zPt;
      
      printASCII(asciiStr, asciiLocX, asciiLocY, asciiLocZ, rI);
      
      if (asciiLocY+1 < rI.yWin)
      if (asciiLocX+1 < rI.xWin)
      if (asciiLocY >= 0)
      if (asciiLocX >= 0)
      {
         rI.scrnBuff[asciiLocX][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY] = 0x444444;
         rI.scrnBuff[asciiLocX][asciiLocY+1] = 0x444444;
         rI.scrnBuff[asciiLocX+1][asciiLocY+1] = 0x444444;
      }
      
      lineCnt = lineCnt + 3;
   }
}

void printASCII(char *asciiStr, int asciiX, int asciiY, float asciiZ, struct rendrInfo rI)
{
   #define asciiWidth 613
   #define asciiHeight 13
   int i, x, y;
   int asciiChar;
   
   asciiY = asciiY - 12;
   asciiX = asciiX + 1;
   i = 0;
   
   while ((int)asciiStr[i])
   {
      asciiChar = (int)asciiStr[i]-33;
      
      if (asciiChar != -1)
      for (y = asciiY+2; y < asciiHeight+asciiY; y++) //added the 2 -- y = asciiY+2;
      for (x = asciiSMPos[asciiChar]; x <= asciiSMPos[asciiChar+1]; x++)
      if ((x-(asciiSMPos[asciiChar])+asciiX >= 0) && (y >= 0))
      if ((x-(asciiSMPos[asciiChar])+asciiX < rI.xWin) && (y < rI.yWin))
      if (asciiZ >= rI.zIndex[x-(asciiSMPos[asciiChar])+asciiX][y])
      {
         rI.scrnBuff[x-(asciiSMPos[asciiChar])+asciiX][y] = asciiSM[(y-asciiY)*asciiWidth + x];
         rI.zIndex[x-(asciiSMPos[asciiChar])+asciiX][y] = asciiZ;
      }
      
      if (asciiChar == -1)
      asciiX = asciiX + 5; // Space Length
      else
      asciiX = asciiX + (asciiSMPos[asciiChar+1] - asciiSMPos[asciiChar]);
      i++;
   }
}
