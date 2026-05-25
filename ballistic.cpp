#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "ballistic.h"
#include "texture.h"


void BALLISTIC::Init()
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

	BALLISTIC::SetBool(true);

	m_Position = Vector3(offset, 4.0f, 10.0f);

	m_Scale = Vector3(0.2f, 0.2f, 0.2f);
}

void BALLISTIC::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BALLISTIC::Update()
{
	int count = 3;
	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 10;
			m_Particle[i].Position = m_Position;
			m_Particle[i].Velocity.x = (rand() % 100 - 50) / 300.0f;
			//m_Particle[i].Velocity.y = (rand() % 50 - 60) / 300.0f;
			m_Particle[i].Velocity.z = (rand() % 50 + 1) / 500.0f;

			m_Particle[i].Diffuse.x = 0.2f;
			m_Particle[i].Diffuse.y = 0.2f;
			m_Particle[i].Diffuse.z = 1.0f;

			m_Particle[i].alpha = 1.0f;
			count--;
			if (count == 0)
				break;
		}
	}

	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			m_Particle[i].Velocity += Vector3(0.0f, 0.02f, -0.05f);

			m_Particle[i].Velocity.x -= m_Particle[i].Velocity.x * 0.05f;
			m_Particle[i].Velocity.y += m_Particle[i].Velocity.y * 0.1f;
			m_Particle[i].Velocity.z -= m_Particle[i].Velocity.z * 0.05f;

			m_Particle[i].Position += m_Particle[i].Velocity;

			m_Particle[i].Life--;

			m_Particle[i].alpha -= 0.05f;
			if (m_Particle[i].Life == 0)
			{
				SetBool(false);
				m_Particle[i].Enable = false;
			}
		}
	}
}

void BALLISTIC::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ビューの逆行列
	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();
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
	Renderer::SetAddBlendEnable(true);

	if (BALLISTIC::GetBool())
	{
		for (int i = 0; i < Particle_MAX; i++)
		{
			if (m_Particle[i].Enable == true)
			{
				//マテリアル設定
				MATERIAL material{};
				material.Diffuse = { m_Particle[i].Diffuse.x, m_Particle[i].Diffuse.y, m_Particle[i].Diffuse.z, m_Particle[i].alpha };
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

			if (m_Particle[i].Enable == true)
			{
				//マテリアル設定
				MATERIAL material{};
				material.Diffuse = { m_Particle[i].Diffuse.y, m_Particle[i].Diffuse.z, m_Particle[i].Diffuse.x, m_Particle[i].alpha };
				material.TextureEnable = true;
				Renderer::SetMaterial(material);

				XMMATRIX world, scale, trans;

				scale = XMMatrixScaling(m_Scale.x * 2.0f, m_Scale.y * 2.0f, m_Scale.z * 2.0f);
				trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
				world = scale * invView * trans;
				Renderer::SetWorldMatrix(world);

				//ポリゴン描画
				Renderer::GetDeviceContext()->Draw(4, 0);
			}
		}
	}

	Renderer::SetAddBlendEnable(false);
	Renderer::SetDepthEnable(true);
}
