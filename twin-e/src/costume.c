/***************************************************************************
                          costume.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
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

void LoadFicPerso(void)
{
#ifdef PRELOAD_ALL
	file3D1 = HQR_GetCopy(HQR_Fic, 1);
#else
    HQRM_Load("file3d.hqr", 1, &file3D1);
#endif
    twinsen->entityDataPtr = file3D1;
    TCos1Init = getAnimIndexForBody(0, 0);

#ifdef PRELOAD_ALL
	file3D2 = HQR_GetCopy(HQR_Fic, 2);
#else
    HQRM_Load("file3d.hqr", 2, &file3D2);
#endif
    twinsen->entityDataPtr = file3D2;
    TCos2Init = getAnimIndexForBody(0, 0);

#ifdef PRELOAD_ALL
	file3D3 = HQR_GetCopy(HQR_Fic, 3);
#else
    HQRM_Load("file3d.hqr", 3, &file3D3);
#endif
    twinsen->entityDataPtr = file3D3;
    TCos3Init = getAnimIndexForBody(0, 0);

#ifdef PRELOAD_ALL
	file3D4 = HQR_GetCopy(HQR_Fic, 4);
#else
    HQRM_Load("file3d.hqr", 4, &file3D4);
#endif
    twinsen->entityDataPtr = file3D4;
    TCos4Init = getAnimIndexForBody(0, 0);

#ifdef PRELOAD_ALL
	file3D0 = HQR_GetCopy(HQR_Fic, 0);
#else
    HQRM_Load("file3d.hqr", 0, &file3D0);
#endif
    twinsen->entityDataPtr = file3D0;
    TCos0Init = getAnimIndexForBody(0, 0);

    twinsen->animExtraData = loadTwinsenCostumesVar1;

}

void InitBody(char bodyNum, short int actorNumber)
{
    actor *lactor;
    int temp;
    int temp2;
    int currentIndex;
    unsigned short int *ptr;
    short int var1, var2, var3, var4;

   // short int var5, var6;
    int result, result1, result2;

    result = 0;

    lactor = &actors[actorNumber];

    if (lactor->staticFlagsBF.bIsSpriteActor)
	    return;

    if (actorNumber == 0 && comportementHero == 4 && lactor->field_14 != 0 && lactor->field_14 != 1)	// Si c'est twinsen qu'on load
	{
	    SetComportement(0);
	}

    if (bodyNum != -1)
	{
	    temp = SearchBody(bodyNum, actorNumber);
	}
    else
	{
	    temp = -1;
	}

    if (temp != -1)
	{
	    if (lactor->costumeIndex == temp)
		return;

	    temp2 = lactor->costumeIndex;
	    lactor->costumeIndex = temp;
	    lactor->body = bodyNum;
	    currentIndex = lactor->costumeIndex;

	    if (loadCostumeVar == -32000)
		{
		    ptr = (unsigned short int *) bodyPtrTab[lactor->costumeIndex];
		    ptr++;

		    var1 = *(ptr++);
		    var2 = *(ptr++);
		    lactor->boudingBox.Y.bottomLeft = *(ptr++);
		    lactor->boudingBox.Y.topRight = *(ptr++);
		    var3 = *(ptr++);
		    var4 = *ptr;

            if (lactor->staticFlagsBF.bIsUsingMiniZv)
			{
			    result1 = var2 - var1; // take smaller for bound
			    result2 = var4 - var3;

                if(result1<result2)
                    result=result1;
                else
                    result=result2;

                result = abs(result);
			    result >>= 1;
			}
		    else
			{
			    result1 = var2 - var1; // take average for bound
			    result2 = var4 - var3;

			    result = result2 + result1;
                result = abs(result);
			    result >>= 2;
			}

		    lactor->boudingBox.X.bottomLeft = -result;
		    lactor->boudingBox.X.topRight = result;
		    lactor->boudingBox.Z.bottomLeft = -result;
		    lactor->boudingBox.Z.topRight = result;
		}
	    else
		{
		    lactor->boudingBox.X.bottomLeft = loadCostumeVar;
		    lactor->boudingBox.X.topRight = loadCostumeVar4;
		    lactor->boudingBox.Y.bottomLeft = loadCostumeVar2;
		    lactor->boudingBox.Y.topRight = loadCostumeVar5;
		    lactor->boudingBox.Z.bottomLeft = loadCostumeVar3;
		    lactor->boudingBox.Z.topRight = loadCostumeVar6;
		}

	    if (currentIndex == -1)
		    return;

	    if (lactor->previousAnimIndex == -1)
		    return;

	    CopyInterAnim(bodyPtrTab[currentIndex], bodyPtrTab[lactor->costumeIndex]);

	    return;
	}

    lactor->body = -1;
    lactor->costumeIndex = -1;

    lactor->boudingBox.X.bottomLeft = 0;
    lactor->boudingBox.X.topRight = 0;
    lactor->boudingBox.Y.bottomLeft = 0;
    lactor->boudingBox.Y.topRight = 0;
    lactor->boudingBox.Z.bottomLeft = 0;
    lactor->boudingBox.Z.topRight = 0;

}

/*
 * cette fonction doit permettre de transferer l'etat de l'anim d'un costume � l'autre 
 */

