#include <stdio.h>
#include <math.h>
#include "renderer/renderer.h"

void ptRelAxises(float, float, float,
                 float, float, float,
                 float, float, float,
                 float, float, float,
                 float *, float *, float *);

// global rotation
void globalRot(float *mesh_GLOBAL, float *mesh_TRANS, int mesh_MESHCNT,
               float xCenter, float yCenter, float zCenter, float x, float y, float z)
{
   int i;
   
   float xHori = 1.0, yHori = 0.0, zHori = 0.0;
   float xVert = 0.0, yVert = 1.0, zVert = 0.0;
   float xFtBk = 0.0, yFtBk = 0.0, zFtBk = 1.0;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] - xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] - yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] - zCenter;
   }
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] - xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] - yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] - zCenter;
   
   addRotAngsToAngs(x, y, z, &mesh_GLOBAL[0], &mesh_GLOBAL[1], &mesh_GLOBAL[2]);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   addRotAngsToAngs(x, y, z, &mesh_TRANS[i+0], &mesh_TRANS[i+1], &mesh_TRANS[i+2]);
   
   rotate(&xHori, &yHori, &zHori, 0, -x, 0);
   rotate(&xHori, &yHori, &zHori, y, 0, 0);
   rotate(&xHori, &yHori, &zHori, 0, 0, -z);
   
   rotate(&xVert, &yVert, &zVert, 0, -x, 0);
   rotate(&xVert, &yVert, &zVert, y, 0, 0);
   rotate(&xVert, &yVert, &zVert, 0, 0, -z);
   
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, -x, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, y, 0, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -z);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   ptRelAxises(xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               mesh_TRANS[i+3], mesh_TRANS[i+4], mesh_TRANS[i+5],
               &mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5]);
   
   ptRelAxises(xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               mesh_GLOBAL[3], mesh_GLOBAL[4], mesh_GLOBAL[5],
               &mesh_GLOBAL[3], &mesh_GLOBAL[4], &mesh_GLOBAL[5]);
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] + xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] + yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] + zCenter;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] + xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] + yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] + zCenter;
   }
}

// local rotation
void localRot(float *mesh_TRANS, int meshNum,
              float xCenter, float yCenter, float zCenter, float x, float y, float z)
{
   int i;
   
   i = meshNum * 9;
   
   addRotAngsToAngs(x, y, z, &mesh_TRANS[i+0], &mesh_TRANS[i+1], &mesh_TRANS[i+2]);
   
   rotatePoint(&mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5],
               xCenter, yCenter, zCenter, 0, -x, 0);
   rotatePoint(&mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5],
               xCenter, yCenter, zCenter, y, 0, 0);
   rotatePoint(&mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5],
               xCenter, yCenter, zCenter, 0, 0, -z);
}

// global rotation relative to screen
void globalRotRelToScrn(float *mesh_GLOBAL, float *mesh_TRANS, int mesh_MESHCNT,
                        float xCenter, float yCenter, float zCenter, float lr, float tb, float cc)
{
   int i;
   
   float xHori = 1.0, yHori = 0.0, zHori = 0.0;
   float xVert = 0.0, yVert = 1.0, zVert = 0.0;
   float xFtBk = 0.0, yFtBk = 0.0, zFtBk = 1.0;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] - xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] - yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] - zCenter;
   }
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] - xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] - yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] - zCenter;
   
   addParallelRotAngsToAngs(lr, tb, cc, &mesh_GLOBAL[0], &mesh_GLOBAL[1], &mesh_GLOBAL[2]);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   addParallelRotAngsToAngs(lr, tb, cc, &mesh_TRANS[i+0], &mesh_TRANS[i+1], &mesh_TRANS[i+2]);
   
   rotate(&xHori, &yHori, &zHori, lr, tb, cc);
   rotate(&xVert, &yVert, &zVert, lr, tb, cc);
   rotate(&xFtBk, &yFtBk, &zFtBk, lr, tb, cc);
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   ptRelAxises(xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               mesh_TRANS[i+3], mesh_TRANS[i+4], mesh_TRANS[i+5],
               &mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5]);
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] + xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] + yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] + zCenter;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] + xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] + yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] + zCenter;
   }
}

