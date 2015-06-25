//Written by Crailin Mogged, 2013

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include "renderer/renderer.h"
#include "rmCComments.h"
#include "readModelData.h"
#include "findStrInStr.h"
#include "polyToLine.h"
#include "prtCCode.h"
#include "nameWnd/nameWnd.h"
#include "helpWnd/helpWnd.h"
#include "dataView/dataView.h"
#include "editMode/editMode.h"
#include "resource.h"

#define winWidth 440
#define winHeight 320
int xWinPos, yWinPos;

float lrMove;
float tbMove;
float ccMove;

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

int pBitsAllocated = 0;
int scrnBuffAllocated = 0;
int zIndexAllocated = 0;

int meshOld_LINEDATSZAllocated = 0;
int meshOld_COLORDATSZAllocated = 0;
int meshOld_LINEAllocated = 0;
int meshOld_COLORAllocated = 0;

int mesh_TRANSAllocated = 0;
int mesh_POINTAllocated = 0;

int mesh_STATICPOINTAllocated = 0;

int mesh_STATICTRANSAllocated = 0;

int mesh_LINEAllocated = 0;
int mesh_COLORAllocated = 0;
int mesh_POINTDATSZAllocated = 0;
int mesh_LINEDATSZAllocated = 0;
int mesh_COLORDATSZAllocated = 0;
      
BITMAPINFO pbmi[40];
BYTE *pBits;
HDC hdc;
PAINTSTRUCT ps;
RENDRINFO rI;

float xMainCent = 0, yMainCent = 0, zMainCent = 0;
float xMainHori = 1.0, yMainHori = 0, zMainHori = 0;
float xMainVert = 0, yMainVert = 1.0, zMainVert = 0;
float xMainFtBk = 0, yMainFtBk = 0, zMainFtBk = 1.0;

float *mesh_TRANS;
float *mesh_POINT;
int *mesh_LINE;
int *mesh_COLOR;
int *mesh_POINTDATSZ;
int *mesh_LINEDATSZ;
int *mesh_COLORDATSZ;
int mesh_MESHCNT = 0;
//nonmoving point data
float *mesh_STATICPOINT;

float *mesh_STATICTRANS;

int *meshOld_LINE;
int *meshOld_COLOR;
int *meshOld_LINEDATSZ;
int *meshOld_COLORDATSZ;

int editMode = 0;

int offset = 0;
struct stat stat_p;
FILE *fileHandle;
char *fileData;
int fileDataSize;
int transSz = -1, pointSz = -1, lineSz = -1, colorSz = -1;

void drawScene(HWND);

void rotatePt(float *, float *, float *,
              float, float, float, float, float, float);

HWND hwndNameWnd;
HWND hwndHelpWnd;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szAppName[] = TEXT("thisApp");

