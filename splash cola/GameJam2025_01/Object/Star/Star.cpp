#include "Star.h"
#include "DxLib.h"

#include "../../Utility/ResourceManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

//デグリーからラジアンに変換
#define DEGREE_RADIAN(_deg) (M_PI*(_deg)/180.0)


Star::Star(Vector2D set_location, Vector2D set_direction)
	: location(set_location), direction(set_direction)
{
	speed = 1.0f;
	count = 0.0f;

	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/Effect/star.png");
	star_image = tmp[0];

	delete_flg = false;
	alpha = 255;
	angle = 0.0;
	degree = 360.0;
}

Star::~Star()
{
}

void Star::Initialize()
{
}

void Star::Update()
{
	if (count < 2.7f)
	{
		count += 0.1f;
		location = location + direction * (speed * count);
	}

	alpha -= 3;
	if (alpha < 0)
	{
		delete_flg = true;
	}

	// 左回り
	if (degree > 0.0)
	{
		degree -= 3.0;
	}
	else
	{
		degree = 360.0;
	}

	// 画像の角度変換
	angle = DEGREE_RADIAN(degree);
}

void Star::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 星の描画
	DrawRotaGraphF(location.x, location.y, 1.0, angle, star_image, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool Star::GetDeleteFlg() const
{
	return delete_flg;
}