// local rotation relative to screen
void localRotRelToScrn(float *mesh_TRANS, int meshNum,
                       float xCenter, float yCenter, float zCenter, float lr, float tb, float cc)
{
   int i;
   
   i = meshNum * 9;
   
   addParallelRotAngsToAngs(lr, tb, cc,
                            &mesh_TRANS[i+0], &mesh_TRANS[i+1], &mesh_TRANS[i+2]);
   
   rotatePoint(&mesh_TRANS[i+3], &mesh_TRANS[i+4], &mesh_TRANS[i+5],
               xCenter, yCenter, zCenter, lr, tb, cc);
}


// global reposition
void globalMove(float *mesh_GLOBAL, float *mesh_TRANS, int mesh_MESHCNT, float x, float y, float z)
{
   int i;
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] + x;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] + y;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] + z;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] + x;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] + y;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] + z;
   }
}

// local reposition
void localMove(float *mesh_TRANS, int meshNum, float x, float y, float z)
{
   int i;
   
   i = meshNum * 9;
   
   mesh_TRANS[i+3] = mesh_TRANS[i+3] + x;
   mesh_TRANS[i+4] = mesh_TRANS[i+4] + y;
   mesh_TRANS[i+5] = mesh_TRANS[i+5] + z;
}


// global resize
void globalResize(float *mesh_GLOBAL, float *mesh_TRANS, int mesh_MESHCNT,
                  float xCenter, float yCenter, float zCenter, float x, float y, float z)
{
   int i;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] - xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] - yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] - zCenter;
   }
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] - xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] - yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] - zCenter;
   
   mesh_GLOBAL[6] = mesh_GLOBAL[6] * x;
   mesh_GLOBAL[7] = mesh_GLOBAL[7] * y;
   mesh_GLOBAL[8] = mesh_GLOBAL[8] * z;
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] * x;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] * y;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] * z;
   
   mesh_GLOBAL[3] = mesh_GLOBAL[3] + xCenter;
   mesh_GLOBAL[4] = mesh_GLOBAL[4] + yCenter;
   mesh_GLOBAL[5] = mesh_GLOBAL[5] + zCenter;
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+6] = mesh_TRANS[i+6] * x;
      mesh_TRANS[i+7] = mesh_TRANS[i+7] * y;
      mesh_TRANS[i+8] = mesh_TRANS[i+8] * z;
   }
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] * x;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] * y;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] * z;
   }
   
   for (i = 0; i < mesh_MESHCNT*9; i=i+9)
   {
      mesh_TRANS[i+3] = mesh_TRANS[i+3] + xCenter;
      mesh_TRANS[i+4] = mesh_TRANS[i+4] + yCenter;
      mesh_TRANS[i+5] = mesh_TRANS[i+5] + zCenter;
   }
}

// local resize
void localResize(float *mesh_TRANS, int meshNum,
                 float xCenter, float yCenter, float zCenter, float x, float y, float z)
{
   int i;
   
   i = meshNum * 9;
   
   mesh_TRANS[i+3] = mesh_TRANS[i+3] - xCenter;
   mesh_TRANS[i+4] = mesh_TRANS[i+4] - yCenter;
   mesh_TRANS[i+5] = mesh_TRANS[i+5] - zCenter;
   
   mesh_TRANS[i+6] = mesh_TRANS[i+6] * x;
   mesh_TRANS[i+7] = mesh_TRANS[i+7] * y;
   mesh_TRANS[i+8] = mesh_TRANS[i+8] * z;
   
   mesh_TRANS[i+3] = mesh_TRANS[i+3] * x;
   mesh_TRANS[i+4] = mesh_TRANS[i+4] * y;
   mesh_TRANS[i+5] = mesh_TRANS[i+5] * z;
   
   mesh_TRANS[i+3] = mesh_TRANS[i+3] + xCenter;
   mesh_TRANS[i+4] = mesh_TRANS[i+4] + yCenter;
   mesh_TRANS[i+5] = mesh_TRANS[i+5] + zCenter;
}
