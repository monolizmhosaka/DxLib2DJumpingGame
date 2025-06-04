#pragma once
#include "DxLibEx.h"

#include "BoxCollider.h"

class Paddle : public BoxCollider
{
public:
	Paddle(const BOX& box);
	virtual ~Paddle();

	void freeze();
	bool isFreeze() const { return _freeze; }

	void setColor(int r, int g, int b);
	void setVecolity(int v);

	//void requestToDie() { _die = true; };
	bool isDie() const { return _die; };


	void visibility(bool flag) { _visibility = flag; };
	bool isVisibility() const { return _visibility; };

private:	//BoxCollider override;
	void innerOnCollisionEvent(BoxCollider*) override;

	void innerUpdate() override;
	void innerDraw() override;

private:
	unsigned int _color = 0xFFFFFF;
	bool _freeze = false;

	int _velocity = 1;	// 速度
	int _soundHandle = -1;	// 音楽ハンドル

	bool _die = false;
	bool _visibility = true;
};

