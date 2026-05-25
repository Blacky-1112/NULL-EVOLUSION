#pragma once
#include "gameObject.h"
#include "scene.h"
class Aura : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	struct Particle
	{
		bool Enable;
		int Life;
		int LifeMax;
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Diffuse;

		float FadeStart;
		float AlphaMax;
		float alpha;
	};

	static const int Particle_MAX = 100;
	Particle m_Particle[Particle_MAX];

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Burst(const Vector3& pos, int num, float power);
	void BurstHitBig(const Vector3& pos);
};
