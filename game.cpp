#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "seManager.h"
#include "game.h"
#include <random>

#include "camera.h"
#include "field.h"
#include "meshField.h"
#include "wave.h"
#include "polygon.h"
#include "polygon2D.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "particle.h"
#include "score.h"
#include "playerhpUI.h"
#include "sky.h"
#include "rock.h"
#include "testObject.h"
#include "box.h"
#include "shadowVolume.h"
#include "grassInstance.h"
#include "treeInstance.h"
#include "enemyHpUI.h"
#include "overheatGauge.h"
#include "polygon2D.h"
#include "rain.h"
#include "homingUI.h"

#include "result.h"
#include "gameover.h"
#include "input.h"

void Game::Init()
{
	AddGameObject<Camera>(0);

	AddGameObject<Sky>(0)->SetScale({75.0, 75.0, 75.0});
	auto meshField = AddGameObject<MeshField>(0);

	srand(static_cast<unsigned int>(time(0)));

	AddGameObject<GrassInstance>(1);
	AddGameObject<TreeInstance>(1);

	for (int i = 0; i < 50; i++)
	{
		Vector3 pos;
		pos.x = rand() % 100 - 50.0f;
		pos.z = rand() % 100 - 50.0f;

		pos.y = meshField->GetHeight(pos);

		Vector3 scale;
		scale.x = 1.0f + (rand() % 100 / 100.0f);
		scale.y = 1.0f + (rand() % 100 / 100.0f);
		scale.z = 1.0f;

		auto rock = AddGameObject<Rock>(1);
		rock->SetPosition(pos);
		rock->SetScale(scale);
	}


	AddGameObject<Wave>(0);
	AddGameObject<Player>(1)->SetPosition({ 0.0f,0.0f,-3.0f });

	Enemy* enemy = AddGameObject<Enemy>(1);
	enemy->SetPosition({ 3.0f,0.0f,10.0f });


	AddGameObject<Rain>(2)->SetPosition({ 0.0f,10.0f,0.0f});
	AddGameObject<PlayerHP>(2)->SetPosition({ 0.0f,0.0f,0.0f });

	m_BGM = new Audio();
	m_BGM->Load("asset\\sound\\game.wav");
	m_BGM->Play(true, 0.2f);

	//ゲーム開始ステート初期化
	m_State = GameFlowState::ENEMY_SPAWN_START;
	m_FlowTimer = 0.0f;

	enemy->BeginSpawn();	//登場演出開始
	First = true;
}

void Game::Uninit()
{
	m_BGM->Uninit();
	delete m_BGM;

	Scene::Uninit();
}

void Game::Update()
{
	if (First)
	{
		First = false;
		return;
	}
	Player* player = Manager::GetScene()->GetGameObject<Player>();

	switch (m_State)
	{
	case GameFlowState::GAME_START:
		player->SetControllEnable(true);
		break;
	default:
		player->SetControllEnable(false);
		break;
	}

	Scene::Update();

	Enemy* enemy = Manager::GetScene()->GetGameObject<Enemy>();
	m_FlowTimer += Manager::GetDeltaTime();
	switch (m_State)
	{
	case GameFlowState::ENEMY_SPAWN_START:
		m_State = GameFlowState::ENEMY_SPAWNING;
		m_FlowTimer = 0.0f;
		break;
	case GameFlowState::ENEMY_SPAWNING:
		enemy->UpdateSpawn(Manager::GetDeltaTime());
		if (enemy->IsSpawnFinished())
		{
			m_State = GameFlowState::GAME_READY;
			m_FlowTimer = 0.0f;
		}
		break;
	case GameFlowState::GAME_READY:
		if (m_FlowTimer > 3.0f)
		{
			m_State = GameFlowState::GAME_START;
			AddGameObject<EnemyHPUI>(2)->Init();
			AddGameObject<OverHeatGauge>(2)->Init();
			m_FlowTimer = 0.0f;
		}
		break;
	case GameFlowState::GAME_START:
		if (enemy->GetLife() <= 0)
		{
			SoundManager::PlaySE(SE::KnockOut);
			m_State = GameFlowState::ENEMY_DEFEATED;
			m_FlowTimer = 0.0f;

			player->SetControllEnable(false);
			enemy->OnDefeated();
		}

		if (player->GetLife() <= 0)
		{
			SoundManager::PlaySE(SE::KnockOut);
			player->SetControllEnable(false);
			m_FlowTimer = 0.0f;
			m_State = GameFlowState::PLAYER_DEFEATED;
		}
		break;
	case GameFlowState::ENEMY_DEFEATED:
		if (enemy->IsDeadFinished())
		{
			Manager::SetScene<Result>();
		}
		break;
	case GameFlowState::PLAYER_DEFEATED:
		if (m_FlowTimer > 1.5f)
		{
			Manager::SetScene<Gameover>();
		}
		break;
	}
}
