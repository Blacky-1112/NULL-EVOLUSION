#pragma once
#include "polygon2D.h"
class OverHeatGauge : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayOut;
	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11ShaderResourceView* m_Texture;

	float maxRate = SCREEN_HEIGHT / 4;

	XMFLOAT4 m_Color;

	float tx = SCREEN_WIDTH / 2 + 50.0f;
	float ty = SCREEN_HEIGHT / 2 + 100.0f;
	float tw = 80.0f;
	float th = maxRate;

	float Alpha;

	class Player* player = nullptr;
	
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};