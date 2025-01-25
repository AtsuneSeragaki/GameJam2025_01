#include "Player.h"

#include "../../Utility/InputManager.h"
#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <vector>

Player::Player()
{
	// 変数の初期化
	location.x = 0.0f;
	location.y = 100.0f;

	// 画像データ格納
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Cola/kan.png");
	img = tmp[0];
}

Player::~Player()
{
}

void Player::Update()
{
	// 入力制御インスタンス取得
	InputManager* input = InputManager::GetInstance();

	// マウスのY座標をプレイヤーのY座標に代入
	location.y = input->GetMouseLocation().y;

	// 移動範囲制御
	if (location.y >= 370.0f)
	{// 下
		location.y = 375.0f;
	}
	else if (location.y <= 105.0f)
	{// 上
		location.y = 105.0f;
	}
}

void Player::Draw()
{
	// プレイヤー画像描画
	DrawRotaGraphF(320, location.y, 1.0, 0, img, TRUE, 0);
}
