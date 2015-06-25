#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "renderer/renderer.h"
#include "meshes/course1.h"
#include "meshes/character.h"
#include "meshes/run.h"

#define winWidth 400
#define winHeight 300
int xWinPos, yWinPos;

int pBitsAllocated = 0;
int scrnBuffAllocated = 0;
int zIndexAllocated = 0;

BITMAPINFO pbmi[40];
BYTE *pBits;
HDC hdc;
RECT rect;
PAINTSTRUCT ps;
RENDRINFO rI;

int winWidthOld;
int **scrnBuff;
float **zIndex;

#define ID_TIMER 1

void drawScene(HWND);

int sceneNumber = 0;
int resetDrawScene = 0;

int leftMouseDown = 0;
int rightMouseDown = 0;
int middleMouseDown = 0;
int leftMouseUp = 0;
int rightMouseUp = 0;
int middleMouseUp = 0;
float mouseWheelUp = 0;
float mouseWheelDown = 0;
float xMousePos;
float yMousePos;
float xDragLgth;
float yDragLgth;

int keyDown[128];
int keyUp[128];

float turnLeft = 0;
float turnRight = 0;
float forward = 0;
float backward = 0;
int moveCloser = 0;
int moveAway = 0;
int jumpUp = 0;
int endJumpUp = 0;

int changeWindow = 0;

int antialiasing = 0;

int initializeDraw = 1;
int windowResize = 0;
int windowReposition = 0;
int windowFocus = 0;
int exitMsg = 0;

float xScreenRot = 0.0, yScreenRot = 0.0, zScreenRot = 0.0;
float xScreenLoc = 0.0, yScreenLoc = 0.0, zScreenLoc = 0.0;
float xScreenSize = 1.0, yScreenSize = 1.0, zScreenSize = 1.0;

int wndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK mainWndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwndMainWnd;

