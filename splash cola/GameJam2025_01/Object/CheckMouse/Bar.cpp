#include "Bar.h"
#include "DxLib.h"

Bar::Bar()
{
	cnt_bar_shake = 0;
	is_bottom_hit = true;
	is_upper_hit = true;
	second_cnt = 0.0f;
	second_bonus = 0;
}

Bar::~Bar()
{
}

void Bar::Update()
{
	InputManager* input=InputManager::GetInstance();
	
	if (input->GetMouseLocation().y < 20 && is_upper_hit==true)
	{
		if (second_cnt < 3.0f) { second_bonus += 1; }
		is_upper_hit = false;
		is_bottom_hit = true;
		cnt_bar_shake += 0.5;
		second_cnt = 0.0f;
	}

	if (input->GetMouseLocation().y > 440 && is_bottom_hit==true)
	{
		if (second_cnt < 3.0f) { second_bonus += 1; }
		is_upper_hit = true;
		is_bottom_hit = false;
		cnt_bar_shake += 0.5;
		second_cnt = 0.0f;
	}

	second_cnt += 0.1f;

}

void Bar::Draw() const
{
	DrawFormatString(0, 40, 0x000000, "shake%f", cnt_bar_shake);
	DrawFormatString(0, 60, 0x000000, "cnt%f", second_cnt);
	DrawFormatString(0, 80, 0x000000, "bonus%d", second_bonus);

	if (is_upper_hit == true) {
		DrawBox(0, 20, 640, 40, 0xff0000, TRUE);
	}
	else {
		DrawBox(0, 20, 640, 40, 0x000000, TRUE);
	}

	if (is_bottom_hit == true)
	{
		DrawBox(0, 440, 640, 460, 0xff0000, TRUE);
	}
	else {
		DrawBox(0, 440, 640, 460, 0x000000, TRUE);
	}
}
