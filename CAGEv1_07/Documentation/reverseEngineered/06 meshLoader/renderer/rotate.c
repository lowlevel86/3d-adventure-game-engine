#include <math.h>

void lrRot(float *, float *, float *, float); // left to right or reverse rotation
void tbRot(float *, float *, float *, float); // top to bottom or reverse rotation
void ccRot(float *, float *, float *, float); // clockwise or reverse rotation

void rotate(float *xP, float *yP, float *zP, float lr, float tb, float cc)
{
   float lrtbR, lrccR, tbccR;
   
   if ((lr) && (!tb) && (!cc))
   {
      lrRot(&*xP, &*yP, &*zP, lr);
      return;
   }
   
   if ((!lr) && (tb) && (!cc))
   {
      tbRot(&*xP, &*yP, &*zP, tb);
      return;
   }
   
   if ((!lr) && (!tb) && (cc))
   {
      ccRot(&*xP, &*yP, &*zP, cc);
      return;
   }
   
   if ((lr) && (tb) && (!cc))
   {
      // get the rotation length that lr and tb produce
      lrtbR = sqrt(lr * lr + tb * tb);
      //lrtbR
      if (lr < 0)
      lrtbR = -lrtbR;
      
      // find the amount of rotation to move lrtbR to the x axis
      cc = asin(tb / lrtbR);
      
      // rotate lrtbR to the x axis
      ccRot(&*xP, &*yP, &*zP, cc);
      
      // rotate the amount of lrtbR
      lrRot(&*xP, &*yP, &*zP, lrtbR);
      
      // rotate back from the x axis
      ccRot(&*xP, &*yP, &*zP, -cc);
      
      return;
   }
   
   if ((lr) && (!tb) && (cc))
   {
      lrccR = sqrt(lr * lr + cc * cc);
      if (lr < 0)
      lrccR = -lrccR;
      
      tb = asin(cc / -lrccR);
      
      tbRot(&*xP, &*yP, &*zP, tb);
      lrRot(&*xP, &*yP, &*zP, lrccR);
      tbRot(&*xP, &*yP, &*zP, -tb);
      return;
   }
   
   if ((!lr) && (tb) && (cc))
   {
      tbccR = sqrt(tb * tb + cc * cc);
      if (tb < 0)
      tbccR = -tbccR;
      
      lr = asin(cc / tbccR);
      
      lrRot(&*xP, &*yP, &*zP, lr);
      tbRot(&*xP, &*yP, &*zP, tbccR);
      lrRot(&*xP, &*yP, &*zP, -lr);
      return;
   }
   
   if ((lr) && (tb) && (cc))// mentally hard
   {
      lrtbR = sqrt(lr * lr + tb * tb);
      if (lr > 0)
      lrtbR = -lrtbR;
      
      tbccR = sqrt(lrtbR * lrtbR + cc * cc);
      if (tb < 0)
      tbccR = -tbccR;
      
      lr = asin(cc / tbccR);
      
      cc = asin(tb / lrtbR);
      
      ccRot(&*xP, &*yP, &*zP, cc);
      lrRot(&*xP, &*yP, &*zP, lr);
      
      tbRot(&*xP, &*yP, &*zP, tbccR);
      
      lrRot(&*xP, &*yP, &*zP, -lr);
      ccRot(&*xP, &*yP, &*zP, -cc);
      
      return;
   }
}

void lrRot(float *xP, float *yP, float *zP, float lrLgth)
{
   if (!isnormal(lrLgth))
   return;
   
   float lgthP = sqrt(*xP * *xP + *zP * *zP);
   
   if (!isnormal(lgthP))
   return;

   float asinPt = asin(*xP / lgthP);

   if (*zP > 0)
   {
      *xP = sin(asinPt + lrLgth) * lgthP;
      *zP = cos(asinPt + lrLgth) * lgthP;
   }
   else
   {
      *xP = sin(asinPt - lrLgth) * lgthP;
      *zP = -cos(asinPt - lrLgth) * lgthP;
   }
}

void tbRot(float *xP, float *yP, float *zP, float tbLgth)
{
   if (!isnormal(tbLgth))
   return;
   
   float lgthP = sqrt(*yP * *yP + *zP * *zP);

   if (!isnormal(lgthP))
   return;

   float asinPt = asin(*yP / lgthP);

   if (*zP > 0)
   {
      *yP = sin(asinPt + tbLgth) * lgthP;
      *zP = cos(asinPt + tbLgth) * lgthP;
   }
   else
   {
      *yP = sin(asinPt - tbLgth) * lgthP;
      *zP = -cos(asinPt - tbLgth) * lgthP;
   }
}

void ccRot(float *xP, float *yP, float *zP, float ccLgth)
{
   if (!isnormal(ccLgth))
   return;

   float lgthP = sqrt(*xP * *xP + *yP * *yP);

   if (!isnormal(lgthP))
   return;
   
   float asinPt = asin(*xP / lgthP);
   
   if (*yP > 0)
   {
      *xP = sin(asinPt + ccLgth) * lgthP;
      *yP = cos(asinPt + ccLgth) * lgthP;
   }
   else
   {
      *xP = sin(asinPt - ccLgth) * lgthP;
      *yP = -cos(asinPt - ccLgth) * lgthP;
   }
}
