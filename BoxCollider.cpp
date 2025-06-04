#include "BoxCollider.h"

// 画面内＋α内にいるか判定する。
bool BoxCollider::isScreenArea(int bias)
{
	if ((0 - bias > _box.pos.x + _box.size.w / 2) ||
		(WIN_SIZE.x + bias < _box.pos.x - _box.size.w / 2) ||
		(0 - bias > _box.pos.y + _box.size.h / 2) ||
		(WIN_SIZE.y + bias  < _box.pos.y - _box.size.h / 2)
		)
	{
		//画面外
		return false;
	}
	//画面内
	return true;

}

// 衝突イベント
void BoxCollider::OnCollisionEvent(BoxCollider* pBox)
{
	if (_collisionEvent == false) return;

	innerOnCollisionEvent(pBox);
}

// 更新処理
void BoxCollider::update()
{
	// 更新前のものを退避しておくの。
	_prevBox = _box;

	innerUpdate();
}


// 矩形描画（主にデバッグ用やね。）
void BoxCollider::draw()
{
	innerDraw();

	if (_debug) {
		drawBoxEx(_box, GetColor(255, 255, 255), FALSE);
		DrawFormatString((int)_box.pos.x, (int)_box.pos.y, 0xFF9999, "x=%.0lf y=%.0lf w=%.0lf h=%.0lf",
			_box.pos.x, _box.pos.y, _box.size.w, _box.size.h);
		DrawFormatString((int)_box.pos.x, (int)_box.pos.y+16, 0xFF9999, "x=%.0lf y=%.0lf w=%.0lf h=%.0lf",
			_prevBox.pos.x, _prevBox.pos.y, _prevBox.size.w, _prevBox.size.h);
	}

}