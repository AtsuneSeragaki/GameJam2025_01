#include "GameMainScene.h"
#include "DxLib.h"

GameMainScene::GameMainScene()
{
	game_state = GameState::start;

	fps_count = 0;
	start_count = 3;
	timer = 10;			// 10秒でリザルト表示
	bar = new Bar;
}

GameMainScene::~GameMainScene()
{
	delete bar;
}

// 初期化処理
void GameMainScene::Initialize()
{
	fps_count = 0;
	start_count = 3;
	timer = 10;			// 10秒でリザルト表示
}

void GameMainScene::Update()
{
	switch (game_state)
	{
	case GameState::start:
		InStartUpdate();
		break;
	case GameState::in_game:
		InGameUpdate();
		break;
	case GameState::result:
		InGameResultUpdate();
		break;
	}
}

void GameMainScene::Draw() const
{
	DrawFormatString(0, 0, 0xffffff, "GameMain");
	switch (game_state)
	{
	case GameState::start:
		DrawFormatString(0, 20, 0xffffff, "Start");
		DrawFormatString(0, 180, 0xffffff, "Shake the cola!!!");
		if (start_count > 0)
		{
			DrawFormatString(0, 200, 0xffffff, "count: %d", start_count);
		}
		else
		{
			DrawFormatString(0, 200, 0xffffff, "start!!!");
		}
		break;
	case GameState::in_game:
		DrawFormatString(0, 20, 0xffffff, "InGame");
		DrawFormatString(0, 200, 0xffffff, "timer: %d", timer);
		bar->Draw();
		break;
	case GameState::result:
		DrawFormatString(0, 20, 0xffffff, "Result");
		break;
	}
}

AbstractScene* GameMainScene::Change()
{
	return this;
}

void GameMainScene::InStartUpdate()
{
	if (fps_count < 60)
	{
		fps_count++;
	}
	else
	{
		start_count--;
		fps_count = 0;
	}

	if (start_count < 0)
	{
		// ゲーム開始
		game_state = GameState::in_game;
	}
}

void GameMainScene::InGameUpdate()
{
	if (fps_count < 60)
	{
		fps_count++;
	}
	else
	{
		timer--;
		fps_count = 0;
	}

	if (timer < 0)
	{
		// ゲーム終了・リザルトへ
		game_state = GameState::result;
	}

	bar->Update();
}

void GameMainScene::InGameResultUpdate()
{

}
