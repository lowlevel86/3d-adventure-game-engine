//Written by Crailin Mogged, 2013

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "renderer/renderer.h"
#include "shapes.h"

#define winWidth 440
#define winHeight 320
int xWinPos, yWinPos;

float xMove;
float yMove;
float zMove;

float lftk = 0;
float rghtk = 0;
float upk = 0;
float downk = 0;
float forwdk = 0;
float bckwdk = 0;
float mvLftk = 0;
float mvRghtk = 0;
float mvUpk = 0;
float mvDownk = 0;
float clckwzk = 0;
float cntrClckwzk = 0;
float szDeck = 1.0;
float szInck = 1.0;

#define ID_TIMER 1
time_t now;
int nowHold;
float fps = 20;
float fpsNum = 1;
int startStopTimer = 0;

int pBitsAllocated = 0;
int scrnBuffAllocated = 0;
int zIndexAllocated = 0;

BITMAPINFO pbmi[40];
BYTE *pBits;
HDC hdc;
PAINTSTRUCT ps;
RENDRINFO rI;

void drawScene(HWND);

void rotatePt(float *, float *, float *,
              float, float, float, float, float, float);

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

   hwnd = CreateWindowEx(WS_EX_LEFT | WS_EX_ACCEPTFILES,
                         szAppName, TEXT("Shapes"),
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

   static int winWidthOld;
   static int **scrnBuff;
   static float **zIndex;

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
      
      for (i = 0; i < sizeof(shapes_POINT)/sizeof(float); i++)
      shapes_STATICPOINT[i] = shapes_POINT[i];
      
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

      rI.camLenZ = 350;
      rI.camEndZ = -1200;
      rI.fogBgnZ = 50;
      rI.fogEndZ = -1200;
      rI.perspctv = 400;
      rI.ortho = 0;
      rI.zIndex = zIndex;
      rI.scrnBuff = scrnBuff;
      
      drawScene(hwnd);
      return 0;
   }

   
	if (WM_TIMER == message)
	{
      if (startStopTimer)
      {
         xMove = (lftk + rghtk) * fpsNum;
         yMove = (upk + downk) * fpsNum;
         zMove = (clckwzk + cntrClckwzk) * fpsNum;
         
         //rotate
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[0+i] = shapes_TRANS[0+i] + xMove;
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[1+i] = shapes_TRANS[1+i] + yMove;
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[2+i] = shapes_TRANS[2+i] + zMove;
         
         //Move
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[3+i] = shapes_TRANS[3+i] + mvRghtk - mvLftk;//x
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[4+i] = shapes_TRANS[4+i] + mvUpk - mvDownk;//y
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[5+i] = shapes_TRANS[5+i] + forwdk - bckwdk;//z
         
         //resize
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         {
            shapes_TRANS[6+i] = shapes_TRANS[6+i] * szInck;//x
            shapes_TRANS[7+i] = shapes_TRANS[7+i] * szInck;//y
            shapes_TRANS[8+i] = shapes_TRANS[8+i] * szInck;//z
         }
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         {
            shapes_TRANS[3+i] = (shapes_TRANS[3+i] - shapes_TRANS[3]) * szInck + shapes_TRANS[3];//x
            shapes_TRANS[4+i] = (shapes_TRANS[4+i] - shapes_TRANS[4]) * szInck + shapes_TRANS[4];//y
            shapes_TRANS[5+i] = (shapes_TRANS[5+i] - shapes_TRANS[5]) * szInck + shapes_TRANS[5];//z
         }
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         {
            shapes_TRANS[6+i] = shapes_TRANS[6+i] * szDeck;//x
            shapes_TRANS[7+i] = shapes_TRANS[7+i] * szDeck;//y
            shapes_TRANS[8+i] = shapes_TRANS[8+i] * szDeck;//z
         }
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         {
            shapes_TRANS[3+i] = (shapes_TRANS[3+i] - shapes_TRANS[3]) * szDeck + shapes_TRANS[3];//x
            shapes_TRANS[4+i] = (shapes_TRANS[4+i] - shapes_TRANS[4]) * szDeck + shapes_TRANS[4];//y
            shapes_TRANS[5+i] = (shapes_TRANS[5+i] - shapes_TRANS[5]) * szDeck + shapes_TRANS[5];//z
         }
         
         drawScene(hwnd);
         
         nowHold = (int)now;
         now = time((time_t *)NULL);
         fps++;
         if ((int)now != nowHold)
         {
            fpsNum = 30.0 / fps;
            fps = 0;
         }
      }
   }
   
   
   if (WM_KEYUP == message)
   {
      if (70 == wParam)//Move forward f
      forwdk = 0;
      if (66 == wParam)//Move backward b
      bckwdk = 0;
      if (100 == wParam)//Move left keypad 4
      mvLftk = 0;
      if (102 == wParam)//Move right keypad 6
      mvRghtk = 0;
      if (104 == wParam)//Move up keypad 8
      mvUpk = 0;
      if (98 == wParam)//Move down keypad 2
      mvDownk = 0;
      
      if (37 == wParam)//Rotate Left
      lftk = 0;
      if (39 == wParam)//Rotate Right
      rghtk = 0;
      if (38 == wParam)//Rotate Up
      upk = 0;
      if (40 == wParam)//Rotate Down
      downk = 0;
      
      if (105 == wParam)//Rotate Clockwise
      clckwzk = 0;
      if (103 == wParam)//Rotate Counterclockwise
      cntrClckwzk = 0;
      
      if ((187 == wParam) || (107 == wParam))//Increase Size +
      szInck = 1.0;
      if ((189 == wParam) || (109 == wParam))//Increase Size -
      szDeck = 1.0;
      
      if (!forwdk)
      if (!bckwdk)
      if (!mvLftk)
      if (!mvRghtk)
      if (!mvUpk)
      if (!mvDownk)
      if (!lftk)
      if (!rghtk)
      if (!upk)
      if (!downk)
      if (!clckwzk)
      if (!cntrClckwzk)
      if (szInck == 1.0)
      if (szDeck == 1.0)
      {
         startStopTimer = 0;
         KillTimer(hwnd, ID_TIMER);
      }
      return 0;
   }
   
   
   if (WM_KEYDOWN == message)
   {
      if (70 == wParam)//Move forward f
      {
         bckwdk = 0;
         forwdk = 2;
      }
      if (66 == wParam)//Move backward b
      {
         forwdk = 0;
         bckwdk = 2;
      }
      if (100 == wParam)//Move left keypad 4
      {
         mvRghtk = 0;
         mvLftk = 2;
      }
      if (102 == wParam)//Move right keypad 6
      {
         mvLftk = 0;
         mvRghtk = 2;
      }
      if (104 == wParam)//Move up keypad 8
      {
         mvDownk = 0;
         mvUpk = 2;
      }
      if (98 == wParam)//Move down keypad 2
      {
         mvUpk = 0;
         mvDownk = 2;
      }
      
      if (37 == wParam)//Rotate Left
      {
         rghtk = 0;
         lftk = -2.0 / 80;
      }
      if (39 == wParam)//Rotate Right
      {
         lftk = 0;
         rghtk = 2.0 / 80;
      }
      if (38 == wParam)//Rotate Up
      {
         downk = 0;
         upk = 2.0 / 80;
      }
      if (40 == wParam)//Rotate Down
      {
         upk = 0;
         downk = -2.0 / 80;
      }
      
      if (105 == wParam)//Rotate Clockwise
      {
         cntrClckwzk = 0;
         clckwzk = 2.0 / 80;
      }
      if (103 == wParam)//Rotate Counterclockwise
      {
         clckwzk = 0;
         cntrClckwzk = -2.0 / 80;
      }
      
      if ((187 == wParam) || (107 == wParam))//Increase Size +
      {
         szDeck = 1.0;
         szInck = 1.1;
      }
      if ((189 == wParam) || (109 == wParam))//Increase Size -
      {
         szInck = 1.0;
         szDeck = 0.9;
      }
      
      
      if (startStopTimer == 0)
      {
         SetTimer(hwnd, ID_TIMER, 50, NULL);
         now = time((time_t *)NULL);
         startStopTimer = 1;
      }
      
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
      if (drag == 1)
      {
         xMove = (LOWORD(lParam) - xMmem);
         yMove = -(HIWORD(lParam) - yMmem);

         xMmem = LOWORD(lParam);
         yMmem = HIWORD(lParam);

         xMove = xMove / 80;
         yMove = yMove / 80;
         zMove = 0;
         
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         addParallelRotAngsToAngs(xMove, yMove, zMove, 
                                  &shapes_TRANS[0+i], &shapes_TRANS[1+i], &shapes_TRANS[2+i]);
         
         drawScene(hwnd);
      }
      return 0;
   }

   if (WM_MOUSEWHEEL == message)
   {
      if (HIWORD(wParam) == 120)
      {
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[5+i] = shapes_TRANS[5+i] + 10;//z
      }
      if (HIWORD(wParam) == 65416)
      {
         for (i = 0; i < sizeof(shapes_TRANS)/sizeof(float); i=i+9)
         shapes_TRANS[5+i] = shapes_TRANS[5+i] - 10;//z
      }
      
      drawScene(hwnd);
      return 0;
   }

   if (WM_PAINT == message)
   if (startStopTimer == 0)
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

