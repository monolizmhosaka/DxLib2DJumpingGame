#pragma once

#include "DxLibEx.h"

#include "BoxCollider.h"

class Paddle;

class Player : public BoxCollider
{
public:
	Player();
	virtual ~Player();

	void init();

	int isJump() const { return _jumpFlag;  };
	int isDie() const { return _dieFlag; };

	int getJumpVector() const;	// 0はジャンプの頂点の可能性もあるので別途isJumpは必要
	void getPosition(DOUBLE2* pD2) const { *pD2 = _box.pos; };

private:	//BoxCollider override;
	void innerOnCollisionEvent(BoxCollider*) override;

	void innerUpdate() override;
	void innerDraw() override;
private:
	int _gHandle = -1;		// 画像ハンドル
	int _gHandleWait = -1;
	int _soundHandle = -1;	// 

	int _vectorX = -1;
	int _upPower = 0;	// ジャンプの加速度
	int _jumpFlag = 0;	// ジャンプ中フラグ
	int _animationCount = 0;	// アニメーション調整カウント

	double _rotate = 0;		// 回転ラジアン
	bool _dieFlag = false;		// 死亡フラグ
};

