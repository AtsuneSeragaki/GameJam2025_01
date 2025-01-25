#pragma once
#include "AbstractScene.h"
#include "../Utility/Vector2D.h"
#include "../Object/CheckMouse/Bar.h"

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

	int fps_count;		// fpsカウント
	int start_count;	// ゲームスタートのカウント
	int timer;			// 制限時間

	Bar* bar;//上と下の判定のバー
public:
	GameMainScene();
	~GameMainScene();

	void Initialize();		// 初期化処理

	void Update() override;
	void Draw() const override;
	AbstractScene* Change() override;

private:
	//プレイ中
	void InStartUpdate();//ゲームスタートのアップデート
	void InGameUpdate();//ゲームプレイ中のアップデート
	void InGameResultUpdate();//ゲームクリアアップデート

	

};

