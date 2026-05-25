#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

#include "polygon2D.h"
#include "title.h"
#include "result.h"
#include "audio.h"
#include "input.h"


void Result::Init()
{
	AddGameObject<Polygon2D>(2)->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\Gameclear.png");
	AddGameObject<Polygon2D>(2)->Init(SCREEN_WIDTH / 2 - 300, 430, 500.0f, 200.0f, "asset\\texture\\finish.png");

	m_BGM = new Audio();
	m_BGM->Load("asset\\sound\\result.wav");
	m_BGM->Play(true);

	CountUp = 1.0;
	AddSize = 0.0f;
}

void Result::Update()
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
	polygon[1]->SetScale(Vector3(polygon[1]->GetScale().x + AddSize,
		polygon[1]->GetScale().y + AddSize, 0.0f));


	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_BGM->Uninit();
		delete m_BGM;
		Manager::SetScene<Title>();
	}
}
