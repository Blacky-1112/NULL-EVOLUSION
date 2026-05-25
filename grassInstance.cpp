#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "grassInstance.h"
#include "meshField.h"
#include "texture.h"
#include "player.h"


void GrassInstance::Init()
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
	m_Texture = Texture::Load("asset\\texture\\grass.png");

	//ストラクチャーバッファ生成
	{//座標指定
		Vector3* pos = new Vector3[GRASS_MAX]{};
		MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();

		for (int i = 0; i < GRASS_MAX; i++)
		{
			pos[i].x = ((rand() % 1000) / 1000.0f) * 100.0f - 50.0f;
			pos[i].z = ((rand() % 1000) / 1000.0f) * 100.0f - 50.0f;
			pos[i].y = mesh->GetHeight(pos[i]);

			if (pos[i].y < 0.2)
				i--;
		}


		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vector3) * GRASS_MAX;
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
		srvd.Buffer.NumElements = GRASS_MAX;

		Renderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srvd, &m_PositionSRV);
	}

	{//スケール設定
		Vector3* scale = new Vector3[GRASS_MAX]{};
		MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();

		for (int i = 0; i < GRASS_MAX; i++)
		{
			scale[i].x = ((rand() % 100) / 100.0f) + 0.01f;
			scale[i].z = ((rand() % 100) / 100.0f) + 0.01f;
			scale[i].y = ((rand() % 100) / 100.0f) + 0.01f;

			if (scale[i].y > 0.3 || scale[i].x < 0.2 || scale[i].z < 0.2)
				i--;
		}


		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vector3) * GRASS_MAX;
		bd.StructureByteStride = sizeof(Vector3);
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = scale;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ScaleBuffer);

		delete[] scale;

		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;
		srvd.Buffer.NumElements = GRASS_MAX;

		Renderer::GetDevice()->CreateShaderResourceView(m_ScaleBuffer, &srvd, &m_ScaleSRV);
	}

	{//草の揺れ設定
		D3D11_BUFFER_DESC cbd{};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(WindParamCB);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(&cbd, nullptr, &m_WindBuffer);
		m_Elapsed = 0.0f;
	}

	{//草がプレイヤーによって潰れる設定
		D3D11_BUFFER_DESC cbd{};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(PlayerParamCB);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = Renderer::GetDevice()->CreateBuffer(&cbd, nullptr, &m_PlayerBuffer);
	}

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\grassVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\grassPS.cso");
}

void GrassInstance::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	if (m_VertexBuffer) m_VertexBuffer->Release();

	if (m_PositionBuffer) m_PositionBuffer->Release();
	if (m_PositionSRV) m_PositionSRV->Release();

	if (m_ScaleBuffer) m_ScaleBuffer->Release();
	if (m_ScaleSRV) m_ScaleSRV->Release();

	if (m_WindBuffer) m_WindBuffer->Release();

	if (m_VertexLayout) m_VertexLayout->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();
}

void GrassInstance::Update()
{
	{//草を揺らす
		m_Elapsed += Manager::GetDeltaTime();

		WindParamCB wp{};
		wp.Time = m_Elapsed;
		wp.WindSpeed = 1.5f;
		wp.WindStrength = 0.3f;
		wp.WindDir = XMFLOAT2(1.0f, 0.3f);

		D3D11_MAPPED_SUBRESOURCE mapped{};
		auto ctx = Renderer::GetDeviceContext();

		if (m_WindBuffer)
		{
			if (SUCCEEDED(ctx->Map(m_WindBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
			{
				memcpy(mapped.pData, &wp, sizeof(WindParamCB));
				ctx->Unmap(m_WindBuffer, 0);
			}
		}
		ID3D11Buffer* windArr[1] = {m_WindBuffer};
		ctx->VSSetConstantBuffers(6, 1, windArr);
	}

	{//プレイヤーの位置にある草を潰す
		PlayerParamCB pp{};
		Player* player = Manager::GetScene()->GetGameObject<Player>();

		pp.PlayerPos = player->GetPosition();

		pp.PlayerRadius = 2.0f;

		D3D11_MAPPED_SUBRESOURCE mapped{};
		auto ctx = Renderer::GetDeviceContext();

		if (m_PlayerBuffer)
		{
			if (SUCCEEDED(ctx->Map(m_PlayerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
			{
				memcpy(mapped.pData, &pp, sizeof(PlayerParamCB));
				ctx->Unmap(m_PlayerBuffer, 0);
			}
		}
		ID3D11Buffer* cbArr[1] = {m_PlayerBuffer};
		ctx->VSSetConstantBuffers(7, 1, cbArr);
	}
}

void GrassInstance::Draw()
{
	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();

	//視錐台カリング
	//if (!pCamera->CheckView(m_Position, 1.0 * m_Scale.x))
	//	return;

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
	ID3D11Buffer* windArr[1] = { m_WindBuffer };
	ID3D11Buffer* playerArr[1] = { m_PlayerBuffer };

	Renderer::GetDeviceContext()->VSSetConstantBuffers(6, 1, windArr);
	Renderer::GetDeviceContext()->VSSetConstantBuffers(7, 1, playerArr);

	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);
	Renderer::GetDeviceContext()->VSSetShaderResources(3, 1, &m_ScaleSRV);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetATCEnable(true);

	//ポリゴン描画(インスタンス済み)
	Renderer::GetDeviceContext()->DrawInstanced(4, GRASS_MAX, 0, 0);

	Renderer::SetATCEnable(false);
}
