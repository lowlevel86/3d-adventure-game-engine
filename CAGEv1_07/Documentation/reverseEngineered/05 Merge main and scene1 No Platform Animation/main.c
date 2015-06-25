#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "renderer/renderer.h"
#include "meshes/course1.h"
#include "meshes/character.h"

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
   
   float xCourseRot;
   float yCourseRot;
   float zCourseRot;
   
   static float zCourseRotSave = 0;
   
   float xPlatform;
   float yPlatform;
   float zPlatform;
   float xEdge, yEdge;
   
      
   static int initializeDrawScene1 = 1;

   // GAME LOGIC //

   if (initializeDrawScene1)
   {
      // rotate character forwards
      globalRot(character_GLOBAL, character_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
      
      // copy the mesh data so you will always have the original data
      for (i = 0; i < 9; i++)
      character_STATICGLOBAL[i] = character_GLOBAL[i];
      for (i = 0; i < sizeof(character_TRANS)/sizeof(float); i++)
      character_STATICTRANS[i] = character_TRANS[i];
      for (i = 0; i < sizeof(character_POINT)/sizeof(float); i++)
      character_STATICPOINT[i] = character_POINT[i];
      
      // resize the course
      globalResize(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, 0, 20, 20, 20);
      
      for (i = 0; i < 9; i++)
      course1_STATICGLOBAL[i] = course1_GLOBAL[i];
      for (i = 0; i < sizeof(course1_TRANS)/sizeof(float); i++)
      course1_STATICTRANS[i] = course1_TRANS[i];
      for (i = 0; i < sizeof(course1_POINT)/sizeof(float); i++)
      course1_STATICPOINT[i] = course1_POINT[i];
      
      initializeDrawScene1 = 0;
   }

   // reset
   if (resetDrawScene)
   {
      // starting position
      xCharacterPosSave = 0;
      yCharacterPosSave = 0;
      zCharacterPosSave = 500;
      
      // starting character rotation
      zCharacterRotSave = 0;
      
      // starting course rotation
      zCourseRotSave = 0;
      
      // rotataion of the screen
      xScreenRot = 0;
      
      resetDrawScene = 0;
   }


   // return global and transformation data back to its original state
   // this will keep rounding errors from deforming the meshes
   for (i = 0; i < 9; i++)
   course1_GLOBAL[i] = course1_STATICGLOBAL[i];
   for (i = 0; i < sizeof(course1_TRANS)/sizeof(float); i++)
   course1_TRANS[i] = course1_STATICTRANS[i];

   for (i = 0; i < 9; i++)
   character_GLOBAL[i] = character_STATICGLOBAL[i];
   for (i = 0; i < sizeof(character_TRANS)/sizeof(float); i++)
   character_TRANS[i] = character_STATICTRANS[i];


   // apply the correct orientations

   // After changing the course's orientation its x's and y's 
   // will no longer be aligned horizontally and vertically.
   // So position objects before this point because it might be harder
   // to position them once the course's orientation has changed.

   // rotate the course back to the correct direction
   globalRot(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, 0,
             0, 0, zCourseRotSave);

   // move the course back to the correct position
   globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT,
              -xCharacterPosSave, -yCharacterPosSave, -zCharacterPosSave);


   // move forward and backward by moving the course
   globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, -(forward+backward), 0);

   // turn left and right by turning the course
   globalRot(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, 0, 0, 0, (turnLeft+turnRight));

   // get the character's saved rotation
   xCharacterRot = xCharacterRotSave;
   yCharacterRot = yCharacterRotSave;
   zCharacterRot = zCharacterRotSave;


   // if the character falls past a certain point
   if (course1_TRANS[5] > 50)
   {
      // rotate the character while falling
      xCharacterRot = xCharacterRot + 0.2;
      zCharacterRot = zCharacterRot + 0.1;
      
      forward = 0;
      backward = 0;
      
      // if the character falls past a certain point reset the game
      if (course1_TRANS[5] > 5000)
      {
         xCharacterRot = 0;
         yCharacterRot = 0;
         zCharacterRot = 0;
         
         fallSpeed = 0;
         
         resetDrawScene = 1;
      }
   }


   // Temporarily rotate the course so that its
   // x's and y's are aligned horizontally and vertically.
   // Keep the rotation data to return the course back
   // to its normal rotation.
   xCourseRot = course1_GLOBAL[0];
   yCourseRot = course1_GLOBAL[1];
   zCourseRot = course1_GLOBAL[2];

   globalRot(course1_GLOBAL, course1_TRANS, course1_MESHCNT,
             0, 0, 0, -course1_GLOBAL[0], -course1_GLOBAL[1], -course1_GLOBAL[2]);


   // make the character fall by moving the course up
   if (timeToFall)
   {
      fallSpeed = fallSpeed + 15;
      globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, fallSpeed);
   }

   timeToFall = 1;

   // make the character jump on key down if ready
   if (jumpUp)
   {
      if (allowJump)
      {
         flyUpSpeed = 60;
         flyUpHeight = 140;
         allowFlyUp = 1;
      }
      
      if (allowFlyUp)
      {
         if (flyUpSpeed < flyUpHeight)
         {
            // move the course down making the character look like it is jumping
            globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, -flyUpSpeed);
            
            flyUpSpeed = flyUpSpeed + 15;
            timeToFall = 0;
            alreadyJumped = 1;
         }
         else
         {
            allowFlyUp = 0;
            flyUpSpeed = 0;
            timeToFall = 1;
         }
      }
   }

   // let the character fall on key up
   if (endJumpUp)
   {
      allowFlyUp = 0;
      flyUpSpeed = 0;
      timeToFall = 1;
      alreadyJumped = 0;
      endJumpUp = 0;
   }

   // Get the character's position from the position of the course.
   // Because the course moves and the character does not.
   xCharacterPos = -course1_GLOBAL[3];
   yCharacterPos = -course1_GLOBAL[4];
   zCharacterPos = -course1_GLOBAL[5];

   // keep the character from falling through platforms
   allowJump = 0;
   for (i = 0; i < course1_MESHCNT*9; i=i+9)
   {
      xPlatform = course1_TRANS[3+i] - course1_GLOBAL[3];
      yPlatform = course1_TRANS[4+i] - course1_GLOBAL[4];
      zPlatform = course1_TRANS[5+i] - course1_GLOBAL[5];
      xEdge = course1_TRANS[6+i];
      yEdge = course1_TRANS[7+i];
      
      // if the character is within the platform's boundaries
      if ((xCharacterPos <= xPlatform+xEdge) &&
          (xCharacterPos >= xPlatform-xEdge) &&
          (yCharacterPos <= yPlatform+yEdge) &&
          (yCharacterPos >= yPlatform-yEdge))
      {
         // if the character is close to the floor
         if (zCharacterPos-22.0 <= zPlatform)
         // if the character fell way past the floor because it
         // was falling very fast, then bring it up to the platform
         if (zCharacterPos+2.0 >= zPlatform-fallSpeed)
         {
            floorToChrAmt = zPlatform - (zCharacterPos-20);
            // move the course down so that the character is moved up
            globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, -floorToChrAmt);
            
            timeToFall = 0;
            if (!alreadyJumped)
            allowJump = 1;
            fallSpeed = 0;
         }
         
      }
   }

   // return the course back to its normal rotation
   globalRot(course1_GLOBAL, course1_TRANS, course1_MESHCNT,
             0, 0, 0, xCourseRot, yCourseRot, zCourseRot);


   // save the character's position
   xCharacterPosSave = -course1_GLOBAL[3];
   yCharacterPosSave = -course1_GLOBAL[4];
   zCharacterPosSave = -course1_GLOBAL[5];

   // save the character's rotation
   xCharacterRotSave = xCharacterRot;
   yCharacterRotSave = yCharacterRot;
   zCharacterRotSave = zCharacterRot;

   // save the rotation of the course
   zCourseRotSave = fmod(zCourseRotSave + (turnLeft+turnRight), M_PI * 2);

   // END OF GAME LOGIC //

   // DRAW //

   // fill z-index buffer
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.zIndex[x][y] = rI.camEndZ;

   // color background
   bgColor = 0xFFFFFF;
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.scrnBuff[x][y] = bgColor;

   // draw course
   addRotLocSz(course1_TRANS, course1_POINT, course1_POINTDATSZ,
               course1_STATICPOINT, course1_MESHCNT);
   addScreenRotLocSz(xScreenRot, yScreenRot, zScreenRot,
                     xScreenSize, yScreenSize, zScreenSize,
                     xScreenLoc, yScreenLoc, zScreenLoc,
                     course1_POINT, course1_POINTDATSZ, course1_MESHCNT);
   drawMesh(course1_POINT, course1_LINE, course1_COLOR,
            course1_LINEDATSZ, course1_MESHCNT, bgColor, antialiasing, rI);

   // draw character
   addRotLocSz(character_TRANS, character_POINT, character_POINTDATSZ,
               character_STATICPOINT, character_MESHCNT);
   addScreenRotLocSz(xScreenRot, yScreenRot, zScreenRot,
                     xScreenSize, yScreenSize, zScreenSize,
                     xScreenLoc, yScreenLoc, zScreenLoc,
                     character_POINT, character_POINTDATSZ, character_MESHCNT);
   drawMesh(character_POINT, character_LINE, character_COLOR,
            character_LINEDATSZ, character_MESHCNT, bgColor, antialiasing, rI);

   // END OF DRAW //

   
   
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
