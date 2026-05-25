#pragma once
#include "scene.h"

class Loading : public Scene
{
private:
	class Audio* m_BGM;
	bool m_Finish;
public:
	void Init() override;
	void Update() override;
};