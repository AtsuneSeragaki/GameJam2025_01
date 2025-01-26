#pragma once
#include "../../Utility/Vector2D.h"
class Cloud
{
private:
	int cloud_img;
	int star_img;
	int speed;
	int gMileage;

public:
	Cloud();
	~Cloud();

	void Update();
	void Draw()const;
	void Initialize();

};

