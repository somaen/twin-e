/***************************************************************************
                          fullRedraw.cpp  -  description
                             -------------------
    begin                : Sun Apr 28 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lba.h"

void fullRedraw(int param)
{
    short int temp1;
    short int temp2;

   // short int temp4;
    int counter;
    int counter2;

   // int i;

    int arg_1A;
    int a12;
    int arg_46;
    int arg_42;
    int a0E;
    int arg_1E;
    actor *lactor;
    int temp3;
    int spriteWidth;
    int spriteHeight;

	temp1 = fullRedrawVar1;
    temp2 = fullRedrawVar2;

    UnSetClip();

    if (!param)
	{
	    blitBackgroundOnDirtyBoxes();	// blit background on dirty boxes
	}
    else			// fully redraw the background
	{
	    freezeTime();
	    Cls();
	    redrawCube(); // draw cube background to frontVideoBuffer
	    updateOverlayObjectsPosition(temp1, temp2, fullRedrawVar1, fullRedrawVar2);
	    CopyScreen(frontVideoBuffer, workVideoBuffer);
	}

    arg_1A = 0;
    a12 = 0;
    arg_46 = arg_1A + 0x1000;
    arg_42 = arg_1A + 0x0C00;

   // loop1

    for (arg_1A = 0; arg_1A < numActorInRoom; arg_1A++, arg_46++, arg_42++)	// process actors (and draw shadow if applicable)
	{
	    lactor = &actors[arg_1A];
	    lactor->dynamicFlagsMask &= 0xFFEF;	// recheck -> reinit the draw flags for the current objects

	    if ((useAnotherGrm == -1) || (lactor->Z <= READ_LE_S16((currentGrid2) * 24 + (char*)zoneData + 8))) // eject characters hidden when using another GRM
		{
            if ( lactor->staticFlagsBF.bIsBackgrounded && param == 0 )	// background actor, no redraw required
			{
			   // calculate the actor position on screen
			    projectPositionOnScreen(lactor->X - cameraX, lactor->Z - cameraZ,lactor->Y - cameraY);

			   // is the actor in the viewable screen ?
			    if (projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580)
				{
				    lactor->dynamicFlagsMask |= 0x10; // flag ?
				}
			}
		    else
			{
			    if (lactor->costumeIndex != -1	&& !(lactor->staticFlagsBF.bNoDisplay)) // 0x200 -> visible
				{
				   // calculate the actor position on screen
				    projectPositionOnScreen(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);

                    if (((lactor->staticFlagsBF.bIsUsingClipping) && projectedPositionX > -112 && projectedPositionX < 752 && projectedPositionY > -50 && projectedPositionY < 651)
						|| ((!(lactor->staticFlagsBF.bIsUsingClipping)) && projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580))
					{
					    temp3 = lactor->Y + lactor->X - cameraX - cameraY;

					    if (lactor->standOn != -1)	// if actor is on another actor
						{
						    temp3 = actors[lactor->standOn].X - cameraX + actors[lactor->standOn].Y - cameraY + 2;
						}

                        if (lactor->staticFlagsBF.bIsSpriteActor)
						{
						    drawList[a12].field_2 = arg_46;	// 0x1000
                            if (lactor->staticFlagsBF.bIsUsingClipping)
							{
							    temp3 =	lactor->lastX - cameraX + lactor->lastY - cameraY;
							}
						}
					    else
						{
						    drawList[a12].field_2 = arg_1A;
						}

					    drawList[a12].field_0 = temp3;

					    a12++;

                        if (shadowMode != 0 && !(lactor->staticFlagsBF.bDoesntCastShadow))	// 0x1000 -> no shadow cast
						{
                            if(lactor->standOn!=-1) // quick shadow calc if on another actor
						    {
							    shadowX = lactor->X;
							    shadowZ = lactor->Z - 1;
							    shadowY = lactor->Y;
						    }
						    else
						    {
						        GetShadow(lactor->X,lactor->Z,lactor->Y);
						    }

						    temp3--;
						    drawList[a12].field_0 = temp3;	// save the shadow entry in the drawList
						    drawList[a12].field_2 = arg_42;	// 0xC00
						    drawList[a12].X = shadowX;
						    drawList[a12].Z = shadowZ;
						    drawList[a12].Y = shadowY;
						    drawList[a12].field_A = 2;
						    a12++;

						}

					    if (drawInGameTransBox)
						{
						    if (currentlyFollowedActor != arg_1A)
							{
							   /*
							    * arg_16=projectedPositionX; arg_12=projectedPositionY;
							    */
							}
						}
					}
				}
			}
		}
	}

    arg_1A = 0;
    counter = arg_1A + 0x1800;
    counter2 = 0;

   // loop2

	
    do				// process extras (like key, hearts,...)
	{
	    if (extraList[counter2].field_0 != -1)
		{
		    if (extraList[counter2].field_14 & 0x400)
			{
			    if (time - extraList[counter2].time > 35)
				{
				    extraList[counter2].time = time;
				    extraList[counter2].field_14 &= 0xFBFF;
				    HQ_3D_MixSample(11, 0x1000, 1, extraList[counter2].X, extraList[counter2].Z, extraList[counter2].Y);
				}
			}
		    else
			{
			    if ((extraList[counter2].field_14 & 1) || (extraList[counter2].field_14 & 0x40) || (extraList[counter2].field_1C + extraList[counter2].time - 150 < time) || (!((time + extraList[counter2].time) & 8)))
				{
				    projectPositionOnScreen(extraList[counter2].X - cameraX, extraList[counter2].Z - cameraZ, extraList[counter2].Y - cameraY);

				    if (projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580)
					{
					    drawList[a12].field_0 = extraList[counter2].X - cameraX + extraList[counter2].Y - cameraY;
					    drawList[a12].field_2 = counter;
					    a12++;

					    if (shadowMode == 2 && extraList[counter2].field_0 & 0x8000)	// cast shadow
						{
						    GetShadow(extraList[counter2].X,extraList[counter2].Z,extraList[counter2].Y);
						    drawList[a12].field_0 = extraList[counter2].X - cameraX + extraList[counter2].Y - cameraY - 1;
						    drawList[a12].field_2 = 0xC00;
						    drawList[a12].X = shadowX;
						    drawList[a12].Z = shadowZ;
						    drawList[a12].Y = shadowY;

						    a12++;
						}
					}
				}
			}
		}
	    arg_1A++;
	    counter++;
	    counter2++;
	}
    while (arg_1A < 50);

    SmallSort(drawList,a12,12); 

   
  /*  if(twinsen->costumeIndex != -1 && !(twinsen->staticFlagsBF.bNoDisplay))
    {
        int arg_32;
        int arg_36;
        int arg_52;
        int arg_4A;
        int arg_1A;
        int result;
        int result2;

        arg_32=twinsen->X+twinsen->boudingBox.X.bottomLeft;
        arg_36=twinsen->Y+twinsen->boudingBox.Z.bottomLeft;
        arg_52=twinsen->X+twinsen->boudingBox.X.topRight;
        arg_4A=twinsen->Y+twinsen->boudingBox.Z.topRight;
        arg_1A=0;
        result=-1;

      //loop3 
        while(arg_1A<a12)
        {
            if(drawList[arg_1A].field_2 == 0)
            {
                result=arg_1A;
                result2=drawList[arg_1A].field_0;
                break;
            }
            arg_1A++;
        }

        if(result!=-1)
        {
            printf("Special draw in fullRedraw!\n");
            exit(1);
            arg_1A=0; // loop 4
            while(arg_1A<a12)
            {
            }
        }

    } */
    

    a0E = 0;
    fullRedrawVar8 = 0;
    arg_1A = 0;
    arg_1E = 0;

   // loop6 -> final drawing code
    if (a12 > 0)
	{
	    unsigned int flags;
	    int actorNumber;
	    int positionInDebugBox = 0;

	    do
		{
		    actorNumber = drawList[arg_1E].field_2 & 0x3FF;
		    lactor = &actors[actorNumber];
		    flags = ((unsigned int) drawList[arg_1E].field_2) & 0xFC00;

		    if (flags < 0xC00)	// actor
			{
			    if (!flags)
				{
				    if (!actorNumber)
					{
					}

					SetInterAnimObjet2(lactor->animPosition,(char*)HQR_Get(HQR_Anims,lactor->previousAnimIndex),(char*)bodyPtrTab[lactor->costumeIndex]);

					if(_debugger.bShowBoundingBoxes)
						MDL_DrawBoundingBoxHiddenPart(lactor);
				    if (!AffObjetIso(lactor->X - cameraX, lactor->Z - cameraZ,lactor->Y - cameraY, 0, lactor->angle, 0,bodyPtrTab[lactor->costumeIndex]))
					{
						if(_debugger.bShowBoundingBoxes)  
							MDL_DrawBoundingBoxShownPart(lactor);

					    SetClip(renderLeft, renderTop, renderRight,renderBottom);

					    if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom)
						{
						    int tempX;
						    int tempZ;
						    int tempY;

						    lactor->dynamicFlagsMask |= 0x10;

						    tempX = (lactor->X + 0x100 )>> 9;
						    tempZ = lactor->Z >> 8;

						    if (lactor->field_3 & 0x7F)
								tempZ++;

						    tempY = (lactor->Y + 0x100) >> 9;

						    DrawOverBrick(tempX, tempZ, tempY);

						    AddPhysBox(textWindowLeft,textWindowTop,renderRight, renderBottom);

						    if ((lactor->staticFlagsMask & 0x2000) && param == 1) // the actor is in background. Copy it to the back buffer
							{
							    blitRectangle(textWindowLeft, textWindowTop, textWindowRight, textWindowBottom, (char *) frontVideoBuffer, textWindowLeft, textWindowTop, (char *) workVideoBuffer);
							}
						}
					}

				    _debugger.actorBox[positionInDebugBox].actorNum = actorNumber;
				    _debugger.actorBox[positionInDebugBox].left = renderLeft;
				    _debugger.actorBox[positionInDebugBox].right = renderRight;
				    _debugger.actorBox[positionInDebugBox].top = renderTop;
				    _debugger.actorBox[positionInDebugBox].bottom = renderBottom;

				    positionInDebugBox++;

					if(_debugger.bShowActorNumbers)
				    {
						char stringTemp[256];

						sprintf(stringTemp, "%d", actorNumber);
						CoulFont(57);

						projectPositionOnScreen(lactor->X - cameraX, lactor->Z - cameraZ,
								lactor->Y - cameraY);

						if (projectedPositionX > 40 && projectedPositionX < 600
							&& projectedPositionY > 40 && projectedPositionY < 440)
							Font(projectedPositionX, projectedPositionY,
									stringTemp);

						AddPhysBox(projectedPositionX, projectedPositionY, projectedPositionX + 50, projectedPositionY + 50);
				    }

				}
			}
		    else if (flags == 0xC00)	// shadows
			{
			   
			     if(!(drawList[arg_1E].field_2&0x3FF))
				 {
				//	arg_0E=1;
				 }
			     
			     projectPositionOnScreen(drawList[arg_1E].X-cameraX,drawList[arg_1E].Z-cameraZ,drawList[arg_1E].Y-cameraY);
                 GetDxDyGraph(drawList[arg_1E].field_A,&spriteWidth,&spriteHeight,(char*)shadowSprite);

			     renderLeft=projectedPositionX-(spriteWidth - drawList[arg_1E].field_2&0x3FF)/2; 			     
			     renderTop=projectedPositionY-(spriteHeight - drawList[arg_1E].field_2&0x3FF)/2;

				 renderBottom=projectedPositionY+(spriteHeight - drawList[arg_1E].field_2&0x3FF)/2;
			     renderRight=projectedPositionX+(spriteWidth - drawList[arg_1E].field_2&0x3FF)/2;

			     SetClip(renderLeft,renderTop,renderRight,renderBottom);
			     
			     if(textWindowLeft<=textWindowRight && textWindowTop<=textWindowBottom)
				 {
					AffGraph(drawList[arg_1E].field_A,renderLeft,renderTop,shadowSprite);
				 }

				 DrawOverBrick(drawList[arg_1E].X, drawList[arg_1E].Z, drawList[arg_1E].Y);

				 AddPhysBox(textWindowLeft,textWindowTop,renderRight, renderBottom);
			    
			}
		    else if (flags < 0x1000)
			{
			    printf("Draw actor <0x1000 %d\n",
				   drawList[arg_1E].field_2 & 0x3FF);
			}
		    else if (flags == 0x1000)	// sprite actor
			{
			    projectPositionOnScreen(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);
			    GetDxDyGraph(0, &spriteWidth, &spriteHeight,(char *) HQR_Get(HQR_Sprites,lactor->costumeIndex));

			    renderLeft = projectedPositionX + *(short int *) (spriteActorData + lactor->costumeIndex * 16);	// calculate center
			    renderTop = projectedPositionY + *(short int *) (spriteActorData + lactor->costumeIndex * 16 + 2);

			    renderRight = renderLeft + spriteWidth;
			    renderBottom = renderTop + spriteHeight;

			    if (lactor->staticFlagsBF.bIsUsingClipping)
				{
					// if sprite actor use croping
				    SetClip(fullRedrawVar1 + lactor->cropLeft, fullRedrawVar2 + lactor->cropTop, fullRedrawVar1 + lactor->cropRight, fullRedrawVar2 + lactor->cropBottom);
				}
			    else
				{
				    SetClip(renderLeft, renderTop, renderRight, renderBottom);
				}

			    if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom)
				{
				    AffGraph(0, renderLeft, renderTop, HQR_Get(HQR_Sprites, lactor->costumeIndex));
					if(_debugger.bShowBoundingBoxes)
					{
						MDL_DrawBoundingBoxHiddenPart(lactor);
						MDL_DrawBoundingBoxShownPart(lactor);
					}

				    lactor->dynamicFlagsMask |= 0x10;

				    if (lactor->staticFlagsMask & 8)
					{
					    DrawOverBrick3((lactor->lastX+0x100)>> 9,lactor->lastZ >> 8,(lactor->lastY+0x100) >> 9);
					}
				    else
					{
					    int tempX;
					    int tempZ;
					    int tempY;

					    tempX = (lactor->X + lactor->boudingBox.X.topRight +0x100) >> 9;
					    tempZ = lactor->Z >> 8;
					    if (lactor->field_3 & 0x7F)
						tempZ++;
					    tempY = (lactor->Y + lactor->boudingBox.Z.topRight +0x100) >> 9;

					    DrawOverBrick3(tempX, tempZ, tempY);
					}

				    AddPhysBox(textWindowLeft, textWindowTop,textWindowRight, textWindowBottom);

				    if (lactor->staticFlagsMask & 0x2000 && param == 1)
					{
					    blitRectangle(textWindowLeft, textWindowTop,textWindowRight, textWindowBottom,(char *) frontVideoBuffer, textWindowLeft,textWindowTop, (char *) workVideoBuffer);
					}
				}

				{ // code to add the sprite actor box to the debugger handler
					_debugger.actorBox[positionInDebugBox].actorNum = actorNumber;
					_debugger.actorBox[positionInDebugBox].left = textWindowLeft;
					_debugger.actorBox[positionInDebugBox].right = textWindowRight;
					_debugger.actorBox[positionInDebugBox].top = textWindowTop;
					_debugger.actorBox[positionInDebugBox].bottom = textWindowBottom;

					positionInDebugBox++;
				}

				if(_debugger.bShowActorNumbers)
				{ // print the actor number
					char stringTemp[256];

					sprintf(stringTemp, "%d", actorNumber);
					CoulFont(157);

					projectPositionOnScreen(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);

					if (projectedPositionX > 40 && projectedPositionX < 600 && projectedPositionY > 40 && projectedPositionY < 440)
						Font(projectedPositionX, projectedPositionY, stringTemp);

					AddPhysBox(projectedPositionX, projectedPositionY, projectedPositionX + 50, projectedPositionY + 50);
				}

			}
		    else if (flags == 0x1800) // extras
			{
				int extraFlag;
			    extraListStruct* extraEntry = &extraList[actorNumber];

				projectPositionOnScreen(extraEntry->X-cameraX,extraEntry->Z-cameraZ,extraEntry->Y-cameraY);

				extraFlag = extraEntry->field_0;

				if(extraFlag & 0x8000)
				{
					AffSpecial(actorNumber,projectedPositionX,projectedPositionY);
				}
				else
				{
                    GetDxDyGraph(0,&spriteWidth,&spriteHeight,(char*)HQR_Get(HQR_Sprites,extraEntry->field_0));

                    renderLeft = (*(short int*)(((extraEntry->field_0*8)*2)+spriteActorData))+projectedPositionX;
                    renderTop = (*(short int*)(((extraEntry->field_0*8)*2)+spriteActorData)+2)+projectedPositionY;

                    renderRight = renderLeft + spriteWidth;
                    renderBottom = renderTop + spriteHeight;

                    AffGraph(0,renderLeft,renderTop,HQR_Get(HQR_Sprites,extraEntry->field_0));

				}

				SetClip(renderLeft,renderTop,renderRight,renderBottom);

				if((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom))
				{
					DrawOverBrick(drawList[arg_1E].X, drawList[arg_1E].Z, drawList[arg_1E].Y);
					AddPhysBox(textWindowLeft,textWindowTop,renderRight, renderBottom);
				}
			}

		    arg_1A++;
		    UnSetClip();
		    arg_1E++;
		}
	    while (arg_1A < a12);

	    _debugger.numOfActorOnScreen = positionInDebugBox;

	   // unknownLoop

	   // loop5
	}

