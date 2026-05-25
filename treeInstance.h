#pragma once
#include "gameObject.h"
#include "scene.h"
class TreeInstance : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11Buffer* m_PositionBuffer;
	ID3D11ShaderResourceView* m_PositionSRV;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	int m_Frame;

	int TREE_MAX = 100;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
