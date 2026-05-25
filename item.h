#pragma once
#include "gameObject.h"
class ModelRenderer;
class Input;

class Item : public GameObject //åpè≥
{
private:
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ModelRenderer* m_ModelRenderer;

	int m_ItemCount;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void AddItemCount(int n)
	{
		m_ItemCount += n;
	}

	int GetItemCount()
	{
		return m_ItemCount;
	}
};
