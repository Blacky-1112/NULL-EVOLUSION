#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include <thread>

#include "polygon.h"
#include "fade.h"
#include "audio.h"
#include "loading.h"
#include "game.h"
#include "polygonFilter.h"
#include "spinner.h"
#include "sky.h"


void Loading::Init()
{
	SPINNER* spinner = AddGameObject<SPINNER>(2);
	spinner->Init(-100.0f, -100.0f, 200.0f, 200.0f, "asset\\texture\\loading.png");
	spinner->SetPosition({1100.0f,600.0f, 0.0f});
	m_Finish = false;

	//ƒXƒŒƒbƒh
	std::thread th([=] 
		{
			Sky::Load();
			m_Finish = true;
		});
	th.detach();
}

void Loading::Update()
{
	Scene::Update();

	if (m_Finish)
	{
		Manager::SetScene<Game>();
	}
}
