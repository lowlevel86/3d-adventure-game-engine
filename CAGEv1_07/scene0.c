int menuHighlightOption;

static int optFullScreen = 0;
static char optFullScreenTxt[64] = "Full_Screen [ no ]";

static int optKeepAspectRatio = 0;
static char optKeepAspectRatioTxt[64] = "   Keep_Aspect_Ratio [ no ]";
static float defaultAspectRatio;

static char optDisplayScrnResTxt[64];

static char optWindowSizeTxt[64];
static int optStaticWinWidth, optStaticWinHeight;

static int optAntialiasing = 0;
static char optAntialiasingTxt[64] = "Antialiasing [ no ]";

static int initializeDrawScene0 = 1;


if (initializeDrawScene0)
{
   displayIndex = getCurrentDisplayRes(&xScrnRes, &yScrnRes, &colorDepth);
   originalDisplayIndex = displayIndex;
   
   sprintf(optDisplayScrnResTxt, "   Display < %ix%i >", xScrnRes, yScrnRes);
   
   defaultAspectRatio = (float)yScrnRes / xScrnRes;
   
   optStaticWinWidth = staticWinWidth;
   optStaticWinHeight = staticWinHeight;
   
   if ((optStaticWinWidth >= xScrnRes) && (optStaticWinHeight >= yScrnRes))
   sprintf(optWindowSizeTxt, "Window < %ix%i > (Full_Size)", optStaticWinWidth, optStaticWinHeight);
   else
   sprintf(optWindowSizeTxt, "Window < %ix%i >", optStaticWinWidth, optStaticWinHeight);
   
   initializeDrawScene0 = 0;
}

menuHighlightOption = 0;

if (viewMainMenu)
{
   if ((xMousePos-xFrameOffset >= rI.xCenter) &&
       (yMousePos-yFrameOffset >= rI.yCenter-5) &&
       (yMousePos-yFrameOffset <= rI.yCenter+15))
   menuHighlightOption = 1;

   if ((xMousePos-xFrameOffset >= rI.xCenter) &&
       (yMousePos-yFrameOffset >= rI.yCenter+15) &&
       (yMousePos-yFrameOffset <= rI.yCenter+35))
   menuHighlightOption = 2;

   if ((xMousePos-xFrameOffset >= rI.xCenter) &&
       (yMousePos-yFrameOffset >= rI.yCenter+35) &&
       (yMousePos-yFrameOffset <= rI.yCenter+55))
   menuHighlightOption = 3;

   if ((xMousePos-xFrameOffset >= rI.xCenter) &&
       (yMousePos-yFrameOffset >= rI.yCenter+55) &&
       (yMousePos-yFrameOffset <= rI.yCenter+75))
   menuHighlightOption = 4;
}

if (viewControls)
{
   if ((xMousePos-xFrameOffset <= rI.xCenter) &&
       (yMousePos-yFrameOffset >= 85) &&
       (yMousePos-yFrameOffset <= 105))
   menuHighlightOption = 1;
}

if (viewOptions)
{
   if ((xMousePos-xFrameOffset >= 80) &&
       (xMousePos-xFrameOffset <= 140) &&
       (yMousePos-yFrameOffset >= 5) &&
       (yMousePos-yFrameOffset <= 25))
   menuHighlightOption = 1;
   
   if ((xMousePos-xFrameOffset >= 140) &&
       (xMousePos-xFrameOffset <= 200) &&
       (yMousePos-yFrameOffset >= 25) &&
       (yMousePos-yFrameOffset <= 45))
   menuHighlightOption = 2;
   
   if ((xMousePos-xFrameOffset >= 70) &&
       (xMousePos-xFrameOffset <= 100) &&
       (yMousePos-yFrameOffset >= 45) &&
       (yMousePos-yFrameOffset <= 65))
   menuHighlightOption = 3;
   if ((xMousePos-xFrameOffset >= 140) &&
       (xMousePos-xFrameOffset <= 170) &&
       (yMousePos-yFrameOffset >= 45) &&
       (yMousePos-yFrameOffset <= 65))
   menuHighlightOption = 4;
   
   if ((xMousePos-xFrameOffset >= 50) &&
       (xMousePos-xFrameOffset <= 80) &&
       (yMousePos-yFrameOffset >= 65) &&
       (yMousePos-yFrameOffset <= 85))
   menuHighlightOption = 5;
   if ((xMousePos-xFrameOffset >= 120) &&
       (xMousePos-xFrameOffset <= 150) &&
       (yMousePos-yFrameOffset >= 65) &&
       (yMousePos-yFrameOffset <= 85))
   menuHighlightOption = 6;
   
   if ((xMousePos-xFrameOffset >= 90) &&
       (xMousePos-xFrameOffset <= 150) &&
       (yMousePos-yFrameOffset >= 85) &&
       (yMousePos-yFrameOffset <= 105))
   menuHighlightOption = 7;
   
   if ((xMousePos-xFrameOffset <= rI.xCenter) &&
       (yMousePos-yFrameOffset >= 125) &&
       (yMousePos-yFrameOffset <= 145))
   menuHighlightOption = 8;
}

