#include "lba.h"
/*
#include <sn_fcntl.h>
#include <usrsnasm.h>
*/
void AdelineLogo(void)
{
    playMidi(31);
    Load_HQR("ress.hqr", workVideoBuffer, 27);
    CopyScreen(workVideoBuffer, frontVideoBuffer);
    Load_HQR("ress.hqr", palette, 28);
    convertPalToRGBA(palette, paletteRGBA);

/*	int debugH = debug_open("palette.raw", SNASM_O_BINARY|SNASM_O_CREAT|SNASM_O_RDWR);
	debug_write(debugH, (char*)palette, 256*3);
	debug_close(debugH); */

    blackToWhite();
    osystem->Flip(frontVideoBuffer);
    fadeIn(paletteRGBA);
   // SDL_Delay(2000);
}

void CopyScreen(byte * source, byte * destination)
{

    memcpy(destination, source, 307200);
}

void fadeIn(byte * palette)
{
    int i = 100;

	osystem->setPalette(palette);
	return;

    for (i = 0; i < 100; i += 3)
	{
	    adjustPalette(255, 255, 255, palette, i);
	    readKeyboard();
	}
}

void adjustPalette(byte R, byte G, byte B, byte * palette, int intensity)
{
    byte localPalette[1024];
    byte *newR;
    byte *newG;
    byte *newB;
    byte *newA;

    int local;
    int counter = 0;
    int i;

    local = intensity;

    newR = &localPalette[0];
    newG = &localPalette[1];
    newB = &localPalette[2];
    newA = &localPalette[3];

    for (i = 0; i < 256; i++)
	{
	    *newR = RegleTrois32(R, palette[counter], 100, local);
	    *newG = RegleTrois32(G, palette[counter + 1], 100, local);
	    *newB = RegleTrois32(B, palette[counter + 2], 100, local);
	    *newA = 0;

	    newR += 4;
	    newG += 4;
	    newB += 4;
	    newA += 4;

	    counter += 4;
	}

    osystem->setPalette(localPalette);
}

int RegleTrois32(int modifier, int color, int param, int intensity)
{
    if (!param)
	return (color);
    return (((color - modifier) * intensity) / param) + modifier;
}

void RessPict(int imageNumber)
{
    Load_HQR("ress.hqr", workVideoBuffer, imageNumber);
    CopyScreen(workVideoBuffer, frontVideoBuffer);
    Load_HQR("ress.hqr", palette, imageNumber + 1);
    convertPalToRGBA(palette, paletteRGBA);
    osystem->Flip(frontVideoBuffer);
    FadeToPal((char *) paletteRGBA);
}

void loadImageCrossFade(int imageNumber)
{
    Load_HQR("ress.hqr", workVideoBuffer, imageNumber);
    CopyScreen(workVideoBuffer, frontVideoBuffer);
    Load_HQR("ress.hqr", palette, imageNumber + 1);
    convertPalToRGBA(palette, paletteRGBA);

    osystem->crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
}

void FadeToBlack(char *palette)
{
    int i = 0;

    if (palReseted == 0)
	{
	//    for (i = 100; i >= 0; i -= 3)
		{
		    adjustPalette(0, 0, 0, (byte *) palette, i);
		    readKeyboard();
		}
	}

    palReseted = 1;
}

void FadeToPal(char *palette)
{
    int i = 100;

/*    for (i = 0; i <= 100; i += 3)
	{
	    adjustPalette(0, 0, 0, (byte *) palette, i);
	    readKeyboard();
	} */

	osystem->setPalette( (byte*)palette );

    palReseted = 0;

}

void blackToWhite(void)
{
    byte palette[1024];
    int i;

	i = 256;
//    for (i = 0; i < 256; i += 3)
	{
	    memset(palette, i, 1024);

	    osystem->setPalette(palette);
	    readKeyboard();
	}
}

void SetBackPal(void)
{
   // int i;

   /*
    * for(i=0;i<768;i++) palette[i]=0; 
    */

    memset(palette, 0, 768);
    memset(paletteRGBA, 0, 1024);

    osystem->setPalette(paletteRGBA);

    palReseted = 1;
}

void Cls(void)
{
   /*
    * int i;
    * 
    * for(i=0;i<307200;i++) { frontVideoBuffer[i]=0; }
    */

    memset(frontVideoBuffer, 0, 307200);
}

void convertPalToRGBA(byte * palSource, byte * palDest)
{
    int i;

    int *colorDest = (int *) palDest;

#ifdef PCLIKE
    for (i = 0; i < 256; i++)
	{
	    *(colorDest++) = (*(int *) palSource);	// little optimisation trick
	    palSource += 3;
	}
#else
    for (i = 0; i < 256*3; i++)
	{
	    *(palDest++) = *(palSource++);	// little optimisation trick
	}
#endif

}
