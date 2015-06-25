#include <stdio.h>
#include "findStrInStr.h"

void rmCComments(char *fileData, int fileSize)
{
   int i;
   int charNum = 0, endCharNum;
   char endChar[2] = {10,0};
   
   // erase all line comments
   while (charNum != -1)
   {
      findStrInStr(fileData, "//", &charNum);
      
      if (charNum != -1)
      {
         endCharNum = charNum;
         findStrInStr(fileData, endChar, &endCharNum);
         
         if (endCharNum == -1)// if no end of line char
         endCharNum = fileSize;
         
         for (i=charNum; i < endCharNum; i++)
         fileData[i] = 0x20;
      }
   }
   
   // erase all page comments (/* */)
   charNum = 0;
   while (charNum != -1)
   {
      findStrInStr(fileData, "/*", &charNum);
      
      if (charNum != -1)
      {
         endCharNum = charNum;
         findStrInStr(fileData, "*/", &endCharNum);
         
         for (i=charNum; i < endCharNum+2; i++)
         fileData[i] = 0x20;
      }
   }
}
