#pragma once

#include <list>
#include <vector>

using namespace std;

#include "BoxCollider.h"

class Collider
{
public:
	// 当たり判定の処理を行います。毎フレーム呼び出すこと。
	void updateCollision();

	// 当たり判定を行うやつ登録
	void push(BoxCollider*);
	void erase(BoxCollider*);

	using ExclusionPair = std::pair<BoxCollider::ColliderTAG, BoxCollider::ColliderTAG>;
	void addExclusion(ExclusionPair pair);

	Collider(const Collider&) = delete;	// コピーコンストラクタ禁止
	Collider& operator=(const Collider&) = delete;	// 代入も禁止
private:
	Collider() {};
	virtual ~Collider() {};

	// 挿入が頻繁にあるかもなのでlistのがいい。
	list<BoxCollider*> _pBoxList;

	// 当たり判定の組み合わせ除外リスト
	vector<ExclusionPair> _exclusionList;

private:
	static Collider* pInstance;

public:
	static Collider* getInstance();
	static void create();	// 基本的に使用する際には呼び出す
	static void destroy();	// 必ず呼び出す
};

