static int initializeDrawScene1 = 1;

// GAME LOGIC //

if (initializeDrawScene1)
{
   // rotate character forwards
   globalRot(character_GLOBAL, character_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
   globalRot(run1_GLOBAL, run1_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
   globalRot(run2_GLOBAL, run2_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
   globalRot(run3_GLOBAL, run3_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
   globalRot(run4_GLOBAL, run4_TRANS, character_MESHCNT, 0, 0, 0, 0, 0, M_PI);
   
   // copy the mesh data so you will always have the original data
   for (i = 0; i < 9; i++)
   character_STATICGLOBAL[i] = character_GLOBAL[i];
   for (i = 0; i < sizeof(character_TRANS)/sizeof(float); i++)
   character_STATICTRANS[i] = character_TRANS[i];
   for (i = 0; i < sizeof(character_POINT)/sizeof(float); i++)
   character_STATICPOINT[i] = character_POINT[i];
   
   for (i = 0; i < 9; i++)
   shadow_STATICGLOBAL[i] = shadow_GLOBAL[i];
   for (i = 0; i < sizeof(shadow_TRANS)/sizeof(float); i++)
   shadow_STATICTRANS[i] = shadow_TRANS[i];
   for (i = 0; i < sizeof(shadow_POINT)/sizeof(float); i++)
   shadow_STATICPOINT[i] = shadow_POINT[i];
   
   // resize the dome
   globalResize(dome1_GLOBAL, dome1_TRANS, dome1_MESHCNT, 0, 0, 0, 1000, 1000, 100);
   
   // resize the course
   globalResize(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, 0, 20, 20, 20);
   
   // copy the mesh data so you will always have the original data
   for (i = 0; i < 9; i++)
   dome1_STATICGLOBAL[i] = dome1_GLOBAL[i];
   for (i = 0; i < sizeof(dome1_TRANS)/sizeof(float); i++)
   dome1_STATICTRANS[i] = dome1_TRANS[i];
   for (i = 0; i < sizeof(dome1_POINT)/sizeof(float); i++)
   dome1_STATICPOINT[i] = dome1_POINT[i];
   
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
dome1_GLOBAL[i] = dome1_STATICGLOBAL[i];
for (i = 0; i < sizeof(dome1_TRANS)/sizeof(float); i++)
dome1_TRANS[i] = dome1_STATICTRANS[i];

for (i = 0; i < 9; i++)
course1_GLOBAL[i] = course1_STATICGLOBAL[i];
for (i = 0; i < sizeof(course1_TRANS)/sizeof(float); i++)
course1_TRANS[i] = course1_STATICTRANS[i];

for (i = 0; i < 9; i++)
character_GLOBAL[i] = character_STATICGLOBAL[i];
for (i = 0; i < sizeof(character_TRANS)/sizeof(float); i++)
character_TRANS[i] = character_STATICTRANS[i];

for (i = 0; i < 9; i++)
shadow_GLOBAL[i] = shadow_STATICGLOBAL[i];
for (i = 0; i < sizeof(shadow_TRANS)/sizeof(float); i++)
shadow_TRANS[i] = shadow_STATICTRANS[i];


// apply the correct orientations

// put the moving platform in the right position
course1_TRANS[9*9+3] = course1_TRANS[9*9+3] + movingPlatform;

// rotate the sky and apply the correct rotation to the dome
zDomeRotSave = fmod(zDomeRotSave + 0.002, M_PI * 2);
dome1_TRANS[2+9] = zDomeRotSave;
globalRot(dome1_GLOBAL, dome1_TRANS, dome1_MESHCNT, 0, 0, 0,
          0, 0, zCourseRotSave+turnLeft+turnRight);

// rotate the shadow with the course
globalRot(shadow_GLOBAL, shadow_TRANS, shadow_MESHCNT, 0, 0, 0,
          0, 0, zCourseRotSave+turnLeft+turnRight);


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

// animate the character as it moves
runCharacter = 0;

if (forward+backward)
{
   runCycle = runCycle + 0.5;
   runCharacter = 1;
   
   if (turnLeft+turnRight)
   {
      runCycle = runCycle + 0.15;
      runCharacter = 1;
   }
}

// jump with legs apart
if ((allowFlyUp) || (timeToFall))
{
   for (i = 0; i < character_MESHCNT*9; i++)
   character_TRANS[i] = run1_TRANS[i];
   runCharacter = 0;
}

// run animation
if (runCharacter)
{
   if (runCycle >= 4)
   runCycle = (int)runCycle%4 + (runCycle - (int)runCycle);
   btwnFrames = runCycle - (int)runCycle;
   
   if ((int)runCycle == 0)
   for (i = 0; i < character_MESHCNT*9; i++)
   character_TRANS[i] = (run2_TRANS[i] - run1_TRANS[i]) * btwnFrames + run1_TRANS[i];
   if ((int)runCycle == 1)
   for (i = 0; i < character_MESHCNT*9; i++)
   character_TRANS[i] = (run3_TRANS[i] - run2_TRANS[i]) * btwnFrames + run2_TRANS[i];
   if ((int)runCycle == 2)
   for (i = 0; i < character_MESHCNT*9; i++)
   character_TRANS[i] = (run4_TRANS[i] - run3_TRANS[i]) * btwnFrames + run3_TRANS[i];
   if ((int)runCycle == 3)
   for (i = 0; i < character_MESHCNT*9; i++)
   character_TRANS[i] = (run1_TRANS[i] - run4_TRANS[i]) * btwnFrames + run4_TRANS[i];
}

// turn the character toward the correct direction
// if it moves forwards, backwards, left or right
if ((turnLeft) || (turnRight))
zCharacterRot = zCharacterRot + turnRight + turnLeft;

if ((forward) && (!backward))
{
   zCharacterRot = 0;
   
   if ((turnLeft) && (!turnRight))
   zCharacterRot = M_PI/5;
   
   if ((turnRight) && (!turnLeft))
   zCharacterRot = -M_PI/5;
}

if ((backward) && (!forward))
{
   zCharacterRot = M_PI;
   
   if ((turnLeft) && (!turnRight))
   zCharacterRot = M_PI/5 + M_PI;
   
   if ((turnRight) && (!turnLeft))
   zCharacterRot = -M_PI/5 + M_PI;
}

globalRot(character_GLOBAL, character_TRANS, character_MESHCNT,
          0, 0, 0, xCharacterRot, yCharacterRot, zCharacterRot);


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
         
         // move the camera at an angle as the character jumps
         xScreenRot = xScreenRot + 0.2;
         
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

//move the character's shadow far down out of view
shadow_TRANS[5] = -1000000;

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
      // move the character's shadow on top of the correct platform
      if (zPlatform < zCharacterPos)
      if (shadow_TRANS[5] < course1_TRANS[5+i])
      shadow_TRANS[5] = course1_TRANS[5+i];
      
      // if the character is close to the floor
      if (zCharacterPos-22.0 <= zPlatform)
      // if the character fell way past the floor because it
      // was falling very fast, then bring it up to the platform
      if (zCharacterPos+2.0 >= zPlatform-fallSpeed)
      {
         floorToChrAmt = zPlatform - (zCharacterPos-20);
         // move the course down so that the character is moved up
         globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, 0, 0, -floorToChrAmt);
         
         // move the character's shadow on this platform
         shadow_TRANS[5] = course1_TRANS[5+i];
         
         timeToFall = 0;
         if (!alreadyJumped)
         allowJump = 1;
         fallSpeed = 0;
      }
      
   }
}


// If the character falls or jumps this changes the z
// position of the course and if you have objects placed on
// the course this is the place to change the z positions
// of those objects to keep them with the course.

// keep the dome's z position with the course
dome1_TRANS[5] = course1_GLOBAL[5];
dome1_TRANS[5+9] = course1_GLOBAL[5];

// move platform back and forth
xPlatform = course1_TRANS[3+9*9] - course1_GLOBAL[3];
yPlatform = course1_TRANS[4+9*9] - course1_GLOBAL[4];
zPlatform = course1_TRANS[5+9*9] - course1_GLOBAL[5];
xEdge = course1_TRANS[6+9*9];
yEdge = course1_TRANS[7+9*9];

// When the character is on the platform make
// the character move with the platform by moving
// the entire course.
if ((xCharacterPos <= xPlatform+xEdge) &&
    (xCharacterPos >= xPlatform-xEdge) &&
    (yCharacterPos <= yPlatform+yEdge) &&
    (yCharacterPos >= yPlatform-yEdge))
if (zCharacterPos-22.0 <= zPlatform)
if (zCharacterPos+2.0 >= zPlatform-fallSpeed)
globalMove(course1_GLOBAL, course1_TRANS, course1_MESHCNT, -characterOnMovingPlatform, 0, 0);

// when the platform goes past certain points switch direction
if (course1_TRANS[9*9+3]+course1_TRANS[9*9+6] > course1_TRANS[1*9+3]+course1_TRANS[1*9+6])
platformDirection = -1;
if (course1_TRANS[9*9+3]-course1_TRANS[9*9+6] < course1_TRANS[1*9+3]-course1_TRANS[1*9+6])
platformDirection = 1;

movingPlatform = movingPlatform + 10 * platformDirection;
characterOnMovingPlatform = 10 * platformDirection;


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


// move the camera closer or further away
if (moveCloser)
{
   if (cameraDistance < 100)
   cameraDistance = cameraDistance + 100;
   moveCloser = 0;
}

if (moveAway)
{
   if (cameraDistance > -100)
   cameraDistance = cameraDistance - 100;
   moveAway = 0;
}

if (zScreenLoc < cameraDistance)
zScreenLoc = zScreenLoc + 20;

if (zScreenLoc > cameraDistance)
zScreenLoc = zScreenLoc - 20;

// angle the camera to normal position
if (xScreenRot > -1.3)
xScreenRot = xScreenRot - 0.1;

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

// draw dome
rI.camEndZ = -75000 * zScreenSize;
rI.fogEndZ = -75000 * zScreenSize;

addRotLocSz(dome1_TRANS, dome1_POINT, dome1_POINTDATSZ,
            dome1_STATICPOINT, dome1_MESHCNT);
addScreenRotLocSz(xScreenRot, yScreenRot, zScreenRot,
                  xScreenSize, yScreenSize, zScreenSize,
                  xScreenLoc, yScreenLoc, zScreenLoc,
                  dome1_POINT, dome1_POINTDATSZ, dome1_MESHCNT);
drawMesh(dome1_POINT, dome1_LINE, dome1_COLOR,
         dome1_LINEDATSZ, dome1_MESHCNT, bgColor, antialiasing, rI);

rI.camEndZ = -1200 * zScreenSize;
rI.fogEndZ = -1200 * zScreenSize;

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

// draw shadow
addRotLocSz(shadow_TRANS, shadow_POINT, shadow_POINTDATSZ,
            shadow_STATICPOINT, shadow_MESHCNT);
addScreenRotLocSz(xScreenRot, yScreenRot, zScreenRot,
                  xScreenSize, yScreenSize, zScreenSize,
                  xScreenLoc, yScreenLoc, zScreenLoc,
                  shadow_POINT, shadow_POINTDATSZ, shadow_MESHCNT);
drawMesh(shadow_POINT, shadow_LINE, shadow_COLOR,
         shadow_LINEDATSZ, shadow_MESHCNT, bgColor, antialiasing, rI);

// END OF DRAW //
