#define EXT_GLOBAL
#include "Phybal.h"

//���� �ִ� 25*16
HWND I_hWnd;

CBall* Balls[1000];
CBlock* Objects[1000];	// nature�� �����ϴ� ��ü��
int O_Count;
CBlock* StartPoint=NULL;
CBlock* EndPoint=NULL;
CButton* Button[10];
int Level;

int mx, my;//���콺 ��ǥ

RECT crt;
RECT block_rect;
RECT map;

char p_addr[100];//������ ���� ���


int rec_block;//0:�����, 1:��, 2:����, 3:����, 4:L����Ʈ, 5:R����Ʈ, 6:������


void LoadLevel(int level){
	int a,b;
	int t;
	char filename[100];

	sprintf(filename,"Block%d.txt",level);

	FILE *in=fopen(filename,"r");

	if(in!=NULL){
		block_rect.left = 0;
		block_rect.right = 0;
		block_rect.top = 0;
		block_rect.bottom = 0;

		while(fscanf(in,"%d %d %d",&t,&a,&b)!=EOF){

			a++;
			b++;

			//ȭ��ũ�� �ڵ������� ���� ����
			if(block_rect.left == 0)	block_rect.left = a*50, block_rect.right = (a+1)*50, block_rect.top = b*50, block_rect.bottom = (b+1)*50;
			if(block_rect.left > a*50)			block_rect.left = a*50;
			if(block_rect.right < (a+1)*50)		block_rect.right = (a+1)*50;
			if(block_rect.top > b*50)			block_rect.top = b*50;
			if(block_rect.bottom < (b+1)*50)	block_rect.bottom = (b+1)*50;
			
			a--;
			b--;


			if(t==-1){//������
				StartPoint = new CBlock(t,50*a,50*b);
			}else if(t==-2){//����
				EndPoint = new CBlock(t,50*a,50*b);
			}else{//���
				CBlock *New_Block;
				New_Block = new CBlock(t,50*a,50*b,50,50);
				Objects[O_Count++]=New_Block;
			}
		}
		fclose(in);
	}
	Level = level;
}


