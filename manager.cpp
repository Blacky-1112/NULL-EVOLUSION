#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "loading.h"
#include "gameover.h"
#include "audio.h"
#include "seManager.h"
#include "fade.h"

Scene* Manager::m_Scene = nullptr;
Scene* Manager::m_NextScene = nullptr;
Scene* Manager::m_FadeScene = nullptr;

float Manager::m_DeltaTime = 0.0f;
long long Manager::m_PrevTime = 0;
double Manager::m_Frequency = 0.0;

bool once = true;
bool FadeOut = false;
bool FadeOutFinished = false;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	SoundManager::Init();

	m_FadeScene = new Fade();
	m_FadeScene->Init();

	m_Scene = new Title();
	m_Scene->Init();

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_Frequency = double(freq.QuadPart); // 1秒あたりのカウント数

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	m_PrevTime = now.QuadPart;
	ShowCursor(FALSE);
}


void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Input::Uninit();

	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	long long diff = now.QuadPart - m_PrevTime;
	m_DeltaTime = float(diff / m_Frequency);   // 秒数に変換
	m_PrevTime = now.QuadPart;
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();


	//画面遷移
	if (m_NextScene != nullptr)
	{
		//まずフェードアウト
		if (once)
		{
			m_FadeScene->FadeStart();
			FadeOut = true;
			once = false;
		}

		//フェードアウト中
		if (FadeOut)
		{
			m_FadeScene->SceneFadeOut();
		}

		//フェードアウト完了したらシーン切り替え
		if (m_FadeScene->FadeOutFinishedCheck() && FadeOut)
		{
			m_Scene->Uninit();
			delete m_Scene;

			m_Scene = m_NextScene;
			m_Scene->Init();

			FadeOut = false;
			FadeOutFinished = true;
		}

		//フェードイン
		if (FadeOutFinished)
		{
			m_FadeScene->SceneFadeIn();
		}

		//フェードイン完了
		if (m_FadeScene->FadeInFinishedCheck() && !FadeOut)
		{
			once = true;
			FadeOutFinished = false;
			m_NextScene = nullptr;

		}
	}

	Renderer::End();
}
