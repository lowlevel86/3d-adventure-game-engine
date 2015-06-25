#include <math.h>
#include <stdio.h>

struct rendrInfo
{
   int xWin;
   int yWin;
   float xCenter;
   float yCenter;
   float camLenZ;
   float camEndZ;
   float fogBgnZ;
   float fogEndZ;
   float perspctv;
   int ortho;
   float **zIndex;
   int **scrnBuff;
};
typedef struct rendrInfo RENDRINFO;

int colorTransparency(int, int, float);


void line(float xA, float yA, float zA,
          float xB, float yB, float zB, int color, struct rendrInfo rI)
{
	int i, length, x, y;
   float z;
   float perspctvA, perspctvB;
   float xAB, yAB, zAB;

   if ((zA > rI.camLenZ) && (zB > rI.camLenZ))
   return;
   if ((zA < rI.camEndZ) && (zB < rI.camEndZ))
   return;

   if (zA > rI.camLenZ)
   {
      xA = xB - (rI.camLenZ - zB) / (zA - zB) * (xB - xA);
      yA = yB - (rI.camLenZ - zB) / (zA - zB) * (yB - yA);
      zA = rI.camLenZ;
   }
   if (zB > rI.camLenZ)
   {
      xB = xA - (rI.camLenZ - zA) / (zB - zA) * (xA - xB);
      yB = yA - (rI.camLenZ - zA) / (zB - zA) * (yA - yB);
      zB = rI.camLenZ;
   }

   if (zA < rI.camEndZ)
   {
      xA = xB - (zB - rI.camEndZ) / (zB - zA) * (xB - xA);
      yA = yB - (zB - rI.camEndZ) / (zB - zA) * (yB - yA);
      zA = rI.camEndZ;
   }
   if (zB < rI.camEndZ)
   {
      xB = xA - (zA - rI.camEndZ) / (zA - zB) * (xA - xB);
      yB = yA - (zA - rI.camEndZ) / (zA - zB) * (yA - yB);
      zB = rI.camEndZ;
   }

   if (!rI.ortho)
   {
      perspctvA = rI.perspctv / (rI.perspctv - zA);
      perspctvB = rI.perspctv / (rI.perspctv - zB);

      xA = xA * perspctvA;
      yA = yA * perspctvA;
      zA = zA * perspctvA;

      xB = xB * perspctvB;
      yB = yB * perspctvB;
      zB = zB * perspctvB;
   }

   xA = xA + rI.xCenter;
   yA = yA + rI.yCenter;
   xB = xB + rI.xCenter;
   yB = yB + rI.yCenter;

   if (xA < 0)
   {
      zA = zB - (xB - 0) / (xB - xA) * (zB - zA);
      yA = yB - (xB - 0) / (xB - xA) * (yB - yA);
      xA = 0;
   }
   if (xA >= rI.xWin)
   {
      zA = zB - (xB - rI.xWin) / (xB - xA) * (zB - zA);
      yA = yB - (xB - rI.xWin) / (xB - xA) * (yB - yA);
      xA = rI.xWin;
   }
   if (yA < 0)
   {
      zA = zB - (yB - 0) / (yB - yA) * (zB - zA);
      xA = xB - (yB - 0) / (yB - yA) * (xB - xA);
      yA = 0;
   }
   if (yA >= rI.yWin)
   {
      zA = zB - (yB - rI.yWin) / (yB - yA) * (zB - zA);
      xA = xB - (yB - rI.yWin) / (yB - yA) * (xB - xA);
      yA = rI.yWin;
   }

   if (xB < 0)
   {
      zB = zA - (xA - 0) / (xA - xB) * (zA - zB);
      yB = yA - (xA - 0) / (xA - xB) * (yA - yB);
      xB = 0;
   }
   if (xB >= rI.xWin)
   {
      zB = zA - (xA - rI.xWin) / (xA - xB) * (zA - zB);
      yB = yA - (xA - rI.xWin) / (xA - xB) * (yA - yB);
      xB = rI.xWin;
   }
   if (yB < 0)
   {
      zB = zA - (yA - 0) / (yA - yB) * (zA - zB);
      xB = xA - (yA - 0) / (yA - yB) * (xA - xB);
      yB = 0;
   }
   if (yB >= rI.yWin)
   {
      zB = zA - (yA - rI.yWin) / (yA - yB) * (zA - zB);
      xB = xA - (yA - rI.yWin) / (yA - yB) * (xA - xB);
      yB = rI.yWin;
   }

   if (fabs(round(xB) - round(xA)) > fabs(round(yB) - round(yA)))
   length = fabs(round(xB) - round(xA));
   else
   length = fabs(round(yB) - round(yA));
   
   xAB = (xB - xA) / length;
   yAB = (yB - yA) / length;
   zAB = (zB - zA) / length;

   for (i = 0; i <= length; i++)
   {
      x = round(xAB * i + xA);
      y = round(yAB * i + yA);
      z = zAB * i + zA;
      
      if ((x >= 0) && (x < rI.xWin) &&
          (y >= 0) && (y < rI.yWin))
      if (z >= rI.zIndex[x][y])
      {
         rI.scrnBuff[x][y] = color;
         rI.zIndex[x][y] = z;
      }
   }
}

