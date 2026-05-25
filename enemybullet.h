#pragma once
#include "gameObject.h"
#include "scene.h"
class ModelRenderer;
class Input;

enum BulletType
{
	BULLET_STRAIGHT,
	BULLET_HOMING,
	BULLET_SPEAD,
	BULLET_WAVE,
	BULLET_BIG,
	BULLET_BEAM,
	BULLET_METEOR
};

class EnemyBullet : public GameObject //継承
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ModelRenderer* m_ModelRenderer;
	Input* m_Input;

	Vector3 m_Velocity{};

	BulletType m_Type;
	float m_Speed;
	float m_RotateSpeed;

	//追尾システム
	Vector3 m_StartPos;
	Vector3 m_EndPos;
	Vector3 m_StartVec;
	Vector3 m_EndVec;

	float m_Time;
	float m_Frame;

	//隕石用
	float m_Gravity = 0.002f;
	float m_FallSpeed = 0.0f;
	float m_TargetY = 0.0f;
	bool m_HitGround = false;
	float m_TotalFallTime = 180.0f;
	float m_CurrentTime = 0.0f;
	float m_StartHeight = 20.0f;
	float m_WaitTime = 30.0f;

	//空気中で揺れる
	Vector3 m_Drift;

	class MeshField* m_Mesh = nullptr;
	class MeteorShadow* m_Shadow = nullptr;
	class BulletEffect* m_Effect = nullptr;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void UpdateStraight();
	void UpdateHoming();
	void UpdateWave();
	void UpdateBig();
	void UpdateBeam();
	void UpdateMeteor();

	void SetVelocity(Vector3 vel)
	{
		m_Velocity = vel;
	
	}

	void SetType(BulletType type)
	{
		m_Type = type;
	}

	void Shot(Vector3 StartPosition, Vector3 EndPosition)
	{
		m_StartPos = StartPosition;
		m_EndPos = EndPosition;
		m_StartVec = Vector3(10.0f, 10.0f, 0.0f);
		m_EndVec = Vector3(10.0f, -10.0f, 0.0f);
		m_Time = 0.0f;
	}

	void ShotMeteor(Vector3 centerPos, float range);
};
