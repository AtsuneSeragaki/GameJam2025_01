#pragma once

#include "../../Utility/Vector2D.h"

class Player
{
private:
	Vector2D location; // 手とコーラの座標
	double angle;      // 画像の角度
	int img;           // 画像データ
	int img_cola[4];   // 画像データ(蓋を開けるアニメーション)
	int cola_num;      // 表示する画像の番号
	int cola_cnt;      // 画像切り替え時間

	float hand_down;        // 画像を下げるか？
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
	// 時間終了した後の更新処理
	void ResultUpdate();
	// 時間終了した後の描画処理
	void ResultDraw();
	// 表示する画像の番号を返す処理
	int GetColaNum() { return cola_num; }
};