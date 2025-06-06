#include <sstream>

#include "PaddleFactory.h"

#include "Paddle.h"
#include "Collider.h"
#include "Camera.h"
#include "Player.h"

const DOUBLE2 paddleSize = {95, 16};	// ちょうどいいのは95, 16
const INT2 paddleSpeedRange = {2, 8};	// ちょうどいいのは2, 8
struct LEVEL_STEP {
	double timing;	// yの速度　これ以下の時
	INT2 range;	// min 、　maxの出現スピード　となる
};
const LEVEL_STEP levelStep[] =
{// 必ずtimingは昇順になっていること。
	// たとえば一番上はスクロールスピードが-2.0以下だったとき、
	// 10フレ～20フレで出現する範囲が決定する。
	{  -2.0,  10,  20},
	{  -1.5,  20,  30},
	{ -1.25,  30,  60},	// 早くなってくるので出やすくする
	{ -1.00,  50, 120},	// 結構でやすい
	{ -0.75,  70, 200},	// 出やすくなってくる　ここからむずくなるから
	{ -0.50,  80, 400},	// ちょっとずつ難しく 出なくなってくる
	{ -0.25,  70, 300},	// さらにちょっと出やすく 一時的に
	{ -0.15,  80, 400},	// ちょっと出やすくする 一時的に
	{  0.00, 100, 600}	// かなりでない
};


// 唯一のインスタンス
PaddleFactory* PaddleFactory::pInstance = nullptr;
PaddleFactory* PaddleFactory::getInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new PaddleFactory();
	}
	return pInstance;
}

void PaddleFactory::create()
{
	if (pInstance == nullptr)
	{
		pInstance = new PaddleFactory();
	}
}

void PaddleFactory::destroy()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

void PaddleFactory::init()
{
	_startTime = -1;
	_durationTime = 0;

	// 全て解放しパドルをリセットする。
	for (auto it = _paddles.begin(); it != _paddles.end(); it++)
	{
		Collider::getInstance()->erase(*it);	// 衝突配列から削除
		delete (*it);
	}
	_paddles.clear();
}

void PaddleFactory::update(const Player& player)
{
	// 経過時間の難易度調整 これは全体スクロールの難度調整
	if ( _startTime == -1 ) _startTime = GetNowCount();
	_durationTime = (GetNowCount() - _startTime) / 1000;	// ミリ秒から秒へ
	Camera::y = -_durationTime / 100.f;
	//printfDx("y=%lf\n", Camera::y);

	//スクロール速度に応じてパドル出力間隔を調整
	int spanLow = 0, spanUp = 1;
	for (int i = 0; i < sizeof(levelStep) / sizeof(LEVEL_STEP); i++)
	{
		if (Camera::y <= levelStep[i].timing)
		{
			spanLow = levelStep[i].range.a;
			spanUp = levelStep[i].range.b;
			break;
		}
	}

	DOUBLE2 playerPos;
	player.getPosition(&playerPos);
	if (_firingSpan < 0)
	{
		//発射方向を決める
		double startPosX = -100;
		int vNorm = 1;
		if (rand() % 100 < 50 )
		{
			startPosX = WIN_SIZE.x + 100;
			vNorm = -1;
		}


		DOUBLE2 d2 = playerPos;
		d2.y +=  (rand() % 150 * -1);
		Paddle* p = new Paddle(BOX{ startPosX, d2.y, paddleSize.x, paddleSize.y});
		p->setColor(255, 255, 255);
		p->setVecolity((paddleSpeedRange.a + (rand()% paddleSpeedRange.b)) * vNorm);

		// コライダー登録
		Collider::getInstance()->push(p);

		// 追加
		_paddles.push_back(p);

		// 出現タイムを再設定 死にそうなら結構出すように調整
		_firingSpan = spanLow + rand() % spanUp;
	}
	else
	{// 時間が来ていなくても、死にそうなら出しちゃえ10倍即
		if (playerPos.y > WIN_SIZE.y - 200) {
			_firingSpan -= 5;
		}
	}
	_firingSpan--;

	// 画面外判定していないやつは削除 vectorを走査して削除するのはきをつけるべし
	// 以下は安全に削除するやり方
	auto itr = _paddles.begin();
	while (itr != _paddles.end())
	{
		if ( (*itr)->isDie() == true)
		{
			delete (*itr);	// 解放
			Collider::getInstance()->erase(*itr);	// 衝突配列から削除
			itr = _paddles.erase(itr);	// パドル管理から削除
		}
		else
		{
			itr++;
		}
	}

	// 全アップデータ
	ITERATOR_CALL_FUNC(_paddles, update);
}

void PaddleFactory::drawDurationTime()
{
	stringstream ss;
	ss << "ふんばった時間：" << _durationTime << "秒";
	DrawString(30, 30, ss.str().c_str(), 0xFFFFFF);
}


void PaddleFactory::draw()
{
	// めんどいからここで出す
	drawDurationTime();

	ITERATOR_CALL_FUNC(_paddles, draw);
}

Paddle* PaddleFactory::getPaddle(int i) const
{
	if (i < 0 || _paddles.size() <= i) return nullptr;

	return _paddles[i];
}

void PaddleFactory::createPaddle(BOX bbox)
{
	Paddle* p = new Paddle(bbox);

	// コライダー登録
	Collider::getInstance()->push(p);

	// 追加
	_paddles.push_back(p);
}
