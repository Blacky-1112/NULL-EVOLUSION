#pragma once
#include "gameObject.h"
#include "scene.h"
class ModelRenderer;
class Input;

class TestObject : public GameObject //継承
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_EnvTexture{};	//環境マッピング用

	ModelRenderer* m_ModelRenderer;
	Input* m_Input;

	XMFLOAT4 m_Quaternion;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
