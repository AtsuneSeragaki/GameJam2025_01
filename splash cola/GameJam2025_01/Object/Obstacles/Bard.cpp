#include "Bard.h"

#include "../../Utility/ResourceManager.h"
#include "DxLib.h"
#include <vector>

Bard::Bard(int num)
{
	location.x = 320.0f;
	location.y = 0.0f;
	angle = 0.0f;
	is_hit = false;

	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;

	switch (num)
	{
	case 1:
		tmp = resource->GetImages("Resource/Images/GameMain/balloon.png");
		break;

	case 2:
		tmp = resource->GetImages("Resource/Images/GameMain/plane.png");
		break;

	case 3:
		tmp = resource->GetImages("Resource/Images/GameMain/UFO.png");
		break;

	case 4:
		tmp = resource->GetImages("Resource/Images/GameMain/sun.png");
		break;

	default:
		tmp = resource->GetImages("Resource/Images/GameMain/bard.png");
		break;
	}
	
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
	is_hit = false;

	ResourceManager* resource = ResourceManager::GetInstance();
	std::vector<int> tmp;
	tmp = resource->GetImages("Resource/Images/GameMain/bard.png");
	img = tmp[0];
}

void Bard::Update()
{
	if (is_hit == true)
	{
		location.x += 20.0f;
		location.y -= 3.0f;
		angle += 1.0f;
	}
	else
	{
		if (location.y >= 150.0f)
		{
			is_hit = true;
		}
		else
		{
			location.y += 10.0f;
		}
	}
}

void Bard::Draw() const
{
	DrawRotaGraph2F(location.x, location.y, 20, 20, 2.0f, angle, img, TRUE, 0, 0);
	//DrawBox(location.x - 20.0f, location.y - 20.0f, location.x + 20.0f, location.y + 20.0f, 0xff0000, TRUE);
}
