#include "Cloud.h"
#include "DxLib.h"
#include "../../Utility/ResourceManager.h"


Cloud::Cloud()
{
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/cloud.png");
	cloud_img = tmp[0];
	tmp = resource->GetImages("Resource/Images/GameMain/star.png");
	star_img = tmp[0];
	speed =15;
	gMileage = 0;
}

Cloud::~Cloud()
{
}

void Cloud::Update()
{
	gMileage += speed;
}

void Cloud::Draw() const
{
	if (gMileage < 1800) {
		DrawGraph(0, gMileage % 480 - 480, cloud_img, TRUE);
		DrawGraph(0, gMileage % 480, cloud_img, TRUE);

		DrawGraph(540, gMileage % 480 - 480, cloud_img, TRUE);
		DrawGraph(540, gMileage % 480, cloud_img, TRUE);
	}
	else
	{
		DrawGraph(0, gMileage % 480 - 480, star_img, TRUE);
		DrawGraph(0, gMileage % 480, star_img, TRUE);

		DrawGraph(540, gMileage % 480 - 480, star_img, TRUE);
		DrawGraph(540, gMileage % 480, star_img, TRUE);
	}
	DrawFormatString(0, 10, 0xffffff, "%d", gMileage);

}

void Cloud::Initialize()
{
	speed = 15;
	gMileage = 0;
}
