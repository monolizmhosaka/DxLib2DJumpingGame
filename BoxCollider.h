#pragma once

#include "DxLibEx.h"

class BoxCollider
{
public:
	enum ColliderTAG
	{
		SCN_OUT_BOX = 0,
		DEAD_BOX = 1,
		PLAYER = 2,
		UNKNOWN = -1
	};

	void setHitedBox(const BOX& box) { _hitedBox = box; }
	BOX getHitedBox() const { return _hitedBox; }

	void setBox(const BOX& box) { _box = box; }
	BOX getBox() const { return _box; }

	// 1フレーム前のBOXを取得
	BOX getPrevBox() { return _prevBox; };


	ColliderTAG getTag() const { return _tag;  }
	void setTag(ColliderTAG tag) { _tag = tag;  }

	bool isScreenArea(int bias = 0);	// 画面外判定 bias=画面判定に余力を持たせたい場合指定

	// 衝突時に呼ばれます
	void OnCollisionEvent(BoxCollider*);

	// 衝突有効化・無効化
	void setCollisionEnable(bool flag) { _collisionEvent = flag;  };
	bool isCollisionEnable() { return _collisionEvent;  };

	void debugValidFlag(bool flag) { _debug = flag; };

	void update();
	void draw();
protected:
	BOX _box = {0,0,0,0};
	BOX _prevBox = { 0, 0, 0, 0 };
	BOX _hitedBox = { 0,0,0.0 };

	ColliderTAG _tag = UNKNOWN;

	bool _collisionEvent = true;

private:
	bool _debug = false;


	virtual void innerUpdate() = 0;
	virtual void innerDraw() = 0;
	virtual void innerOnCollisionEvent(BoxCollider*) = 0;
};

