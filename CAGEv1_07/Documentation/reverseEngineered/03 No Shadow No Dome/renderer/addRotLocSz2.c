# include "renderer.h"

void ptRelAxises(float, float, float,
                 float, float, float,
                 float, float, float,
                 float, float, float,
                 float *, float *, float *);

//add a Screen rotation, location and size
void addScreenRotLocSz(float xRot, float yRot, float zRot,
                       float xSize, float ySize, float zSize,
                       float xLoc, float yLoc, float zLoc,
                       float *mesh_POINT, int *mesh_POINTDATSZ, int meshCnt)
{
   int i;
   float xHori, yHori, zHori;
   float xVert, yVert, zVert;
   float xFtBk, yFtBk, zFtBk;
   
   //add the Screen rotation
   xHori = 1.0;
   yHori = 0.0;
   zHori = 0.0;
   rotate(&xHori, &yHori, &zHori, 0, -xRot, 0);
   rotate(&xHori, &yHori, &zHori, yRot, 0, 0);
   rotate(&xHori, &yHori, &zHori, 0, 0, -zRot);
   
   xVert = 0.0;
   yVert = 1.0;
   zVert = 0.0;
   rotate(&xVert, &yVert, &zVert, 0, -xRot, 0);
   rotate(&xVert, &yVert, &zVert, yRot, 0, 0);
   rotate(&xVert, &yVert, &zVert, 0, 0, -zRot);
   
   xFtBk = 0.0;
   yFtBk = 0.0;
   zFtBk = 1.0;
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, -xRot, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, yRot, 0, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -zRot);
   
   for (i = 0; i < mesh_POINTDATSZ[meshCnt-1]; i=i+3)
   ptRelAxises(xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               mesh_POINT[0+i], mesh_POINT[1+i], mesh_POINT[2+i],
               &mesh_POINT[0+i], &mesh_POINT[1+i], &mesh_POINT[2+i]);
   
   //add the Screen location and size
   for (i = 0; i < mesh_POINTDATSZ[meshCnt-1]; i=i+3)
   {
      mesh_POINT[i+0] = (mesh_POINT[i+0] + xLoc) * xSize;
      mesh_POINT[i+1] = (mesh_POINT[i+1] + yLoc) * ySize;
      mesh_POINT[i+2] = (mesh_POINT[i+2] + zLoc) * zSize;
   }
}
/*
void addGlobalRotLocSz(float *mesh_GLOBAL, float *mesh_TRANS, float *mesh_POINT,
                       int *mesh_POINTDATSZ, float *mesh_STATICPOINT, int meshCnt)
   int h, i, PDsize;
   
   float xHori, yHori, zHori;
   float xVert, yVert, zVert;
   float xFtBk, yFtBk, zFtBk;
   
   PDsize = mesh_POINTDATSZ[meshCnt-1];
   
   //Add the correct size to each mesh
   h = 0;
   for (i = 0; i < PDsize; i=i+3)
   {
      if (i == mesh_POINTDATSZ[h])
      h++;
      mesh_POINT[i+0] = mesh_STATICPOINT[i+0] * mesh_TRANS[6+9*h];//x
      mesh_POINT[i+1] = mesh_STATICPOINT[i+1] * mesh_TRANS[7+9*h];//y
      mesh_POINT[i+2] = mesh_STATICPOINT[i+2] * mesh_TRANS[8+9*h];//z
   }
   
   //Add the correct rotation to each mesh
   h = 0;
   for (i = 0; i < meshCnt * 9; i=i+9)
   {
      xHori = 1.0;
      yHori = 0.0;
      zHori = 0.0;
      rotate(&xHori, &yHori, &zHori, 0, -mesh_TRANS[0+i], 0);
      rotate(&xHori, &yHori, &zHori, mesh_TRANS[1+i], 0, 0);
      rotate(&xHori, &yHori, &zHori, 0, 0, -mesh_TRANS[2+i]);
      
      xVert = 0.0;
      yVert = 1.0;
      zVert = 0.0;
      rotate(&xVert, &yVert, &zVert, 0, -mesh_TRANS[0+i], 0);
      rotate(&xVert, &yVert, &zVert, mesh_TRANS[1+i], 0, 0);
      rotate(&xVert, &yVert, &zVert, 0, 0, -mesh_TRANS[2+i]);
      
      xFtBk = 0.0;
      yFtBk = 0.0;
      zFtBk = 1.0;
      rotate(&xFtBk, &yFtBk, &zFtBk, 0, -mesh_TRANS[0+i], 0);
      rotate(&xFtBk, &yFtBk, &zFtBk, mesh_TRANS[1+i], 0, 0);
      rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -mesh_TRANS[2+i]);
      
      if (i == 0)
      h = 0;
      else
      h = mesh_POINTDATSZ[i/9-1];
      
      while (h < mesh_POINTDATSZ[i/9])
      {
         ptRelAxises(xHori, yHori, zHori,
                     xVert, yVert, zVert,
                     xFtBk, yFtBk, zFtBk,
                     mesh_POINT[h], mesh_POINT[h+1], mesh_POINT[h+2],
                     &mesh_POINT[h], &mesh_POINT[h+1], &mesh_POINT[h+2]);
         h=h+3;
      }
   }
   
   //Add the correct location to each mesh
   h = 0;
   for (i = 0; i < PDsize; i=i+3)
   {
      if (i == mesh_POINTDATSZ[h])
      h++;
      mesh_POINT[i+0] = mesh_POINT[i+0] + mesh_TRANS[3+9*h];//x
      mesh_POINT[i+1] = mesh_POINT[i+1] + mesh_TRANS[4+9*h];//y
      mesh_POINT[i+2] = mesh_POINT[i+2] + mesh_TRANS[5+9*h];//z
   }
}
*/
