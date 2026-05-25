#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "wave.h"


void Wave::Init()
{
	// 頂点バッファ生成
	{
		for (int x = 0; x < 21; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				m_Vertex[x][z].Position =
					XMFLOAT3((x - 10) * 5.0f, 0.0f, (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}


		//法線ベクトル算出
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	// インデックスバッファ生成
	{
		unsigned int index[((21 + 1) * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}


	// テクスチャ読み込み
	m_Texture = Texture::Load("asset\\texture\\water.png");
	m_EnvTexture = Texture::Load("asset\\model\\planet.jpg");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\waveVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wavePS.cso");

	m_Position = {0.0f, 0.25f, 0.0f};
}


void Wave::Uninit()
{

	m_VertexBuffer->Release();
	//m_Texture->Release();
	//m_EnvTexture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}


void Wave::Update()
{

	for (int x = 0; x < 21; x++)
	{
		for (int z = 0; z < 21; z++)
		{
			float dx = m_Vertex[z][x].Position.x - m_Vertex[0][0].Position.x;
			float dz = m_Vertex[z][x].Position.z - m_Vertex[0][0].Position.z;
			float length = sqrtf(dx * dx + dz * dz);

			float phase = m_Vertex[z][x].Position.x * wx + m_Vertex[z][x].Position.z * wz;

			m_Vertex[x][z].Position.y = 0.25f * sinf(phase * 0.2f + m_Time * -1.0f /*+ length * 0.5f*/);

			//m_Vertex[x][z].Position.y += 0.1f * sinf();
		}
	}

	for (int x = 1; x < 20; x++)
	{
		for (int z = 1; z < 20; z++)
		{
			Vector3 vx, vz, vn;

			vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
			vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
			vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

			vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
			vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
			vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

			vn = Vector3::cross(vz, vx);	//外積
			vn.normalize();					//正規化

			m_Vertex[x][z].Normal.x = vn.x;
			m_Vertex[x][z].Normal.y = vn.y;
			m_Vertex[x][z].Normal.z = vn.z;
		}
	}

	m_Time += 1.0f / 60.0f;



	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	memcpy(vertex, m_Vertex, sizeof(VERTEX_3D) * 21 * 21);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}



void Wave::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(
		m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// マテリアル設定
	MATERIAL material{};
	//ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	//Renderer::GetDeviceContext()->Draw(21 * 21, 0);
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);

}


