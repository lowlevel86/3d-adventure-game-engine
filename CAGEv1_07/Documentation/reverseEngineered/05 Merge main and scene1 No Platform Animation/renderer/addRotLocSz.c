# include "renderer.h"

void ptRelAxises(float, float, float,
                 float, float, float,
                 float, float, float,
                 float, float, float,
                 float *, float *, float *);
/*
//add global rotation, location and size from the global transformation data to the transformation data
void addGlobalRotLocSz(float *mesh_GLOBAL, float *mesh_TRANS, float *mesh_POINT, int *mesh_POINTDATSZ, float *mesh_STATICPOINT, int mesh_MESHCNT)
{
   int h, i, PDsize;
   
   float xHori, yHori, zHori;
   float xVert, yVert, zVert;
   float xFtBk, yFtBk, zFtBk;
   
   //Add the correct size to each transformation block
   //for (i = 0; i < mesh_MESHCNT; i=i+9)
   //{
   //   mesh_TRANS[i+6] = mesh_STATICTRANS[i+6] * mesh_GLOBAL[6];//x
   //   mesh_TRANS[i+7] = mesh_STATICTRANS[i+7] * mesh_GLOBAL[7];//y
   //   mesh_TRANS[i+8] = mesh_STATICTRANS[i+8] * mesh_GLOBAL[8];//z
   //}
   
   //Add the correct rotation to each transformation block
   xHori = 1.0;
   yHori = 0.0;
   zHori = 0.0;
   rotate(&xHori, &yHori, &zHori, 0, -mesh_GLOBAL[0+i], 0);
   rotate(&xHori, &yHori, &zHori, mesh_GLOBAL[1+i], 0, 0);
   rotate(&xHori, &yHori, &zHori, 0, 0, -mesh_GLOBAL[2+i]);
   
   xVert = 0.0;
   yVert = 1.0;
   zVert = 0.0;
   rotate(&xVert, &yVert, &zVert, 0, -mesh_GLOBAL[0+i], 0);
   rotate(&xVert, &yVert, &zVert, mesh_GLOBAL[1+i], 0, 0);
   rotate(&xVert, &yVert, &zVert, 0, 0, -mesh_GLOBAL[2+i]);
   
   xFtBk = 0.0;
   yFtBk = 0.0;
   zFtBk = 1.0;
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, -mesh_GLOBAL[0+i], 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, mesh_GLOBAL[1+i], 0, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -mesh_GLOBAL[2+i]);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   addRotAngsToAngs(mesh_GLOBAL[0], mesh_GLOBAL[1], mesh_GLOBAL[2],
                    &mesh_TRANS[i+0], &mesh_TRANS[i+1], &mesh_TRANS[i+2]);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   ptRelAxises(xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               mesh_TRANS[i+3], mesh_TRANS[i+4], mesh_TRANS[i+5],
               &mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5]);
   
   //Add the correct location to each transformation block
   //for (i = 0; i < mesh_MESHCNT; i=i+9)
   //{
   //   mesh_TRANS[i+3] = mesh_TRANS[i+3] + mesh_GLOBAL[3];//x
   //   mesh_TRANS[i+4] = mesh_TRANS[i+4] + mesh_GLOBAL[4];//y
   //   mesh_TRANS[i+5] = mesh_TRANS[i+5] + mesh_GLOBAL[5];//z
   //}
}
*/
//add rotation, location and size from the transformation data to the vertices
void addRotLocSz(float *mesh_TRANS, float *mesh_POINT, int *mesh_POINTDATSZ, float *mesh_STATICPOINT, int mesh_MESHCNT)
{
   int h, i, PDsize;
   
   float xHori, yHori, zHori;
   float xVert, yVert, zVert;
   float xFtBk, yFtBk, zFtBk;
   
   PDsize = mesh_POINTDATSZ[mesh_MESHCNT-1];
   
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
   for (i = 0; i < mesh_MESHCNT * 9; i=i+9)
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

// Find the position of a point by using an object's axises
// so you don't have to rotate each and every point using sin.
// You find the position of a point relative to an object's axises
// which has not been rotated, then you use that data to find the position
// of that same point relative to the new axises which has been rotated.
void ptRelAxises(float xObjHori, float yObjHori, float zObjHori,
                 float xObjVert, float yObjVert, float zObjVert,
                 float xObjFtBk, float yObjFtBk, float zObjFtBk,
                 float xStatic, float yStatic, float zStatic,
                 float *x, float *y, float *z)
{
   float xVH, yVH, zVH;
   
   zStatic = zStatic * 2;
   yStatic = yStatic * 4;
   xStatic = xStatic * 4;
   
   xObjHori = xObjHori * xStatic;
   yObjHori = yObjHori * xStatic;
   zObjHori = zObjHori * xStatic;
   
   xObjVert = xObjVert * yStatic;
   yObjVert = yObjVert * yStatic;
   zObjVert = zObjVert * yStatic;
   
   xObjFtBk = xObjFtBk * zStatic;
   yObjFtBk = yObjFtBk * zStatic;
   zObjFtBk = zObjFtBk * zStatic;
   
   xVH = (xObjVert - xObjHori) / 2 + xObjHori;
   yVH = (yObjVert - yObjHori) / 2 + yObjHori;
   zVH = (zObjVert - zObjHori) / 2 + zObjHori;
   
   *x = (xVH - xObjFtBk) / 2 + xObjFtBk;
   *y = (yVH - yObjFtBk) / 2 + yObjFtBk;
   *z = (zVH - zObjFtBk) / 2 + zObjFtBk;
}
