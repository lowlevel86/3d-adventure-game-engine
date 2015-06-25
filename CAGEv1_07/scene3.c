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

drawAscII("The End", 10.0*rI.xWin/400, 10.0*rI.xWin/400,
          -3.5*10*rI.xWin/400, 5*rI.xWin/400, 0, 0, 0, 0, 0x0, bgColor, 1, rI);

drawAscII("Crailin's Adventure Game Engine", 10.0*rI.xWin/400, 10.0*rI.xWin/400,
          -150.0*rI.xWin/400, -15*rI.xWin/400, 0, 0, 0, 0, 0x0, bgColor, 1, rI);
