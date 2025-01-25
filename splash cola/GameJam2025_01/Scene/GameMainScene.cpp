#include "GameMainScene.h"
#include "DxLib.h"

GameMainScene::GameMainScene()
{
	game_state = GameState::in_game;
}

GameMainScene::~GameMainScene()
{
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
		break;
	case GameState::in_game:
		DrawFormatString(0, 20, 0x000000, "InGame");
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
	
}

void GameMainScene::InGameUpdate()
{
}

void GameMainScene::InGameResultUpdate()
{

}
