#include "Bard.h"

#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <vector>

Bard::Bard(int num)
{
	// 変数の初期化
	location.x = 320.0f;
	location.y = 0.0f;
	angle = 0.0f;
	is_hit = false;
	int ran = GetRand(1);
	if (ran == 0)
	{
		left_flg = true;
	}
	else
	{
		left_flg = false;
	}

	// リソース管理クラスのインスタンス取得
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;

	this->num = num;

	// 画像の読み込み(numの値によって読み込む画像＆拡大率を変える)
	switch (num)
	{
	case 1:
		tmp = resource->GetImages("Resource/Images/GameMain/balloon.png");
		extrate = 6.0f;
		break;

	case 2:
		tmp = resource->GetImages("Resource/Images/GameMain/plane.png");
		extrate = 8.0f;
		break;

	case 3:
		tmp = resource->GetImages("Resource/Images/GameMain/UFO.png");
		extrate = 8.0f;
		break;

	case 4:
		tmp = resource->GetImages("Resource/Images/GameMain/sun.png");
		extrate = 15.0f;
		break;

	default:
		tmp = resource->GetImages("Resource/Images/GameMain/bard.png");
		extrate = 2.0f;
		break;
	}
	
	img = tmp[0];

	// 音源データ格納
	attack_se = resource->GetSounds("Resource/Sounds/SE/attack.mp3");

	// 音量調整
	ChangeVolumeSoundMem(255, attack_se);
}

Bard::~Bard()
{
}

void Bard::Update()
{
	if (is_hit == true)
	{// 泡に当たったら横に吹き飛ばす

		if (left_flg == true)
		{
			location.x -= 20.0f;
		}
		else
		{
			location.x += 20.0f;
		}

		location.y -= 3.0f;
		angle += 1.0f;

		if (CheckSoundMem(attack_se) == FALSE)
		{
			// 飛んでいくSE再生
			PlaySoundMem(attack_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{// それ以外

		switch (num)
		{
		case 1:
			if (location.y >= 140.0f)
			{// 泡に当たったフラグをONにする

				is_hit = true;
			}
			else
			{
				location.y += 10.0f;
			}
			break;

		case 2:
			if (location.y >= 130.0f)
			{// 泡に当たったフラグをONにする

				is_hit = true;
			}
			else
			{
				location.y += 10.0f;
			}
			break;

		case 3:
			if (location.y >= 130.0f)
			{// 泡に当たったフラグをONにする

				is_hit = true;
			}
			else
			{
				location.y += 10.0f;
			}
			break;

		case 4:
			if (location.y >= 30.0f)
			{// 泡に当たったフラグをONにする

				is_hit = true;
			}
			else
			{
				location.y += 10.0f;
			}
			break;

		default:
			if (location.y >= 150.0f)
			{// 泡に当たったフラグをONにする

				is_hit = true;
			}
			else
			{
				location.y += 5.0f;
			}
			break;
		}

		
	}
}

void Bard::Draw() const
{
	// 障害物の描画
	DrawRotaGraph2F(location.x, location.y, 16.0f,16.0f,extrate, angle, img, TRUE, 0, 0);
	DrawFormatString(0, 150, 0x000000, "%f", extrate);
}
