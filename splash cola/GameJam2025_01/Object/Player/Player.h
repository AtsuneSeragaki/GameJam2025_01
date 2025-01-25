#pragma once

#include "../../Utility/Vector2D.h"

class Player
{
private:
	Vector2D location; // 手とコーラの座標
	int img;           // 画像データ

public:
	Player();
	~Player();

public:
	void Update();
	void Draw();
};