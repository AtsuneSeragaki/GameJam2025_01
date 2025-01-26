#include "Cloud.h"
#include "DxLib.h"
#include "../../Utility/ResourceManager.h"


Cloud::Cloud()
{
	location = 0.0f;
	move = 0.0f;
	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/cloud.png");
	cloud_img = tmp[0];
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
	DrawGraph(0, gMileage % 480 - 480, cloud_img, TRUE);
	DrawGraph(0, gMileage % 480, cloud_img, TRUE);

	DrawGraph(540, gMileage % 480 - 480, cloud_img, TRUE);
	DrawGraph(540, gMileage % 480, cloud_img, TRUE);

}

void Cloud::Initialize()
{
}
