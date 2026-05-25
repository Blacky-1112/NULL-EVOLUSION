#pragma once

class Fade: public Scene
{

private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_FadeTexture;

	float FadeSpeed;
	bool once = true;

	bool FadeOutFinished = true;
	bool FadeInFinished = true;
	bool fadeout;

	float a;

public:
	void Init();
	void FadeStart();
	void SceneFadeOut();
	void SceneFadeIn();
	bool FadeOutFinishedCheck();
	bool FadeInFinishedCheck();
};