void Init(){

	//������ ���� ��ȣ
	int cnt=0;
	FILE *exist = NULL;
	do{//����� ���� ã��
		if(exist != NULL)	fclose(exist);
		cnt++;
		sprintf(p_addr,"Map%d.mpm",cnt);
		exist = fopen(p_addr,"r");
	}while(exist != NULL);

	sprintf(p_addr,"Map%d.mpm",cnt);


	Bwidth=50;
	Bheight=50;
	hBlock1 = (HBITMAP)LoadImage(NULL,"Image\\Block1.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock2 = (HBITMAP)LoadImage(NULL,"Image\\Block2.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock3 = (HBITMAP)LoadImage(NULL,"Image\\Block3.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock4 = (HBITMAP)LoadImage(NULL,"Image\\Block4.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock5 = (HBITMAP)LoadImage(NULL,"Image\\Block5.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock6 = (HBITMAP)LoadImage(NULL,"Image\\Block6.bmp",NULL,Bwidth,Bheight,LR_LOADFROMFILE);
	hBlock7 = (HBITMAP)LoadImage(NULL,"Image\\Save.bmp",NULL,50,10,LR_LOADFROMFILE);
	O_Count=0;

	{
		Button[0] = new CButton(700,50*1,50,50,0,NULL,"�����");
		Button[1] = new CButton(700,50*2,50,50,1,&hBlock1);
		Button[2] = new CButton(700,50*3,50,50,2,&hBlock2);
		Button[3] = new CButton(700,50*4,50,50,-2,&hBlock3);
		Button[4] = new CButton(700,50*5,50,50,-3,&hBlock4);
		Button[5] = new CButton(700,50*6,50,50,-4,&hBlock5);
		Button[6] = new CButton(700,50*7,50,50,-1,&hBlock6);
		Button[7] = new CButton(700,50*10,50,10,10,&hBlock7);
	}

	map.left=20;
	map.top=20;
	map.right=680;
	map.bottom=580;
	LoadLevel(1);
}

int CompactRect(int* x,int* y){
	int lx,rx,ty,by;
	for(int i=0;i<25;i++){
		for(int j=0;j<16;j++){
			lx = map.left + i*Button[rec_block]->Width;
			rx = map.left + (i+1)*Button[rec_block]->Width;
			ty = map.top + j*Button[rec_block]->Height;
			by = map.top + (j+1)*Button[rec_block]->Height;
			if(map.right < rx)	continue;
			if(map.bottom < by)	continue;
			if( lx <= (*x) && (*x) < rx && ty <= (*y) && (*y) < by ){
				(*x) = map.left + i*Button[rec_block]->Width + Button[rec_block]->Width/2;
				(*y) = map.top + j*Button[rec_block]->Height + Button[rec_block]->Height/2;
				return 1;
			}
		}
	}
	return 0;
}

void MSG_PAINT(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc, hMemDC;
	HDC SurfaceDC;
	HBITMAP hBitmap, OldBitmap;
	HBRUSH BGBrush;
    HFONT Font, OldFont;

	hdc = BeginPaint(hWnd,&ps);
	hMemDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		SurfaceDC = CreateCompatibleDC(hMemDC);

	Font = CreateFont(13,0,0,0,400,0,0,0,HANGEUL_CHARSET,0,0,0,0,TEXT("����ü"));
	OldFont = (HFONT)SelectObject(hMemDC,Font);
//	SetBkMode(hMemDC, 1);
	SetTextColor(hMemDC, 0x000000);	// ��Ʈ��
	SetBkColor(hMemDC, 0xffffff);	// ����

		BGBrush = CreateSolidBrush(0x000000);
		FillRect(hMemDC, &block_rect, BGBrush);
		DeleteObject(BGBrush);

		
		{//������ ��� ���
			BGBrush = CreateSolidBrush(0xEEEEEE);
			FillRect(hMemDC, &map, BGBrush);
			DeleteObject(BGBrush);

			for(int i=0;i<O_Count;i++){

				int B_Type = Objects[i]->Type;
				if(B_Type == 1)		OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock1);
				if(B_Type == 2)		OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock2);
				if(B_Type == -2)	OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock3);
				if(B_Type == -3)	OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock4);
				if(B_Type == -4)	OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock5);
				if(B_Type == -1)	OldBlock = (HBITMAP)SelectObject(SurfaceDC, hBlock6);

				BitBlt(hMemDC, block_rect.left+Objects[i]->x, block_rect.top+Objects[i]->y, Objects[i]->width, Objects[i]->height, SurfaceDC, 0, 0, SRCCOPY);

				SelectObject(SurfaceDC, OldBlock);

			}
		}


		{//��ư
			for(int i=0;i<8;i++){
				if(Button[i]->Image!=NULL){
					OldBlock = (HBITMAP)SelectObject(SurfaceDC, *(Button[i]->Image));

					BitBlt(hMemDC, block_rect.left+Button[i]->X, block_rect.top+Button[i]->Y, Button[i]->Width, Button[i]->Height, SurfaceDC, 0, 0, SRCCOPY);

					SelectObject(SurfaceDC, OldBlock);
				}else{
					TextOut(hMemDC,block_rect.left+Button[i]->X, block_rect.top+Button[i]->Y,Button[i]->String,lstrlen(Button[i]->String));
				}
			}
		}

		{//���콺
			if(Button[rec_block]->Image!=NULL){
				OldBlock = (HBITMAP)SelectObject(SurfaceDC, *(Button[rec_block]->Image));

				int t_width = Button[rec_block]->Width;
				int t_height = Button[rec_block]->Height;
				int tx = mx;
				int ty = my;

				CompactRect(&tx,&ty);

				if(tx+t_width/2 < map.right)
					BitBlt(hMemDC, tx-t_width/2, ty-t_height/2, t_width, t_height, SurfaceDC, 0, 0, SRCCOPY);

				SelectObject(SurfaceDC, OldBlock);
			}
		}

	BitBlt(hdc, crt.left, crt.top, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(hMemDC, OldFont));
	DeleteObject(SelectObject(hMemDC, OldBitmap));
	DeleteDC(hMemDC);
	DeleteDC(SurfaceDC);
	EndPaint(hWnd,&ps);
}

void LoadFile(){
	;
}

void SaveFile(){
	//�˻�
	if(StartPoint==NULL){
		MessageBox(I_hWnd,"�������� �����ϴ�.",NULL,NULL);
		return;
	}
	if(EndPoint==NULL){
		MessageBox(I_hWnd,"�������� �����ϴ�.",NULL,NULL);
		return;
	}

	int i;
	
	FILE *out = fopen(p_addr,"w");
	fprintf(out, "-1 %d %d\n",(StartPoint->x-map.left)/Bwidth, (StartPoint->y-map.top)/Bheight);
	fprintf(out, "-2 %d %d\n",(EndPoint->x-map.left)/Bwidth, (EndPoint->y-map.top)/Bheight);
	fprintf(out, "\n");

	for(i=0;i<O_Count;i++){
		if(Objects[i]->Type!=0){
			fprintf(out, "%d %d %d\n",Objects[i]->Type, (Objects[i]->x-map.left)/Bwidth, (Objects[i]->y-map.top)/Bheight);
		}
	}

	fclose(out);

	char str[50];
	sprintf(str,"%s : ����Ǿ����ϴ�",p_addr);
	MessageBox(I_hWnd,str,NULL,NULL);
	
}