#ifdef PCLIKE
    ZONE_DrawZones();

	if(_debugger.bShowFlags)
	{
		for(arg_1A = 0; arg_1A < numFlags; arg_1A++)	// affichage des flags 
		{
			char stringTemp[256];

			AffObjetIso(flagData[arg_1A].x - cameraX, flagData[arg_1A].z - cameraZ, flagData[arg_1A].y - cameraY, 0, 0, 0, flagModelPtr);

			sprintf(stringTemp, "%d", arg_1A);
			CoulFont(255);

			projectPositionOnScreen(flagData[arg_1A].x - cameraX, flagData[arg_1A].z - cameraZ, flagData[arg_1A].y - cameraY);

			if (projectedPositionX > 40 && projectedPositionX < 600 && projectedPositionY > 40 && projectedPositionY < 440)
				Font(projectedPositionX, projectedPositionY, stringTemp);
		}
	}
#endif

    counter2 = 0;

   // loop7
    do
	{
	    if (overlayObjectList[counter2].field_0 != -1)
		{
		   // another process
		}
	    arg_1A++;
	    counter2++;
	}
    while (arg_1A < 10);

    UnSetClip();

   // if(waitVSync!=0)
   // waitRetrace();

    if (drawInGameTransBox != 0)
	{
	   // fullRedrawSub10(arg_16,arg_12,arg_E,arg_A);
	    fullRedrawSub5();
	    if (param != 0)
		unfreezeTime();
	}
    else
	{
	    if (param == 0)
		{
		    FlipBoxes();	// update dirtyBox to screen and move all the nextDirtyBox to dirtyBox
		}
	    else
		{
            if(!lockPalette)
			    osystem->Flip(frontVideoBuffer);
		    fullRedrawSub5();	// move all the nextDirtyBox to dirtyBox
		    unfreezeTime();
		}
	}

	osystem->updateImage();

    if (!lockPalette)
	return;

    Load_HQR("ress.hqr", (byte *) & palette, 0);
    convertPalToRGBA(palette, paletteRGBA);

   /*
    * if(useAlternatePalette) FadeToPal((char*)menuPalRGBA); else FadeToPal((char*)paletteRGBA); 
    */

    if (useAlternatePalette)
	{
	    osystem->crossFade((char *) frontVideoBuffer, (char *) menuPalRGBA);
	}
    else
	{
	    osystem->crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
	}

    osystem->setPalette((byte *) & paletteRGBA);

    readKeyboard();

    lockPalette = 0;

	osystem->updateImage();

    return;
}

