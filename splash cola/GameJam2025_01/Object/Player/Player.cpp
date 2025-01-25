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
	location.y = 250.0f;
	angle = 0.0f;

	// 画像データ格納
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Cola/kan.png");
	img = tmp[0];

	angle = 0.0;
}

Player::~Player()
{
}

void Player::Update()
{
	// 入力制御インスタンス取得
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

void Player::Draw()
{
	// プレイヤー画像描画
	DrawRotaGraph2F(location.x, 312.0f, 640.0f,150.0f,1.0, PI / 180 * angle, img, TRUE, 0);
}