void CopyInterAnim(unsigned char *arg_0, unsigned char *arg_4)
{
	short int cx;
	short int ax;
	int i;

	if(!(READ_LE_S16(arg_0)&2))
		return;

	if(!(READ_LE_S16(arg_4)&2))
		return;

	arg_0+=16;
	arg_4+=16;

	WRITE_LE_U32(arg_4,READ_LE_U32(arg_0));
	WRITE_LE_U32(arg_4+4,READ_LE_U32(arg_0+4));

	arg_0=arg_0+READ_LE_S16(arg_0-2);
	arg_0=arg_0+(READ_LE_S16(arg_0))*6+2;
	cx=READ_LE_S16(arg_0);

	arg_4=arg_4+READ_LE_S16(arg_4-2);
	arg_4=arg_4+(READ_LE_S16(arg_4))*6+2;
	ax=READ_LE_S16(arg_4);

	if(cx>ax)
		cx=ax;

	arg_0+=10;
	arg_4+=10;

	for(i=0;i<cx;i++)
	{
		WRITE_LE_U32( arg_4, READ_LE_U32( arg_0 ));
		WRITE_LE_U32( arg_4+4, READ_LE_U32( arg_0+4 ));

		arg_4+=30;
		arg_0+=30;
	}
}

/*

     Y
     |
     |
    / \
  /     \
Z        X

*/

void MDL_DrawBoundingBox_ProjectPoints(pointTab* pPoint3d, point2dStruct* pPoint2d)
{
    projectPositionOnScreen(pPoint3d->x,pPoint3d->y,pPoint3d->z);

    pPoint2d->x = projectedPositionX;
	pPoint2d->y = projectedPositionY;

	if(renderLeft>projectedPositionX)
		renderLeft = projectedPositionX;

	if(renderRight<projectedPositionX)
		renderRight = projectedPositionX;

	if(renderTop>projectedPositionY)
		renderTop = projectedPositionY;

	if(renderBottom<projectedPositionY)
		renderBottom = projectedPositionY;

}

void MDL_DrawBoundingBoxHiddenPart(actor* pActor)
{
	int onScreenActorX;
	int onScreenActorY;
	int onScreenActorZ;

	pointTab frontBottomLeftPoint;
	pointTab frontBottomRightPoint;

	pointTab frontTopLeftPoint;
	pointTab frontTopRightPoint;

	pointTab backBottomLeftPoint;
	pointTab backBottomRightPoint;

	pointTab backTopLeftPoint;
	pointTab backTopRightPoint;

    point2dStruct frontBottomLeftPoint2D;
	point2dStruct frontBottomRightPoint2D;

	point2dStruct frontTopLeftPoint2D;
	point2dStruct frontTopRightPoint2D;

	point2dStruct backBottomLeftPoint2D;
	point2dStruct backBottomRightPoint2D;

	point2dStruct backTopLeftPoint2D;
	point2dStruct backTopRightPoint2D;

	onScreenActorX = pActor->X - cameraX;
	onScreenActorY = pActor->Z - cameraZ;
	onScreenActorZ = pActor->Y - cameraY;

    // compute the points in 3D

	frontBottomLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	frontBottomLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    frontBottomLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontBottomRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	frontBottomRightPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    frontBottomRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontTopLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	frontTopLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    frontTopLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontTopRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	frontTopRightPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    frontTopRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	backBottomLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	backBottomLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    backBottomLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backBottomRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	backBottomRightPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    backBottomRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backTopLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	backTopLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    backTopLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backTopRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	backTopRightPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    backTopRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

    // project all points

    MDL_DrawBoundingBox_ProjectPoints( &frontBottomLeftPoint,    &frontBottomLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontBottomRightPoint,   &frontBottomRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontTopLeftPoint,       &frontTopLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontTopRightPoint,      &frontTopRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backBottomLeftPoint,     &backBottomLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backBottomRightPoint,    &backBottomRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backTopLeftPoint,        &backTopLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backTopRightPoint,       &backTopRightPoint2D );

    // draw all lines

    // draw back part
    drawLine(backBottomLeftPoint2D.x,backBottomLeftPoint2D.y,backTopLeftPoint2D.x,backTopLeftPoint2D.y,255);
    drawLine(backTopLeftPoint2D.x,backTopLeftPoint2D.y,backTopRightPoint2D.x,backTopRightPoint2D.y,255);
    drawLine(backTopRightPoint2D.x,backTopRightPoint2D.y,backBottomRightPoint2D.x,backBottomRightPoint2D.y,255);
    drawLine(backBottomRightPoint2D.x,backBottomRightPoint2D.y,backBottomLeftPoint2D.x,backBottomLeftPoint2D.y,255);

    // draw bottom part
    drawLine(frontBottomLeftPoint2D.x,frontBottomLeftPoint2D.y,backBottomLeftPoint2D.x,backBottomLeftPoint2D.y,255);
    drawLine(backBottomLeftPoint2D.x,backBottomLeftPoint2D.y,backBottomRightPoint2D.x,backBottomRightPoint2D.y,255);
    drawLine(backBottomRightPoint2D.x,backBottomRightPoint2D.y,frontBottomRightPoint2D.x,frontBottomRightPoint2D.y,255);
    drawLine(frontBottomRightPoint2D.x,frontBottomRightPoint2D.y,frontBottomLeftPoint2D.x,frontBottomLeftPoint2D.y,255);

}

