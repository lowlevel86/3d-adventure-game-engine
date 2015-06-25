//Written by Crailin Mogged, 2011

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "renderer/renderer.h"

#define winWidth 400
#define winHeight 400

int xWinPos, yWinPos;

BITMAPINFO pbmi[40];
BYTE *pBits;
int pBitsAllocated = 0;
HDC hdc;
PAINTSTRUCT ps;
RENDRINFO rI;

float xCent = 0, yCent = 0, zCent = 0;
float xHori = 1.0, yHori = 0, zHori = 0;
float xVert = 0, yVert = 1.0, zVert = 0;
float xFtBk = 0, yFtBk = 0, zFtBk = 1.0;

float xP[11] = {-100, 100,-100, 100,-100, 100,-100, 100, 150, 0, 0};
float yP[11] = { 100, 100,-100,-100, 100, 100,-100,-100, 0, 150, 0};
float zP[11] = {-100,-100,-100,-100, 100, 100, 100, 100, 0, 0, 150};

float xOL[11];
float yOL[11];
float zOL[11];

void drawScene(HWND);

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
   static int exitMsg;
   static int drag;
   static float xMmem;
   static float yMmem;
   float lrRot = 0;
   float tbRot = 0;
   float ccRot = 0;

   static int winWidthOld;
   static int **scrnBuff;
   int scrnBuffAllocated = 0;
   static float **zIndex;
   int zIndexAllocated = 0;

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
      
      for(i = 10; i >= 0; i--)
      getObjLgths(xP[i], yP[i], zP[i], &xOL[i], &yOL[i], &zOL[i]);
      
      return 0;
   }

   if (WM_SIZE == message)
   {
      rI.xWin = LOWORD(lParam);
      rI.yWin = HIWORD(lParam);
      rI.xCenter = LOWORD(lParam) / 2;
      rI.yCenter = HIWORD(lParam) / 2;
      
      pbmi->bmiHeader.biWidth = rI.xWin;
      pbmi->bmiHeader.biHeight = rI.yWin;
      pbmi->bmiHeader.biSizeImage = rI.xWin * rI.yWin;
      
      if (scrnBuffAllocated)
      {
         for (i = 0; i < winWidthOld; i++)
         free(scrnBuff[i]);
         free(scrnBuff);
      }
      scrnBuff = (int **)malloc(rI.xWin * sizeof(int *));
      for (i = 0; i < rI.xWin; i++)
      scrnBuff[i] = (int *)malloc(rI.yWin * sizeof(int));
      scrnBuffAllocated = 1;
      
      if (pBitsAllocated)
      free(pBits);
      pBits = (BYTE *)malloc((rI.xWin*3+rI.xWin%4) * rI.yWin);
      pBitsAllocated = 1;
      
      if (zIndexAllocated)
      {
         for (i = 0; i < winWidthOld; i++)
         free(zIndex[i]);
         free(zIndex);
      }
      zIndex = (float **)malloc(rI.xWin * sizeof(float *));
      for (i = 0; i < rI.xWin; i++)
      zIndex[i] = (float *)malloc(rI.yWin * sizeof(float));
      zIndexAllocated = 1;
      
      winWidthOld = rI.xWin;
      
      rI.camLenZ = 300;
      rI.camEndZ = -300;
      rI.fogBgnZ = 100;
      rI.fogEndZ = -300;
      rI.perspctv = 400;
      rI.ortho = 0;
      rI.zIndex = zIndex;
      rI.scrnBuff = scrnBuff;
      
      drawScene(hwnd);
      
      return 0;
   }

   if (WM_KEYDOWN == message)
   {
      if (37 == wParam)//Rotate Left
      lrRot = -50.0 / 360;
      
      if (39 == wParam)//Rotate Right
      lrRot = 50.0 / 360;
      
      if (38 == wParam)//Rotate Up
      tbRot = 50.0 / 360;
      
      if (40 == wParam)//Rotate Down
      tbRot = -50.0 / 360;
      
      if (105 == wParam)//Rotate Clockwise "R"
      ccRot = 50.0 / 360;
      
      if (103 == wParam)//Rotate Counterclockwise "L"
      ccRot = -50.0 / 360;
      
      rotate(&xHori, &yHori, &zHori, lrRot, tbRot, ccRot);
      
      rotate(&xVert, &yVert, &zVert, lrRot, tbRot, ccRot);
      
      rotate(&xFtBk, &yFtBk, &zFtBk, lrRot, tbRot, ccRot);
      
      drawScene(hwnd);
      
      if (27 == wParam)
      exitMsg = 1;
      return 0;
   }

   if (WM_LBUTTONDOWN == message)
   {
      drag = 1;
      xMmem = LOWORD(lParam);
      yMmem = HIWORD(lParam);
      return 0;
   }
   
   if (WM_LBUTTONUP == message)
   {
      drag = 0;
      return 0;
   }
   
   if (WM_MOUSEMOVE == message)
   {
      if (drag)
      {
         lrRot = LOWORD(lParam) - xMmem;
         tbRot = -(HIWORD(lParam) - yMmem);
         
         xMmem = LOWORD(lParam);
         yMmem = HIWORD(lParam);
         
         lrRot = lrRot / 80;
         tbRot = tbRot / 80;
         
         rotate(&xHori, &yHori, &zHori, lrRot, tbRot, ccRot);
         
         rotate(&xVert, &yVert, &zVert, lrRot, tbRot, ccRot);
         
         rotate(&xFtBk, &yFtBk, &zFtBk, lrRot, tbRot, ccRot);
         
         drawScene(hwnd);
      }
      return 0;
   }

   if (WM_PAINT == message)
   {
      hdc = BeginPaint(hwnd, &ps);
      SetDIBitsToDevice(hdc, 0, 0, rI.xWin, rI.yWin,
                        0, 0, 0, rI.yWin, pBits, pbmi, DIB_RGB_COLORS);
      EndPaint(hwnd, &ps);
      return 0;
   }

   if ((WM_DESTROY == message) || (exitMsg))
   {
      if (scrnBuffAllocated)
      {
         for (i = 0; i < rI.xWin; i++)
         free(scrnBuff[i]);
         free(scrnBuff);
      }
      if (zIndexAllocated)
      {
         for (i = 0; i < rI.xWin; i++)
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
   int x, y, i;
   int rowSize;
   
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.zIndex[x][y] = rI.camEndZ;

   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.scrnBuff[x][y] = 0x000000;

   for(i = 10; i >= 0; i--)
   getRelObjPt(xCent, yCent, zCent,
               xHori, yHori, zHori,
               xVert, yVert, zVert,
               xFtBk, yFtBk, zFtBk,
               xOL[i], yOL[i], zOL[i], &xP[i], &yP[i], &zP[i]);

   
   fLine(0, 0, 0, xP[8], yP[8], zP[8], 0x0000FF, 0x0, rI);
   fLine(0, 0, 0, xP[9], yP[9], zP[9], 0x00FF00, 0x0, rI);
   fLine(0, 0, 0, xP[10], yP[10], zP[10], 0xFF0000, 0x0, rI);
   
   fLine(xP[0], yP[0], zP[0],
         xP[1], yP[1], zP[1], 0xCCCCCC, 0x0, rI);
   fLine(xP[1], yP[1], zP[1],
         xP[3], yP[3], zP[3], 0xCCCCCC, 0x0, rI);
   fLine(xP[0], yP[0], zP[0],
         xP[2], yP[2], zP[2], 0xCCCCCC, 0x0, rI);
   fLine(xP[2], yP[2], zP[2],
         xP[3], yP[3], zP[3], 0xCCCCCC, 0x0, rI);

   fLine(xP[4], yP[4], zP[4],
         xP[5], yP[5], zP[5], 0xCCCCCC, 0x0, rI);
   fLine(xP[5], yP[5], zP[5],
         xP[7], yP[7], zP[7], 0xCCCCCC, 0x0, rI);
   fLine(xP[4], yP[4], zP[4],
         xP[6], yP[6], zP[6], 0xCCCCCC, 0x0, rI);
   fLine(xP[6], yP[6], zP[6],
         xP[7], yP[7], zP[7], 0xCCCCCC, 0x0, rI);
   
   fLine(xP[0], yP[0], zP[0],
         xP[4], yP[4], zP[4], 0xCCCCCC, 0x0, rI);
   fLine(xP[1], yP[1], zP[1],
         xP[5], yP[5], zP[5], 0xCCCCCC, 0x0, rI);
   fLine(xP[2], yP[2], zP[2],
         xP[6], yP[6], zP[6], 0xCCCCCC, 0x0, rI);
   fLine(xP[3], yP[3], zP[3],
         xP[7], yP[7], zP[7], 0xCCCCCC, 0x0, rI);

   rowSize = rI.xWin * 3 + rI.xWin % 4;

   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   {
      pBits[0+x*3+rowSize*y] = (char)((rI.scrnBuff[x][y] & 0xFF0000) >> 16);
      pBits[1+x*3+rowSize*y] = (char)((rI.scrnBuff[x][y] & 0x00FF00) >> 8);
      pBits[2+x*3+rowSize*y] = (char)rI.scrnBuff[x][y] & 0x0000FF;
   }
   
   InvalidateRect(hwnd, NULL, TRUE);
   UpdateWindow(hwnd);
   hdc = GetDC(hwnd);

   SetDIBitsToDevice(hdc, 0, 0, rI.xWin, rI.yWin,
                     0, 0, 0, rI.yWin, pBits, pbmi, DIB_RGB_COLORS);
   ReleaseDC(hwnd, hdc);
}
