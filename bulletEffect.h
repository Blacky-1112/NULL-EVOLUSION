#pragma once
#include "gameObject.h"
#include "scene.h"
class BulletEffect : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	int m_Frame;

	XMFLOAT4 m_Color;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetColor(XMFLOAT4 col)
	{
		m_Color = col;
	}
};
