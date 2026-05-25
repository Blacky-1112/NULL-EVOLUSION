#pragma once
#pragma once
#include "gameObject.h"

class PlayerHP : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	class Player* m_Player = nullptr;

	float Rate = 0.0f;
	float maxRate = 0.0f;

	float Alpha = 0.0f;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