void updateOverlayObjectsPosition(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
    int i;
    short int var1;
    short int var2;

    var1 = arg_8 - arg_0;
    var2 = arg_C - arg_4;

    for (i = 0; i < 10; i++)
	{
	    if (overlayObjectList[i].field_6 == 1)
		{
		    overlayObjectList[i].field_2 = var1;
		    overlayObjectList[i].field_4 = var2;
		}
	}
}

void blitBackgroundOnDirtyBoxes(void)
{
    int i;
    currentDirtyBoxListStruct* currentBox;

    currentBox = currentDirtyBoxList;

    for (i = 0; i < numOfRedrawBox; i++)
	{
	    blitRectangle(currentBox->left, currentBox->top, currentBox->right, currentBox->bottom,
			 (char *) workVideoBuffer, currentBox->left, currentBox->top, (char *) frontVideoBuffer);
        currentBox++;
	}
}

void fullRedrawSub5(void)
{
    int i;

    numOfRedrawBox = 0;

    for (i = 0; i < fullRedrawVar8; i++)
	{
	    addToRedrawBox(nextDirtyBoxList[i].left, nextDirtyBoxList[i].top,
			   nextDirtyBoxList[i].right, nextDirtyBoxList[i].bottom);
	}
}

void addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
    int var1;
    int i = 0;
    int var_C;
    int var_8;
    int var_10;
    int var_4;

    var1 = (arg_8 - arg_0) * (arg_C - arg_4);

    while (i < numOfRedrawBox)
	{
	    if (currentDirtyBoxList[i].left >= arg_0)
		var_C = arg_0;
	    else
		var_C = currentDirtyBoxList[i].left;

	    if (currentDirtyBoxList[i].right <= arg_8)
		var_8 = arg_8;
	    else
		var_8 = currentDirtyBoxList[i].right;

	    if (currentDirtyBoxList[i].top >= arg_4)
		var_10 = arg_4;
	    else
		var_10 = currentDirtyBoxList[i].top;

	    if (currentDirtyBoxList[i].bottom <= arg_C)
		var_4 = arg_C;
	    else
		var_4 = currentDirtyBoxList[i].bottom;

	    if ((var_8 - var_C) * (var_4 - var_10) <
		((currentDirtyBoxList[i].bottom -
		  currentDirtyBoxList[i].top) * (currentDirtyBoxList[i].right -
						currentDirtyBoxList[i].left) + var1))
		{
		    currentDirtyBoxList[i].left = var_C;
		    currentDirtyBoxList[i].top = var_10;
		    currentDirtyBoxList[i].right = var_8;
		    currentDirtyBoxList[i].bottom = var_4;

            assert(currentDirtyBoxList[i].bottom < 480);
		    return;
		}

	    i++;
	};

    currentDirtyBoxList[i].left = arg_0;
    currentDirtyBoxList[i].top = arg_4;
    currentDirtyBoxList[i].right = arg_8;
    currentDirtyBoxList[i].bottom = arg_C;

    assert(currentDirtyBoxList[i].bottom < 480);

    numOfRedrawBox++;
}

