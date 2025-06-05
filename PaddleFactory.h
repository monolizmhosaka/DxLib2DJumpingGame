#pragma once

#include "DxLibEx.h"

#include <vector>

using namespace std;

class Paddle;
class Player;

class PaddleFactory
{
public:

	void init();
	void update(const Player& player);
	void draw();

	PaddleFactory(const PaddleFactory&) = delete;	// コピーコンストラクタ禁止
	void operator=(const PaddleFactory&) = delete;	// 代入も禁止

	Paddle* getPaddle(int i) const;
	void createPaddle(BOX bbox);

private:
	PaddleFactory() {};
	virtual ~PaddleFactory() {};

private:
	static PaddleFactory* pInstance;

	int _firingSpan = 100;
	int _startTime = -1;
	int _oldTimeCount = 0;

	vector<Paddle*> _paddles;

public:
	static PaddleFactory* getInstance();
	static void create();	// 基本的に使用する際には呼び出す
	static void destroy();	// 必ず呼び出す
};

