// Extend the xyz axises of an object to a certain length
// beyond a point in order to derive its position later.
void getObjLgths(float x, float y, float z,
                 float *xObjLgth, float *yObjLgth, float *zObjLgth)
{
   *zObjLgth = z * 2;
   *yObjLgth = y * 4;
   *xObjLgth = x * 4;
}

// Find the position of a point by using an object's axises
// so you don't have to rotate each and every point using sin.
// Find a point using an object's extended axises.
void getRelObjPt(float xObjCent, float yObjCent, float zObjCent,
                 float xObjHori, float yObjHori, float zObjHori,
                 float xObjVert, float yObjVert, float zObjVert,
                 float xObjFtBk, float yObjFtBk, float zObjFtBk,
                 float xObjLgth, float yObjLgth, float zObjLgth,
                 float *x, float *y, float *z)
{
   float xVH, yVH, zVH;
   
   xObjHori = (xObjHori - xObjCent) * xObjLgth + xObjCent;
   yObjHori = (yObjHori - yObjCent) * xObjLgth + yObjCent;
   zObjHori = (zObjHori - zObjCent) * xObjLgth + zObjCent;
   
   xObjVert = (xObjVert - xObjCent) * yObjLgth + xObjCent;
   yObjVert = (yObjVert - yObjCent) * yObjLgth + yObjCent;
   zObjVert = (zObjVert - zObjCent) * yObjLgth + zObjCent;
   
   xObjFtBk = (xObjFtBk - xObjCent) * zObjLgth + xObjCent;
   yObjFtBk = (yObjFtBk - yObjCent) * zObjLgth + yObjCent;
   zObjFtBk = (zObjFtBk - zObjCent) * zObjLgth + zObjCent;
   
   xVH = (xObjVert - xObjHori) / 2 + xObjHori;
   yVH = (yObjVert - yObjHori) / 2 + yObjHori;
   zVH = (zObjVert - zObjHori) / 2 + zObjHori;
   
   *x = (xVH - xObjFtBk) / 2 + xObjFtBk;
   *y = (yVH - yObjFtBk) / 2 + yObjFtBk;
   *z = (zVH - zObjFtBk) / 2 + zObjFtBk;
}
