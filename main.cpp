#include <time.h>

#include "DxLibEx.h"

#include "Player.h"
#include "Paddle.h"
#include "Collider.h"
#include "PaddleFactory.h"

// トゲトゲ描画
void drawSplinter()
{
	const int HEIGHT = 30;
	const int X_SIZE = 20;

	// トゲトゲ地獄を表示
	for (int x = 0; x < WIN_SIZE.x / X_SIZE; x++)
	{
		int ofsX = x * X_SIZE;
		DrawTriangle(X_SIZE /2 + ofsX, WIN_SIZE.y-HEIGHT, 0 + ofsX, WIN_SIZE.y, X_SIZE + ofsX,
			WIN_SIZE.y, GetColor(255, 0, 0), FALSE);
	}
}

// あらゆる独立した初期化
int init()
{
	srand((unsigned int)time(NULL));

	// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	SetWindowText("上に登ってゲー");

	SetGraphMode(WIN_SIZE.x, WIN_SIZE.y, 16);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// あらゆるい初期化処理
	if (init() == -1) return -1;

	// プレイヤー情報
	Player player;
	player.setTag(BoxCollider::ColliderTAG::PLAYER);
	player.debugValidFlag(false);	// デバッグ表示ON/OFF

	//画面外に左右下の画面外判定用BOX
	Paddle screenOutBoxes[] =
	{
		Paddle(SCN_OUT_BOXES[0]),Paddle(SCN_OUT_BOXES[1]),Paddle(SCN_OUT_BOXES[2])
	};

	// トゲトゲ当たり用
	Paddle deadBox(BOX{ WIN_SIZE.x / 2.0, double(WIN_SIZE.y - 15), double(WIN_SIZE.x), 30 });
	deadBox.setTag(BoxCollider::ColliderTAG::DEAD_BOX);
	deadBox.visibility(false);

	// コライダーを生成
	Collider::create();
	Collider* pCollider = Collider::getInstance();

	// コライダーに登録
	pCollider->push(&player);
	for (int i = 0; i < sizeof(SCN_OUT_BOXES) / sizeof(BOX); i++)
	{
		screenOutBoxes[i].setTag(BoxCollider::ColliderTAG::SCN_OUT_BOX);
		pCollider->push(&screenOutBoxes[i]);
	}
	pCollider->push(&deadBox);

	// BGM　ループ再生
	int soundHandle = LoadSoundMem("bgm.wav");
	PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);

	// パドルマネージャー生成
	PaddleFactory::create();
	PaddleFactory* pPaddleMgr = PaddleFactory::getInstance();
	pPaddleMgr->init();

	// お立ち台作成 フリーズ状態
	pPaddleMgr->createPaddle(STAGE_BOX);
	pPaddleMgr->getPaddle(0)->freeze();

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 描画対象クリア
		ClearDrawScreen();

		// --- 更新系処理
		player.update();
		if (player.isScreenArea(200) == false && player.isDie()) 
		{//一定範囲の画面外に出て、かつ死んでる場合はゲームループ

			StopSoundMem(soundHandle);
			DrawString(30, 30, "おわり　なんか押して", 0xFFFFFF);

			DrawString(50, 50, "チーフプログラマ：ホッピー", 0xFFFFFF);
			DrawString(50, 70, "サブプログラマ：あなた", 0xFFFFFF);
			DrawString(50, 90, "絵の製作：どうなつ", 0xFFFFFF);
			ScreenFlip();
			WaitKey();

			// 初期化して
			PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);

			player.init();
			pPaddleMgr->init();
			pPaddleMgr->createPaddle(STAGE_BOX);
			pPaddleMgr->getPaddle(0)->freeze();

		}

		pPaddleMgr->update(player);

		// 子リジョン処理を実施
		pCollider->updateCollision();

		// --- 描画処理画面を初期化する
		player.draw();
		pPaddleMgr->draw();

		drawSplinter();
		deadBox.draw();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// サウンドハンドルの削除
	DeleteSoundMem(soundHandle);

	// 色々解放
	Collider::destroy();
	PaddleFactory::destroy();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}