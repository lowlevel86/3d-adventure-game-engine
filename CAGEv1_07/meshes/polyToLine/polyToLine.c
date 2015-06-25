#include <stdio.h>

int colorTransparency(int, int, float);

void polyToLineNewSzs(int *meshOld_LINEDATSZ, int *mesh_LINE,
                      int *mesh_LINEDATSZ, int mesh_MESHCNT)
{
   int i, lineCnt, sidesNum, newLineCnt;
   
   lineCnt = 0;
   newLineCnt = 0;
   for (i=0; i < mesh_MESHCNT; i++)
   while (lineCnt < meshOld_LINEDATSZ[i])
   {
      sidesNum = lineCnt;
      
      if (mesh_LINE[sidesNum] == 3)
      {
         newLineCnt = newLineCnt + 3+2+2+2;
         lineCnt = lineCnt + 4;
      }
      
      if (mesh_LINE[sidesNum] == 4)
      {
         newLineCnt = newLineCnt + 4+2+2+2+2;
         lineCnt = lineCnt + 5;
      }
      
      mesh_LINEDATSZ[i] = newLineCnt;
   }
}

void polyToLine(int *meshOld_LINE, int *meshOld_COLOR,
                int *meshOld_LINEDATSZ, int *meshOld_COLORDATSZ,
                int *mesh_LINE, int *mesh_COLOR,
                int *mesh_LINEDATSZ, int *mesh_COLORDATSZ, int mesh_MESHCNT)
{
   int i, lineCnt, lineCnt2, sidesNum, newLineCnt, colorCnt;
   float colorMixAmt;
   int negColorDatSz;
   
   //convert from polygons to lines
   lineCnt = 0;
   newLineCnt = 0;
   colorCnt = 0;
   while (lineCnt < meshOld_LINEDATSZ[mesh_MESHCNT-1])
   {
      sidesNum = lineCnt;
      
      if (meshOld_LINE[sidesNum] == 3)
      {
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+1];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+2];
         newLineCnt++;
         
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+2];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+3];
         newLineCnt++;
         
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+3];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+1];
         newLineCnt++;
         
         colorCnt++;
         lineCnt = lineCnt + 4;
      }
      
      if (meshOld_LINE[sidesNum] == 4)
      {
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+1];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+2];
         newLineCnt++;
         
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+2];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+3];
         newLineCnt++;
         
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+3];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+4];
         newLineCnt++;
         
         mesh_COLOR[newLineCnt/3] = meshOld_COLOR[colorCnt];
         mesh_LINE[newLineCnt] = 2;
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+4];
         newLineCnt++;
         mesh_LINE[newLineCnt] = meshOld_LINE[sidesNum+1];
         newLineCnt++;
         
         colorCnt++;
         lineCnt = lineCnt + 5;
      }
   }
   
   //mark the lines that are redundant and mix the colors
   lineCnt = 0;
   lineCnt2 = 3;
   colorMixAmt = 0.5;
   for (i=0; i < mesh_MESHCNT; i++)
   while (lineCnt < mesh_LINEDATSZ[i])
   {
      if (((mesh_LINE[lineCnt+1] == mesh_LINE[lineCnt+lineCnt2+1]) &&
           (mesh_LINE[lineCnt+2] == mesh_LINE[lineCnt+lineCnt2+2])) ||
          ((mesh_LINE[lineCnt+1] == mesh_LINE[lineCnt+lineCnt2+2]) &&
           (mesh_LINE[lineCnt+2] == mesh_LINE[lineCnt+lineCnt2+1])))
      {
         mesh_LINE[lineCnt+lineCnt2] = 0;
         
         mesh_COLOR[lineCnt/3] = colorTransparency(mesh_COLOR[lineCnt/3],
                                                   mesh_COLOR[(lineCnt+lineCnt2)/3], colorMixAmt);
         colorMixAmt = colorMixAmt / 2;
      }
      
      lineCnt2 = lineCnt2 + 3;
      
      if (lineCnt+lineCnt2 >= mesh_LINEDATSZ[i])
      {
         lineCnt = lineCnt + 3;
         lineCnt2 = 3;
         colorMixAmt = 0.5;
      }
   }
   
   //change the color data size
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
   
   //remove the redundant lines
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
}
