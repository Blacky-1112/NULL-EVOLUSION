#pragma once
#include "scene.h"

enum class GameFlowState
{
	ENEMY_SPAWN_START,
	ENEMY_SPAWNING,
	GAME_READY,
	GAME_START,

	ENEMY_DEFEATED,
	PLAYER_DEFEATED
};

class Game : public Scene
{
private:
	class Audio* m_BGM;
	GameFlowState m_State = GameFlowState::ENEMY_SPAWN_START;
	float m_FlowTimer;
	bool First;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
};