#include "main.h"
#include "manager.h"
#include "camera.h"
#include "titleCamera.h"
#include "renderer.h"
#include "scene.h"
#include "particle.h"
#include "texture.h"


void Particle::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	m_Texture = Texture::Load("asset\\texture\\particle.png");

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	for (int i = 0; i < Particle_MAX; i++)
	{
		m_Particle[i].Enable = false;
	}

	m_Position = Vector3(-3.0f, 0.0f, -1.0f);

	m_Scale = Vector3(0.2f, 0.2f, 0.2f);
}

void Particle::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Particle::Update()
{
	int count = 10;
	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 60;
			m_Particle[i].Position.x = (rand() % 100 - 50);
			m_Particle[i].Position.y = (rand() % 6 + 1);
			m_Particle[i].Position.z = (rand() % 100 - 50);

			//m_Particle[i].Velocity = Vector3{0.98f,0.98f,0.98f};

			m_Particle[i].Diffuse.x = (rand() % 3 + 1) / 10.0f;
			m_Particle[i].Diffuse.y = (rand() % 3 + 1) / 10.0f;
			m_Particle[i].Diffuse.z = (rand() % 3 + 3) / 10.0f;

			m_Particle[i].alpha = 0.7f;
			count--;
			if (count == 0)
				break;
		}
	}

	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			if (rand() % 240 == 0)
			{
				m_Particle[i].Velocity = Vector3(
					(rand() % 5 - 2) / 100.0f,
					(rand() % 3 - 1) / 100.0f,
					(rand() % 5 - 2) / 100.0f
				);
			}
			m_Particle[i].Position += m_Particle[i].Velocity;

			if (m_Particle[i].Life <= 0.0f)
				m_Particle[i].Enable = false;
		}
	}
}

void Particle::Draw()
{
	TitleCamera* pTcamera = Manager::GetScene()->GetGameObject<TitleCamera>();
	Renderer::SetViewMatrix(pTcamera->GetCameraView());

	Renderer::SetProjectionMatrix(
		XMMatrixPerspectiveFovLH(
			1.0f,
			(float)SCREEN_WIDTH / SCREEN_HEIGHT,
			1.0f,
			1000.0f
		)
	);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ビューの逆行列
	TitleCamera* pCamera = Manager::GetScene()->GetGameObject<TitleCamera>();
	XMMATRIX view = pCamera->GetCameraView();
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetDepthEnable(false);
	//Renderer::SetAddBlendEnable(true);

	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//マテリアル設定
			MATERIAL material{};
			material.Diffuse = { m_Particle[i].Diffuse.x, m_Particle[i].Diffuse.y, m_Particle[i].Diffuse.z, m_Particle[i].alpha};
			material.TextureEnable = true;
			Renderer::SetMaterial(material);

			XMMATRIX world, scale, trans;

			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}

	//for (int i = 0; i < Particle_MAX; i++)
	//{
	//	if (m_Particle[i].Enable == true)
	//	{
	//		//マテリアル設定
	//		MATERIAL material{};
	//		material.Diffuse = { m_Particle[i].Diffuse.y, m_Particle[i].Diffuse.z, m_Particle[i].Diffuse.x, m_Particle[i].alpha};
	//		material.TextureEnable = true;
	//		Renderer::SetMaterial(material);

	//		XMMATRIX world, scale, trans;

	//		scale = XMMatrixScaling(m_Scale.x * 2.0f, m_Scale.y * 2.0f, m_Scale.z * 2.0f);
	//		trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
	//		world = scale * invView * trans;
	//		Renderer::SetWorldMatrix(world);

	//		//ポリゴン描画
	//		Renderer::GetDeviceContext()->Draw(4, 0);
	//	}
	//}

	//Renderer::SetAddBlendEnable(false);
	Renderer::SetDepthEnable(true);
}
