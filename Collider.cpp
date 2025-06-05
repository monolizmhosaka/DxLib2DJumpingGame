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
//	_pBoxList.push_back(pBox);

	// 本当は挿入先を検索しながら、、、でいいんだけどね.というかそっちのが早い
	// でもソート関数の使い方もかねてるん。
//	std::sort(_pBoxList.begin(), _pBoxList.end(), compareByBoxY);
	auto it = _pBoxList.begin();
	for (; it != _pBoxList.end(); it++)
	{
		double y = (*it)->getBox().pos.y;
		if (y > pBox->getBox().pos.y)
		{
			//printfDx("%d>%d\n", (int)y, (int)pBox->getBox().pos.y);
			it = _pBoxList.insert(it, pBox);
			break;
		}
	}
	// 見つかっていない場合は最後に入れる
	if (it == _pBoxList.end()) _pBoxList.push_back(pBox);
	
	//for (auto it = _pBoxList.begin(); it != _pBoxList.end(); it++)
	//{
	//	double y = (*it)->getBox().pos.y;
	//	printfDx("%d ", (int)y);
	//}
	//printfDx("\n");

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
			VECTOR_D box1v = { box.pos.x - prevBox.pos.x, box.pos.y - prevBox.pos.y, .0f };
			VECTOR_D box2v = { box2.pos.x - prevBox2.pos.x, box2.pos.y - prevBox2.pos.y, .0f };
			
			// とりあえず刻んで当たり判定を厳密に行う
			for (int i = 10; i >= 0; i--) {
				BOX relativeBox = { prevBox.pos.x + (box1v.x * i/10), prevBox.pos.y + (box1v.y * i / 10), box.size.w, box.size.h };
				BOX relativeBox2 = { prevBox2.pos.x + (box2v.x * i / 10), prevBox2.pos.y + (box2v.y * i / 10), box2.size.w, box2.size.h };

				if (collisionBoxBox(relativeBox, relativeBox2))
				{
					(*it)->setHitedBox(relativeBox);
					(*it2)->setHitedBox(relativeBox2);

					(*it)->OnCollisionEvent(*it2);
					(*it2)->OnCollisionEvent(*it);
					break;

				}
			}
		}
	}
}