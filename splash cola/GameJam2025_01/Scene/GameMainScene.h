#pragma once
#include "AbstractScene.h"
#include "../Utility/Vector2D.h"
class GameMainScene :
    public AbstractScene
{
public:
	GameMainScene();
	~GameMainScene();

	void Update() override;
	void Draw() const override;
	AbstractScene* Change() override;


};

