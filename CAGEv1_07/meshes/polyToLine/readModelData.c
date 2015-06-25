#include <stdio.h>
#include <stdlib.h>
#include "findStrInStr.h"

void getModelDataSzs(char *fileData, int *transSz,
                     int *pointSz, int *lineSz, int *colorSz)
{
   int i;
   int charNum = 0, endCharNum;
   char numberInTxt[32];
   
   // find transfomation data size
   findStrInStr(fileData, "_TRANS", &charNum);
   if (charNum == -1)
   return;
   
   endCharNum = charNum;
   findStrInStr(fileData, "]", &endCharNum);
   
   for (i=0; i+charNum+7 < endCharNum; i++)
   numberInTxt[i] = fileData[i+charNum+7];
   
   numberInTxt[i] = 0;
   
   *transSz = atoi(numberInTxt);
   
   // find point data size
   findStrInStr(fileData, "_POINT", &charNum);
   if (charNum == -1)
   return;
   
   endCharNum = charNum;
   findStrInStr(fileData, "]", &endCharNum);
   
   for (i=0; i+charNum+7 < endCharNum; i++)
   numberInTxt[i] = fileData[i+charNum+7];
   
   numberInTxt[i] = 0;
   
   *pointSz = atoi(numberInTxt);
   
   // find line data size
   findStrInStr(fileData, "_LINE", &charNum);
   if (charNum == -1)
   return;
   
   endCharNum = charNum;
   findStrInStr(fileData, "]", &endCharNum);
   
   for (i=0; i+charNum+6 < endCharNum; i++)
   numberInTxt[i] = fileData[i+charNum+6];
   
   numberInTxt[i] = 0;
   
   *lineSz = atoi(numberInTxt);
   
   // find color data size
   findStrInStr(fileData, "_COLOR", &charNum);
   if (charNum == -1)
   return;
   
   endCharNum = charNum;
   findStrInStr(fileData, "]", &endCharNum);
   
   for (i=0; i+charNum+7 < endCharNum; i++)
   numberInTxt[i] = fileData[i+charNum+7];
   
   numberInTxt[i] = 0;
   
   *colorSz = atoi(numberInTxt);
}

void readModelData(char *fileData, int transSz, int pointSz, int lineSz, int colorSz,
                   float *mesh_TRANS, float *mesh_POINT, int *mesh_LINE, int *mesh_COLOR,
                   int *mesh_POINTDATSZ, int *mesh_LINEDATSZ, int *mesh_COLORDATSZ)
{
   int i, meshDataCnt;
   int charNum = 0, endCharNum, endCharNum2;
   char numberInTxt[32];
   
   // get transfomation data
   meshDataCnt = 0;
   findStrInStr(fileData, "_TRANS", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < transSz; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_TRANS[meshDataCnt] = atof(numberInTxt);
      
      charNum = endCharNum;
   }
   
   // get point data
   meshDataCnt = 0;
   findStrInStr(fileData, "_POINT", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < pointSz; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_POINT[meshDataCnt] = atof(numberInTxt);
      
      charNum = endCharNum;
   }
   
   // get line data
   meshDataCnt = 0;
   findStrInStr(fileData, "_LINE", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < lineSz; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_LINE[meshDataCnt] = atoi(numberInTxt);
      
      charNum = endCharNum;
   }
   
   // get color data
   meshDataCnt = 0;
   findStrInStr(fileData, "_COLOR", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < colorSz; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      sscanf(numberInTxt, "%x", &mesh_COLOR[meshDataCnt]);
      
      charNum = endCharNum;
   }
   
   // get point data size
   meshDataCnt = 0;
   findStrInStr(fileData, "_POINTDATSZ", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < transSz / 9; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_POINTDATSZ[meshDataCnt] = atoi(numberInTxt);
      
      charNum = endCharNum;
   }
   
   // get line data size
   meshDataCnt = 0;
   findStrInStr(fileData, "_LINEDATSZ", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < transSz / 9; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_LINEDATSZ[meshDataCnt] = atoi(numberInTxt);
      
      charNum = endCharNum;
   }
   
   // get color data size
   meshDataCnt = 0;
   findStrInStr(fileData, "_COLORDATSZ", &charNum);
   findStrInStr(fileData, "{", &charNum);
   
   for (meshDataCnt=0; meshDataCnt < transSz / 9; meshDataCnt++)
   {
      endCharNum = charNum+1;
      findStrInStr(fileData, ",", &endCharNum);
      endCharNum2 = charNum+1;
      findStrInStr(fileData, "}", &endCharNum2);
      
      if (endCharNum2 < endCharNum)
      endCharNum = endCharNum2;
      
      for (i=0; i+charNum+1 < endCharNum; i++)
      numberInTxt[i] = fileData[i+charNum+1];
      
      numberInTxt[i] = 0;
      
      mesh_COLORDATSZ[meshDataCnt] = atoi(numberInTxt);
      
      charNum = endCharNum;
   }
}
