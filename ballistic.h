#pragma once
#include "gameObject.h"
#include "scene.h"
class BALLISTIC : public GameObject
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
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Diffuse;

		float alpha;
	};
	static const int Particle_MAX = 100;
	Particle m_Particle[Particle_MAX];

	bool m_Boom;

	float offset;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetBool(bool a)
	{
		m_Boom = a;
	}

	bool GetBool()
	{
		return m_Boom;
	}
};
