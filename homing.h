#pragma once
#include "gameObject.h"
#include "scene.h"
class ModelRenderer;
class Input;

class HOMING : public GameObject //åpè≥
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ModelRenderer* m_ModelRenderer;
	Input* m_Input;

	Vector3 m_Velocity{};

	Vector3 m_StartPos;
	Vector3 m_EndPos;
	Vector3 m_StartVec;
	Vector3 m_EndVec;

	float m_Time;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetVelocity(Vector3 vel)
	{
		m_Velocity = vel;
	}

	void Shot(Vector3 StartPosition, Vector3 EndPosition)
	{
		m_StartPos = StartPosition;
		m_EndPos = EndPosition;
		m_StartVec = Vector3(10.0f, 10.0f, 0.0f);
		m_EndVec = Vector3(10.0f, -10.0f, 0.0f);
		m_Time = 0.0f;
	}
};
