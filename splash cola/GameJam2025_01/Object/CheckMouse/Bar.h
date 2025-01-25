#pragma once
#include "../../Utility/InputManager.h"
class Bar
{
private:

	int cnt_bar_shake;//降った回数のカウント
	bool is_bottom_hit;//上のバーにヒットしていいか
	bool is_upper_hit;//下のバーにヒットしていいか

	float cnt_bottom_hit;//下のバーに触れられるまでのカウント
	float cnt_upper_hit;//上のバーに触れられるまでのカウント

public:
	Bar();
	~Bar();

	void Update();
	void Draw() const;
};