void MDL_DrawBoundingBoxShownPart(actor* pActor)
{
	int onScreenActorX;
	int onScreenActorY;
	int onScreenActorZ;

	pointTab frontBottomLeftPoint;
	pointTab frontBottomRightPoint;

	pointTab frontTopLeftPoint;
	pointTab frontTopRightPoint;

	pointTab backBottomLeftPoint;
	pointTab backBottomRightPoint;

	pointTab backTopLeftPoint;
	pointTab backTopRightPoint;

    point2dStruct frontBottomLeftPoint2D;
	point2dStruct frontBottomRightPoint2D;

	point2dStruct frontTopLeftPoint2D;
	point2dStruct frontTopRightPoint2D;

	point2dStruct backBottomLeftPoint2D;
	point2dStruct backBottomRightPoint2D;

	point2dStruct backTopLeftPoint2D;
	point2dStruct backTopRightPoint2D;

	onScreenActorX = pActor->X - cameraX;
	onScreenActorY = pActor->Z - cameraZ;
	onScreenActorZ = pActor->Y - cameraY;

    // compute the points in 3D

	frontBottomLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	frontBottomLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    frontBottomLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontBottomRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	frontBottomRightPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    frontBottomRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontTopLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	frontTopLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    frontTopLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	frontTopRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	frontTopRightPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    frontTopRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.topRight;

	backBottomLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	backBottomLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    backBottomLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backBottomRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	backBottomRightPoint.y = onScreenActorY + pActor->boudingBox.Y.bottomLeft;
    backBottomRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backTopLeftPoint.x = onScreenActorX + pActor->boudingBox.X.bottomLeft;
	backTopLeftPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    backTopLeftPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

	backTopRightPoint.x = onScreenActorX + pActor->boudingBox.X.topRight;
	backTopRightPoint.y = onScreenActorY + pActor->boudingBox.Y.topRight;
    backTopRightPoint.z = onScreenActorZ + pActor->boudingBox.Z.bottomLeft;

    // project all points

    MDL_DrawBoundingBox_ProjectPoints( &frontBottomLeftPoint,    &frontBottomLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontBottomRightPoint,   &frontBottomRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontTopLeftPoint,       &frontTopLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &frontTopRightPoint,      &frontTopRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backBottomLeftPoint,     &backBottomLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backBottomRightPoint,    &backBottomRightPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backTopLeftPoint,        &backTopLeftPoint2D );
    MDL_DrawBoundingBox_ProjectPoints( &backTopRightPoint,       &backTopRightPoint2D );

    // draw all lines

    // draw front part
    drawLine(frontBottomLeftPoint2D.x,frontBottomLeftPoint2D.y,frontTopLeftPoint2D.x,frontTopLeftPoint2D.y,255);
    drawLine(frontTopLeftPoint2D.x,frontTopLeftPoint2D.y,frontTopRightPoint2D.x,frontTopRightPoint2D.y,255);
    drawLine(frontTopRightPoint2D.x,frontTopRightPoint2D.y,frontBottomRightPoint2D.x,frontBottomRightPoint2D.y,255);
    drawLine(frontBottomRightPoint2D.x,frontBottomRightPoint2D.y,frontBottomLeftPoint2D.x,frontBottomLeftPoint2D.y,255);

    // draw top part
    drawLine(frontTopLeftPoint2D.x,frontTopLeftPoint2D.y,backTopLeftPoint2D.x,backTopLeftPoint2D.y,255);
    drawLine(backTopLeftPoint2D.x,backTopLeftPoint2D.y,backTopRightPoint2D.x,backTopRightPoint2D.y,255);
    drawLine(backTopRightPoint2D.x,backTopRightPoint2D.y,frontTopRightPoint2D.x,frontTopRightPoint2D.y,255);
    drawLine(frontTopRightPoint2D.x,frontTopRightPoint2D.y,frontTopLeftPoint2D.x,frontTopLeftPoint2D.y,255);
}