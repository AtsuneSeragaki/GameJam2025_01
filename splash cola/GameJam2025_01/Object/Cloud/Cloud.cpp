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

}

Cloud::~Cloud()
{
}

void Cloud::Update()
{
}

void Cloud::Draw() const
{
}

void Cloud::Initialize()
{
}
