#include "Bar.h"
#include "DxLib.h"

#include "../../Utility/ResourceManager.h"
#include "../../Object/Star/Star.h"
#define _USE_MATH_DEFINES
#include <math.h>

Bar::Bar()
{
	cnt_bar_shake = 0;
	is_bottom_hit = true;
	is_upper_hit = true;
	second_cnt = 0.0f;
	second_bonus = 0;

	ResourceManager* resource = ResourceManager::GetInstance();
	touch_se = resource->GetSounds("Resource/Sounds/SE/touch.mp3");
	star_se = resource->GetSounds("Resource/Sounds/SE/star.mp3");

	// 音量調整
	ChangeVolumeSoundMem(150, touch_se);
	ChangeVolumeSoundMem(150, star_se);
}

Bar::~Bar()
{
}

void Bar::Initialize()
{
	cnt_bar_shake = 0;
	is_bottom_hit = true;
	is_upper_hit = true;
	second_cnt = 0.0f;
	second_bonus = 0;
}

void Bar::Finalize()
{
	if (stars.empty())
	{
		// starsが空だったら関数終了
		return;
	}

	// 動的配列の要素を削除
	stars.clear();
}

void Bar::Update()
{
	InputManager* input=InputManager::GetInstance();
	
	if (input->GetMouseLocation().y < 20 && is_upper_hit==true)
	{
		// タッチSE再生
		PlaySoundMem(touch_se, DX_PLAYTYPE_BACK, TRUE);

		if (second_cnt < 0.6f)
		{
			second_bonus += 1; 

			// 星エフェクト生成SE
			PlaySoundMem(star_se, DX_PLAYTYPE_BACK, TRUE);

			int num = rand() % 4;
			Vector2D star_center(60.0f * (float)num + 300.0f, 30.0f);
			// 星の生成
			CreateStar(star_center);
		}
		is_upper_hit = false;
		is_bottom_hit = true;
		cnt_bar_shake += 0.5;
		second_cnt = 0.0f;
	}

	if (input->GetMouseLocation().y > 440 && is_bottom_hit==true)
	{
		// タッチSE再生
		PlaySoundMem(touch_se, DX_PLAYTYPE_BACK, TRUE);

		if (second_cnt < 0.6f)
		{
			second_bonus += 1; 

			// 星エフェクト生成SE
			PlaySoundMem(star_se, DX_PLAYTYPE_BACK, TRUE);

			int num = rand() % 4;
			Vector2D star_center(60.0f * (float)num + 220.0f, 450.0f);
			// 星の生成
			CreateStar(star_center);
		}
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

	for (Star* star : stars)
	{
		// 星の描画
		star->Draw();
	}
}

// 星エフェクトの生成
void Bar::CreateStar(Vector2D center)
{
	for (int i = 0; i < 4; i++)
	{
		// 各星の角度を計算
		double angle = 2 * M_PI * i / 4;

		// 角度から移動方向を計算
		Vector2D direction(cosf((float)angle), sinf((float)angle));

		// 星を作成し、リストに追加
		stars.push_back(new Star(center, direction));
	}
}

void Bar::StarUpdate()
{
	for (Star* star : stars)
	{
		// 星の更新
		star->Update();
	}
}
