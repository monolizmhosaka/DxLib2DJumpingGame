#include "Player.h"

#include "Paddle.h"
#include "Camera.h"

#include "PlayerInfo.h"

Player::Player()
{
	_soundHandle = LoadSoundMem("oops.wav");
	//_gHandle = LoadGraph("Player2.png");	// けいたくん
	_gHandle = LoadGraph("yuta/PlayerJump.bmp");	// ゆうた
	_gHandleWait = LoadGraph("yuta/PlayerWait.bmp");	// ゆうた

	// 当たり判定用のボックス　＝　画像位置も兼ねてます。
	INT2 i2 = { 0,0 };
	GetGraphSize(_gHandle, &i2.x, &i2.y);
	i2.x /= SPLIT_W;	// １つ当たりのセルの大きさを求める
	_box.size.w = i2.x;
	_box.size.h = i2.y;
	_box.pos.x = WIN_SIZE.x / 2.0;

	// 初期化
	init();
}

Player::~Player()
{
	DeleteGraph(_gHandle);
	DeleteSoundMem(_soundHandle);
}

void Player::init()
{
	_box.pos.y = 0;		// 上から降ってくる
	_prevBox.pos.y = 0;

	_upPower = 0;	// ジャンプの加速度
	_jumpFlag = 1;	// ジャンプ中フラグ
	_animationCount = 0;

	_rotate = 0;		// ラジアンね。
	_dieFlag = false;

	setCollisionEnable(true);	// 当たり判定有効
}

// 現在の上下の加速度
int Player::getJumpVector() const
{
	return (_upPower > 0) - (_upPower < 0);
}

void Player::innerUpdate()
{
	static int oldKey = 0;
	static int adjCnt = 0;	// ジャンプをいい感じにする調整用

	// カメラ座標空間に
	_box.pos.y -= Camera::y;

	if (_dieFlag == true)
	{ //死んでるとき 死んでるアニメションして終わる感じにするか。
		_box.pos.y -= _upPower;
		_upPower -= 4;
		_rotate += 0.5;
		_rotate = _rotate < 2 * PI ? _rotate : 0;
	}
	else
	{
		// キー入力取得
		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		_box.pos.y = _box.pos.y - _upPower;
		_upPower = _upPower - GRAVITY;

		// 落下処理　落下加速度を加える
#ifdef TAIKEN_JUMP








#include "Jump.h"

















#else
		if ((key & PAD_INPUT_M) && !(oldKey & PAD_INPUT_M))
		{// 前フレーム押されていなくて、今回おされたら
			if (_jumpFlag == FALSE)
			{//ジャンプしていないならジャンプ開始。
				_upPower = 10;

				_jumpFlag = TRUE;
				_animationCount = 1;	// アニメ開始
				adjCnt = 0;
			}
		}
		else if ((key & PAD_INPUT_M) && (oldKey & PAD_INPUT_M) && isJump())
		{ //継続して押されているなら、ジャンプの飛距離を伸ばす
			if (adjCnt < 5) {
				_upPower += 5;
			}
			else if (adjCnt < 30) {
				_upPower += 3;
			}
			adjCnt++;
		}
#endif

		// アニメーション制御　上昇中と下降中で切り分け
		if (_jumpFlag == FALSE)
		{
			_animationCount++;
			if (_animationCount > TO_ANIMATION_WAIT_TIME(7)) _animationCount = 0;
		}
		else
		{//ジャンプ中
			//WaitKey();
			if (getJumpVector() >= 0)
			{//上昇中か頂点
				if (_animationCount < TO_ANIMATION_JUMP_TIME(4)) _animationCount++;
			}
			else if (getJumpVector() == -1)
			{//下降中
				if (_animationCount < TO_ANIMATION_JUMP_TIME(7)) _animationCount++;
			}
		}

		// 入力キー退避
		oldKey = key;
	}
}

void Player::innerDraw()
{
	BOX box = _box;

	if (isJump())
	{
		DrawRectRotaGraph(int(box.pos.x), int(box.pos.y),
			32 * (_animationCount / ANIMATION_JUMP_WAIT), 0, 32, 64,
			1.0, _rotate, _gHandle, TRUE, FALSE);
	}
	else
	{
		DrawRectRotaGraph(int(box.pos.x), int(box.pos.y),
			32 * (_animationCount / ANIMATION_WAIT_WAIT), 0, 32, 64,
			1.0, _rotate, _gHandleWait, TRUE, FALSE);
	}

}

// 衝突イベント
void Player::innerOnCollisionEvent(BoxCollider* tBox)
{
	if (tBox->getTag() == BoxCollider::ColliderTAG::DEAD_BOX)
	{// 死亡なり。
		_dieFlag = true;
		_upPower = 50;
		setCollisionEnable(false);

		// 読みこんだ音をノーマル再生します(『PlaySoundMem』関数使用)
		PlaySoundMem(_soundHandle, DX_PLAYTYPE_BACK);
	}
	else
	{
		if (getJumpVector() == -1) {
			// 下降中

			// ■！！！２つのパドルの高さがあまりないと当たり判定の順番のせいで
			// 下側が最終的に着地点として判定されることがある。　パドルをソートするか。パドルの高さとかでソートするか。
			// 
			// 足元がパドル矩形底辺よりも上なら か
			// 足元がパドル矩形底辺よりも下　かつ　
			// 前のフレームの位置がパドル矩形より上なら、当たったことにするので。
			if ((_box.pos.y + _box.size.h / 2 < tBox->getBox().pos.y + tBox->getBox().size.h / 2) ||
				(_box.pos.y + _box.size.h / 2 > tBox->getBox().pos.y + tBox->getBox().size.h / 2) &&
				(_prevBox.pos.y + _prevBox.size.h / 2 < tBox->getBox().pos.y + tBox->getBox().size.h / 2))
			{
				Paddle* p = dynamic_cast<Paddle*>(tBox);
				if (p != nullptr) {
					//パドルなら
					if (isJump() == TRUE || (isJump() == FALSE && p->isFreeze()))
					{// ジャンプ中、またはジャンプ中ではなければフリーズ物体ならそのまま
						_box.pos.y = (tBox->getBox().pos.y - tBox->getBox().size.h / 2 - _box.size.h / 2);
						//printfDx("_box.pos.y=%d\n", (int)_box.pos.y);
						_upPower = 0;
						_jumpFlag = FALSE;

						p->freeze();
						p->setBox(p->getHitedBox());
					}
				}
			}
		}
	}
}