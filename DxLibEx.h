#pragma once

#include <cmath>

#include "DxLib.h"

//#define TAIKEN_COLLISION
//#define TAIKEN_JUMP


#define PI (3.14159)	// 円周率

typedef struct _tagINT2
{
	int x;
	int y;
} INT2;

// 共用体
union DOUBLE2
{
	struct {
		double x, y;
	};
	struct {
		double w, h;
	};
};

// 構造体
struct BOX
{
	DOUBLE2 pos;
	DOUBLE2 size;
};


// 重力
const int GRAVITY = 4;

// ウィンドウサイズ
const INT2 WIN_SIZE = {480, 640};

const BOX SCN_OUT_BOXES[] = {
	{WIN_SIZE.x / 2.0, WIN_SIZE.y + 300.0, double(WIN_SIZE.x), 100.0},	// under
	{ 0 - 300.0, WIN_SIZE.y / 2.0, 100.0, double(WIN_SIZE.y) },			// left
	{ WIN_SIZE.x + 300.0, WIN_SIZE.y / 2.0, 100.0, double(WIN_SIZE.y) }	// right
};

//お立ち台
const BOX STAGE_BOX = { WIN_SIZE.x / 2.0, WIN_SIZE.y / 2.0, 64.0, 64.0 };	//お立ち台


// イテレーターマクロ 
#define ITERATOR_CALL_FUNC(v,f) \
		for (auto it = v.begin(); it != v.end(); it++) \
		{\
			(*it)->f();\
		}



#ifdef TAIKEN_COLLISION 
static bool collisionBoxBox(const BOX& a, const BOX& b)
{







#include "InputCollision.h"








return false;
}





#else
// BOX同士の当たり判定を求める。
static bool collisionBoxBox(const BOX& a, const BOX& b)
{
	//それぞれの中心点からの距離を求める
	double dx = std::fabs(a.pos.x - b.pos.x);
	double dy = std::fabs(a.pos.y - b.pos.y);

	// それぞれの半分を足した長さを求める
	double abHW = a.size.w / 2 + b.size.w / 2;
	double abHH = a.size.h / 2 + b.size.h / 2;

	if (dx < abHW && dy < abHH)
	{
		return true;
	}
	return false;
}
#endif


//// 原点中心としてx,yに配置するw,hの大きさの箱を描画する
static void drawBoxEx(BOX box, unsigned int col, int fill)
{
	int halfW = int(box.size.w / 2); int halfH = int(box.size.h / 2);
	DrawBox(int(box.pos.x) - halfW, int(box.pos.y) - halfH, int(box.pos.x) + halfW, int(box.pos.y) + halfH, col, fill);
}
