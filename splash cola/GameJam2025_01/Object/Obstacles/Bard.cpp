#include "Bard.h"

#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <vector>

Bard::Bard()
{
	location.x = 320.0f;
	location.y = 0.0f;
	angle = 0.0f;

	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/bard.png");
	img = tmp[0];
}

Bard::~Bard()
{
}

void Bard::Initialize()
{
	location.x = 320.0f;
	location.y = 200.0f;
	angle = 0.0f;

	/*ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resources/Images/GameMain/bard.png");
	img = tmp[0];*/
}

void Bard::Update()
{
	if (location.y >= 150.0f)
	{
		location.x += 10.0f;
		angle += 1.0f;
	}
	else
	{
		location.y += 7.0f;
	}
}

void Bard::Draw() const
{
	DrawRotaGraph2F(location.x, location.y, 20, 20, 1.0f, angle, img, TRUE, 0, 0);
	//DrawBox(location.x - 20.0f, location.y - 20.0f, location.x + 20.0f, location.y + 20.0f, 0xff0000, TRUE);
}
