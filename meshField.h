#pragma once
#include "main.h"
#include "gameObject.h"
#include "renderer.h"

class MeshField : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11Buffer*				m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_TextureSoil{};

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	VERTEX_3D					m_Vertex[21][21]{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	float GetHeight(Vector3 Position);
};