struct cubeEntry
{
    unsigned char brickType1;
    unsigned char brickType2;
};
typedef cubeEntry cubeType[64][64][25];

void redrawCube(void)
{

    int i;
    int x, y, z;
    unsigned char *localBufCube = bufCube;
    unsigned char val;
    cubeType* cube = (cubeType*)bufCube;

    cameraX = newCameraX << 9;
    cameraZ = newCameraZ << 8;
    cameraY = newCameraY << 9;

    projectPositionOnScreen(-cameraX, -cameraZ, -cameraY);

    fullRedrawVar1 = projectedPositionX;
    fullRedrawVar2 = projectedPositionY;

    for (i = 0; i < 28; i++)
	{
	    zbufferTab[i] = 0;
	}

    if (changeRoomVar10 == 0)
	    return;

    if( _debugger.cubeClipEnabled )
    {
        for (z= 0; z < _debugger.cubeClipZ; z++)
        {
            for (x = 0; x < _debugger.cubeClipX; x++)
		    {
                for (y = 0; y < _debugger.cubeClipY; y++)
			    {
                    val = (*cube)[z][x][y].brickType1;
                    if(val)
                    {
                        zbuffer(val-1, (*cube)[z][x][y].brickType2,x,y,z);
                    }
			    }
		    }
        }
    }
    else
    {
        for (z = 0; z < 64; z++)
	    {
	        for (x = 0; x < 64; x++)
		    {
		        for (y = 0; y < 25; y++)
			    {
                    val = (*cube)[z][x][y].brickType1;
                    if(val)
                    {
                        zbuffer(val-1, (*cube)[z][x][y].brickType2,x,y,z);
                    }
			    }
		    }
	    }
    }
}

