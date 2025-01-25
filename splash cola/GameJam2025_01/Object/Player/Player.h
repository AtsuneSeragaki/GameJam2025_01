#pragma once

#include "../../Utility/Vector2D.h"

class Player
{
private:
	Vector2D location; // 手とコーラの座標
	int img;           // 画像データ

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
};