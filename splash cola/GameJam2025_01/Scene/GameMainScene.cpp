#include "GameMainScene.h"
#include "DxLib.h"

GameMainScene::GameMainScene()
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Update()
{
}

void GameMainScene::Draw() const
{
	DrawFormatString(0, 0, 0xffffff, "GameMain");
}

AbstractScene* GameMainScene::Change()
{
	return this;
}
