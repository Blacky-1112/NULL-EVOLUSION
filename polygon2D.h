#pragma once
#include "main.h"
#include "gameObject.h"


class Polygon2D : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayOut;
	ID3D11PixelShader* m_PixelShader;
	ID3D11VertexShader* m_VertexShader;

	ID3D11ShaderResourceView* m_Texture;

	float maxRate = SCREEN_WIDTH / 2;

	float tx;
	float ty;
	float tw;
	float th;
public:
	void Init() {}
	void Init(float x, float y, float Width, float Height, const char* FileName, float col=1.0f);
	void Uninit() override;
	void Update() override;
	void Draw() override;
};