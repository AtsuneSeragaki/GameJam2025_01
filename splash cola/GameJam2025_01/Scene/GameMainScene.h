#pragma once
#include "AbstractScene.h"
#include "../Utility/Vector2D.h"


enum class GameState
{
	start,//
	in_game,//プレイ中
	in_fly,//結果発表中
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

	int retry_button_color;		// リトライボタンの色
	int end_button_color;		// エンドボタンの色

	bool end_flg;				// ゲーム終了フラグ

	class Bar* bar;//上と下の判定のバー

	class Player* player; // プレイヤー

	int background_img;  // 背景画像

	float background_y; // 背景Y座標


	int in_game_bgm;			// インゲームのBGM
	int count_down_se;			// スタートのカウントダウンSE
	int start_se;				// ゲーム開始SE
	int end_se;					// ゲーム終了SE

	bool play_count_down_se;	// スタートのカウントダウンSE再生フラグ


	int bubble_num;
	int bubble_cnt;
	int bubble_img[2];//泡画像
	Vector2D bubble_location;
	float up_bubble;
	
	float score;

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
	void InFlyUpdate();//結果発表中のアップデート
	void InGameResultUpdate();//ゲームクリアアップデート

	void ColaBubbleUpdate();//Colaの泡更新処理

	void RetryButtonUpdate();		// リトライボタンの更新処理
	void EndButtonUpdate();			// エンドボタンの更新処理

};

