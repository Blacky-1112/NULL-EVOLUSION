#pragma once
#include "scene.h"

class Title : public Scene
{
private:
	class Audio* m_BGM;
	float alpha = 0.0f;
	int CountUp;

	float AddSize;
public:
	void Init() override;
	void Update() override;
};