void zbuffer(int var1, int var2, int x, int z, int y)
{
    unsigned char *ptr;
    unsigned short int bx;
    int zbufferIndex;
    zbufferDataStruct *currentZbufferData;

    ptr = zbufferSub1(var1) + 3 + var2 * 4 + 2;

    bx = READ_LE_U16(ptr);

    if (!bx)
	return;

    zbufferSub2(x - newCameraX, z - newCameraZ, y - newCameraY);

    if (zbufferVar1 < -24)
	return;
    if (zbufferVar1 >= 640)
	return;
    if (zbufferVar2 < -38)
	return;
    if (zbufferVar2 >= 480)
	return;

	// draw the background brick
    AffGraph(bx-1, zbufferVar1, zbufferVar2, bufferBrick);

    zbufferIndex = (zbufferVar1 + 24) / 24;

    if (zbufferTab[zbufferIndex] >= 150)
	{
	    printf("Arg MAX_BRICK Z BUFFER atteint\n");
	    exit(1);
	}

    currentZbufferData = &zbufferData[zbufferIndex][zbufferTab[zbufferIndex]];

    currentZbufferData->y = y;
    currentZbufferData->z = z;
    currentZbufferData->x = x;
	currentZbufferData->drawX = zbufferVar1;
    currentZbufferData->drawY = zbufferVar2;
    currentZbufferData->spriteNum = bx - 1;

    zbufferTab[zbufferIndex]++;
}

unsigned char *zbufferSub1(int var)
{
    return (currentBll + *(unsigned int *) (currentBll + 4 * var));
}

void zbufferSub2(int x, int z, int y)
{
    zbufferVar1 = ((x - y) * 16) + (x - y) * 8 + 288;	// x pos
    zbufferVar2 = (((x + y) * 8 + (x + y) * 4) - ((z * 16) - z)) + 215;	// y pos
}

