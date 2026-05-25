#pragma once
#include "gameObject.h"
#include "scene.h"
class Input;

class Sky : public GameObject //åpè≥
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11ShaderResourceView* m_Texture{};

	static class ModelRenderer* m_ModelRenderer;
	Input* m_Input;

public:
	static void Load();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};
