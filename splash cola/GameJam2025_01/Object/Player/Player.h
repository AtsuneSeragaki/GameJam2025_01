#pragma once

#include "../../Utility/Vector2D.h"

class Player
{
private:
	Vector2D location; // 手とコーラの座標
	double angle;      // 画像の角度
	int img;           // 画像データ
	int img_cola[4];//画像データ
	int cola_num;
	int cola_cnt;

	float hand_down;
	int open_can_se;		// 缶を開けるSE
	int up_se;				// 噴き上がるSE
	bool play_open_can_se;	// 缶を開けるSE再生フラグ

public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

public:
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw() const;

	void ResultUpdate();
	void ResultDraw();
	void SetBubbleHeight();
	int GetColaNum() { return cola_num; }
};