void AffGraph(int num, int var1, int var2, unsigned char *localBufferBrick)
{
    unsigned char *ptr;
    int top;
    int bottom;
    int left;
    int right;
    unsigned char *outPtr;
    unsigned char *outPtr2;
    int offset;
    int c1;
    int c2;
    int vc3;

    int temp;
    int iteration;
    int i;

    assert( textWindowLeft >= 0 );
    assert( textWindowRight <= 639 );
    assert( textWindowTop >= 0 );
    assert( textWindowBottom <= 479 );

    ptr = localBufferBrick + READ_LE_U32(localBufferBrick + num * 4);

    left = var1 + *(ptr + 2);
    top = var2 + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    ptr += 4;

    if (left >= textWindowLeft && top >= textWindowTop && right <= textWindowRight
	&& bottom <= textWindowBottom)
	{			// no crop
	    right++;
	    bottom++;

	    outPtr = frontVideoBuffer + screenLockupTable[top] + left;

	    offset = -((right - left) - largeurEcran);

	    for (c1 = 0; c1 < bottom - top; c1++)
		{
		    vc3 = *(ptr++);
		    for (c2 = 0; c2 < vc3; c2++)
			{
			    temp = *(ptr++);
			    iteration = temp & 0x3F;
			    if (temp & 0xC0)
				{
				    iteration++;
				    if (!(temp & 0x40))
					{
					    temp = *(ptr++);
					    for (i = 0; i < iteration; i++)
						*(outPtr++) = temp;
					}
				    else
					{
					    for (i = 0; i < iteration; i++)
						*(outPtr++) = *(ptr++);
					}
				}
			    else
				{
				    outPtr += iteration + 1;
				}
			}
		    outPtr += offset;
		}
	}
    else
	{
	    if (left <= textWindowRight && top <= textWindowBottom && right >= textWindowLeft
		&& bottom >= textWindowTop)
		{
		    if (top < textWindowTop)	// si il manque un bout du haut de la brique
			{

			    for (c1 = 0; c1 < (textWindowTop - top); c1++)
				{
				    vc3 = *(ptr++);
				    for (c2 = 0; c2 < vc3; c2++)
					{
					    temp = *(ptr++);
					    if (temp & 0xC0)
						{
						    iteration = 0;
						    if (temp & 0x40)
							{
							    iteration = temp & 0x3F;
							}
						    ptr += iteration + 1;
						}
					}
				}
			    top = textWindowTop;
			}

		    if (bottom > textWindowBottom)
			bottom = textWindowBottom;

		    if (left < textWindowLeft)
			cropLeft = textWindowLeft - left;
		    else
			cropLeft = 0;

		    offset = -(left + cropLeft - 1 - right);

		    right -= textWindowRight;
		    if (right > 0)
			offset -= right;

		    outPtr2 = screenLockupTable[top] + frontVideoBuffer;
		    if (left >= 0)
			outPtr2 += left;
		    outPtr = outBuffer;

		    for (c1 = 0; c1 <= (bottom - top); c1++)
			{
			    outPtr = outBuffer;

			    for (i = 0; i < 512; i++)
				outBuffer[i] = 0;

			    vc3 = *(ptr++);
			    for (c2 = 0; c2 < vc3; c2++)
				{
				    temp = *(ptr++);
				    iteration = temp & 0x3F;
				    if (temp & 0xC0)
					{
					    iteration++;
					    if (!(temp & 0x40))
						{
						    temp = *(ptr++);
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = temp;
						}
					    else
						{
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = *(ptr++);
						}
					}
				    else
					{
					    outPtr += iteration + 1;
					}
				}

			    for (i = 0; i < offset; i++)
				{
				    if (outBuffer[cropLeft + i] != 0)
					*(outPtr2) = outBuffer[cropLeft + i];
				    outPtr2++;
				}
			    outPtr2 += 640 - offset;
			}
		}
	}
}

void drawSprite2(int num, int var1, int var2, unsigned char *localBufferBrick)
{
    unsigned char *ptr;
    int top;
    int bottom;
    int left;
    int right;
    unsigned char *outPtr;
    unsigned char *outPtr2;
    int offset;
    int c1;
    int c2;
    int vc3;

    int temp;
    int iteration;
    int i;

    ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + num * 4);

    left = var1 + *(ptr + 2);
    top = var2 + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    ptr += 4;

   // check des bords

    if (left >= textWindowLeft && top >= textWindowTop && right <= textWindowRight
	&& bottom <= textWindowBottom)
	{
	    right++;
	    bottom++;

	    outPtr = workVideoBuffer + screenLockupTable[top] + left;

	    offset = -((right - left) - largeurEcran);

	    for (c1 = 0; c1 < bottom - top; c1++)
		{
		    vc3 = *(ptr++);
		    for (c2 = 0; c2 < vc3; c2++)
			{
			    temp = *(ptr++);
			    iteration = temp & 0x3F;
			    if (temp & 0xC0)
				{
				    iteration++;
				    if (!(temp & 0x40))
					{
					    temp = *(ptr++);
					    for (i = 0; i < iteration; i++)
						*(outPtr++) = temp;
					}
				    else
					{
					    for (i = 0; i < iteration; i++)
						*(outPtr++) = *(ptr++);
					}
				}
			    else
				{
				    outPtr += iteration + 1;
				}
			}
		    outPtr += offset;
		}
	}
    else
	{
	    if (left <= textWindowRight && top <= textWindowBottom && right >= textWindowLeft
		&& bottom >= textWindowTop)
		{
		    if (top < textWindowTop)	// si il manque un bout du haut de la brique
			{

			    for (c1 = 0; c1 < (textWindowTop - top); c1++)
				{
				    vc3 = *(ptr++);
				    for (c2 = 0; c2 < vc3; c2++)
					{
					    temp = *(ptr++);
					    if (temp & 0xC0)
						{
						    iteration = 0;
						    if (temp & 0x40)
							{
							    iteration = temp & 0x3F;
							}
						    ptr += iteration + 1;
						}
					}
				}
			    top = textWindowTop;
			}

		    if (bottom > textWindowBottom)
			bottom = textWindowBottom;

		    if (left < textWindowLeft)
			cropLeft = textWindowLeft - left;
		    else
			cropLeft = 0;

		    offset = -(left + cropLeft - 1 - right);

		    right -= textWindowRight;
		    if (right > 0)
			offset -= right;

		    outPtr2 = screenLockupTable[top] + workVideoBuffer;
		    if (left >= 0)
			outPtr2 += left;
		    outPtr = outBuffer;

		    for (c1 = 0; c1 <= bottom - top; c1++)
			{
			    outPtr = outBuffer;

			    for (i = 0; i < 512; i++)
				outBuffer[i] = 0;

			    vc3 = *(ptr++);
			    for (c2 = 0; c2 < vc3; c2++)
				{
				    temp = *(ptr++);
				    iteration = temp & 0x3F;
				    if (temp & 0xC0)
					{
					    iteration++;
					    if (!(temp & 0x40))
						{
						    temp = *(ptr++);
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = temp;
						}
					    else
						{
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = *(ptr++);
						}
					}
				    else
					{
					    outPtr += iteration + 1;
					}
				}

			    for (i = 0; i < offset; i++)
				{
				    if (outBuffer[cropLeft + i] != 0)
					*(outPtr2) = outBuffer[cropLeft + i];
				    outPtr2++;
				}
			    outPtr2 += 640 - offset;
			}

		}
	}
}

