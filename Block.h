#ifndef BLOCK_H
#define BLOCK_H

#include"Phybal.h"


#ifdef EXTB_GLOBAL //����� ���������� �����ϱ� ���� Ʈ��
#define EXTB
#else
#define EXTB extern
#endif
EXTB HBITMAP hBlock1;//����� ���������� �����Ͽ���.
EXTB HBITMAP hBlock2;
EXTB HBITMAP hBlock3;
EXTB HBITMAP hBlock4;
EXTB HBITMAP hBlock5;
EXTB HBITMAP hBlock6;
EXTB HBITMAP hBlock7;
EXTB HBITMAP OldBlock;
EXTB int Bwidth;
EXTB int Bheight;

class CBlock{
public:
	int x;
	int y;
	int Type;
	int width;
	int height;
	CBlock(int t_Type,int t_x, int t_y, int t_width=Bwidth, int t_height=Bheight){
		Type	=	t_Type;
		x		=	t_x;
		y		=	t_y;
		width	=	t_width;
		height	=	t_height;
	}
};

extern CBlock* Objects[1000];
extern CBlock* StartPoint;
extern CBlock* EndPoint;
extern int O_Count;


#endif