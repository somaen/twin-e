/***************************************************************************
                          osystem.h  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

class OSystem
{
  public:
    OSystem(int argc, char *argv[]);

    int mouseRight;
    int mouseLeft;

    void delay(int time);
    void crossFade(char *buffer, char *palette);
    void fadeBlackToWhite();
	void updateImage();
    void initBuffer(char *buffer, int width, int height);
	void initVideoBuffer(char *buffer, int width, int height);
    void putpixel(int x, int y, int pixel);
    void setColor(byte i, byte R, byte G, byte B);
    void setPalette(byte * palette);
	void setPalette320x200(byte * palette);
    void Flip(unsigned char *videoBuffer);
	void draw320x200BufferToScreen(unsigned char *videoBuffer);
    void CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom);
    void drawText(int X, int Y, char *text);
    void drawTextColor(int X, int Y, char *string, unsigned char R, unsigned char G, unsigned char B);
    void drawLine(int X1,int X2,int Y1,int Y2,unsigned char color, unsigned char* palette);
    void getMouseStatus(mouseStatusStruct * mouseData);

	void set320x200Mode(bool mode);

  private:
};
