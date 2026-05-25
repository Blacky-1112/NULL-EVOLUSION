#pragma once
#include "main.h"
#include "gameObject.h"

struct DIAGONAL_GLOW_BUFFER
{
	float GlowPower;
	float GlowWidth;
	float time;
	float enable;
};

class PolygonBright : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	int m_Use;
	float m_Time;
	ID3D11Buffer* m_DiagonalGlowCB = nullptr;
	float m_GlowPower = 1.0f;   // ”­Œõ‹­“x
	float m_GlowWidth = 0.1f;   // Œõ‘Ñ‚Ì‘¾‚³
	bool  m_DiagonalGlow = false;

public:
	void Init() {}
	void Init(float x, float y, float w, float h, const char* FileName);
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
