#ifndef PHYBAL_H
#define PHYBAL_H

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Ball.h"
#include "Block.h"
#include "Button.h"

#pragma comment(lib, "msimg32.lib");

#define sign(x) (x>=0?1:-1)

#ifdef EXT_GLOBAL //����� ���������� �����ϱ� ���� Ʈ��
#define EXT
#else
#define EXT extern
#endif


#endif