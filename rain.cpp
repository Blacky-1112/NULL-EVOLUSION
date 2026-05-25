#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "rain.h"
#include "texture.h"
#include "meshField.h"


void Rain::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-0.1f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.1f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.1f, -1.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.1f, -1.0f, 0.0f);
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
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\rainVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\rainPS.cso");

	for (int i = 0; i < Rain_MAX; i++)
	{
		m_Rain[i].Enable = false;
	}

	m_Position = Vector3(-3.0f, 0.0f, -1.0f);

	m_Scale = Vector3(0.2f, 0.2f, 0.2f);
}

void Rain::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Rain::Update()
{
	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();
	int count = 20;

	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	for (int i = 0; i < Rain_MAX; i++)
	{
		if (m_Rain[i].Enable == false)
		{
			m_Rain[i].Enable = true;
			m_Rain[i].Position.x = pCamera->GetPosition().x + (rand() % 10 - 5);
			m_Rain[i].Position.y = pCamera->GetPosition().y + 5.0f;
			m_Rain[i].Position.z = pCamera->GetPosition().z + (rand() % 10 - 5);
			m_Rain[i].Velocity.x = (rand() % 100 - 50) / 500.0f;
			m_Rain[i].Velocity.y = -(rand() % 100 + 50) / 500.0f;
			m_Rain[i].Velocity.z = (rand() % 100 - 50) / 500.0f;

			/*m_Rain[i].Diffuse.x = (rand() % 9 + 1) / 10.0f;
			m_Rain[i].Diffuse.y = (rand() % 9 + 1) / 10.0f;
			m_Rain[i].Diffuse.z = (rand() % 9 + 1) / 10.0f;*/

			m_Rain[i].Diffuse.x = 0.5f;
			m_Rain[i].Diffuse.y = 0.5f;
			m_Rain[i].Diffuse.z = 0.5f;

			m_Rain[i].alpha = 1.0f;
			count--;
			if (count == 0)
				break;
		}
	}

	for (int i = 0; i < Rain_MAX; i++)
	{
		if (m_Rain[i].Enable == true)
		{
			m_Rain[i].Velocity += Vector3(0.0f, -0.01f, 0.0f);

			m_Rain[i].Position += m_Rain[i].Velocity;

			if (m_Rain[i].Position.y <= mesh->GetHeight(m_Position))
				m_Rain[i].Enable = false;
		}
	}
}

void Rain::Draw()
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

	for (int i = 0; i < Rain_MAX; i++)
	{
		if (m_Rain[i].Enable == true)
		{
			//マテリアル設定
			MATERIAL material{};
			material.Diffuse = { m_Rain[i].Diffuse.x, m_Rain[i].Diffuse.y, m_Rain[i].Diffuse.z, m_Rain[i].alpha};
			material.TextureEnable = true;
			Renderer::SetMaterial(material);

			XMMATRIX world, scale, trans;

			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			trans = XMMatrixTranslation(m_Rain[i].Position.x, m_Rain[i].Position.y, m_Rain[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
	Renderer::SetDepthEnable(true);
}
