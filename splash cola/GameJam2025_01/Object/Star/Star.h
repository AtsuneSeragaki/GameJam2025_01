#pragma once
#include "../../Utility/Vector2D.h"

class Star
{
private:
	Vector2D location;		// 座標
	Vector2D direction;		// 移動方向
	float speed;			// 移動速度
	float count;
	int star_image;			// 星画像
	bool delete_flg;		// 削除フラグ
	int alpha;
	double angle;			// 画像の角度
	double degree;			// 画像の角度計算用

public:
	Star(Vector2D set_location, Vector2D set_direction);
	~Star();

	void Initialize();
	void Update();
	void Draw() const;

	bool GetDeleteFlg() const;
};

