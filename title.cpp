#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

#include "polygon.h"
#include "polygon2D.h"
#include "fade.h"
#include "audio.h"
#include "seManager.h"
#include "title.h"
#include "loading.h"
#include "game.h"
#include "input.h"
#include "polygonFilter.h"
#include "particle.h"

#include "titleCamera.h"
#include "titleMesh.h"
#include "wave.h"



void Title::Init()
{
	AddGameObject<TitleCamera>(0);
	AddGameObject<TitleMesh>(0);
	AddGameObject<Wave>(0);
	AddGameObject<PolygonBright>(2)->Init(SCREEN_WIDTH / 2 - 300, 100.0f, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "asset\\texture\\titleLogo.png");
	AddGameObject<Polygon2D>(2)->Init(SCREEN_WIDTH / 2 - 300, 430, 500.0f, 200.0f, "asset\\texture\\start.png");
	AddGameObject<Particle>(2)->SetPosition({0.0f,10.0f,0.0f});

	m_BGM = new Audio();
	m_BGM->Load("asset\\sound\\title.wav");
	m_BGM->Play(true, 0.2f);

	CountUp = 1.0;
	AddSize = 0.0f;
}

void Title::Update()
{
	AddSize += 0.001f * CountUp;

	if (AddSize > 0.01f)
	{
		CountUp = -1;
	}
	else if (AddSize < -0.01f)
	{
		CountUp = 1;
	}

	auto polygon = Manager::GetScene()->GetGameObjects<Polygon2D>();
	polygon[0]->SetScale(Vector3(polygon[0]->GetScale().x + AddSize, 
		polygon[0]->GetScale().y + AddSize, 0.0f));

	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN) && m_BGM)
	{
		SoundManager::PlaySE(SE::Click);
		m_BGM->Uninit();
		delete m_BGM;
		Manager::SetScene<Loading>();
	}
}
