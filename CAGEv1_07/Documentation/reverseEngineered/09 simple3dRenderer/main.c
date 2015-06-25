//Written by Crailin Mogged, 2011
#include <windows.h>
#include <math.h>
#include <stdio.h>

#define winWidth 400
#define winHeight 400

int xWinPos, yWinPos;
int xWin, yWin;
float xCenter, yCenter;
float perspctv;
float **zIndex;
int zIndexAllocated = 0;
int **scrnBuff;
int scrnBuffAllocated = 0;


BITMAPINFO pbmi[40];
BYTE *pBits;
int pBitsAllocated = 0;
HDC hdc;
PAINTSTRUCT ps;

float xP[8] = {-100, 100,-100, 100,-100, 100,-100, 100};
float yP[8] = { 100, 100,-100,-100, 100, 100,-100,-100};
float zP[8] = {-100,-100,-100,-100, 100, 100, 100, 100};

void drawScene(HWND);

void lrRot(float *, float *, float *, float);
void tbRot(float *, float *, float *, float);
void ccRot(float *, float *, float *, float);

void line(float, float, float,
          float, float, float, int);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szAppName[] = TEXT("thisApp");

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
   HWND hwnd;
   MSG msg;
   WNDCLASS wndclass;

   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = 0;
   wndclass.lpszMenuName = szAppName;
   wndclass.lpszClassName = szAppName;

   if (!RegisterClass(&wndclass))
   return 0;

   xWinPos = GetSystemMetrics(SM_CXSCREEN);
   yWinPos = GetSystemMetrics(SM_CYSCREEN);
   xWinPos = xWinPos / 2 - winWidth / 2;
   yWinPos = yWinPos / 2 - winHeight / 2;

   hwnd = CreateWindow(szAppName, TEXT("3d"),
                       WS_OVERLAPPEDWINDOW,//WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
                       xWinPos, yWinPos,
                       winWidth+6, winHeight+25, 
                       NULL, NULL, hInstance, NULL);

   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);

   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   int i;
   static int exitMsg = 0;
   float xMove = 0;
   float yMove = 0;
   float zMove = 0;

   static int winWidthOld;

   if (WM_CREATE == message)
   {
      pbmi->bmiHeader.biSize = 40;
      pbmi->bmiHeader.biWidth = winWidth;
      pbmi->bmiHeader.biHeight = winHeight;
      pbmi->bmiHeader.biPlanes = 1;
      pbmi->bmiHeader.biBitCount = 24;
      pbmi->bmiHeader.biCompression = BI_RGB;
      pbmi->bmiHeader.biSizeImage = winWidth * winHeight;
      pbmi->bmiHeader.biXPelsPerMeter = 0;
      pbmi->bmiHeader.biYPelsPerMeter = 0;
      pbmi->bmiHeader.biClrUsed = 0;
      pbmi->bmiHeader.biClrImportant = 0;
      return 0;
   }

   if (WM_SIZE == message)
   {
      xWin = LOWORD(lParam);
      yWin = HIWORD(lParam);
      xCenter = LOWORD(lParam) / 2;
      yCenter = HIWORD(lParam) / 2;

      pbmi->bmiHeader.biWidth = xWin;
      pbmi->bmiHeader.biHeight = yWin;
      pbmi->bmiHeader.biSizeImage = xWin * yWin;

      if (scrnBuffAllocated)
      {
         for (i = 0; i < winWidthOld; i++)
         free(scrnBuff[i]);
         free(scrnBuff);
      }
      scrnBuff = (int **)malloc(xWin * sizeof(int *));
      for (i = 0; i < xWin; i++)
      scrnBuff[i] = (int *)malloc(yWin * sizeof(int));
      scrnBuffAllocated = 1;
      
      if (pBitsAllocated)
      free(pBits);
      pBits = (BYTE *)malloc((xWin*3+xWin%4) * yWin);
      pBitsAllocated = 1;
      
      if (zIndexAllocated)
      {
         for (i = 0; i < winWidthOld; i++)
         free(zIndex[i]);
         free(zIndex);
      }
      zIndex = (float **)malloc(xWin * sizeof(float *));
      for (i = 0; i < xWin; i++)
      zIndex[i] = (float *)malloc(yWin * sizeof(float));
      zIndexAllocated = 1;
      
      winWidthOld = xWin;

      perspctv = 400;

      drawScene(hwnd);

      return 0;
   }

   if (WM_KEYDOWN == message)
   {
      if (37 == wParam)//Rotate Left
      xMove = -20.0 / 360;

      if (39 == wParam)//Rotate Right
      xMove = 20.0 / 360;

      if (38 == wParam)//Rotate Up
      yMove = 20.0 / 360;

      if (40 == wParam)//Rotate Down
      yMove = -20.0 / 360;
      
      if (82 == wParam)//Rotate Clockwise
      zMove = 20.0 / 360;

      if (76 == wParam)//Rotate Counterclockwise
      zMove = -20.0 / 360;

      for(i = 7; i >= 0; i--)
      lrRot(&xP[i], &yP[i], &zP[i], xMove);
      
      for(i = 7; i >= 0; i--)
      tbRot(&xP[i], &yP[i], &zP[i], yMove);
      
      for(i = 7; i >= 0; i--)
      ccRot(&xP[i], &yP[i], &zP[i], zMove);
      
      drawScene(hwnd);
      
      if (27 == wParam)
      exitMsg = 1;
      return 0;
   }

   if (WM_PAINT == message)
   {
      hdc = BeginPaint(hwnd, &ps);
      SetDIBitsToDevice(hdc, 0, 0, xWin, yWin,
                        0, 0, 0, yWin, pBits, pbmi, DIB_RGB_COLORS);
      EndPaint(hwnd, &ps);
      return 0;
   }

   if ((WM_DESTROY == message) || (exitMsg))
   {
      if (scrnBuffAllocated)
      {
         for (i = 0; i < xWin; i++)
         free(scrnBuff[i]);
         free(scrnBuff);
      }
      if (zIndexAllocated)
      {
         for (i = 0; i < xWin; i++)
         free(zIndex[i]);
         free(zIndex);
      }
      if (pBitsAllocated)
      free(pBits);

      PostQuitMessage(0);
      return 0;
   }

   return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawScene(HWND hwnd)
{
   int x, y;
   int rowSize;
   
   for (y = 0; y < yWin; y++)
   for (x = 0; x < xWin; x++)
   zIndex[x][y] = -500;

   for (y = 0; y < yWin; y++)
   for (x = 0; x < xWin; x++)
   scrnBuff[x][y] = 0x000000;

   line(xP[0], yP[0], zP[0],
        xP[1], yP[1], zP[1], 0x0000FF);
   line(xP[1], yP[1], zP[1],
        xP[3], yP[3], zP[3], 0x00FF00);
   line(xP[0], yP[0], zP[0],
        xP[2], yP[2], zP[2], 0xFF0000);
   line(xP[2], yP[2], zP[2],
        xP[3], yP[3], zP[3], 0xFFFFFF);

   line(xP[4], yP[4], zP[4],
        xP[5], yP[5], zP[5], 0x00FFFF);
   line(xP[5], yP[5], zP[5],
        xP[7], yP[7], zP[7], 0xFFFF00);
   line(xP[4], yP[4], zP[4],
        xP[6], yP[6], zP[6], 0x000088);
   line(xP[6], yP[6], zP[6],
        xP[7], yP[7], zP[7], 0x008800);
   
   line(xP[0], yP[0], zP[0],
        xP[4], yP[4], zP[4], 0x880000);
   line(xP[1], yP[1], zP[1],
        xP[5], yP[5], zP[5], 0x888888);
   line(xP[2], yP[2], zP[2],
        xP[6], yP[6], zP[6], 0x008888);
   line(xP[3], yP[3], zP[3],
        xP[7], yP[7], zP[7], 0x888800);


   rowSize = xWin * 3 + xWin % 4;

   for (y = 0; y < yWin; y++)
   for (x = 0; x < xWin; x++)
   {
      pBits[0+x*3+rowSize*y] = (char)((scrnBuff[x][y] & 0xFF0000) >> 16);
      pBits[1+x*3+rowSize*y] = (char)((scrnBuff[x][y] & 0x00FF00) >> 8);
      pBits[2+x*3+rowSize*y] = (char)scrnBuff[x][y] & 0x0000FF;
   }
   
   InvalidateRect(hwnd, NULL, TRUE);
   UpdateWindow(hwnd);
   hdc = GetDC(hwnd);

   SetDIBitsToDevice(hdc, 0, 0, xWin, yWin,
                     0, 0, 0, yWin, pBits, pbmi, DIB_RGB_COLORS);
   ReleaseDC(hwnd, hdc);
}

void line(float xA, float yA, float zA,
          float xB, float yB, float zB, int color)
{
   int i, length, x, y;
   float z;
   float perspctvA, perspctvB;
   float xAB, yAB, zAB;

   perspctvA = perspctv / (perspctv - zA);
   perspctvB = perspctv / (perspctv - zB);

   xA = xA * perspctvA + xCenter;
   yA = yA * perspctvA + yCenter;
   zA = zA * perspctvA;

   xB = xB * perspctvB + xCenter;
   yB = yB * perspctvB + yCenter;
   zB = zB * perspctvB;
   
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
      
      if ((x >= 0) && (x < xWin) &&
          (y >= 0) && (y < yWin))
      if (z >= zIndex[x][y])
      {
         scrnBuff[x][y] = color;
         zIndex[x][y] = z;
      }
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
