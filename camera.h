#pragma once
#include "gameObject.h"
#include "scene.h"

enum class CameraMode
{
	NORMAL,			//プレイヤー追従
	ENEMY_INTRO,	//敵登場中
	ENEMY_FINISH,	//敵撃破
	RETURN_TO_PLAYER//プレイヤーに戻る
};

class Camera : public GameObject
{
private:
	XMMATRIX m_Projection;
	XMMATRIX m_View;

	Vector3 m_Target{ 0.0f, 0.0f, 0.0f };

	float m_ShakeTime{ 0.0f };
	Vector3 m_ShakeVector{ 0.0f,0.0f,0.0f };

	// 持続シェイク用
	Vector3 m_ContinuousShake{};
	float   m_ContinuousShakeTime = 0.0f;
	bool    m_EnableContinuousShake = false;

	CameraMode m_Mode = CameraMode::NORMAL;

	//敵登場用
	GameObject* m_EnemyTarget = nullptr;
	float m_IntroTimer = 0.0f;
	float m_IntroDuration = 2.0f;	//敵を見つめている時間

	//プレイヤー戻り補間用
	float m_ReturnTime = 0.0f;
	float m_ReturnDuration = 2.0f;
	Vector3 m_StartPos;
	Vector3 m_EndPos;
	Vector3 m_StartTarget;
	Vector3 m_EndTarget;

	//敵撃破用
	float m_FinishTimer = 0.0f;
	float m_FinishDuration = 1.5f;
	Vector3 m_FinishStartPos;
	Vector3 m_FinishStartTarget;

	bool m_EnemyFinished = false;

	//カメラ操作限度
	const float MAX_PITCH = XMConvertToRadians(80.0f);
	const float MIN_PITCH = XMConvertToRadians(0.0f);
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Update_Normal(float dt);
	void Update_EnemyIntro(float dt);
	void Update_EnemyFinish(float dt);
	void Update_ReturnToPlayer(float dt);

	void Shake(Vector3 ShakeVector);
	void StartContinuousShake(Vector3 power);
	void StopContinuousShake();

	bool CheckView(Vector3 Pos, float size);

	XMMATRIX GetCameraView() {
		return m_View;
	}

	void StartEnemyIntro(GameObject* enemy);
	void StartEnemyFinish(GameObject* enemy);

	float Lerp(float a, float b, float c)
	{
		return a + (a - b) * c;
	}

	bool isFinished()
	{
		return m_EnemyFinished;
	}
};
