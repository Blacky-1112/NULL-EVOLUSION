#include "main.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "modelRenderer.h"

#include "meteorShadow.h"
#include "polygon2D.h"



void MeteorShadow::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\cylinder.obj");

	m_Polygon2D = new Polygon2D;
	m_Polygon2D->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\black.png", 0.0f);

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void MeteorShadow::Uninit()
{
	delete m_ModelRenderer;

	m_Polygon2D->Uninit();
	delete m_Polygon2D;

	SetDestroy();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void MeteorShadow::Update()
{

}

void MeteorShadow::Draw()
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


	Renderer::SetStencilShadowWrite();

	//シャドウボリューム描画
	m_ModelRenderer->Draw();

	Renderer::SetStencilShadowRead();

	//影2Dポリゴン
	m_Polygon2D->Draw();
	
	Renderer::SetStencilShadowNone();

	Manager::GetScene()->GetGameObject<Camera>()->Draw();
}
