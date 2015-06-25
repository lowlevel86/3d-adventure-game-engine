#include <windows.h>
#include <stdio.h>
#include "nameWnd.h"

HINSTANCE hInstance;
HWND hwndEdit;
HWND hwndOwnerDraw;
HWND hwndCloseBtn;

char meshName[4096] = "mesh";

LRESULT CALLBACK nameWnd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   int i;
   
	if (WM_CREATE == message)
	{
      hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
      
      hwndEdit = CreateWindow(TEXT("edit"), "",
                              WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
                              12, 20, 180, 22,
                              hwnd, (HMENU)1, hInstance, NULL);
      
      hwndOwnerDraw = CreateWindow(TEXT("button"), 0,
                                   WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_THICKFRAME | WS_DISABLED,
                                   0, 61, 204, 42,
                                   hwnd, (HMENU)2, hInstance, NULL);
      
		hwndCloseBtn = CreateWindow(TEXT("button"), "OK",
			                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			                         204/2-64, 69, 128, 24,
                                  hwnd, (HMENU)3, hInstance, NULL);
      
		return 0;
	}
   
   if (WM_CTLCOLORBTN == message)
   return (LRESULT)CreateSolidBrush(RGB(0xC0,0xC0,0xC0));
   
   if (WM_SETFOCUS == message)
   {
      SetFocus(hwndEdit);
      SendMessage(hwndEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)TEXT(meshName));
      SendMessage(hwndEdit, EM_SETSEL, (WPARAM)1024, (LPARAM)1024);
      return 0;
   }
   
   
   if (WM_COMMAND == message)
   {
      if (LOWORD(wParam) == 1) // edit event
      {
         GetDlgItemText(hwnd, 1, meshName, 1024);
         
         for (i = 0; i < 1024; i++)
         if (meshName[i] == 13)// char after hitting enter
         {
            meshName[i] = 0;
            SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)TEXT(meshName));
            ShowWindow(hwnd, SW_HIDE);
            i = 1024;
         }
      }
      
      if (LOWORD(wParam) == 3) // if press ok button
      {
         GetDlgItemText(hwnd, 1, meshName, 1024);
         ShowWindow(hwnd, SW_HIDE);
      }
      
      return 0;
   }
   
	return DefWindowProc(hwnd, message, wParam, lParam);
}
