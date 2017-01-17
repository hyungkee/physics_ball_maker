#include "Button.h"

int CButton::isClicked(int x,int y){
	if(X<x && x<X+Width)
		if(Y<y && y<Y+Height)
			return 1;
	return 0;
}