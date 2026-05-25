#pragma once
#include "gameObject.h"

class SPINNER : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

public:
	void Init() {}
	void Init(float x, float y, float w, float h, const char* FileName);
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
