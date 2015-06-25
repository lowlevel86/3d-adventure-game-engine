#include <stdio.h>
#include <stdlib.h>

void findStrInStr(char *, char *, int *);

void prtCCode(float *mesh_TRANS, float *mesh_POINT, int *mesh_LINE, int *mesh_COLOR,
              int *mesh_POINTDATSZ, int *mesh_LINEDATSZ, int *mesh_COLORDATSZ,
              float *mesh_STATICPOINT, int meshCnt, char *meshName, char *fileName)
{
   int i, j, line;
   char fileName2[4096];
   int strPos = 0;
   FILE *outFilePtr;
   
   outFilePtr = fopen(fileName, "w");
   if (!outFilePtr)
   return;
   
   findStrInStr(fileName, ".c", &strPos);
   
   fprintf(outFilePtr, "//global transformation data\n");
	fprintf(outFilePtr, "float %s_GLOBAL[9]={\n", meshName);
	fprintf(outFilePtr, "0.000000, 0.000000, 0.000000, //rotation\n");
	fprintf(outFilePtr, "0.000000, 0.000000, 0.000000, //location\n");
	fprintf(outFilePtr, "1.000000, 1.000000, 1.000000, //size\n");
	fprintf(outFilePtr, "};\n\n");
   
   fprintf(outFilePtr, "//transformation data\n");
   fprintf(outFilePtr, "float %s_TRANS[%i]={\n", meshName, meshCnt * 9);
   j = 0;
   for (i = 0; i < meshCnt * 9; i++)
   {
      if (9 * j == i)
      {
         fprintf(outFilePtr, "//data block %i\n", j);
         j++;
      }
      
      fprintf(outFilePtr, "%f, ", mesh_TRANS[i]);
      i++;
      fprintf(outFilePtr, "%f, ", mesh_TRANS[i]);
      i++;
      fprintf(outFilePtr, "%f,\n", mesh_TRANS[i]);
   }
   fprintf(outFilePtr, "};\n\n");
   
   
   fprintf(outFilePtr, "//point data\n");
   fprintf(outFilePtr, "float %s_POINT[%i]={\n", meshName, mesh_POINTDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "//data array 0\n");
   j = 0;
   for (i = 0; i < mesh_POINTDATSZ[meshCnt-1]; i++)
   {
      if (mesh_POINTDATSZ[j] == i)
      {
         j++;
         fprintf(outFilePtr, "//data array %i\n", j);
      }
      
      fprintf(outFilePtr, "%f, ", mesh_STATICPOINT[i]);
      i++;
      fprintf(outFilePtr, "%f, ", mesh_STATICPOINT[i]);
      i++;
      fprintf(outFilePtr, "%f,\n", mesh_STATICPOINT[i]);
   }
   fprintf(outFilePtr, "};\n\n");
   
   
   fprintf(outFilePtr, "//line data\n");
   fprintf(outFilePtr, "int %s_LINE[%i]={\n", meshName, mesh_LINEDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "//data array 0\n");
   j = 0;
   i = 0;
   while (i < mesh_LINEDATSZ[meshCnt-1])
   {
      if (mesh_LINEDATSZ[j] == i)
      {
         j++;
         fprintf(outFilePtr, "//data array %i\n", j);
      }
      
      line = mesh_LINE[i];
      
      if (line == 2)
      {
         fprintf(outFilePtr, "%i, ", 2);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i,\n", mesh_LINE[i]);
         i++;
      }
      
      if (line == 3)
      {
         fprintf(outFilePtr, "%i, ", 3);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i,\n", mesh_LINE[i]);
         i++;
      }
      
      if (line == 4)
      {
         fprintf(outFilePtr, "%i, ", 4);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i, ", mesh_LINE[i]);
         i++;
         fprintf(outFilePtr, "%i,\n", mesh_LINE[i]);
         i++;
      }
   }
   fprintf(outFilePtr, "};\n\n");
   
   
   fprintf(outFilePtr, "//color data\n");
   fprintf(outFilePtr, "int %s_COLOR[%i]={\n", meshName, mesh_COLORDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "//data array 0\n");
   j = 0;
   for (i = 0; i < mesh_COLORDATSZ[meshCnt-1]; i++)
   {
      if (mesh_COLORDATSZ[j] == i)
      {
         j++;
         fprintf(outFilePtr, "//data array %i\n", j);
      }
      
      fprintf(outFilePtr, "0x%x,\n", mesh_COLOR[i]);
   }
   fprintf(outFilePtr, "};\n\n");
   
   fprintf(outFilePtr, "//point data array sizes\n");
   fprintf(outFilePtr, "int %s_POINTDATSZ[%i]={\n", meshName, meshCnt);
   for (i = 0; i < meshCnt; i++)
   fprintf(outFilePtr, "%i,\n", mesh_POINTDATSZ[i]);
   fprintf(outFilePtr, "};\n\n");
   
   fprintf(outFilePtr, "//line data array sizes\n");
   fprintf(outFilePtr, "int %s_LINEDATSZ[%i]={\n", meshName, meshCnt);
   for (i = 0; i < meshCnt; i++)
   fprintf(outFilePtr, "%i,\n", mesh_LINEDATSZ[i]);
   fprintf(outFilePtr, "};\n\n");
   
   fprintf(outFilePtr, "//color data array sizes\n");
   fprintf(outFilePtr, "int %s_COLORDATSZ[%i]={\n", meshName, meshCnt);
   for (i = 0; i < meshCnt; i++)
   fprintf(outFilePtr, "%i,\n", mesh_COLORDATSZ[i]);
   fprintf(outFilePtr, "};\n\n");
   
   fprintf(outFilePtr, "//number of meshes\n");
   fprintf(outFilePtr, "int %s_MESHCNT = %i;\n", meshName, meshCnt);
   
   fclose(outFilePtr);
   
   for (i=0; i < strPos+1; i++)
   fileName2[i] = fileName[i];
   
   fileName2[strPos+1] = (char)104;//104 = h
   fileName2[strPos+2] = (char)0;//104 = h
   
   outFilePtr = fopen(fileName2, "w");
   if (!outFilePtr)
   return;
   
   fprintf(outFilePtr, "extern float %s_GLOBAL[9];\n", meshName);
   fprintf(outFilePtr, "extern float %s_TRANS[%i];\n", meshName, meshCnt * 9);
   fprintf(outFilePtr, "extern float %s_POINT[%i];\n", meshName, mesh_POINTDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "extern int %s_LINE[%i];\n", meshName, mesh_LINEDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "extern int %s_COLOR[%i];\n", meshName, mesh_COLORDATSZ[meshCnt-1]);
   fprintf(outFilePtr, "extern int %s_POINTDATSZ[%i];\n", meshName, meshCnt);
   fprintf(outFilePtr, "extern int %s_LINEDATSZ[%i];\n", meshName, meshCnt);
   fprintf(outFilePtr, "extern int %s_COLORDATSZ[%i];\n\n", meshName, meshCnt);
   
	fprintf(outFilePtr, "//unchanging global transformation data\n");
	fprintf(outFilePtr, "float %s_STATICGLOBAL[9];\n\n", meshName);

	fprintf(outFilePtr, "//unchanging transformation data\n");
	fprintf(outFilePtr, "float %s_STATICTRANS[%i];\n\n", meshName, meshCnt * 9);

   fprintf(outFilePtr, "//unchanging point data\n");
   fprintf(outFilePtr, "float %s_STATICPOINT[%i];\n\n", meshName, mesh_POINTDATSZ[meshCnt-1]);
   
   fprintf(outFilePtr, "extern int %s_MESHCNT;\n\n", meshName);
   
   fclose(outFilePtr);
   
   return;
}
