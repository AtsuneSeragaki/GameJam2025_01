#pragma once

#include "../../Utility/Vector2D.h"

class Bard
{
private:
	Vector2D location;  // 表示する中心座標
	double angle;       // 画像の角度
	int img;            // 画像データ
	bool is_hit;        // 泡に当たったか？
	double extrate;     // 画像の拡大率
	bool left_flg;      // 左に飛んでいくか？
	int num;            // 表示する障害物の番号
	int attack_se;		// 飛んでいく時のSE

public:
	// コンストラクタ
	Bard(int num);
	// デストラクタ
	~Bard();

public:
	// 更新処理
	void Update();
	// 描画処理
	void Draw() const;

public:
	// 座標を返す処理
	Vector2D GetLocation() { return location; }
};