if (leftMouseUp)
{
   if (viewMainMenu)
   {
      if (menuHighlightOption == 1)
      {
         // change into full screen mode
         if ((optFullScreen) && (!fullScreenMode))
         {
            changefullScreenMode = 1;
            xWinSave = rI.xWin + xFrameOffset * 2;
            yWinSave = rI.yWin + yFrameOffset * 2;
         }
         // change out of full screen mode
         if ((!optFullScreen) && (fullScreenMode))
         changefullScreenMode = 1;
         
         // change display resolution
         if (optFullScreen)
         {
            changeDisplayMode = 1;
            
            if (fullScreenMode)
            changeDisplayRes(displayIndex);
         }
         
         if (staticWinHeight != optStaticWinHeight)
         {
            staticWinWidth = optStaticWinWidth;
            staticWinHeight = optStaticWinHeight;
            
            if (staticWinWidth > xScrnRes)
            staticWinWidth = xScrnRes;
            
            if (staticWinHeight > yScrnRes)
            staticWinHeight = yScrnRes;
            
            changeStaticWindow = 1;
         }
         
         if ((optKeepAspectRatio) && (optFullScreen))
         {
            xScreenSize = zScreenSize;
            xScreenSize = xScreenSize * (defaultAspectRatio / ((float)yScrnRes / xScrnRes));
            staticWinWidth = staticWinWidth * (defaultAspectRatio / ((float)yScrnRes / xScrnRes));
            changeStaticWindow = 1;
         }
         else
         {
            xScreenSize = zScreenSize;
            staticWinWidth = optStaticWinWidth;
            changeStaticWindow = 1;
         }
         
         if (optAntialiasing)
         antialiasing = 1;
         else
         antialiasing = 0;
         
         resetDrawScene = 1;
         sceneNumber = 1;
      }
      if (menuHighlightOption == 2)
      {
         viewMainMenu = 0;
         viewOptions = 1;
         menuHighlightOption = 0;
      }
      if (menuHighlightOption == 3)
      {
         viewMainMenu = 0;
         viewControls = 1;
         menuHighlightOption = 0;
      }
      if (menuHighlightOption == 4)
      {
         exitMsg = 1;
         return;
      }
   }
   
   if (viewControls)
   {  
      if (menuHighlightOption == 1)
      {
         viewMainMenu = 1;
         viewControls = 0;
         menuHighlightOption = 0;
      }
   }
   
   if (viewOptions)
   {
      if (menuHighlightOption == 1)
      {
         optFullScreen = !optFullScreen;
         if (optFullScreen)
         sprintf(optFullScreenTxt, "%s", "Full_Screen [ yes ]");
         else
         sprintf(optFullScreenTxt, "%s", "Full_Screen [ no ]");
      }
      
      if (menuHighlightOption == 2)
      {
         optKeepAspectRatio = !optKeepAspectRatio;
         if (optKeepAspectRatio)
         sprintf(optKeepAspectRatioTxt, "%s", "   Keep_Aspect_Ratio [ yes ]");
         else
         sprintf(optKeepAspectRatioTxt, "%s", "   Keep_Aspect_Ratio [ no ]");
      }
      
      if (menuHighlightOption == 3)
      {
         i = displayIndex;
         displayIndex = getNextDisplayResDown(&xScrnRes, &yScrnRes, colorDepth);
         if (displayIndex == -1)
         displayIndex = i;
         sprintf(optDisplayScrnResTxt, "   Display < %ix%i >", xScrnRes, yScrnRes);
      }
      if (menuHighlightOption == 4)
      {
         i = displayIndex;
         displayIndex = getNextDisplayResUp(&xScrnRes, &yScrnRes, colorDepth);
         if (displayIndex == -1)
         displayIndex = i;
         sprintf(optDisplayScrnResTxt, "   Display < %ix%i >", xScrnRes, yScrnRes);
      }
      
      if (menuHighlightOption == 5)
      {
         if (optStaticWinHeight > 180)
         {
            optStaticWinWidth = optStaticWinWidth - 80;
            optStaticWinHeight = optStaticWinHeight - 60;
            
            if (optStaticWinWidth*scrnRatio >= yScrnRes)
            sprintf(optWindowSizeTxt, "Window < %ix%i > (Full_Size)", optStaticWinWidth, optStaticWinHeight);
            else
            sprintf(optWindowSizeTxt, "Window < %ix%i >", optStaticWinWidth, optStaticWinHeight);
         }
      }
      if (menuHighlightOption == 6)
      {
         if (optStaticWinWidth*scrnRatio < yScrnRes)
         {
            optStaticWinWidth = optStaticWinWidth + 80;
            optStaticWinHeight = optStaticWinHeight + 60;
            
            if (optStaticWinWidth*scrnRatio >= yScrnRes)
            sprintf(optWindowSizeTxt, "Window < %ix%i > (Full_Size)", optStaticWinWidth, optStaticWinHeight);
            else
            sprintf(optWindowSizeTxt, "Window < %ix%i >", optStaticWinWidth, optStaticWinHeight);
         }
      }
      
      if (menuHighlightOption == 7)
      {
         optAntialiasing = !optAntialiasing;
         if (optAntialiasing)
         sprintf(optAntialiasingTxt, "%s", "Antialiasing [ yes ]");
         else
         sprintf(optAntialiasingTxt, "%s", "Antialiasing [ no ]");
      }
      
      if (menuHighlightOption == 8)
      {
         viewMainMenu = 1;
         viewOptions = 0;
         menuHighlightOption = 0;
      }
   }
}