void fLine(float xA, float yA, float zA,
           float xB, float yB, float zB,
           int color, int fogColor, struct rendrInfo rI)
{
   int i, length, x, y;
   float z, transAmt;
   float fogBgnZ, fogEndZ, fogAmt;
   float perspctvA, perspctvB;
   float xAB, yAB, zAB;
   
   if ((zA > rI.camLenZ) && (zB > rI.camLenZ))
   return;
   if ((zA < rI.camEndZ) && (zB < rI.camEndZ))
   return;

   if (zA > rI.camLenZ)
   {
      xA = xB - (rI.camLenZ - zB) / (zA - zB) * (xB - xA);
      yA = yB - (rI.camLenZ - zB) / (zA - zB) * (yB - yA);
      zA = rI.camLenZ;
   }
   if (zB > rI.camLenZ)
   {
      xB = xA - (rI.camLenZ - zA) / (zB - zA) * (xA - xB);
      yB = yA - (rI.camLenZ - zA) / (zB - zA) * (yA - yB);
      zB = rI.camLenZ;
   }

   if (zA < rI.camEndZ)
   {
      xA = xB - (zB - rI.camEndZ) / (zB - zA) * (xB - xA);
      yA = yB - (zB - rI.camEndZ) / (zB - zA) * (yB - yA);
      zA = rI.camEndZ;
   }
   if (zB < rI.camEndZ)
   {
      xB = xA - (zA - rI.camEndZ) / (zA - zB) * (xA - xB);
      yB = yA - (zA - rI.camEndZ) / (zA - zB) * (yA - yB);
      zB = rI.camEndZ;
   }

   if (!rI.ortho)
   {
      perspctvA = rI.perspctv / (rI.perspctv - zA);
      perspctvB = rI.perspctv / (rI.perspctv - zB);

      xA = xA * perspctvA;
      yA = yA * perspctvA;
      zA = zA * perspctvA;

      xB = xB * perspctvB;
      yB = yB * perspctvB;
      zB = zB * perspctvB;
   }

   xA = xA + rI.xCenter;
   yA = yA + rI.yCenter;
   xB = xB + rI.xCenter;
   yB = yB + rI.yCenter;

   if (xA < 0)
   {
      zA = zB - (xB - 0) / (xB - xA) * (zB - zA);
      yA = yB - (xB - 0) / (xB - xA) * (yB - yA);
      xA = 0;
   }
   if (xA >= rI.xWin)
   {
      zA = zB - (xB - rI.xWin) / (xB - xA) * (zB - zA);
      yA = yB - (xB - rI.xWin) / (xB - xA) * (yB - yA);
      xA = rI.xWin;
   }
   if (yA < 0)
   {
      zA = zB - (yB - 0) / (yB - yA) * (zB - zA);
      xA = xB - (yB - 0) / (yB - yA) * (xB - xA);
      yA = 0;
   }
   if (yA >= rI.yWin)
   {
      zA = zB - (yB - rI.yWin) / (yB - yA) * (zB - zA);
      xA = xB - (yB - rI.yWin) / (yB - yA) * (xB - xA);
      yA = rI.yWin;
   }

   if (xB < 0)
   {
      zB = zA - (xA - 0) / (xA - xB) * (zA - zB);
      yB = yA - (xA - 0) / (xA - xB) * (yA - yB);
      xB = 0;
   }
   if (xB >= rI.xWin)
   {
      zB = zA - (xA - rI.xWin) / (xA - xB) * (zA - zB);
      yB = yA - (xA - rI.xWin) / (xA - xB) * (yA - yB);
      xB = rI.xWin;
   }
   if (yB < 0)
   {
      zB = zA - (yA - 0) / (yA - yB) * (zA - zB);
      xB = xA - (yA - 0) / (yA - yB) * (xA - xB);
      yB = 0;
   }
   if (yB >= rI.yWin)
   {
      zB = zA - (yA - rI.yWin) / (yA - yB) * (zA - zB);
      xB = xA - (yA - rI.yWin) / (yA - yB) * (xA - xB);
      yB = rI.yWin;
   }

   if (fabs(round(xB) - round(xA)) > fabs(round(yB) - round(yA)))
   length = fabs(round(xB) - round(xA));
   else
   length = fabs(round(yB) - round(yA));
   
   xAB = (xB - xA) / length;
   yAB = (yB - yA) / length;
   zAB = (zB - zA) / length;

   if (!rI.ortho)
   {
      fogBgnZ = (rI.fogBgnZ * (rI.perspctv / (rI.perspctv - rI.fogBgnZ)));
      fogEndZ = (rI.fogEndZ * (rI.perspctv / (rI.perspctv - rI.fogEndZ)));
   }
   
   fogAmt = fogEndZ - fogBgnZ;
   
   for (i = 0; i <= length; i++)
   {
      x = round(xAB * i + xA);
      y = round(yAB * i + yA);
      z = zAB * i + zA;
      
      transAmt = (z - fogBgnZ) / fogAmt;
      if (z > fogBgnZ)
      transAmt = 0;
      if (z <= fogEndZ)
      transAmt = 1;
      
      if ((x >= 0) && (x < rI.xWin) &&
          (y >= 0) && (y < rI.yWin))
      if (z >= rI.zIndex[x][y])
      {
         rI.scrnBuff[x][y] = colorTransparency(color, fogColor, transAmt);
         rI.zIndex[x][y] = z;
      }
   }
}

int colorTransparency(int mainColor, int bgColor, float transAmt)
{
   int r, g, b;
   
   r = transAmt * ((bgColor & 0xFF) - (mainColor & 0xFF)) + (mainColor & 0xFF);
   g = transAmt * (((bgColor & 0xFF00) >> 8) - ((mainColor & 0xFF00) >> 8)) + ((mainColor & 0xFF00) >> 8);
   b = transAmt * (((bgColor & 0xFF0000) >> 16) - ((mainColor & 0xFF0000) >> 16)) + ((mainColor & 0xFF0000) >> 16);
   
   mainColor = (int)b<<16 | (int)g<<8 | (int)r;
   
   return(mainColor);
}