int isClick;

void Click(int cx,int cy){
	if(isClick==2){//��Ŭ��
		for(int i=0;i<O_Count;i++){
			if(Objects[i]->x < cx && cx < Objects[i]->x + Objects[i]->width)
				if(Objects[i]->y < cy && cy < Objects[i]->y + Objects[i]->height){
					if(Objects[i]->Type == -1)
						StartPoint = NULL;
					if(Objects[i]->Type == -2)
						EndPoint = NULL;
					Objects[i]->Type = 0;
					Objects[i]->width = Bwidth;
					Objects[i]->height = Bheight;
					break;
				}
		}
		return;
	}



	//��Ŭ��
	for(int i=0;i<8;i++){
		if( Button[i]->isClicked(cx,cy) ){
			if( Button[i]->Type == 10 ){
				isClick = 0;
				SaveFile();
				return;
			}
			rec_block = i;
		}
	}
	int tx, ty;
	tx = cx, ty = cy;
	if(CompactRect(&tx,&ty)){
		for(i=0;i<O_Count;i++){
			if(Objects[i]->x < tx && tx < Objects[i]->x + Objects[i]->width)
				if(Objects[i]->y < ty && ty < Objects[i]->y + Objects[i]->height){
					if(Button[rec_block]->Type == -1 && StartPoint != NULL)	continue;
					if(Button[rec_block]->Type == -1)	StartPoint = Objects[i];
					if(Button[rec_block]->Type == -2 && EndPoint != NULL)	continue;
					if(Button[rec_block]->Type == -2)	EndPoint = Objects[i];
					if(Objects[i]->Type == -1)	StartPoint = NULL;
					if(Objects[i]->Type == -2)	EndPoint = NULL;
					Objects[i]->Type = Button[rec_block]->Type;
					Objects[i]->width = Button[rec_block]->Width;
					Objects[i]->height = Button[rec_block]->Height;
					break;
				}
		}
		if(i == O_Count){
			tx -= Button[rec_block]->Width/2;
			ty -= Button[rec_block]->Height/2;

			if(Button[rec_block]->Type == -1){
				if(StartPoint == NULL){
					CBlock* NewBlock = new CBlock(Button[rec_block]->Type,tx,ty, Button[rec_block]->Width, Button[rec_block]->Height);
					Objects[O_Count++] = NewBlock;
					StartPoint = NewBlock;
				}
			}else if(Button[rec_block]->Type == -2){
				if(EndPoint == NULL){
					CBlock* NewBlock = new CBlock(Button[rec_block]->Type,tx,ty, Button[rec_block]->Width, Button[rec_block]->Height);
					Objects[O_Count++] = NewBlock;
					EndPoint = NewBlock;
				}
			}else{
				CBlock* NewBlock = new CBlock(Button[rec_block]->Type,tx,ty, Button[rec_block]->Width, Button[rec_block]->Height);
				Objects[O_Count++] = NewBlock;
			}
		}

	}
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	switch(iMessage) {
	case WM_TIMER:		InvalidateRect(hWnd, NULL, FALSE);	break;
	case WM_PAINT:		MSG_PAINT(hWnd);					break;
	case WM_CREATE:		GetClientRect(hWnd, &crt);
						/*SetTimer(hWnd,0,1000,NULL);*/			break;
	case WM_LBUTTONDOWN:
		isClick=1;
		Click(LOWORD(lParam),HIWORD(lParam));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_RBUTTONDOWN:
		isClick=2;
		Click(LOWORD(lParam),HIWORD(lParam));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if(isClick)			Click(LOWORD(lParam),HIWORD(lParam));
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONUP:
		isClick=0;
		break;
	case WM_RBUTTONUP:
		isClick=0;
		break;
	case WM_DESTROY:	PostQuitMessage(0);					break;
	}
	return DefWindowProc( hWnd, iMessage, wParam, lParam );
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow )
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = CreateSolidBrush(RGB( 255,255,255 ) );
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = ( WNDPROC )WndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = TEXT( "PhyBall_Maker" );
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass( &WndClass );

	hWnd = CreateWindow( TEXT( "PhyBall_Maker" ), TEXT( "PhyBall_Maker" ),
	WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
	800, 650, NULL, (HMENU)NULL, hInstance, NULL );

	ShowWindow( hWnd, nCmdShow );

	I_hWnd = hWnd;
	Init();

	while( GetMessage( &Message, NULL, 0, 0 ) ) {
		TranslateMessage( &Message );
		DispatchMessage( &Message );
	}
	return (int)Message.wParam;
}



