#pragma once
#pragma once
#include "gameObject.h"

class HomingUI : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	float tx = 0.0f;
	float ty = 0.0f;
	float tw = 100.0f;
	float th = 100.0f;
	
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
