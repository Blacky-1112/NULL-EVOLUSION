#pragma once
#include "scene.h"

class Gameover : public Scene
{
private:
	class Audio* m_BGM;

	int CountUp;

	float AddSize;
public:
	void Init() override;
	void Update() override;
};