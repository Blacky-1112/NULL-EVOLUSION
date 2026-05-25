#pragma once
#include "polygon2D.h"
class EnemyHPUI : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayOut;
	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11ShaderResourceView* m_Texture;

	float maxRate = SCREEN_WIDTH / 2;

	XMFLOAT4 m_Color;

	float tx = SCREEN_WIDTH / 4;
	float ty = 10.0f;
	float tw = maxRate;
	float th = 30.0f;
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};