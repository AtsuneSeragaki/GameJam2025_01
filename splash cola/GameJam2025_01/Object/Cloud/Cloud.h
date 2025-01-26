#pragma once
#include "../../Utility/Vector2D.h"
class Cloud
{
private:
	Vector2D location;
	Vector2D move;
	int cloud_img;

public:
	Cloud();
	~Cloud();

	void Update();
	void Draw()const;
	void Initialize();

};

