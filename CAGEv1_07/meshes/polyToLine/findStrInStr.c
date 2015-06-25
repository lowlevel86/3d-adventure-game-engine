#include <stdio.h>

extern void findStrInStr(char *lgStr, char *smStr, int *charNum)
{
   int i, j;
   
   i = *charNum;
   j = 0;
   
   while (lgStr[i])
   {
      *charNum = i;
      while (lgStr[i] == smStr[j])
      {
         i++;
         j++;
         
         if (!smStr[j])
         return;
         
         if (!lgStr[i])
         {
            *charNum = -1;
            return;
         }
      }
      j = 0;
      i++;
   }
   *charNum = -1;
   return;
}

extern void findStrInStrRev(char *lgStr, char *smStr, int *charNum)
{
   int i, j, smStrLgth;
   
   smStrLgth = 0;
   while (smStr[smStrLgth])
   smStrLgth++;
   
   i = *charNum;
   
   while (i != -1)
   {
      j = smStrLgth-1;
      while (lgStr[i] == smStr[j])
      {
         i--;
         j--;
         
         if (j == -1)
         {
            *charNum = i-j;
            return;
         }
         
         if (i == -1)
         {
            *charNum = -1;
            return;
         }
      }
      i--;
   }
   *charNum = -1;
   return;
}
