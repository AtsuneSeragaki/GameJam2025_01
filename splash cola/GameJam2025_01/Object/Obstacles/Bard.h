#pragma once

#include "../../Utility/Vector2D.h"

class Bard
{
private:
	Vector2D location;
	double angle;
	int img;

public:
	Bard();
	~Bard();

public:
	void Initialize();
	void Update();
	void Draw() const;

public:
	Vector2D GetLocation() { return location; }
};