// DRAW //
// fill z-index buffer
for (y = 0; y < rI.yWin; y++)
for (x = 0; x < rI.xWin; x++)
rI.zIndex[x][y] = rI.camEndZ;

bgColor = 0x888888;
// color background
for (y = 0; y < rI.yWin; y++)
for (x = 0; x < rI.xWin; x++)
rI.scrnBuff[x][y] = bgColor;

if (viewMainMenu)
{
   drawAscII("PLAY GAME", 10*rI.xWin/400, 10*rI.xWin/400, 5, -5, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   drawAscII("OPTIONS", 10*rI.xWin/400, 10*rI.xWin/400, 5, -25, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   drawAscII("VIEW CONTROLS", 10*rI.xWin/400, 10*rI.xWin/400, 5, -45, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   drawAscII("EXIT", 10*rI.xWin/400, 10*rI.xWin/400, 5, -65, 0, 0, 0, 0, 0x0, bgColor, 1, rI);

   if (menuHighlightOption == 1)
   {
      line(-5, 5, 0, rI.xCenter, 5, 0, 0xFFFF00, rI);
      line(-5, -15, 0, rI.xCenter, -15, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 2)
   {
      line(-5, -15, 0, rI.xCenter, -15, 0, 0xFFFF00, rI);
      line(-5, -35, 0, rI.xCenter, -35, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 3)
   {
      line(-5, -35, 0, rI.xCenter, -35, 0, 0xFFFF00, rI);
      line(-5, -55, 0, rI.xCenter, -55, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 4)
   {
      line(-5, -55, 0, rI.xCenter, -55, 0, 0xFFFF00, rI);
      line(-5, -75, 0, rI.xCenter, -75, 0, 0xFFFF00, rI);
   }
}

if (viewControls)
{
   drawAscII("MOVE = ARROW KEYS", 10*rI.xWin/400, 10*rI.xWin/400,
             15-rI.xCenter, -15+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII("JUMP = SPACE BAR", 10*rI.xWin/400, 10*rI.xWin/400,
             15-rI.xCenter, -35+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII("ZOOM CAMERA = PAGEUP,PAGEDOWN", 10*rI.xWin/400, 10*rI.xWin/400,
             15-rI.xCenter, -55+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII("BACK TO MENU", 10*rI.xWin/400, 10*rI.xWin/400,
             15-rI.xCenter, -95+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   if (menuHighlightOption == 1)
   {
      line(-rI.xCenter, rI.yCenter-85, 0, 0, rI.yCenter-85, 0, 0xFFFF00, rI);
      line(-rI.xCenter, rI.yCenter-105, 0, 0, rI.yCenter-105, 0, 0xFFFF00, rI);
   }
}

if (viewOptions)
{
   drawAscII(optFullScreenTxt, 6.5, 10,
             15-rI.xCenter, -15+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII(optKeepAspectRatioTxt, 6.5, 10,
             15-rI.xCenter, -35+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII(optDisplayScrnResTxt, 6.5, 10,
             15-rI.xCenter, -55+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII(optWindowSizeTxt, 6.5, 10,
             15-rI.xCenter, -75+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII(optAntialiasingTxt, 6.5, 10,
             15-rI.xCenter, -95+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   drawAscII("BACK TO MENU", 6.5, 10,
             15-rI.xCenter, -135+rI.yCenter, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
   
   if (menuHighlightOption == 1)
   {
      line(-rI.xCenter+80, rI.yCenter-5, 0, -rI.xCenter+140, rI.yCenter-5, 0, 0xFFFF00, rI);
      line(-rI.xCenter+80, rI.yCenter-25, 0, -rI.xCenter+140, rI.yCenter-25, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 2)
   {
      line(-rI.xCenter+140, rI.yCenter-25, 0, -rI.xCenter+200, rI.yCenter-25, 0, 0xFFFF00, rI);
      line(-rI.xCenter+140, rI.yCenter-45, 0, -rI.xCenter+200, rI.yCenter-45, 0, 0xFFFF00, rI);
   }
   
   if (menuHighlightOption == 3)
   {
      line(-rI.xCenter+70, rI.yCenter-45, 0, -rI.xCenter+100, rI.yCenter-45, 0, 0xFFFF00, rI);
      line(-rI.xCenter+70, rI.yCenter-65, 0, -rI.xCenter+100, rI.yCenter-65, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 4)
   {
      line(-rI.xCenter+140, rI.yCenter-45, 0, -rI.xCenter+170, rI.yCenter-45, 0, 0xFFFF00, rI);
      line(-rI.xCenter+140, rI.yCenter-65, 0, -rI.xCenter+170, rI.yCenter-65, 0, 0xFFFF00, rI);
   }
   
   if (menuHighlightOption == 5)
   {
      line(-rI.xCenter+50, rI.yCenter-65, 0, -rI.xCenter+80, rI.yCenter-65, 0, 0xFFFF00, rI);
      line(-rI.xCenter+50, rI.yCenter-85, 0, -rI.xCenter+80, rI.yCenter-85, 0, 0xFFFF00, rI);
   }
   if (menuHighlightOption == 6)
   {
      line(-rI.xCenter+120, rI.yCenter-65, 0, -rI.xCenter+150, rI.yCenter-65, 0, 0xFFFF00, rI);
      line(-rI.xCenter+120, rI.yCenter-85, 0, -rI.xCenter+150, rI.yCenter-85, 0, 0xFFFF00, rI);
   }
   
   if (menuHighlightOption == 7)
   {
      line(-rI.xCenter+90, rI.yCenter-85, 0, -rI.xCenter+150, rI.yCenter-85, 0, 0xFFFF00, rI);
      line(-rI.xCenter+90, rI.yCenter-105, 0, -rI.xCenter+150, rI.yCenter-105, 0, 0xFFFF00, rI);
   }
   
   if (menuHighlightOption == 8)
   {
      line(-rI.xCenter, rI.yCenter-125, 0, 0, rI.yCenter-125, 0, 0xFFFF00, rI);
      line(-rI.xCenter, rI.yCenter-145, 0, 0, rI.yCenter-145, 0, 0xFFFF00, rI);
   }
}