int projectPositionOnScreen(int coX, int coZ, int coY)
{
    if (!isUsingOrhoProjection)
	{
        coX -= setSomething3Var12;
        coZ -= setSomething3Var14;
        coY -= setSomething3Var16;

        if(coY >= 0)
        {
            int bp = coY + cameraVar1;

            if(bp <0)
                bp = 0x7FFF;

            projectedPositionX = (coX * cameraVar2)/bp + setSomethingVar1;
            projectedPositionY = (-coZ * cameraVar3)/bp + setSomethingVar2;

            return(-1);
        }
        else
        {
            projectedPositionX = 0;
            projectedPositionY = 0;
            return(0);
        }
	}
    else
	{
	    projectedPositionX = (coX - coY) * 24 / 512 + setSomethingVar1;
	    projectedPositionY = (((coX + coY) * 12) - coZ * 30) / 512 + setSomethingVar2;
	    return (-1);
	}
    return (-1);
}

int HQ_3D_MixSample(int param0, int param1, int param2, int param3, int param4, int param5)
{
    if (samplesLoaded != 0)
	{
	    printf("sampleLoaded != 0 in HQ_3D_MixSample!\n");
	   // exit(1);
	}
    return (0);
}

void GetDxDyGraph(int arg_0, int *arg_4, int *arg_8, char *ptr)
{
    ptr += READ_LE_S32(ptr + arg_0 * 4);

    *arg_4 = *ptr;
    *arg_8 = *(ptr + 1);
}

void DrawOverBrick(int X, int Z, int Y)
{
    int CopyBlockPhysLeft;
    int CopyBlockPhysRight;
    int i;
	int j;
    zbufferDataStruct *currentZbufferData;

    CopyBlockPhysLeft = ((textWindowLeft + 24) / 24 ) - 1;
    CopyBlockPhysRight = ((textWindowRight + 24) / 24);

	for( j = CopyBlockPhysLeft; j <= CopyBlockPhysRight; j++ )
	{
		for (i = 0; i < zbufferTab[j]; i++)
		{
			currentZbufferData = &zbufferData[j][i];

			if (currentZbufferData->drawY + 38 > textWindowTop && currentZbufferData->drawY <= textWindowBottom && currentZbufferData->z >= Z)
			{
				if (currentZbufferData->x + currentZbufferData->y > Y + X)
				{
					CopyMask(currentZbufferData->spriteNum,(j * 24) - 24,currentZbufferData->drawY, bufferBrick2,workVideoBuffer);
				}
			}
		}
	}
}

void DrawOverBrick3(int X, int Z, int Y)
{
    int CopyBlockPhysLeft;
    int CopyBlockPhysRight;
    int i;
    int j;
    zbufferDataStruct *currentZbufferData;

    CopyBlockPhysLeft = (textWindowLeft + 24) / 24 - 1;
    CopyBlockPhysRight = (textWindowRight + 24) / 24;

	for(j = CopyBlockPhysLeft; j <= CopyBlockPhysRight; j++)
	{
		currentZbufferData = zbufferData[j];

		for (i = 0; i < zbufferTab[j]; i++)
		{
			if (currentZbufferData->drawY + 38 > textWindowTop && currentZbufferData->drawY <= textWindowBottom && currentZbufferData->z >= Z)
			{
				if (currentZbufferData->x == Y && currentZbufferData->y == X)
				{
					CopyMask(currentZbufferData->spriteNum, (j * 24) - 24, currentZbufferData->drawY, bufferBrick2, workVideoBuffer);
				}

				if (currentZbufferData->x > Y || currentZbufferData->y > X)
				{
					CopyMask(currentZbufferData->spriteNum, (j * 24) - 24,	currentZbufferData->drawY, bufferBrick2, workVideoBuffer);
				}
			}
			currentZbufferData++;
		}

	}
}

