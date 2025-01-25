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
}

AbstractScene* GameMainScene::Change()
{
	return this;
}

void GameMainScene::InGameUpdate()
{
}

void GameMainScene::InStartUpdate()
{
}

void GameMainScene::InGameResultUpdate()
{
}
