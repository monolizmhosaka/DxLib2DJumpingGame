#include <algorithm>

#include "Collider.h"

#include "DxLibEx.h"
#include "BoxCollider.h"

// 唯一のインスタンス
Collider* Collider::pInstance = nullptr;
Collider* Collider::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new Collider();
	}
	return pInstance;
}

void Collider::create()
{
	if (pInstance == nullptr)
	{
		pInstance = new Collider();
	}
}

void Collider::destroy()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

// 比較関数定義
bool compareByBoxY(const BoxCollider* a, const BoxCollider* b) {

	return a->getBox().pos.y < b->getBox().pos.y;
}

void Collider::push(BoxCollider* pBox)
{
	_pBoxList.push_back(pBox);

	// 本当は挿入先を検索しながら、、、でいいんだけどね.というかそっちのが早い
	// でもソート関数の使い方もかねてるん。
	std::sort(_pBoxList.begin(), _pBoxList.end(), compareByBoxY);
}


void Collider::erase(BoxCollider* pBox)
{
	auto it = std::find(_pBoxList.begin(), _pBoxList.end(), pBox);
	_pBoxList.erase(it);
}

void Collider::updateCollision()
{
	// 全ての組み合わせを当たり判定処理
	//// 当たり判定
	for (auto it = _pBoxList.begin(); it != _pBoxList.end(); it++)
	{
		for (auto it2 = it + 1; it2 != _pBoxList.end(); it2++)
		{

			BOX box = (*it)->getBox();
			BOX prevBox = (*it)->getPrevBox();

			BOX box2 = (*it2)->getBox();
			BOX prevBox2 = (*it2)->getPrevBox();

			// それぞれの速度を求め相対速度を求める
			VECTOR_D box1Volocity = { box.pos.x - prevBox.pos.x, box.pos.y - prevBox.pos.y, .0f };
			VECTOR_D box2Volocity = { box2.pos.x - prevBox2.pos.x, box2.pos.y - prevBox2.pos.y, .0f };
			VECTOR_D v = VSubD(box1Volocity, box2Volocity);
			
			// 惹かれた方が実際に相対座標上の位置を求める


			// とりあえず刻んで当たり判定を厳密に行う
			//printfDx("vx=%lf vy=%lf \n", v.x, v.y);
			for (int i = 1; i <= 10; i++) {
				BOX relativeBox = { prevBox.pos.x + (v.x / i), prevBox.pos.y + (v.y / i), box.size.w, box.size.h };
				BOX relativeBox2 = prevBox2;

				if (collisionBoxBox(relativeBox, relativeBox2))
					//if (collisionBoxBox(box, box2))
				{
					if ((*it)->getTag() == BoxCollider::PLAYER) printfDx("1i=%d\n", i);
					if ((*it2)->getTag() == BoxCollider::PLAYER) printfDx("2i=%d\n", i);

					//!!!□ここは本来、当たり判定が行われて実際に当たった時のBOX構造体をもつ
					//! //colliderを渡す必要がある。そうしないと個々の処理と会わなくなる。
					
					// 当たったタイミングの本当の位置を求める
					//relativeBox.pos.x = prevBox.pos.x + (box1Volocity.x * i / 10.0);
					//relativeBox.pos.y = box.pos.y;

					//relativeBox2.pos.x = prevBox2.pos.x + (box2Volocity.x * i / 10.0);
					//relativeBox2.pos.y = box2.pos.y;

					////両方とも呼び出すことにする。いい方法ないかな。。。
					//(*it)->OnCollisionEvent(*it2, relativeBox, relativeBox2);
					//(*it2)->OnCollisionEvent(*it, relativeBox2, relativeBox);
					(*it)->OnCollisionEvent(*it2);
					(*it2)->OnCollisionEvent(*it);
					break;

				}
			}
		}
	}
}