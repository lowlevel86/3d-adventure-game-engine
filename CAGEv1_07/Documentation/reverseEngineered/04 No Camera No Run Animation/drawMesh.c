#include <stdio.h>
#include <math.h>
#include "renderer/renderer.h"

void drawMeshf(float *, int *, int *, int *, int, int, struct rendrInfo);
void drawMeshaf(float *, int *, int *, int *, int, int, struct rendrInfo);

void drawMesh(float *mesh_POINT, int *mesh_LINE, int *mesh_COLOR,
              int *mesh_LINEDATSZ, int mesh_MESHCNT, int bgColor, int antialiasing, struct rendrInfo rI)
{
   if (!antialiasing)
   drawMeshf(mesh_POINT, mesh_LINE, mesh_COLOR,
             mesh_LINEDATSZ, mesh_MESHCNT, bgColor, rI);
   if (antialiasing)
   drawMeshaf(mesh_POINT, mesh_LINE, mesh_COLOR,
              mesh_LINEDATSZ, mesh_MESHCNT, bgColor, rI);
}

// a = antialiasing, f = fog

void drawMeshf(float *mesh_POINT, int *mesh_LINE, int *mesh_COLOR,
               int *mesh_LINEDATSZ, int mesh_MESHCNT, int bgColor, struct rendrInfo rI)
{
   int sidesNum;
   int pt1, pt2;
   int pt3, pt4;
   int color;
   int colorCnt = 0;
   int lineCnt = 0;
   
   while (lineCnt < mesh_LINEDATSZ[mesh_MESHCNT-1])
   {
      sidesNum = mesh_LINE[lineCnt];
      
      color = mesh_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         
         fLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
               mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
      
      if (sidesNum == 3)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         
         fLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
               mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         fLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
               mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         fLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
               mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 4;
         colorCnt++;
      }
      
      if (sidesNum == 4)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         pt4 = mesh_LINE[lineCnt+4] * 3;
         
         fLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
               mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         fLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
               mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         fLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
               mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2], color, bgColor, rI);
         
         fLine(mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2],
               mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 5;
         colorCnt++;
      }
   }
}

void drawMeshaf(float *mesh_POINT, int *mesh_LINE, int *mesh_COLOR,
                int *mesh_LINEDATSZ, int mesh_MESHCNT, int bgColor, struct rendrInfo rI)
{
   int sidesNum;
   int pt1, pt2;
   int pt3, pt4;
   int color;
   int colorCnt = 0;
   int lineCnt = 0;
   
   while (lineCnt < mesh_LINEDATSZ[mesh_MESHCNT-1])
   {
      sidesNum = mesh_LINE[lineCnt];
      
      color = mesh_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
      
      if (sidesNum == 3)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
                mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
                mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 4;
         colorCnt++;
      }
      
      if (sidesNum == 4)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         pt4 = mesh_LINE[lineCnt+4] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
                mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
                mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2],
                mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 5;
         colorCnt++;
      }
   }
}
