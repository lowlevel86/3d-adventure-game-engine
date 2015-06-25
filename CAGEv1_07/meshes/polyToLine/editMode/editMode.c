#include <math.h>
#include <stdio.h>
#include "../renderer/renderer.h"
#include "editMode.h"

int nodeHighlightNum = -1;

void showEditNodes(float *mesh_POINT, int *mesh_POINTDATSZ, int mesh_MESHCNT, struct rendrInfo rI)
{
   float xPt, yPt, zPt, perspctv;
   int i, color;
   int nodeLocX, nodeLocY;
   float nodeLocZ;
   
   for (i = 0; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i=i+3)
   {
      if (i == nodeHighlightNum)
      color = 0x00FFFF;
      else
      color = 0xFF0000;
      
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
      
      nodeLocX = round(xPt + rI.xCenter);
      nodeLocY = round(yPt + rI.yCenter);
      nodeLocZ = zPt;
      
      if (nodeLocY+1 < rI.yWin)
      if (nodeLocX+1 < rI.xWin)
      if (nodeLocY >= 0)
      if (nodeLocX >= 0)
      {
         rI.scrnBuff[nodeLocX][nodeLocY] = color;
         rI.scrnBuff[nodeLocX+1][nodeLocY] = color;
         rI.scrnBuff[nodeLocX][nodeLocY+1] = color;
         rI.scrnBuff[nodeLocX+1][nodeLocY+1] = color;
      }
   }
}

void removeNode(float *mesh_STATICPOINT, int *mesh_LINE, int *mesh_COLOR,
                int *mesh_POINTDATSZ, int *mesh_LINEDATSZ, int *mesh_COLORDATSZ, int mesh_MESHCNT)
{
   int i, lineCnt, lineCnt2, negColorDatSz;
   
   // remove the highlighted node
   for (i = 0; i < mesh_MESHCNT; i++)
   if (mesh_POINTDATSZ[i] > nodeHighlightNum)
   mesh_POINTDATSZ[i] = mesh_POINTDATSZ[i] - 3;
   
   for (i = nodeHighlightNum; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i++)
   mesh_STATICPOINT[i] = mesh_STATICPOINT[i+3];
   
   // mark the lines that was connected to the node
   for (i=0; i < mesh_LINEDATSZ[mesh_MESHCNT-1]; i=i+3)
   {
      if ((mesh_LINE[i+1] == nodeHighlightNum/3) ||
          (mesh_LINE[i+2] == nodeHighlightNum/3))
      mesh_LINE[i] = 0;
   }
   
   // change the line data so it matches up with the point data
   for (i=0; i < mesh_LINEDATSZ[mesh_MESHCNT-1]; i=i+3)
   {
      if (mesh_LINE[i+1] > nodeHighlightNum/3)
      mesh_LINE[i+1] = mesh_LINE[i+1] - 1;
      
      if (mesh_LINE[i+2] > nodeHighlightNum/3)
      mesh_LINE[i+2] = mesh_LINE[i+2] - 1;
   }
   
   // change the color data size
   lineCnt = 0;
   negColorDatSz = 0;
   for (i=0; i < mesh_MESHCNT; i++)
   {
      while (lineCnt < mesh_LINEDATSZ[i])
      {
         if (mesh_LINE[lineCnt] == 0)
         negColorDatSz++;
         
         lineCnt = lineCnt + 3;
      }
      
      mesh_COLORDATSZ[i] = mesh_COLORDATSZ[i] - negColorDatSz;
   }
   
   // remove the lines that were marked
   lineCnt = 0;
   lineCnt2 = 0;
   while (lineCnt < mesh_LINEDATSZ[mesh_MESHCNT-1])
   {
      if (mesh_LINE[lineCnt] == 0)
      {
         if (lineCnt+lineCnt2 < mesh_LINEDATSZ[mesh_MESHCNT-1])
         while (mesh_LINE[lineCnt+lineCnt2] == 0)
         if (lineCnt+lineCnt2 < mesh_LINEDATSZ[mesh_MESHCNT-1])
         lineCnt2 = lineCnt2 + 3;
         else
         break;
         
         if (lineCnt+lineCnt2 < mesh_LINEDATSZ[mesh_MESHCNT-1])
         {
            mesh_LINE[lineCnt+0] = mesh_LINE[lineCnt+lineCnt2+0];
            mesh_LINE[lineCnt+1] = mesh_LINE[lineCnt+lineCnt2+1];
            mesh_LINE[lineCnt+2] = mesh_LINE[lineCnt+lineCnt2+2];
            mesh_LINE[lineCnt+lineCnt2+0] = 0;
            
            mesh_COLOR[lineCnt/3] = mesh_COLOR[(lineCnt+lineCnt2)/3];
         }
      }
      
      lineCnt = lineCnt + 3;
   }
   
   //change the line data size
   for (i=0; i < mesh_MESHCNT; i++)
   mesh_LINEDATSZ[i] = mesh_COLORDATSZ[i] * 3;
   
   nodeHighlightNum = -1;
}
