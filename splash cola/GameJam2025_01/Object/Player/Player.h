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


public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

public:
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	void ResultUpdate();
	void ResultDraw();
};