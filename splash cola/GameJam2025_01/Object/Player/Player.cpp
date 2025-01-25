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
	cola_num = 0;
	cola_cnt = -10;

	
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

	// 移動範囲制御
	if (location.y >= 370.0f)
	{// 下
		angle = -24.0f;
	}
	else if (location.y <= 105.0f)
	{// 上
		angle = 25.0f;
	}
	else if (location.y > 312.0f)
	{//5.4f
		angle = (location.y - 312.0f) / -2.4f;
	}
	else
	{
		angle = (312.0f - location.y) / 8.28f;
	}
}

void Player::Draw()
{
	// プレイヤー画像描画
	DrawRotaGraph2F(location.x, 312.0f, 640.0f,150.0f,1.0, PI / 180 * angle, img, TRUE, 0);

	DrawFormatString(0, 20, 0x000000, "%f", angle);
}

void Player::ResultUpdate()
{
	cola_cnt += 1;
	if (cola_cnt > 12&&cola_num<2)
	{
		cola_cnt = 0;
		cola_num += 1;
	}

	if (cola_num >= 1)
	{
	}
}

void Player::ResultDraw()
{

	// プレイヤー画像描画
	DrawRotaGraph2F(location.x, 150.0f, 640.0f, 150.0f, 1.0, 0.0, img_cola[cola_num], TRUE, 0);
}