void CopyMask(int spriteNum, int x, int y, byte * localBufferBrick, byte * buffer)
{
    unsigned char *ptr;
    int top;
    int bottom;
    int left;
    int right;
    unsigned char *outPtr;
    unsigned char *inPtr;
    unsigned char *outPtr2;
    int offset;
    int c1;
    int c2;
    int vc3;

    int temp;
    int iteration;
    int i;
    int j;


    UnSetClip();

    assert( textWindowLeft >= 0 );
    assert( textWindowRight <= 639 );
    assert( textWindowTop >= 0 );
    assert( textWindowBottom <= 479 );

    ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + spriteNum * 4);

    left = x + *(ptr + 2);
    top = y + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    ptr += 4;

    if (left >= textWindowLeft && top >= textWindowTop && right <= textWindowRight && bottom <= textWindowBottom)
    {			// no crop
	    right++;
	    bottom++;

	    outPtr = frontVideoBuffer + screenLockupTable[top] + left;
        inPtr = buffer + screenLockupTable[top] + left;

	    offset = -((right - left) - largeurEcran);

	    for (c1 = 0; c1 < bottom - top; c1++)
		{
		    vc3 = *(ptr++);
            do
			{
			    temp = *(ptr++); // skip size
                outPtr += temp;
                inPtr += temp;

                vc3--;
                if(!vc3)
                    break;
                
                temp = *(ptr++); // copy size

                for(j=0;j<temp;j++)
                {
                    *(outPtr++) = *(inPtr++);
                }
			}
            while(--vc3);
		    outPtr += offset;
            inPtr += offset;
		}
	}
    else
	{
        return;
	    if (left <= textWindowRight && top <= textWindowBottom && right >= textWindowLeft && bottom >= textWindowTop)
		{
		    if (top < textWindowTop)	// si il manque un bout du haut de la brique
			{
                top -= textWindowTop;
                top =- top;

			}

		    if (bottom > textWindowBottom)
			bottom = textWindowBottom;

		    if (left < textWindowLeft)
			cropLeft = textWindowLeft - left;
		    else
			cropLeft = 0;

		    offset = -(left + cropLeft - 1 - right);

		    right -= textWindowRight;
		    if (right > 0)
			offset -= right;

		    outPtr2 = screenLockupTable[top] + frontVideoBuffer;
		    if (left >= 0)
			outPtr2 += left;
		    outPtr = outBuffer;

		    for (c1 = 0; c1 <= (bottom - top); c1++)
			{
			    outPtr = outBuffer;

			    for (i = 0; i < 512; i++)
				outBuffer[i] = 0;

			    vc3 = *(ptr++);
			    for (c2 = 0; c2 < vc3; c2++)
				{
				    temp = *(ptr++);
				    iteration = temp & 0x3F;
				    if (temp & 0xC0)
					{
					    iteration++;
					    if (!(temp & 0x40))
						{
						    temp = *(ptr++);
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = temp;
						}
					    else
						{
						    for (i = 0; i < iteration; i++)
							*(outPtr++) = *(ptr++);
						}
					}
				    else
					{
					    outPtr += iteration + 1;
					}
				}

			    for (i = 0; i < offset; i++)
				{
				    if (outBuffer[cropLeft + i] != 0)
					*(outPtr2) = outBuffer[cropLeft + i];
				    outPtr2++;
				}
			    outPtr2 += 640 - offset;
			}
		}
	}

}

void AddPhysBox(int left, int top, int right, int bottom)
{
    if (left < 0)
	left = 0;
    if (top < 0)
	top = 0;
    if (right >= 640)
	right = 639;
    if (bottom >= 480)
	bottom = 479;

    if (left > right || top > bottom)
    	return;

   // DrawCadre (left, top, right, bottom);

    nextDirtyBoxList[fullRedrawVar8].left = left;
    nextDirtyBoxList[fullRedrawVar8].top = top;
    nextDirtyBoxList[fullRedrawVar8].right = right;
    nextDirtyBoxList[fullRedrawVar8].bottom = bottom;

    fullRedrawVar8++;

    addToRedrawBox(left, top, right, bottom);
}

//sort character list using bubble-sort
//TODO: make real use of stepSize to tell the scructure size
void SmallSort(drawListStruct *list, int listSize, int stepSize)
{
	int i;
	int j;

	drawListStruct tempStruct;

	for(i=0;i<listSize-1;i++)
	{
		for(j=0;j<listSize-1-i;j++)
		{
			if(list[j+1].field_0<list[j].field_0)
			{
				memcpy(&tempStruct,&list[j+1],sizeof(drawListStruct));
				memcpy(&list[j+1],&list[j],sizeof(drawListStruct));
				memcpy(&list[j],&tempStruct,sizeof(drawListStruct));
			}
		}
	}
}

void FlipBoxes(void)
{
    int i;

    for (i = 0; i < numOfRedrawBox; i++) //draw the dirty box on screen
	{
	    osystem->CopyBlockPhys(frontVideoBuffer, currentDirtyBoxList[i].left, currentDirtyBoxList[i].top,
			     currentDirtyBoxList[i].right, currentDirtyBoxList[i].bottom);
	}

    numOfRedrawBox = 0;

    for (i = 0; i < fullRedrawVar8; i++) //setup the dirty box for next display
	{
	    addToRedrawBox(nextDirtyBoxList[i].left, nextDirtyBoxList[i].top,
			   nextDirtyBoxList[i].right, nextDirtyBoxList[i].bottom);
	}
}