WNDCLASS wndclass;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
   MSG msg;

   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = mainWndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = 0;
   wndclass.lpszMenuName = 0;
   wndclass.lpszClassName = TEXT("thisApp");

   if (!RegisterClass(&wndclass))
   return 0;

   xWinPos = GetSystemMetrics(SM_CXSCREEN);
   yWinPos = GetSystemMetrics(SM_CYSCREEN);
   xWinPos = xWinPos / 2 - winWidth / 2;
   yWinPos = yWinPos / 2 - winHeight / 2;

   hwndMainWnd = CreateWindowEx(0, TEXT("thisApp"), TEXT("Game Title"),
                                WS_OVERLAPPEDWINDOW,//WS_POPUPWINDOW//WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
                                xWinPos, yWinPos,
                                winWidth+9, winHeight+34, 
                                NULL, NULL, hInstance, NULL);

   ShowWindow(hwndMainWnd, SW_SHOW);
   UpdateWindow(hwndMainWnd);

   while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   int i;
   
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
      
      SetTimer(hwnd, ID_TIMER, 68, NULL);
   }
   
	if (WM_TIMER == message)
	{
      if (changeWindow)
      {
         rI.xCenter = rI.xWin / 2;
         rI.yCenter = rI.yWin / 2;
         
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
         
         rI.zIndex = zIndex;
         rI.scrnBuff = scrnBuff;
         
         rI.xWin = rI.xWin;
         rI.yWin = rI.yWin;
         
         rI.xCenter = rI.xWin / 2;
         rI.yCenter = rI.yWin / 2;
         
         windowResize = 1;
         changeWindow = 0;
      }
      
      drawScene(hwnd);
   }
   
   if (WM_SIZE == message)
   {
      if (LOWORD(lParam))
      rI.xWin = LOWORD(lParam);
      if (HIWORD(lParam))
      rI.yWin = HIWORD(lParam);
      
      changeWindow = 1;
   }
   
   if (WM_SETFOCUS == message)
   windowFocus = 1;
   
   if (WM_MOVE == message)
   windowReposition = 1;
   
   if (WM_KEYUP == message)
   {
      for (i=0; i < 128; i++)
      if (i == wParam)
      keyUp[i] = 1;
      
      for (i=0; i < 128; i++)
      if (keyUp[i])
      keyDown[i] = 0;
   }
   
   if (WM_KEYDOWN == message)
   {
      for (i=0; i < 128; i++)
      if (i == wParam)
      keyDown[i] = 1;
   }
   
   if (WM_LBUTTONDOWN == message)
   leftMouseDown = 1;
   if (WM_RBUTTONDOWN == message)
   rightMouseDown = 1;
   if (WM_MBUTTONDOWN == message)
   middleMouseDown = 1;
   
   if (WM_LBUTTONUP == message)
   {
      leftMouseDown = 0;
      leftMouseUp = 1;
   }
   if (WM_RBUTTONUP == message)
   {
      rightMouseDown = 0;
      rightMouseUp = 1;
   }
   if (WM_MBUTTONUP == message)
   {
      middleMouseDown = 0;
      middleMouseUp = 1;
   }
   
   if (WM_MOUSEMOVE == message)
   {
      if ((leftMouseDown) || (rightMouseDown) || (middleMouseDown))
      {
         xDragLgth = (LOWORD(lParam) - xMousePos);
         yDragLgth = -(HIWORD(lParam) - yMousePos);
      }
      
      xMousePos = LOWORD(lParam);
      yMousePos = HIWORD(lParam);
   }
   else
   {
      xDragLgth = 0;
      yDragLgth = 0;
   }

   if (WM_MOUSEWHEEL == message)
   {
      if (HIWORD(wParam) == 120)
      {
         mouseWheelDown = 0;
         mouseWheelUp = mouseWheelUp + 10;
      }
      
      if (HIWORD(wParam) == 65416)
      {
         mouseWheelUp = 0;
         mouseWheelDown = mouseWheelDown - 10;
      }
   }
   else
   {
      if (mouseWheelUp)
      mouseWheelUp--;
      if (mouseWheelDown)
      mouseWheelDown++;
   }
   
   if ((WM_DESTROY == message) || (exitMsg))
   {
      exitMsg = 0;
      
      KillTimer(hwnd, ID_TIMER);
      
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
   }
   
   return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawScene(HWND hwnd)
{
   int i, x, y;
   int bgColor;
   float windowRelativeSize;
   float windowWidthSize = 1;
   float windowHeightSize = 1;
   static float oldWindowWidth;
   static float oldWindowHeight;

   if (initializeDraw)
   {
      rI.camLenZ = 250;
      rI.camEndZ = -1200;
      rI.fogBgnZ = 50;
      rI.fogEndZ = -1200;
      rI.perspctv = 260;
      rI.ortho = 0;
      
      oldWindowWidth = winWidth;
      oldWindowHeight = winHeight;
   }
   
   if (windowResize)
   {
      if (oldWindowWidth < oldWindowHeight)
      {
         if (rI.xWin < rI.yWin)
         {
            windowWidthSize = windowWidthSize * ((float)rI.xWin / oldWindowWidth);
            windowRelativeSize = windowWidthSize;
         }
         else
         {
            windowHeightSize = windowHeightSize * ((float)rI.yWin / oldWindowWidth);
            windowRelativeSize = windowHeightSize;
         }
      }
      else
      {
         if (rI.xWin < rI.yWin)
         {
            windowWidthSize = windowWidthSize * ((float)rI.xWin / oldWindowHeight);
            windowRelativeSize = windowWidthSize;
         }
         else
         {
            windowHeightSize = windowHeightSize * ((float)rI.yWin / oldWindowHeight);
            windowRelativeSize = windowHeightSize;
         }
      }
      
      rI.camLenZ = rI.camLenZ * windowRelativeSize;
      rI.camEndZ = rI.camEndZ * windowRelativeSize;
      rI.fogBgnZ = rI.fogBgnZ * windowRelativeSize;
      rI.fogEndZ = rI.fogEndZ * windowRelativeSize;
      rI.perspctv = rI.perspctv * windowRelativeSize;
      
      xScreenSize = xScreenSize * windowRelativeSize;
      yScreenSize = yScreenSize * windowRelativeSize;
      zScreenSize = zScreenSize * windowRelativeSize;
      
      oldWindowWidth = rI.xWin;
      oldWindowHeight = rI.yWin;
   }
   
   // Input Events //
   if (keyDown[38])// up arrow key
   forward = 20;
   if (keyUp[38])
   forward = 0;
   if (keyDown[40])// down arrow key
   backward = -20;
   if (keyUp[40])
   backward = 0;
   
   // Fancy way of turning left or right.
   // When moving backwards left and right switch.
   if (backward)
   {
      if (keyDown[39] || keyDown[37] || keyUp[39] || keyUp[37])
      {
         turnLeft = 0;
         turnRight = 0;
      }
      if (keyDown[39])// right arrow key
      turnLeft = -0.15;
      if (keyDown[37])// left arrow key
      turnRight = 0.15;
   }
   else
   {
      if (keyDown[39] || keyDown[37] || keyUp[39] || keyUp[37])
      {
         turnLeft = 0;
         turnRight = 0;
      }
      if (keyDown[37])// left arrow key
      turnLeft = -0.15;
      if (keyDown[39])// right arrow key
      turnRight = 0.15;
   }
   
   if (keyUp[33])// page up key
   moveCloser = 1;
   if (keyUp[34])// page down key
   moveAway = 1;
   
   if (keyDown[32])// space key
   jumpUp = 1;
   if (keyUp[32])// space key
   {
      endJumpUp = 1;
      jumpUp = 0;
   }
   
   if (keyUp[27])//Esc
   {
      exitMsg = 1;
      return;
   }
   // End of Input Events //
   
   
   int runCharacter = 0;
   static float runCycle = 0;
   float btwnFrames;

   static int timeToFall = 1;

   static float floorToChrAmt;
   static float flyUpSpeed;
   static float flyUpHeight;
   static float fallSpeed;

   static int allowJump = 0;
   static int allowFlyUp = 0;
   static int alreadyJumped = 0;

   float xCharacterPos = 0;
   float yCharacterPos = 0;
   float zCharacterPos = 0;
   
   static float xCharacterPosSave = 0;
   static float yCharacterPosSave = 0;
   static float zCharacterPosSave = 0;
   
   float xCharacterRot = 0;
   float yCharacterRot = 0;
   float zCharacterRot = 0;
   
   static float xCharacterRotSave = 0;
   static float yCharacterRotSave = 0;
   static float zCharacterRotSave = 0;
   
   static float cameraDistance = 80;

   float xCourseRot;
   float yCourseRot;
   float zCourseRot;
   
   static float zCourseRotSave = 0;
   
   float xPlatform;
   float yPlatform;
   float zPlatform;
   float xEdge, yEdge;
   
   static float movingPlatform;
   static float characterOnMovingPlatform;
   static float platformDirection = -1;
   
   
   if (sceneNumber == 0)
   {
      sceneNumber = 1;
   }
   if (sceneNumber == 1)
   {
      #include "scene1.c"
   }
   
   
   //draw to screen
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
   
   for (i=0; i < 128; i++)
   keyUp[i] = 0;
   
   leftMouseUp = 0;
   rightMouseUp = 0;
   middleMouseUp = 0;
   
   windowResize = 0;
   windowReposition = 0;
   windowFocus = 0;
   initializeDraw = 0;
}
