#ifndef BUTTON_H
#define BUTTON_H

#include"Phybal.h"

class CButton{
public:
	int X, Y;
	int Width, Height;
	int Type;
	HBITMAP* Image;
	char String[20];
	int isClicked(int x, int y);
	CButton(int x,int y, int width, int height, int type, HBITMAP* image, char string[]=NULL){
		X=x;
		Y=y;
		Type=type;
		Width=width;
		Height=height;
		Image=image;
		if(string!=NULL){
			for(int i=0;i<(int)strlen(string);i++)
				String[i]=string[i];
		}
	}
};

#endif