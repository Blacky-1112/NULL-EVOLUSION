#pragma once
#include "gameObject.h"
#include "scene.h"
class ModelRenderer;
class Input;

class Box : public GameObject //åpè≥
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ModelRenderer* m_ModelRenderer;
	Input* m_Input;

	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_TextureWall{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
