#pragma once
#include "gameObject.h"
#include "scene.h"
class ModelRenderer;

class BULLET : public GameObject //åpè≥
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ModelRenderer* m_ModelRenderer;

	Vector3 m_Velocity{};

	Vector3 m_StartPos{};
	Vector3 m_EndPos{};
	Vector3 m_StartVec{};
	Vector3 m_EndVec{};

	float m_Time;
	class Enemy* m_Enemy = nullptr;
	class BulletEffect* m_Effect = nullptr;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Shot(Vector3 playerPos, Vector3 playerForward, 
		Enemy* target, float side);//ç∂Ç™-1ÅAâEÇ™1

	Vector3 GetPosition() 
	{
		return m_Position;
	}
};
