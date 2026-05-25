#pragma once
#include "gameObject.h"
#include "manager.h"
#include <string>
class ModelRenderer;
class Input;
class Cursor;

class Player : public GameObject //継承
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	//アニメーション用
	class AnimationModel* m_AnimModel;
	std::string m_AnimName;
	std::string m_AnimNameNext;
	float m_AnimBlend;

	//アニメーションフレーム
	unsigned int m_Frame;
	unsigned int m_JumpFrame;

	//キー入力
	Input* m_Input;

	class ShadowVolume* m_ShadowVolume;

	Vector3 m_Gravity = {0.0f, -0.025f, 0.0f};

	float m_Speed = 0.4f;
	float jumpSpeed = 0.0f;

	bool isJump = false;
	bool isMove = false;

	//弾発射、オーバーヒート管理
	bool m_IsShot = false;
	float m_CoolTime = 0.0f;
	float m_Heat = 0.0f;               // 現在の熱量
	float m_HeatMax = 100.0f;          // 最大熱量
	float m_HeatPerShot = 18.0f;       // 1発ごとの発熱
	float m_CoolRate = 35.0f;          // 冷却速度（秒あたり減る量）
	float m_OverheatCoolRate = 35.0f;  // オーバーヒート中の冷却
	bool m_IsOverheated = false;       // オーバーヒート状態

	float m_FireInterval = 0.13f;      // 連射間隔(秒)
	float m_FireTimer = 0.0f;          // 次に撃てるまでの残り秒

	//操作可能か
	bool m_ControllEnable = false;

	//ノックバック
	bool  m_IsKnockback = false;
	float m_KnockTimer = 0.0f;
	float m_KnockDuration = 0.25f;   // 操作不能時間
	Vector3 m_KnockVel{};

	bool  m_IsInvincible = false;
	float m_InvincibleTimer = 0.0f;
	float m_InvincibleDuration = 0.0f;

	//ライフ
	float MaxLife = 10.0f;
	float m_Life = MaxLife;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetControllEnable(bool b)
	{
		m_ControllEnable = b;
	}

	bool GetControllEnable()
	{
		return m_ControllEnable;
	}

	void Blend()
	{
		if (m_AnimNameNext != "Run")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Run";
			m_AnimBlend = 0.0f;
		}

		if (m_AnimNameNext != "Idle")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Idle";
			m_AnimBlend = 0.0f;
		}
	}

	float GetLife()
	{
		return m_Life;
	}
	void Damage()
	{
		if(m_Life >0)
		m_Life--;
	}

	float GetOverHeat()
	{
		return m_Heat;
	}

	void StartKnockback(const Vector3& dir, float speed, float duration);
	bool IsKnockback() const 
	{
		return m_IsKnockback;
	}

	bool IsInvincible() const
	{
		return m_IsInvincible;
	}
	void StartInvincible(float duration);
};
