#include <algorithm>
#include <utility>

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
//bool compareByBoxY(const BoxCollider* a, const BoxCollider* b) {
//
//	return a->getBox().pos.y < b->getBox().pos.y;
//}

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

// 指定されたBoxColliderをリストから削除する
void Collider::erase(BoxCollider* pBox)
{
	auto it = std::find(_pBoxList.begin(), _pBoxList.end(), pBox);
	_pBoxList.erase(it);
}

void Collider::addExclusion(ExclusionPair pair)
{
	// 検索用に入れ替えも作っておく
	_exclusionList.push_back(pair);
	std::swap(pair.first, pair.second);
	_exclusionList.push_back(pair);

}


// 当たり判定のメイン処理　全てのUpdate関数を呼び出した後に呼び出すこと。
void Collider::updateCollision()
{
	// 全ての組み合わせを当たり判定処理
	for (auto it = _pBoxList.begin(); it != _pBoxList.end(); it++)
	{
		for (auto it2 = std::next(it, 1); it2 != _pBoxList.end(); it2++)
		{
			bool flag = false;
			for (auto ite = _exclusionList.begin(); ite != _exclusionList.end(); ite++)
			{
				if ((*it)->getTag() == (*ite).first && (*it2)->getTag() == (*ite).second)
				{
					flag = true;
				}
			}
			if (flag == true) continue;

			BOX box = (*it)->getBox();
			BOX prevBox = (*it)->getPrevBox();

			BOX box2 = (*it2)->getBox();
			BOX prevBox2 = (*it2)->getPrevBox();

			// それぞれの速度を求める
			VECTOR_D box1v = { box.pos.x - prevBox.pos.x, box.pos.y - prevBox.pos.y, .0f };
			VECTOR_D box2v = { box2.pos.x - prevBox2.pos.x, box2.pos.y - prevBox2.pos.y, .0f };
			
			// とりあえず刻んで当たり判定を厳密に行う
			static int resolution = 5;
			for (int i = resolution; i >= 0; i--) {
				BOX relativeBox = { prevBox.pos.x + (box1v.x * i / resolution), prevBox.pos.y + (box1v.y * i / resolution), box.size.w, box.size.h };
				BOX relativeBox2 = { prevBox2.pos.x + (box2v.x * i / resolution), prevBox2.pos.y + (box2v.y * i / resolution), box2.size.w, box2.size.h };

				if (collisionBoxBox(relativeBox, relativeBox2))
				{
					// 衝突時典でのBOX情報を設定
					(*it)->setHitedBox(relativeBox);
					(*it2)->setHitedBox(relativeBox2);

					// 各オブジェクトの衝突処理を呼び出す
					(*it)->OnCollisionEvent(*it2);
					(*it2)->OnCollisionEvent(*it);

					break;
				}
			}
		}
	}
}