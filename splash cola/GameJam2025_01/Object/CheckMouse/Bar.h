#pragma once
#include "../../Utility/InputManager.h"
#include <vector>
class Bar
{
private:

	float cnt_bar_shake;//降った回数のカウント
	bool is_bottom_hit;//上のバーにヒットしていいか
	bool is_upper_hit;//下のバーにヒットしていいか

	float second_cnt;//下のバーに触れられるまでのカウント
	//float cnt_upper_hit;//上のバーに触れられるまでのカウント
	
	int second_bonus;//0,6f以内にバーにヒットしたら加算

	int touch_se;		// バーに触れた時のSE

	std::vector<class Star*> stars;		// 星エフェクト
	int star_se;						// 星エフェクト生成SE

public:
	Bar();
	~Bar();

	void Initialize();
	void Finalize();
	void Update();
	void Draw() const;
	float GetCntBarShake() { return cnt_bar_shake; }
	float GetSecondBonus() { return second_bonus; }

	// 星エフェクトの生成
	void CreateStar(Vector2D center);
	void StarUpdate();
};

