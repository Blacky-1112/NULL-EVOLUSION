#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "treeInstance.h"
#include "meshField.h"
#include "texture.h"


void TreeInstance::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-1.0f, 4.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(1.0f, 4.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	m_Texture = Texture::Load("asset\\texture\\tree.png");

	//ストラクチャーバッファ生成
	{
		Vector3* pos = new Vector3[TREE_MAX]{};
		MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();

		for (int i = 0; i < TREE_MAX; i++)
		{
			pos[i].x = ((rand() % 1000) / 1000.0f) * 100.0f - 50.0f;
			pos[i].z = ((rand() % 1000) / 1000.0f) * 100.0f - 50.0f;

			pos[i].y = mesh->GetHeight(pos[i]);
		}


		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vector3) * TREE_MAX;
		bd.StructureByteStride = sizeof(Vector3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = pos;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_PositionBuffer);

		delete[] pos;

		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = TREE_MAX;

		Renderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srvd, &m_PositionSRV);
	}


	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\treeVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\treePS.cso");

	m_Scale = {1.0f,1.5f,1.0f};
}

void TreeInstance::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	m_VertexBuffer->Release();

	m_PositionBuffer->Release();
	m_PositionSRV->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TreeInstance::Update()
{
}

void TreeInstance::Draw()
{
	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();

	//視錐台カリング
	/*if (!pCamera->CheckView(m_Position, 1.0 * m_Scale.x))
		return;*/

	////距離カリング
	//Vector3 cameraPos = pCamera->GetPosition();
	//float length = (m_Position - cameraPos).length();
	//if (length > 20.0f)
	//	return;

	float alpha = 1.0f;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	XMMATRIX view = pCamera->GetCameraView();

	//ビューの逆行列
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y , m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, alpha };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//ストラクチャーバッファ設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetATCEnable(true);

	//ポリゴン描画(インスタンス済み)
	Renderer::GetDeviceContext()->DrawInstanced(4, TREE_MAX, 0, 0);

	Renderer::SetATCEnable(false);
}
