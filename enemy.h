#pragma once
#include "gameObject.h"
#include <string>
#include "scene.h"
class ModelRenderer;
class Input;

enum EnemyPhase
{
	PHASE_1,
	PHASE_2,
	PHASE_3,
	PHASE_4
};

enum EnemyState
{
	ENEMY_SPAWNING,
	ENEMY_ALIVE,
	ENEMY_DYING,
	ENEMY_DEAD
};

enum ChargeState
{
	CHARGE_NONE,
	CHARGE_WINDUP,   // 溜め
	CHARGE_DASH,     // 突進
	CHARGE_RECOVER   // 硬直
};

enum EnemyAI
{
	ENEMY_IDLE,
	ENEMY_CHASE,
	ENEMY_ATTACK
};

class Enemy : public GameObject //継承
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	class AnimationModel* m_AnimModel;
	std::string m_AnimName;
	std::string m_AnimNameNext;
	unsigned int m_AnimFrame;
	float m_AnimBlend;

	Input* m_Input;

	float m_SpawnTime = 0.0f;
	bool m_SpawnFinished = false;
	bool m_IsSpawn = false;
	float m_FallDuration;

	float m_StartY;
	float m_EndY;

	bool m_Exist;
	int m_Frame;

	float m_Life = 100.0f;

	float m_ShakeX;
	float m_ShakeY;

	EnemyPhase m_Phase;

	EnemyState m_State = ENEMY_SPAWNING;
	float m_DeadTimer = 0.0f;
	float m_DeadDuration = 2.5f;

	bool m_Dead;

	EnemyAI m_AI = ENEMY_IDLE;

	// 目標距離など
	float m_AttackRange = 12.0f;
	float m_AttackExitRange = 14.0f;
	float m_ChaseRange = 25.0f;
	float m_MoveSpeed = 0.06f;

	// ステート遷移の管理
	EnemyAI m_AINext = ENEMY_IDLE;
	bool    m_AIChanged = false;

	void UpdateAI();                 // AI全体の更新
	void ChangeAI(EnemyAI next);     // ステート変更要求
	void ApplyAIChange();            // Enter処理

	void UpdateIdle();
	void UpdateChase();
	void UpdateAttack();

	float m_AttackCD = 0.0f;          // 次の攻撃までの残り秒
	float m_AttackInterval = 0.0f;    // 攻撃間隔

	float GetPlayerDistance() const;	//プレイヤーとの距離

	//突進攻撃用
	ChargeState m_ChargeState = CHARGE_NONE;
	// 突進用パラメータ
	float   m_ChargeTimer = 0.0f;
	Vector3 prev{};
	Vector3 moved{};
	Vector3 m_ChargeDir{};
	Vector3 m_ChargeStart{};
	float   m_ChargeWindup = 0.45f;   // 溜め秒
	float   m_ChargeDuration = 0.90f; // 走る秒
	float   m_ChargeRecover = 0.55f;  // 硬直秒
	float   m_ChargeSpeed = 14.0f;     // 移動速度
	float   m_ChargeMaxDist = 14.0f;  // 最大移動距離
	bool    m_ChargeHitDone = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Damage()
	{
		m_Life--;
	}
	float GetLife()
	{
		return m_Life;
	}

	void BeginSpawn();
	void UpdateSpawn(float dt);
	bool IsSpawnFinished()
	{
		return m_SpawnFinished;
	}

	void OnDefeated();
	bool IsDeadFinished() const;
	void UpdateDying();

	void UpdatePhase1();
	void UpdatePhase2();
	void UpdatePhase3();
	void UpdatePhase4();

	void StartCharge();
	void UpdateCharge(float dt);

	void UpdateFacingToPlayer(float lerpRate);
};
