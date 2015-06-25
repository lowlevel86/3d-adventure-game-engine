#include <windows.h>
#include <stdio.h>

HINSTANCE hInstance;
HWND hwndHelpEdit;
HWND hwndHelpCloseBtn;

char helpInfo[] = "This program takes polygon data written in C and converts it to line data. \
This means the output C code, maybe used for drawing the edges of a cube, could have 12 vectors instead of 24.\r\n\
\r\n\
To load a C file drag and drop it into the program window. You can also load a C file using the command terminal.\r\n\
\r\n\
Navigate:\r\n\
Use the mouse to rotate and zoom in and out of the model.\r\n\
The arrow keys can also be used to rotate the model.\r\n\
Press + to increase the model size.\r\n\
Press - to decrease the model size.\r\n\
Press F to move the model forward.\r\n\
Press B to move the model backward.\r\n\
Press Num key 4, 6, 8, or 2 to move the model left, right, up, or down.\r\n\
Press Num key 7 or 9 to rotate the model.\r\n\
\r\n\
Name the Contents:\r\n\
Press N, type a name and it will show up in the C code.\r\n\
\r\n\
Convert the C Code:\r\n\
Press C to convert polygon data to line data.\r\n\
\r\n\
Edit the Mesh: (code must be converted)\r\n\
Press E to go in and out of edit mode.\r\n\
Right click to highlight a vertex.\r\n\
Press R to remove the highlighted vertex.\r\n\
\r\n\
View Data About the Mesh: (code must be converted)\r\n\
Press O to view the sequential number of each mesh object.\r\n\
Press P to view locations of points in the point data array.\r\n\
Press L to view locations of lines in the line data array.\r\n\
\r\n\
Rewrite the C Files:\r\n\
Press W to write the code to your C files.\r\n\
\r\n\
Example Input C File:\r\n\
//shapes.c\r\n\
float shapes_TRANS[18]={\r\n\
0.000000, -0.000000, 0.000000,\r\n\
100.000000, 0.000000, 0.000000,\r\n\
50.000000, 50.000000, 50.000000,\r\n\
0.000000, -0.000000, 0.000000,\r\n\
-100.000000, 0.000000, 0.000000,\r\n\
50.000000, 50.000000, 50.000000,\r\n\
};\r\n\
\r\n\
float shapes_POINT[39]={\r\n\
1.000000, 1.000000, -1.000000,\r\n\
1.000000, -1.000000, -1.000000,\r\n\
-1.000000, -1.000000, -1.000000,\r\n\
-1.000000, 1.000000, -1.000000,\r\n\
0.000000, -0.000000, 1.000000,\r\n\
1.000000, 1.000000, -1.000000,\r\n\
1.000000, -1.000000, -1.000000,\r\n\
-1.000000, -1.000000, -1.000000,\r\n\
-1.000000, 1.000000, -1.000000,\r\n\
1.000000, 1.000000, 1.000000,\r\n\
1.000000, -1.000001, 1.000000,\r\n\
-1.000000, -1.000000, 1.000000,\r\n\
-1.000000, 1.000000, 1.000000,\r\n\
};\r\n\
\r\n\
int shapes_LINE[51]={\r\n\
4, 0, 1, 2, 3,\r\n\
3, 0, 4, 1,\r\n\
3, 1, 4, 2,\r\n\
3, 2, 4, 3,\r\n\
3, 4, 0, 3,\r\n\
4, 5, 6, 7, 8,\r\n\
4, 9, 12, 11, 10,\r\n\
4, 5, 9, 10, 6,\r\n\
4, 6, 10, 11, 7,\r\n\
4, 7, 11, 12, 8,\r\n\
4, 9, 5, 8, 12,\r\n\
};\r\n\
\r\n\
int shapes_COLOR[11]={\r\n\
0x0,\r\n\
0xff00,\r\n\
0xff00,\r\n\
0xff00,\r\n\
0xff00,\r\n\
0x0,\r\n\
0xff0000,\r\n\
0xff0000,\r\n\
0xff0000,\r\n\
0xff0000,\r\n\
0xff0000,\r\n\
};\r\n\
\r\n\
int shapes_POINTDATSZ[2]={\r\n\
15,\r\n\
39,\r\n\
};\r\n\
\r\n\
int shapes_LINEDATSZ[2]={\r\n\
21,\r\n\
51,\r\n\
};\r\n\
\r\n\
int shapes_COLORDATSZ[2]={\r\n\
5,\r\n\
11,\r\n\
};\r\n\
\r\n\
int shapes_MESHCNT = 2;\r\n\
//end of shapes.c\r\n\
\r\n\
Command Terminal Usage:\r\n\
polyToLine.exe [Input File] -[Option Letter]...\r\n\
\r\n\
-h Show help information.\r\n\
\r\n\
-e Remove the vertex that is furthest away from the local x, y, or z plane of each object listed. \
List the sequential number of each mesh object by using a number and comma array (0,1,3). \
You can also use dashes so you wouldn't have to type every number (5-28).\r\n\
Example:\r\n\
polyToLine.exe mesh.c -e 0,2,4,7-18,22\r\n\
\r\n\
-n Name the contents in the C code.\r\n\
\r\n\
-o Output file name.\r\n\
\r\n\
Example with all options:\r\n\
polyToLine.exe poly.c -e 0,1,2 -n mesh -o line.c";

LRESULT CALLBACK helpWnd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_CREATE == message)
	{
      hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
      
      hwndHelpEdit = CreateWindow(TEXT("edit"), "",
      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,// | ES_READONLY,
                              12, 16, 380, 254,
                              hwnd, (HMENU)1, hInstance, NULL);
      
		hwndHelpCloseBtn = CreateWindow(TEXT("button"), "OK",
			                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			                         404/2-64, 285, 128, 24,
                                  hwnd, (HMENU)3, hInstance, NULL);
      
		return 0;
	}
   
   if (WM_CTLCOLORBTN == message)
   return (LRESULT)CreateSolidBrush(RGB(0xC0,0xC0,0xC0));
   
   if (WM_SETFOCUS == message)
   {
      SetFocus(hwndHelpCloseBtn);
      SendMessage(hwndHelpEdit, WM_SETTEXT, (WPARAM)0, (LPARAM)TEXT(helpInfo));
      return 0;
   }
   
   if (WM_COMMAND == message)
   {
      if (LOWORD(wParam) == 3) // if press ok button
      ShowWindow(hwnd, SW_HIDE);
      
      return 0;
   }
   
	return DefWindowProc(hwnd, message, wParam, lParam);
}
