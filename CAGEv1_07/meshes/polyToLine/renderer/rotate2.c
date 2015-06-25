#include <math.h>
#include <stdio.h>
#include "renderer.h"

/*
NOTE:
addParallelRotToRot()
addRotToRot()
*/

void normOrientRtnRot(float, float, float,
                      float, float, float,
                      float, float, float, float *, float *, float *);

void rotatePoint(float *xP, float *yP, float *zP,
                 float xCenter, float yCenter, float zCenter, float lrR, float tbR, float ccR)
{
   float xPh, yPh, zPh;
   
   xPh = *xP - xCenter;
   yPh = *yP - yCenter;
   zPh = *zP - zCenter;
   rotate(&xPh, &yPh, &zPh, lrR, tbR, ccR);
   *xP = xPh + xCenter;
   *yP = yPh + yCenter;
   *zP = zPh + zCenter;
}

//add parallel rotation (lr, tb, cc) angles to the angles the rotation data creates (xR, yR, zR)
void rotateRelToScrn(float lr, float tb, float cc, float *xR, float *yR, float *zR)
{
   float xHoriAdd = 1.0, yHoriAdd = 0.0, zHoriAdd = 0.0;
   float xVertAdd = 0.0, yVertAdd = 1.0, zVertAdd = 0.0;
   float xFtBkAdd = 0.0, yFtBkAdd = 0.0, zFtBkAdd = 1.0;
   
   float xHori = 1.0, yHori = 0.0, zHori = 0.0;
   float xVert = 0.0, yVert = 1.0, zVert = 0.0;
   float xFtBk = 0.0, yFtBk = 0.0, zFtBk = 1.0;

   // Get the orientation you want to add to xR, yR, zR
   rotate(&xHoriAdd, &yHoriAdd, &zHoriAdd, lr, tb, cc);
   rotate(&xVertAdd, &yVertAdd, &zVertAdd, lr, tb, cc);
   rotate(&xFtBkAdd, &yFtBkAdd, &zFtBkAdd, lr, tb, cc);
   
   // Get the original orientation
   rotate(&xHori, &yHori, &zHori, 0, -*xR, 0);
   rotate(&xHori, &yHori, &zHori, *yR, 0, 0);
   rotate(&xHori, &yHori, &zHori, 0, 0, -*zR);
   
   rotate(&xVert, &yVert, &zVert, 0, -*xR, 0);
   rotate(&xVert, &yVert, &zVert, *yR, 0, 0);
   rotate(&xVert, &yVert, &zVert, 0, 0, -*zR);
   
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, -*xR, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, *yR, 0, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -*zR);
   
   
   normOrientRtnRot(xHoriAdd, yHoriAdd, zHoriAdd,
                    xVertAdd, yVertAdd, zVertAdd,
                    xFtBkAdd, yFtBkAdd, zFtBkAdd, &lr, &tb, &cc);
   
   rotate(&xHori, &yHori, &zHori, 0, -tb, 0);
   rotate(&xVert, &yVert, &zVert, 0, -tb, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, -tb, 0);
   
   rotate(&xHori, &yHori, &zHori, -lr, 0, 0);
   rotate(&xVert, &yVert, &zVert, -lr, 0, 0);
   rotate(&xFtBk, &yFtBk, &zFtBk, -lr, 0, 0);
   
   rotate(&xHori, &yHori, &zHori, 0, 0, -cc);
   rotate(&xVert, &yVert, &zVert, 0, 0, -cc);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, -cc);

   normOrientRtnRot(xHori, yHori, zHori,
                    xVert, yVert, zVert,
                    xFtBk, yFtBk, zFtBk, &lr, &tb, &cc);
   
   *xR = tb;
   *yR = -lr;
   *zR = cc;
}

// normalize orientation and return rotation
void normOrientRtnRot(float xHori, float yHori, float zHori,
                      float xVert, float yVert, float zVert,
                      float xFtBk, float yFtBk, float zFtBk,
                      float *lr, float *tb, float *cc)
{
   float xyHori;
   
   // rotate "Hori" to the x plane
   xyHori = sqrt(xHori * xHori + yHori * yHori);
   if (xHori < 0)
   xyHori = -xyHori;
   
   if ((yHori / xyHori <= 1.0) && (yHori / xyHori >= -1.0))
   *cc = asin(yHori / xyHori);
   
   if (yHori / xyHori > 1.0)
   *cc = asin(1.0);
   if (yHori / xyHori < -1.0)
   *cc = asin(-1.0);
   
   rotate(&xHori, &yHori, &zHori, 0, 0, *cc);
   rotate(&xVert, &yVert, &zVert, 0, 0, *cc);
   rotate(&xFtBk, &yFtBk, &zFtBk, 0, 0, *cc);
   
   
   // rotate "Hori" to the z plane
   if (xHori < 0)
   {
      if ((zHori / -1.0 <= 1.0) && (zHori / -1.0 >= -1.0))
      *lr = asin(zHori / -1.0);
      
      if (zHori / -1.0 > 1.0)
      *lr = asin(1.0);
      if (zHori / -1.0 < -1.0)
      *lr = asin(-1.0);
   }
   else
   {
      if ((zHori / 1.0 <= 1.0) && (zHori / 1.0 >= -1.0))
      *lr = asin(zHori / 1.0);
      
      if (zHori / 1.0 > 1.0)
      *lr = asin(1.0);
      if (zHori / 1.0 < -1.0)
      *lr = asin(-1.0);
   }
   
   // make xHori positive if it is not
   if (xHori < 0)
   *lr = *lr + M_PI;
   
   //rotate(&xHori, &yHori, &zHori, *lr, 0, 0);
   rotate(&xVert, &yVert, &zVert, *lr, 0, 0);
   //rotate(&xFtBk, &yFtBk, &zFtBk, *lr, 0, 0);
   
   
   // rotate "Vert" to the z plane
   if (yVert < 0)
   {
      if ((zVert / -1.0 <= 1.0) && (zVert / -1.0 >= -1.0))
      *tb = asin(zVert / -1.0);
      
      if (zVert / -1.0 > 1.0)
      *tb = asin(1.0);
      if (zVert / -1.0 < -1.0)
      *tb = asin(-1.0);
   }
   else
   {
      if ((zVert / 1.0 <= 1.0) && (zVert / 1.0 >= -1.0))
      *tb = asin(zVert / 1.0);
      
      if (zVert / 1.0 > 1.0)
      *tb = asin(1.0);
      if (zVert / 1.0 < -1.0)
      *tb = asin(-1.0);
   }
   
   // make yVert positive if it is not
   if (yVert < 0)
   *tb = *tb + M_PI;
}
