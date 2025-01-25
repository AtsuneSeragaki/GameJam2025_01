#include "Bar.h"
#include "DxLib.h"

Bar::Bar()
{
	cnt_bar_shake = 0;
	is_bottom_hit = true;
	is_upper_hit = true;
	cnt_bottom_hit = 0.0f;
	cnt_upper_hit = 0.0f;
}

Bar::~Bar()
{
}

void Bar::Update()
{
	InputManager* input=InputManager::GetInstance();
	
	if (input->GetMouseLocation().y < 20)
	{
		is_upper_hit = false;
		is_bottom_hit = true;
	}

	if (input->GetMouseLocation().y > 440)
	{
		is_upper_hit = true;
		is_bottom_hit = false;
	}


}

void Bar::Draw() const
{
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
