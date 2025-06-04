#include "Paddle.h"

#include "Camera.h"

Paddle::Paddle(const BOX& box)
{
	_box = box;

	_prevBox = _box;

	_soundHandle = LoadSoundMem("se1.wav");

}

Paddle::~Paddle()
{
	DeleteSoundMem(_soundHandle);
}

void Paddle::freeze()
{
	if (_freeze == true) return;

	//フリーズしたらランダムでカラーかえちゃえ
	_freeze = true;
	_color = GetColor(rand()%256, rand()% 256, rand()% 256);

	// 読みこんだ音をノーマル再生します(『PlaySoundMem』関数使用)
	PlaySoundMem(_soundHandle, DX_PLAYTYPE_BACK);

}

void Paddle::setVecolity(int v)
{
	_velocity = v;
}

void Paddle::setColor(int r, int g, int b)
{
	_color = GetColor(r,g,b);
}

void Paddle::innerUpdate()
{
	// カメラ座標空間に
	_box.pos.y -= Camera::y;

	if (_freeze) return;

	_box.pos.x += _velocity;
}

void Paddle::innerDraw()
{
	if (_visibility == false) return;

	BOX box = _box;

	int halfW = int(box.size.w) / 2; int halfH = int(box.size.h) / 2;
	DrawBox(int(box.pos.x) - halfW, int(box.pos.y) - halfH, int(box.pos.x) + halfW, int(box.pos.y) + halfH, _color, _freeze);
}


void Paddle::innerOnCollisionEvent(BoxCollider* tBox)
{
	// 画面外当たり判定BOXに触れたら死ぬ
	if (tBox->getTag() == BoxCollider::SCN_OUT_BOX)
	{
		_die = true;
	}
}