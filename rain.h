#pragma once
#include "gameObject.h"
#include "scene.h"
class Rain : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	struct PARTICLE 
	{
		bool Enable;
		Vector3 Position;
		Vector3 Velocity;
		Vector3 Diffuse;

		float alpha;
	};
	static const int Rain_MAX = 100000;
	PARTICLE m_Rain[Rain_MAX];
	float a;

	struct RainParticle
	{
		Vector3 StartPos;
		Vector3 Velocity;
		float   StartTime;
	};

	float m_Time;


public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
