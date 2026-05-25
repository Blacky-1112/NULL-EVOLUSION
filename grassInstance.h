#pragma once
#include "gameObject.h"
#include "scene.h"

class GrassInstance : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11Buffer* m_PositionBuffer;
	ID3D11ShaderResourceView* m_PositionSRV;

	ID3D11Buffer* m_ScaleBuffer;
	ID3D11ShaderResourceView* m_ScaleSRV;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	ID3D11Buffer* m_WindBuffer;
	struct WindParamCB
	{
		float Time;
		float WindStrength;
		float WindSpeed;
		float padding0;

		XMFLOAT2 WindDir;
		float padding1[2];
	};
	float m_Elapsed = 0.0f;

	int m_Frame;

	int GRASS_MAX = 100000;

	ID3D11Buffer* m_PlayerBuffer;
	struct PlayerParamCB
	{
		Vector3 PlayerPos;
		float padding0;

		float PlayerRadius;
		float padding1[3];
	};
	

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
