#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "modelRenderer.h"

#include "testObject.h"
#include "player.h"
#include "aura.h"
#include "shadow.h"
#include "audio.h"
#include "texture.h"


void TestObject::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\cube.obj");

	m_Input = new Input();

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\envMapVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\envMapPS.cso");

	m_EnvTexture = Texture::Load("asset\\model\\blueSky.jpg");

	//クオータニオン初期化
	m_Quaternion.x = 0.0f;
	m_Quaternion.y = 0.0f;
	m_Quaternion.z = 0.0f;
	m_Quaternion.w = 1.0f;

}

void TestObject::Uninit()
{
	delete m_ModelRenderer;

	//m_Aura->SetDestroy();
	SetDestroy();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TestObject::Update()
{
	if (Input::GetKeyPress('A'))
	{
		m_Position.x -= 0.1f;
		//m_Rotation.z += 0.1f;
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.0f,0.0f,0.1f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position.x += 0.1f;
		//m_Rotation.z -= 0.1f;
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, -0.1f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}
	if (Input::GetKeyPress('W'))
	{
		m_Position.z += 0.1f;
		//m_Rotation.x += 0.1f;
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0.1f, 0.0f, 0.0f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position.z -= 0.1f;
		//m_Rotation.x -= 0.1f;
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(-0.1f, 0.0f, 0.0f);
		quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
		XMStoreFloat4(&m_Quaternion, quat);
	}
}

void TestObject::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	//マトリクス設定
	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	//rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	rot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_Quaternion));//クォータニオン→回転マトリックス
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}
