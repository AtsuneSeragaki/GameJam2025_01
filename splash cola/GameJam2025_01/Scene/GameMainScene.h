#pragma once
#include "AbstractScene.h"
#include "../Utility/Vector2D.h"

enum class GameState
{
	start,//
	in_game,//プレイ中
	result,
};


class GameMainScene :
    public AbstractScene
{
private:
	GameState game_state;//現在のゲームメインの状態
public:
	GameMainScene();
	~GameMainScene();

	void Update() override;
	void Draw() const override;
	AbstractScene* Change() override;

private:
	//プレイ中
	void InStartUpdate();//ゲームスタートのアップデート
	void InGameUpdate();//ゲームプレイ中のアップデート
	void InGameResultUpdate();//ゲームクリアアップデート

	

};

