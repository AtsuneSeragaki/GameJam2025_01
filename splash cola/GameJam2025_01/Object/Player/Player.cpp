#include "Player.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <vector>

#define PI    3.1415926535897932384626433832795f

Player::Player()
{
	// 変数の初期化
	location.x = 640.0f;
	location.y = 0.0f;
	angle = 0.0f;
	cola_num = 0;
	cola_cnt = -10;
	hand_down = 0.0f;

	// 画像データ格納
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Cola/kan.png");
	img = tmp[0];
	tmp = resource->GetImages("Resource/Cola/kan2.png");
	img_cola[0] = tmp[0];
	tmp = resource->GetImages("Resource/Cola/kan3.png");
	img_cola[1] = tmp[0];
	tmp = resource->GetImages("Resource/Cola/kan4.png");
	img_cola[2] = tmp[0];

	// 音源データ格納
	open_can_se = resource->GetSounds("Resource/Sounds/SE/open.mp3");
	up_se = resource->GetSounds("Resource/Sounds/SE/up01.mp3");

	// 音量調整
	ChangeVolumeSoundMem(255, open_can_se);
	ChangeVolumeSoundMem(255, up_se);
	play_open_can_se = true;
}

Player::~Player()
{
}

void Player::Initialize()
{
	// 変数の初期化
	location.x = 640.0f;
	location.y = 0.0f;
	angle = 0.0f;
	cola_num = 0;
	cola_cnt = -10;
	hand_down = 0.0f;

	play_open_can_se = true;
}

void Player::Update()
{
	if (play_open_can_se == false)
	{
		play_open_can_se = true;
	}

	// 入力制御クラスのインスタンス取得
	InputManager* input = InputManager::GetInstance();

	// マウスのY座標をプレイヤーのY座標に代入
	float tmp = 0.0f;
	tmp = location.y;
	location.y = input->GetMouseLocation().y;

	// マウスのY座標によって角度を変える
	if (location.y >= 370.0f)
	{// 1番下にあるとき
		angle = -24.0f;
	}
	else if (location.y <= 105.0f)
	{// 1番上にあるとき
		angle = 25.0f;
	}
	else if (location.y > 312.0f)
	{// 真ん中から下にあるとき
		angle = (location.y - 312.0f) / -2.4f;
	}
	else
	{// 真ん中から上にあるとき
		angle = (312.0f - location.y) / 8.28f;
	}
}

void Player::Draw() const
{
	// プレイヤー画像描画
	DrawRotaGraph2F(location.x, 312.0f, 640.0f,150.0f,1.0, PI / 180 * angle, img, TRUE, 0);
}

void Player::ResultUpdate()
{
	if (play_open_can_se == true)
	{
		// 缶を開けるSE再生
		PlaySoundMem(open_can_se, DX_PLAYTYPE_BACK);
		play_open_can_se = false;
	}

	cola_cnt += 1;
	if (cola_cnt > 7&&cola_num<2)
	{
		if (CheckSoundMem(up_se) == FALSE)
		{
			// 噴き上がるSE再生
			PlaySoundMem(up_se, DX_PLAYTYPE_BACK);
		}

		cola_cnt = 0;
		cola_num += 1;
	}
}

void Player::ResultDraw()
{
	// プレイヤー画像描画
	DrawRotaGraph2F(location.x, 150.0f+hand_down, 640.0f, 150.0f, 1.0, 0.0, img_cola[cola_num], TRUE, 0);
}
