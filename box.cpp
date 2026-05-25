#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "modelRenderer.h"

#include "box.h"
#include "texture.h"
#include "player.h"
#include "aura.h"
#include "shadow.h"
#include "audio.h"


void Box::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\box2.obj");

	m_Input = new Input();

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\fieldVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\fieldPS.cso");

	// テクスチャ読み込み
	m_Texture = Texture::Load("asset\\model\\grass2.jpg");
	m_TextureWall = Texture::Load("asset\\model\\gravel2.jpg");


	m_Scale.x = 5.0f;
}

void Box::Uninit()
{
	delete m_ModelRenderer;

	//m_Aura->SetDestroy();
	SetDestroy();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Box::Update()
{
	m_Rotation.y += 0.01f;
}

void Box::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureWall);

	m_ModelRenderer->Draw();
}
