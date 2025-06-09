#pragma once

#include "DxLibEx.h"

class BoxCollider
{
public:
	enum class ColliderTAG
	{
		SCN_OUT_BOX = 0,
		DEAD_BOX = 1,
		PLAYER = 2,
		PADDLE = 3,
		UNKNOWN = -1
	};

	// 衝突判定が起こった時点でのBOX情報の更新と取得
	// setHitedBoxは主にColliderの中で使用されます。
	void setHitedBox(const BOX& box) { _hitedBox = box; }
	BOX getHitedBox() const { return _hitedBox; }

	// 当たり判定のBOX情報を更新と取得
	void setBox(const BOX& box) { _box = box; }
	BOX getBox() const { return _box; }

	// 1フレーム前のBOX情報を取得
	BOX getPrevBox() { return _prevBox; };
	
	// タグ情報の設定と取得
	ColliderTAG getTag() const { return _tag;  }
	void setTag(ColliderTAG tag) { _tag = tag;  }

	// 画面外判定 bias=画面判定に余力を持たせたい場合指定
	bool isScreenArea(int bias = 0);	

	// 衝突時に呼ばれます。内部ではinnerOnCollisionEvent
	void OnCollisionEvent(BoxCollider*);

	// 衝突有効化・無効化と判定
	void setCollisionEnable(bool flag) { _doCollision = flag;  };
	bool isCollisionEnable() { return _doCollision;  };

	// デバッグ情報の表示・非表示
	void debugValidFlag(bool flag) { _debug = flag; };

	// 内部でinnerUpdateが呼ばれます。
	void update();

	// 内部でinnnerDrawが呼ばれます。
	void draw();
protected:
	BOX _box = {0,0,0,0};	// 常に最新のBOX情報
	BOX _prevBox = { 0, 0, 0, 0 };	// 1フレーム前のBOX情報
	// 衝突したBOX情報　衝突判定は_prevBoxと_boxの差分を分解して判定しているので
	// その分解してあたったものとなる。
	BOX _hitedBox = { 0,0,0.0 };	

	ColliderTAG _tag = ColliderTAG::UNKNOWN;

	bool _doCollision = true;

private:
	bool _debug = false;

	virtual void innerUpdate() = 0;
	virtual void innerDraw() = 0;
	virtual void innerOnCollisionEvent(BoxCollider*) = 0;
};