char cmdLine[8][4096];
char editArray[4096];
int editArrayNums[4096];
char outFile[4096];
char fileName[4096];
char meshName[4096];
int showHelpWindow = 0;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
   HWND hwnd;
   MSG msg;
   WNDCLASS wndclass;
   
   LPWSTR *szArgList;
   int h, i, j, k;
   int argCount;
   int charNum = 0;
   char numberInTxt[32];
   int noOutFile = 0;
   int noGUI = 0;
   int useVertexEditing = 0;
   int meshObjNum;

   szArgList = CommandLineToArgvW((void *)GetCommandLineW(), &argCount);
   
   if (szArgList == NULL)
   {
      MessageBox(NULL, TEXT("Unable to parse command line"), TEXT("Error"), MB_OK);
      return 10;
   }
   
   for (i=0; i < argCount; i++)
   sprintf(cmdLine[i], "%ls", szArgList[i]);

   // find if the command asks for help Info
   if (argCount > 1)
   {
      for (i=0; i < argCount; i++)
      if (cmdLine[i][0] == 45)// -
      if (cmdLine[i][1] == 104)// h
      showHelpWindow = 1;
   }
   
   if (!showHelpWindow)
   if (argCount > 1)
   {
      // find if the command has an output file
      noOutFile = 1;
      for (i=0; i < argCount; i++)
      if (cmdLine[i][0] == 45)// -
      if (cmdLine[i][1] == 111)// o
      if (i+1 < argCount)
      {
         sprintf(outFile, "%s", cmdLine[i+1]);
         
         //erase the output file so it isn't confused with the input file
         cmdLine[i+1][0] = 0;
         
         noGUI = 1;
         noOutFile = 0;
         i = argCount;
      }
      
      // find if the command has a "name the contents" option
      for (i=0; i < argCount; i++)
      if (cmdLine[i][0] == 45)// -
      if (cmdLine[i][1] == 110)// n
      if (i+1 < argCount)
      {
         sprintf(meshName, "%s", cmdLine[i+1]);
         
         //erase the output file so it isn't confused with the input file
         cmdLine[i+1][0] = 0;
         
         noGUI = 1;
         i = argCount;
      }
      
      // find if the command has an edit mode option
      for (h=0; h < argCount; h++)
      if (cmdLine[h][0] == 45)// -
      if (cmdLine[h][1] == 101)// e
      if (h+1 < argCount)
      {
         sprintf(editArray, "%s", cmdLine[h+1]);
         
         // get the numbers from "editArray"
         i = 0;
         j = 0;
         while (editArray[i])
         {
            if (i)
            i++;
            k = 0;
            while ((editArray[i] != 44) && //44 = ,
                   (editArray[i] != 45) && //45 = -
                   (editArray[i] != 0))
            {
               numberInTxt[k] = editArray[i];
               k++;
               i++;
            }
            numberInTxt[k] = 0;
            
            editArrayNums[j] = atoi(numberInTxt);
            j++;
            
            if (editArray[i] == 45)
            {
               editArrayNums[j] = -1;
               j++;
            }
            
            if (editArray[i] == 0)
            {
               editArrayNums[j] = -2;
               j++;
            }
         }
         
         useVertexEditing = 1;
         noGUI = 1;
      }
      
      // get the file name from the command terminal
      for (i=0; i < argCount; i++)
      {
         charNum = 0;
         findStrInStr(cmdLine[i], ".c", &charNum);
         if (charNum != -1)
         {
            sprintf(fileName, "%s", cmdLine[i]);
            
            if (noOutFile)
            sprintf(outFile, "%s", cmdLine[i]);
            
            i = argCount;
         }
      }
      
      
      // read the input file
      if (-1 == stat(fileName, &stat_p))
      {
         printf("File does not exist.\n");
         return(0);
      }
      
      fileData = (char *)malloc(stat_p.st_size);
      
      fileHandle = fopen(fileName,"rb");
      if (!fileHandle)
      return(0);
      
      fread(fileData, sizeof(char), stat_p.st_size, fileHandle);
      fclose(fileHandle);
      
      // remove comments
      rmCComments(fileData, stat_p.st_size);
      
      // remove white space
      fileDataSize = stat_p.st_size;
      for (i=0; i+offset < fileDataSize; i++)
      {
         while ((fileData[i+offset] == 0x20) || //spaces
                (fileData[i+offset] == 0x09) || //tabs
                (fileData[i+offset] == 0x0A) || //endline
                (fileData[i+offset] == 0x0D))   //endline (windows)
         offset++;
         
         fileData[i] = fileData[i+offset];
      }
      
      // read 3d model data
      getModelDataSzs(fileData, &transSz, &pointSz, &lineSz, &colorSz);
      
      if (transSz == -1)
      return(0);
      if (pointSz == -1)
      return(0);
      if (lineSz == -1)
      return(0);
      if (colorSz == -1)
      return(0);
      
      mesh_TRANS = (float *)malloc(transSz * sizeof(float));
      mesh_TRANSAllocated = 1;
      mesh_POINT = (float *)malloc(pointSz * sizeof(float));
      mesh_POINTAllocated = 1;
      
      mesh_STATICPOINT = (float *)malloc(pointSz * sizeof(float));
      mesh_STATICPOINTAllocated = 1;
      
      mesh_STATICTRANS = (float *)malloc(transSz * sizeof(float));
      mesh_STATICTRANSAllocated = 1;
      
      mesh_LINE = (int *)malloc(lineSz * sizeof(int));
      mesh_LINEAllocated = 1;
      mesh_COLOR = (int *)malloc(colorSz * sizeof(int));
      mesh_COLORAllocated = 1;
      
      mesh_MESHCNT = transSz / 9;

      mesh_POINTDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_POINTDATSZAllocated = 1;
      mesh_LINEDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_LINEDATSZAllocated = 1;
      mesh_COLORDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_COLORDATSZAllocated = 1;
      
      readModelData(fileData, transSz, pointSz, lineSz, colorSz,
                    mesh_TRANS, mesh_POINT, mesh_LINE, mesh_COLOR,
                    mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ);
      
      for (i = 0; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i++)
      mesh_STATICPOINT[i] = mesh_POINT[i];
      
      for (i = 0; i < mesh_MESHCNT*9; i++)
      mesh_STATICTRANS[i] = mesh_TRANS[i];
      
      if (fileData)
      free(fileData);
      
      
      if (noGUI)
      {
         // convert to lines
         if (mesh_LINE[0] != 2)
         {
            if (meshOld_LINEDATSZAllocated)
            free(meshOld_LINEDATSZ);
            meshOld_LINEDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
            if (meshOld_COLORDATSZAllocated)
            free(meshOld_COLORDATSZ);
            meshOld_COLORDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
            
            for (i=0; i < mesh_MESHCNT; i++)
            meshOld_LINEDATSZ[i] = mesh_LINEDATSZ[i];
            
            //Guess what the new sizes of the line data will be
            polyToLineNewSzs(meshOld_LINEDATSZ, mesh_LINE, mesh_LINEDATSZ, mesh_MESHCNT);
            
            for (i=0; i < mesh_MESHCNT; i++)
            meshOld_COLORDATSZ[i] = mesh_COLORDATSZ[i];
            
            //Guess what the new sizes of the color data will be
            for (i=0; i < mesh_MESHCNT; i++)
            mesh_COLORDATSZ[i] = mesh_LINEDATSZ[i] / 3;
            
            if (meshOld_LINEAllocated)
            free(meshOld_LINE);
            meshOld_LINE = (int *)malloc(meshOld_LINEDATSZ[mesh_MESHCNT-1] * sizeof(int));
            if (meshOld_COLORAllocated)
            free(meshOld_COLOR);
            meshOld_COLOR = (int *)malloc(meshOld_COLORDATSZ[mesh_MESHCNT-1] * sizeof(int));
            
            for (i=0; i < meshOld_LINEDATSZ[mesh_MESHCNT-1]; i++)
            meshOld_LINE[i] = mesh_LINE[i];
            for (i=0; i < meshOld_COLORDATSZ[mesh_MESHCNT-1]; i++)
            meshOld_COLOR[i] = mesh_COLOR[i];
            
            if (mesh_LINEAllocated)
            free(mesh_LINE);
            mesh_LINE = (int *)malloc(mesh_LINEDATSZ[mesh_MESHCNT-1] * sizeof(int));
            if (mesh_COLORAllocated)
            free(mesh_COLOR);
            mesh_COLOR = (int *)malloc(mesh_COLORDATSZ[mesh_MESHCNT-1] * sizeof(int));
            
            polyToLine(meshOld_LINE, meshOld_COLOR,
                       meshOld_LINEDATSZ, meshOld_COLORDATSZ,
                       mesh_LINE, mesh_COLOR,
                       mesh_LINEDATSZ, mesh_COLORDATSZ, mesh_MESHCNT);
         }
         
         if (useVertexEditing)
         {
            // use "editArrayNums" to find the mesh objects that need editing
            h = 0;
            while (editArrayNums[h] != -2)
            {
               if (editArrayNums[h] != -1)
               {
                  meshObjNum = editArrayNums[h];
                  h++;
               }
               else
               {
                  meshObjNum++;
                  if (meshObjNum == editArrayNums[h+1])
                  h = h + 2;
               }
               
               // find the vertex that is furthest away from the local x, y, or z plane
               j = 0;
               if (meshObjNum)
               j = mesh_POINTDATSZ[meshObjNum-1];
               
               float mesh_POINTmem;
               mesh_POINTmem = 0;
               
               for (i=j; i < mesh_POINTDATSZ[meshObjNum]; i++)
               if (fabs(mesh_STATICPOINT[i]) > fabs(mesh_POINTmem))
               {
                  mesh_POINTmem = mesh_STATICPOINT[i];
                  nodeHighlightNum = i - i % 3;
               }
               
               // remove node
               removeNode(mesh_STATICPOINT, mesh_LINE, mesh_COLOR,
                          mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ, mesh_MESHCNT);
            }
         }
         
         prtCCode(mesh_TRANS, mesh_POINT, mesh_LINE, mesh_COLOR,
                  mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ,
                  mesh_STATICPOINT, mesh_MESHCNT, meshName, outFile);
         
         if (mesh_COLORDATSZAllocated)
         free(mesh_COLORDATSZ);
         if (mesh_LINEDATSZAllocated)
         free(mesh_LINEDATSZ);
         if (mesh_POINTDATSZAllocated)
         free(mesh_POINTDATSZ);
         
         if (mesh_COLORAllocated)
         free(mesh_COLOR);
         if (mesh_LINEAllocated)
         free(mesh_LINE);
         if (mesh_POINTAllocated)
         free(mesh_POINT);
         if (mesh_TRANSAllocated)
         free(mesh_TRANS);
         
         if (mesh_STATICPOINTAllocated)
         free(mesh_STATICPOINT);
         
         if (mesh_STATICTRANSAllocated)
         free(mesh_STATICTRANS);
         
         if (meshOld_LINEDATSZAllocated)
         free(meshOld_LINEDATSZ);
         if (meshOld_COLORDATSZAllocated)
         free(meshOld_COLORDATSZ);
         if (meshOld_LINEAllocated)
         free(meshOld_LINE);
         if (meshOld_COLORAllocated)
         free(meshOld_COLOR);
         
         return(0);
      }
   }
   
   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInstance;
   wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
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
                         szAppName, TEXT("Poly To Line (Press H For Help)"),
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
   static HINSTANCE hInstance;
   WNDCLASS wndclass;
   int i;
   static int exitMsg;
   static int drag;
   static float xMmem;
   static float yMmem;

   static int winWidthOld;
   static int **scrnBuff;
   static float **zIndex;

   float perspctv;
   float xMousePos, yMousePos;
   
   HDROP hDrop = (HDROP)wParam;
   
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
      
      //create nameWnd
      hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
      
      wndclass.style = CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = nameWnd;
      wndclass.cbClsExtra = 0;
      wndclass.cbWndExtra = 0;
      wndclass.hInstance = hInstance;
      wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
      wndclass.hbrBackground = CreateSolidBrush(RGB(0xAA,0xAA,0xAA));
      wndclass.lpszMenuName = 0;
      wndclass.lpszClassName = TEXT("nameWnd");

      RegisterClass(&wndclass);
         
      hwndNameWnd = CreateWindow(TEXT("nameWnd"), TEXT(""),
                                 WS_POPUPWINDOW,
                                 xWinPos + (winWidth / 2) - 200 / 2,
                                 yWinPos + (winHeight / 2) - 80 / 2,
                                 200+6, 80+25,
                                 hwnd, NULL, hInstance, NULL);
      
      ShowWindow(hwndNameWnd, SW_HIDE);
      //end of create nameWnd
      
      //create helpWnd
      hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
      
      wndclass.style = CS_HREDRAW | CS_VREDRAW;
      wndclass.lpfnWndProc = helpWnd;
      wndclass.cbClsExtra = 0;
      wndclass.cbWndExtra = 0;
      wndclass.hInstance = hInstance;
      wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
      wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
      wndclass.hbrBackground = CreateSolidBrush(RGB(0xAA,0xAA,0xAA));
      wndclass.lpszMenuName = 0;
      wndclass.lpszClassName = TEXT("helpWnd");

      RegisterClass(&wndclass);
         
      hwndHelpWnd = CreateWindow(TEXT("helpWnd"), TEXT(""),
                                 WS_POPUPWINDOW,
                                 xWinPos + (winWidth / 2) - 400 / 2,
                                 yWinPos + (winHeight / 2) - 300 / 2,
                                 400+6, 300+25,
                                 hwnd, NULL, hInstance, NULL);
      
      ShowWindow(hwndHelpWnd, SW_HIDE);
      //end of create helpWnd
      
      SetTimer(hwnd, ID_TIMER, 50, NULL);
      now = time((time_t *)NULL);
      
      return 0;
   }
   
   if (WM_MOVE == message)
   {
      xWinPos = LOWORD(lParam);
      yWinPos = HIWORD(lParam);
      
      return 0;
   }
   
   if (WM_DROPFILES == message)
   {
      xMainCent = 0;
      yMainCent = 0;
      zMainCent = 0;
      editMode = 0;
      
      SetWindowText(hwnd, "Loading...");
      drawScene(hwnd);
      
      DragQueryFile(hDrop, 0, fileName, 4096);
      
      if (-1 == stat(fileName, &stat_p))
      return(0);
      
      fileData = (char *)malloc(stat_p.st_size);
      
      fileHandle = fopen(fileName,"rb");
      if (!fileHandle)
      return(0);
      
      fread(fileData, sizeof(char), stat_p.st_size, fileHandle);
      fclose(fileHandle);
      
      // remove comments
      rmCComments(fileData, stat_p.st_size);
      
      // remove white space
      fileDataSize = stat_p.st_size;
      offset = 0;
      for (i=0; i+offset < fileDataSize; i++)
      {
         while ((fileData[i+offset] == 0x20) || //spaces
                (fileData[i+offset] == 0x09) || //tabs
                (fileData[i+offset] == 0x0A) || //endline
                (fileData[i+offset] == 0x0D))   //endline (windows)
         offset++;
         
         fileData[i] = fileData[i+offset];
      }
      
      // read 3d model data
      transSz = -1;
      pointSz = -1;
      lineSz = -1;
      colorSz = -1;
      getModelDataSzs(fileData, &transSz, &pointSz, &lineSz, &colorSz);
      
      if (transSz == -1)
      return(0);
      if (pointSz == -1)
      return(0);
      if (lineSz == -1)
      return(0);
      if (colorSz == -1)
      return(0);
      
      if (mesh_TRANSAllocated)
      free(mesh_TRANS);
      mesh_TRANS = (float *)malloc(transSz * sizeof(float));
      mesh_TRANSAllocated = 1;
      if (mesh_POINTAllocated)
      free(mesh_POINT);
      mesh_POINT = (float *)malloc(pointSz * sizeof(float));
      mesh_POINTAllocated = 1;
      
      if (mesh_STATICPOINTAllocated)
      free(mesh_STATICPOINT);
      mesh_STATICPOINT = (float *)malloc(pointSz * sizeof(float));
      mesh_STATICPOINTAllocated = 1;
      
      if (mesh_STATICTRANSAllocated)
      free(mesh_STATICTRANS);
      mesh_STATICTRANS = (float *)malloc(transSz * sizeof(float));
      mesh_STATICTRANSAllocated = 1;
      
      if (mesh_LINEAllocated)
      free(mesh_LINE);
      mesh_LINE = (int *)malloc(lineSz * sizeof(int));
      mesh_LINEAllocated = 1;
      if (mesh_COLORAllocated)
      free(mesh_COLOR);
      mesh_COLOR = (int *)malloc(colorSz * sizeof(int));
      mesh_COLORAllocated = 1;
      
      mesh_MESHCNT = transSz / 9;

      if (mesh_POINTDATSZAllocated)
      free(mesh_POINTDATSZ);
      mesh_POINTDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_POINTDATSZAllocated = 1;
      if (mesh_LINEDATSZAllocated)
      free(mesh_LINEDATSZ);
      mesh_LINEDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_LINEDATSZAllocated = 1;
      if (mesh_COLORDATSZAllocated)
      free(mesh_COLORDATSZ);
      mesh_COLORDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
      mesh_COLORDATSZAllocated = 1;
      
      readModelData(fileData, transSz, pointSz, lineSz, colorSz,
                    mesh_TRANS, mesh_POINT, mesh_LINE, mesh_COLOR,
                    mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ);
      
      for (i = 0; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i++)
      mesh_STATICPOINT[i] = mesh_POINT[i];
      
      for (i = 0; i < mesh_MESHCNT*9; i++)
      mesh_STATICTRANS[i] = mesh_TRANS[i];
      
      if (fileData)
      free(fileData);
      
      SetWindowText(hwnd, "Poly To Line (Press H For Help)");
      drawScene(hwnd);
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
      rI.camEndZ = -120000;
      rI.fogBgnZ = 100;
      rI.fogEndZ = -120000;
      rI.perspctv = 400;
      rI.ortho = 0;
      rI.zIndex = zIndex;
      rI.scrnBuff = scrnBuff;
      
      drawScene(hwnd);
      return 0;
   }

   if (mesh_MESHCNT)
	if (WM_TIMER == message)
	{
      lrMove = (lftk + rghtk) * fpsNum;
      tbMove = (upk + downk) * fpsNum;
      ccMove = (clckwzk + cntrClckwzk) * fpsNum;
      
      //rotate
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      rotateRelToScrn(lrMove, tbMove, ccMove,
                      &mesh_TRANS[0+i], &mesh_TRANS[1+i], &mesh_TRANS[2+i]);
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      rotatePt(&mesh_TRANS[3+i], &mesh_TRANS[4+i], &mesh_TRANS[5+i],
               xMainCent, yMainCent, zMainCent, lrMove, tbMove, ccMove);
      /*
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[0+i] = mesh_TRANS[0+i] + lrMove;
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[1+i] = mesh_TRANS[1+i] + tbMove;
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[2+i] = mesh_TRANS[2+i] + ccMove;
      */
      
      //Move
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[3+i] = mesh_TRANS[3+i] + mvRghtk - mvLftk;//x
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[4+i] = mesh_TRANS[4+i] + mvUpk - mvDownk;//y
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      mesh_TRANS[5+i] = mesh_TRANS[5+i] + forwdk - bckwdk;//z
      
      //Move main center point
      xMainCent = xMainCent + mvRghtk - mvLftk;//x
      yMainCent = yMainCent + mvUpk - mvDownk;//y
      zMainCent = zMainCent + forwdk - bckwdk;//z
      
      //resize
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      {
         mesh_TRANS[6+i] = mesh_TRANS[6+i] * szInck;//x
         mesh_TRANS[7+i] = mesh_TRANS[7+i] * szInck;//y
         mesh_TRANS[8+i] = mesh_TRANS[8+i] * szInck;//z
      }
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      {
         mesh_TRANS[3+i] = (mesh_TRANS[3+i] - xMainCent) * szInck + xMainCent;//x
         mesh_TRANS[4+i] = (mesh_TRANS[4+i] - yMainCent) * szInck + yMainCent;//y
         mesh_TRANS[5+i] = (mesh_TRANS[5+i] - zMainCent) * szInck + zMainCent;//z
      }
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      {
         mesh_TRANS[6+i] = mesh_TRANS[6+i] * szDeck;//x
         mesh_TRANS[7+i] = mesh_TRANS[7+i] * szDeck;//y
         mesh_TRANS[8+i] = mesh_TRANS[8+i] * szDeck;//z
      }
      for (i = 0; i < mesh_MESHCNT*9; i=i+9)
      {
         mesh_TRANS[3+i] = (mesh_TRANS[3+i] - xMainCent) * szDeck + xMainCent;//x
         mesh_TRANS[4+i] = (mesh_TRANS[4+i] - yMainCent) * szDeck + yMainCent;//y
         mesh_TRANS[5+i] = (mesh_TRANS[5+i] - zMainCent) * szDeck + zMainCent;//z
      }
   }
   
   if (WM_TIMER == message)
	{
      nowHold = (int)now;
      now = time((time_t *)NULL);
      fps++;
      if ((int)now != nowHold)
      {
         fpsNum = 30.0 / fps;
         fps = 0;
      }
      
      if (showHelpWindow)// H help window
      {
         SetWindowPos(hwndHelpWnd, 0, xWinPos + rI.xCenter - 400 / 2,
                                      yWinPos + rI.yCenter - 300 / 2,
                                      400+6, 300+25, SWP_NOACTIVATE);
         
         ShowWindow(hwndHelpWnd, SW_SHOW);
         showHelpWindow = 0;
      }
      
      drawScene(hwnd);
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
      
      return 0;
   }
   
   if (WM_KEYDOWN == message)
   {
      if (70 == wParam)//Move forward f
      {
         bckwdk = 0;
         forwdk = 20;
      }
      if (66 == wParam)//Move backward b
      {
         forwdk = 0;
         bckwdk = 20;
      }
      if (100 == wParam)//Move left keypad 4
      {
         mvRghtk = 0;
         mvLftk = 20;
      }
      if (102 == wParam)//Move right keypad 6
      {
         mvLftk = 0;
         mvRghtk = 20;
      }
      if (104 == wParam)//Move up keypad 8
      {
         mvDownk = 0;
         mvUpk = 20;
      }
      if (98 == wParam)//Move down keypad 2
      {
         mvUpk = 0;
         mvDownk = 20;
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
      if ((189 == wParam) || (109 == wParam))//Decrease Size -
      {
         szInck = 1.0;
         szDeck = 0.9;
      }
      
      if (27 == wParam)
      exitMsg = 1;
      
      //view data
      if (79 == wParam)// O
      if (mesh_MESHCNT)
      if (mesh_LINE[0] == 2)
      viewObjCenterLocsOnOff = !viewObjCenterLocsOnOff;
      
      if (80 == wParam)// P
      if (mesh_MESHCNT)
      if (mesh_LINE[0] == 2)
      viewPtLocsOnOff = !viewPtLocsOnOff;
      
      if (76 == wParam)// L
      if (mesh_MESHCNT)
      if (mesh_LINE[0] == 2)
      viewLineLocsOnOff = !viewLineLocsOnOff;
      
      if (69 == wParam)// E edit mode
      if (mesh_MESHCNT)
      if (mesh_LINE[0] == 2)
      {
         editMode = !editMode;
         nodeHighlightNum = -1;
      }
      
      if (82 == wParam)// R remove a node
      if (mesh_MESHCNT)
      if (editMode)
      if (nodeHighlightNum != -1)
      {
         removeNode(mesh_STATICPOINT, mesh_LINE, mesh_COLOR,
                    mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ, mesh_MESHCNT);
         drawScene(hwnd);
      }
      
      if (78 == wParam)// N name content
      {
         SetWindowPos(hwndNameWnd, 0, xWinPos + rI.xCenter - 200 / 2,
                                      yWinPos + rI.yCenter - 80 / 2,
                                      200+6, 80+25, SWP_NOACTIVATE);
         
         ShowWindow(hwndNameWnd, SW_SHOW);
      }
      
      if (72 == wParam)// H help window
      {
         SetWindowPos(hwndHelpWnd, 0, xWinPos + rI.xCenter - 400 / 2,
                                      yWinPos + rI.yCenter - 300 / 2,
                                      400+6, 300+25, SWP_NOACTIVATE);
         
         ShowWindow(hwndHelpWnd, SW_SHOW);
         showHelpWindow = 0;
      }
      
      if (87 == wParam)// W print code
      if (mesh_MESHCNT)
      {
         SetWindowText(hwnd, "Writing Code to Files...");
         
         for (i = 0; i < mesh_MESHCNT*9; i++)
         mesh_TRANS[i] = mesh_STATICTRANS[i];
         xMainCent = 0;
         yMainCent = 0;
         zMainCent = 0;
         drawScene(hwnd);
         
         prtCCode(mesh_TRANS, mesh_POINT, mesh_LINE, mesh_COLOR,
                  mesh_POINTDATSZ, mesh_LINEDATSZ, mesh_COLORDATSZ,
                  mesh_STATICPOINT, mesh_MESHCNT, meshName, fileName);
         
         SetWindowText(hwnd, "Poly To Line (Press H For Help)");
      }
      
      if (67 == wParam)// C convert to lines
      if (mesh_MESHCNT)
      {
         if (mesh_LINE[0] == 2)
         return 0;
         
         SetWindowText(hwnd, "Converting polygons to lines...");
         drawScene(hwnd);
         
         if (meshOld_LINEDATSZAllocated)
         free(meshOld_LINEDATSZ);
         meshOld_LINEDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
         if (meshOld_COLORDATSZAllocated)
         free(meshOld_COLORDATSZ);
         meshOld_COLORDATSZ = (int *)malloc(mesh_MESHCNT * sizeof(int));
         
         for (i=0; i < mesh_MESHCNT; i++)
         meshOld_LINEDATSZ[i] = mesh_LINEDATSZ[i];
         
         //Guess what the new sizes of the line data will be
         polyToLineNewSzs(meshOld_LINEDATSZ, mesh_LINE, mesh_LINEDATSZ, mesh_MESHCNT);
         
         for (i=0; i < mesh_MESHCNT; i++)
         meshOld_COLORDATSZ[i] = mesh_COLORDATSZ[i];
         
         //Guess what the new sizes of the color data will be
         for (i=0; i < mesh_MESHCNT; i++)
         mesh_COLORDATSZ[i] = mesh_LINEDATSZ[i] / 3;
         
         if (meshOld_LINEAllocated)
         free(meshOld_LINE);
         meshOld_LINE = (int *)malloc(meshOld_LINEDATSZ[mesh_MESHCNT-1] * sizeof(int));
         if (meshOld_COLORAllocated)
         free(meshOld_COLOR);
         meshOld_COLOR = (int *)malloc(meshOld_COLORDATSZ[mesh_MESHCNT-1] * sizeof(int));
         
         for (i=0; i < meshOld_LINEDATSZ[mesh_MESHCNT-1]; i++)
         meshOld_LINE[i] = mesh_LINE[i];
         for (i=0; i < meshOld_COLORDATSZ[mesh_MESHCNT-1]; i++)
         meshOld_COLOR[i] = mesh_COLOR[i];
         
         if (mesh_LINEAllocated)
         free(mesh_LINE);
         mesh_LINE = (int *)malloc(mesh_LINEDATSZ[mesh_MESHCNT-1] * sizeof(int));
         if (mesh_COLORAllocated)
         free(mesh_COLOR);
         mesh_COLOR = (int *)malloc(mesh_COLORDATSZ[mesh_MESHCNT-1] * sizeof(int));
         
         polyToLine(meshOld_LINE, meshOld_COLOR,
                    meshOld_LINEDATSZ, meshOld_COLORDATSZ,
                    mesh_LINE, mesh_COLOR,
                    mesh_LINEDATSZ, mesh_COLORDATSZ, mesh_MESHCNT);
         
         SetWindowText(hwnd, "Poly To Line (Press H For Help)");
         drawScene(hwnd);
      }
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
   
   // highlight a node
   if (WM_RBUTTONDOWN == message)
   if (mesh_MESHCNT)
   if (editMode)
   if (mesh_LINE[0] == 2)
   {
      xMousePos = LOWORD(lParam);
      yMousePos = rI.yWin - HIWORD(lParam);
      
      nodeHighlightNum = -1;
      for (i = 0; i < mesh_POINTDATSZ[mesh_MESHCNT-1]; i=i+3)
      {
         perspctv = 1;
         if (!rI.ortho)
         perspctv = rI.perspctv / (rI.perspctv - mesh_POINT[i+2]);
         
         if (xMousePos >= round(mesh_POINT[i]*perspctv+rI.xCenter)-4)
         if (xMousePos <= round(mesh_POINT[i]*perspctv+rI.xCenter)+4)
         if (yMousePos >= round(mesh_POINT[i+1]*perspctv+rI.yCenter)-4)
         if (yMousePos <= round(mesh_POINT[i+1]*perspctv+rI.yCenter)+4)
         nodeHighlightNum = i;
      }
      
      drawScene(hwnd);
      return 0;
   }
   
   if (mesh_MESHCNT)
   if (WM_MOUSEMOVE == message)
   {
      if (drag == 1)
      {
         lrMove = (LOWORD(lParam) - xMmem);
         tbMove = -(HIWORD(lParam) - yMmem);

         xMmem = LOWORD(lParam);
         yMmem = HIWORD(lParam);

         lrMove = lrMove / 80;
         tbMove = tbMove / 80;
         ccMove = 0;
         
         for (i = 0; i < mesh_MESHCNT*9; i=i+9)
         rotateRelToScrn(lrMove, tbMove, ccMove, 
                         &mesh_TRANS[0+i], &mesh_TRANS[1+i], &mesh_TRANS[2+i]);
         
         for (i = 0; i < mesh_MESHCNT*9; i=i+9)
         rotatePt(&mesh_TRANS[3+i], &mesh_TRANS[4+i], &mesh_TRANS[5+i],
                  xMainCent, yMainCent, zMainCent, lrMove, tbMove, ccMove);
         
         drawScene(hwnd);
      }
      return 0;
   }
   
   if (mesh_MESHCNT)
   if (WM_MOUSEWHEEL == message)
   {
      if (HIWORD(wParam) == 120)
      {
         for (i = 0; i < mesh_MESHCNT*9; i=i+9)
         mesh_TRANS[5+i] = mesh_TRANS[5+i] + 10;//z
         //Move Main Center Point
         zMainCent = zMainCent + 10;//z
      }
      if (HIWORD(wParam) == 65416)
      {
         for (i = 0; i < mesh_MESHCNT*9; i=i+9)
         mesh_TRANS[5+i] = mesh_TRANS[5+i] - 10;//z
         //Move Main Center Point
         zMainCent = zMainCent - 10;//z
      }
      
      drawScene(hwnd);
      return 0;
   }
/*
   if (WM_PAINT == message)
   {
      hdc = BeginPaint(hwnd, &ps);
      SetDIBitsToDevice(hdc, 0, 0, rI.xWin, rI.yWin,
                          0, 0, 0, rI.yWin, pBits, pbmi, DIB_RGB_COLORS);
      EndPaint(hwnd, &ps);
      return 0;
   }
*/
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
         
      if (mesh_COLORDATSZAllocated)
      free(mesh_COLORDATSZ);
      if (mesh_LINEDATSZAllocated)
      free(mesh_LINEDATSZ);
      if (mesh_POINTDATSZAllocated)
      free(mesh_POINTDATSZ);
      
      if (mesh_COLORAllocated)
      free(mesh_COLOR);
      if (mesh_LINEAllocated)
      free(mesh_LINE);
      if (mesh_POINTAllocated)
      free(mesh_POINT);
      if (mesh_TRANSAllocated)
      free(mesh_TRANS);
      
      if (mesh_STATICPOINTAllocated)
      free(mesh_STATICPOINT);
      
      if (mesh_STATICTRANSAllocated)
      free(mesh_STATICTRANS);
      
      if (meshOld_LINEDATSZAllocated)
      free(meshOld_LINEDATSZ);
      if (meshOld_COLORDATSZAllocated)
      free(meshOld_COLORDATSZ);
      if (meshOld_LINEAllocated)
      free(meshOld_LINE);
      if (meshOld_COLORAllocated)
      free(meshOld_COLOR);
      
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

   bgColor = 0xAA8888;
   
   for (y = 0; y < rI.yWin; y++)
   for (x = 0; x < rI.xWin; x++)
   rI.scrnBuff[x][y] = bgColor;
   
   if (mesh_MESHCNT)
   addRotLocSz(mesh_TRANS,
               mesh_POINT,
               mesh_POINTDATSZ,
               mesh_STATICPOINT, mesh_MESHCNT);
   
   bgColor = 0xFFFFFF;
   
   colorCnt = 0;
   lineCnt = 0;
   if (mesh_MESHCNT)
   while (lineCnt < mesh_LINEDATSZ[mesh_MESHCNT-1])
   {
      sidesNum = mesh_LINE[lineCnt];
      
      color = mesh_COLOR[colorCnt];
      
      if (sidesNum == 2)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 3;
         colorCnt++;
      }
      
      if (sidesNum == 3)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
                mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
                mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 4;
         colorCnt++;
      }
      
      if (sidesNum == 4)
      {
         pt1 = mesh_LINE[lineCnt+1] * 3;
         pt2 = mesh_LINE[lineCnt+2] * 3;
         pt3 = mesh_LINE[lineCnt+3] * 3;
         pt4 = mesh_LINE[lineCnt+4] * 3;
         
         afLine(mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2],
                mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt2], mesh_POINT[pt2+1], mesh_POINT[pt2+2],
                mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt3], mesh_POINT[pt3+1], mesh_POINT[pt3+2],
                mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2], color, bgColor, rI);
         
         afLine(mesh_POINT[pt4], mesh_POINT[pt4+1], mesh_POINT[pt4+2],
                mesh_POINT[pt1], mesh_POINT[pt1+1], mesh_POINT[pt1+2], color, bgColor, rI);
         
         lineCnt = lineCnt + 5;
         colorCnt++;
      }
   }

   if (viewObjCenterLocsOnOff)
   if (mesh_MESHCNT)
   if (mesh_LINE[0] == 2)
   viewObjCenterLocs(mesh_TRANS, mesh_MESHCNT, rI);
   
   if (viewPtLocsOnOff)
   if (mesh_MESHCNT)
   if (mesh_LINE[0] == 2)
   viewPtLocs(mesh_POINT, mesh_POINTDATSZ, mesh_MESHCNT, rI);
   
   if (viewLineLocsOnOff)
   if (mesh_MESHCNT)
   if (mesh_LINE[0] == 2)
   viewLineLocs(mesh_LINE, mesh_LINEDATSZ, mesh_POINT, mesh_MESHCNT, rI);
   
   if (editMode)
   if (mesh_MESHCNT)
   if (mesh_LINE[0] == 2)
   showEditNodes(mesh_POINT, mesh_POINTDATSZ, mesh_MESHCNT, rI);
   
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