void rotatePt(float *xP, float *yP, float *zP,
              float xCent, float yCent, float zCent, float xR, float yR, float zR)
{
   float xPh;
   float yPh;
   float zPh;
   
   xPh = *xP - xCent;
   yPh = *yP - yCent;
   zPh = *zP - zCent;
   rotate(&xPh, &yPh, &zPh, xR, yR, zR);
   *xP = xPh + xCent;
   *yP = yPh + yCent;
   *zP = zPh + zCent;
}

void drawScene(HWND hwnd)
{
   int x, y;
   int color, colorCnt, bgColor;
   int lineCnt;
   int sidesNum;
   int pt1, pt2;
   int pt3, pt4;
   
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.zIndex[x][y] = rI.camEndZ;

   bgColor = 0xFFFFFF;
   
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.scrnBuff[x][y] = bgColor;

   addRotLocSz(shapes_TRANS,
               shapes_POINT,
               shapes_POINTDATSZ,
               shapes_STATICPOINT, shapes_MESHCNT);
   
   colorCnt = 0;
   lineCnt = 0;
   while (lineCnt < sizeof(shapes_LINE)/sizeof(int))
   {
      sidesNum = shapes_LINE[lineCnt];
      
      color = shapes_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = shapes_LINE[lineCnt+1] * 3;
         pt2 = shapes_LINE[lineCnt+2] * 3;
         
         afLine(shapes_POINT[pt1], shapes_POINT[pt1+1], shapes_POINT[pt1+2],
                shapes_POINT[pt2], shapes_POINT[pt2+1], shapes_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
      
      if (sidesNum == 3)
      {
         pt1 = shapes_LINE[lineCnt+1] * 3;
         pt2 = shapes_LINE[lineCnt+2] * 3;
         pt3 = shapes_LINE[lineCnt+3] * 3;
         
         afLine(shapes_POINT[pt1], shapes_POINT[pt1+1], shapes_POINT[pt1+2],
                shapes_POINT[pt2], shapes_POINT[pt2+1], shapes_POINT[pt2+2], color, bgColor, rI);
         
         afLine(shapes_POINT[pt2], shapes_POINT[pt2+1], shapes_POINT[pt2+2],
                shapes_POINT[pt3], shapes_POINT[pt3+1], shapes_POINT[pt3+2], color, bgColor, rI);
         
         afLine(shapes_POINT[pt3], shapes_POINT[pt3+1], shapes_POINT[pt3+2],
                shapes_POINT[pt1], shapes_POINT[pt1+1], shapes_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 4;
         colorCnt++;
      }
      
      if (sidesNum == 4)
      {
         pt1 = shapes_LINE[lineCnt+1] * 3;
         pt2 = shapes_LINE[lineCnt+2] * 3;
         pt3 = shapes_LINE[lineCnt+3] * 3;
         pt4 = shapes_LINE[lineCnt+4] * 3;
         
         afLine(shapes_POINT[pt1], shapes_POINT[pt1+1], shapes_POINT[pt1+2],
                shapes_POINT[pt2], shapes_POINT[pt2+1], shapes_POINT[pt2+2], color, bgColor, rI);
         
         afLine(shapes_POINT[pt2], shapes_POINT[pt2+1], shapes_POINT[pt2+2],
                shapes_POINT[pt3], shapes_POINT[pt3+1], shapes_POINT[pt3+2], color, bgColor, rI);
         
         afLine(shapes_POINT[pt3], shapes_POINT[pt3+1], shapes_POINT[pt3+2],
                shapes_POINT[pt4], shapes_POINT[pt4+1], shapes_POINT[pt4+2], color, bgColor, rI);
         
         afLine(shapes_POINT[pt4], shapes_POINT[pt4+1], shapes_POINT[pt4+2],
                shapes_POINT[pt1], shapes_POINT[pt1+1], shapes_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 5;
         colorCnt++;
      }
   }

   int rowSize = rI.xWin * 3 + rI.xWin % 4;

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
