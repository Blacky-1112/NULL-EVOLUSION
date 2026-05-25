#include "main.h"
#include "renderer.h"
#include "titleMesh.h"
#include "random"
#include "texture.h"

float g_TitleHeight[21][21] =
{
	{0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,2.0f,2.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,2.0f,1.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,3.0f,2.0f,2.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,3.0f,3.0f,2.0f,2.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{2.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
};

static void GenerateHeightMap(float map[21][21], float minH = 0.0f, float maxH = 5.0f, float roughness = 0.5f)
{
	const int SIZE = 21;
	const int N = SIZE - 1;   // 20
	std::mt19937 mt((unsigned)time(nullptr));
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	// 初期の4隅
	map[0][0] = dist(mt);
	map[0][N] = dist(mt);
	map[N][0] = dist(mt);
	map[N][N] = dist(mt);

	int step = N;
	float scale = (maxH - minH) * 0.5f;

	while (step > 1)
	{
		int half = step / 2;

		// ダイヤモンドステップ
		for (int z = 0; z < N; z += step)
		{
			for (int x = 0; x < N; x += step)
			{
				float a = map[z][x];
				float b = map[z][x + step];
				float c = map[z + step][x];
				float d = map[z + step][x + step];

				float mid = (a + b + c + d) * 0.25f + (dist(mt) - 0.5f) * scale;

				map[z + half][x + half] = mid;
			}
		}

		// スクエアステップ
		for (int z = 0; z <= N; z += half)
		{
			for (int x = ((z + half) % step); x <= N; x += step)
			{
				float sum = 0;
				int cnt = 0;

				if (x - half >= 0) { sum += map[z][x - half]; cnt++; }
				if (x + half <= N) { sum += map[z][x + half]; cnt++; }
				if (z - half >= 0) { sum += map[z - half][x]; cnt++; }
				if (z + half <= N) { sum += map[z + half][x]; cnt++; }

				float mid = (sum / cnt) + (dist(mt) - 0.5f) * scale;
				map[z][x] = mid;
			}
		}

		step /= 2;
		scale *= roughness;
	}

	// スケール調整
	for (int z = 0; z <= N; z++)
	{
		for (int x = 0; x <= N; x++)
		{
			map[z][x] = minH + (map[z][x] * (maxH - minH));
		}
	}
}

void TitleMesh::Init()
{
	GenerateHeightMap(g_TitleHeight, 0.0f, 2.0f, 0.1f);

	// 頂点バッファ生成
	{
		for (int x = 0; x <= 20; x++)
		{
			for (int z = 0; z <= 20; z++)
			{
				float y = g_TitleHeight[z][x];
				m_Vertex[x][z].Position = XMFLOAT3((x - 10) * 5.0f, y, (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x * 0.5f, z * 0.5f);
			}
		}

		m_Vertex[10][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[11][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[12][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[13][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[14][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

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


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// インデックスバッファ生成
	{
		unsigned int index[(22 * 2) * 20 - 2 ];
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
	m_Texture = Texture::Load("asset\\texture\\field.jpg");
	m_TextureSoil = Texture::Load("asset\\texture\\soil.jpg");

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\fieldVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\fieldPS.cso");
}


void TitleMesh::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void TitleMesh::Update()
{

}


void TitleMesh::Draw()
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

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer( 
		m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	material.TextureEnable = true;
	Renderer::SetMaterial( material );

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureSoil);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);

}


//float TitleMesh::GetHeight(Vector3 Position)
//{
//	int x, z;
//
//	//ブロック番号算出
//	x = Position.x / 5.0f + 10.0f;
//	z = Position.z / -5.0f + 10.0f;
//
//	XMFLOAT3 pos0, pos1, pos2, pos3;
//
//	pos0 = m_Vertex[x][z].Position;
//	pos1 = m_Vertex[x + 1][z].Position;
//	pos2 = m_Vertex[x][z + 1].Position;
//	pos3 = m_Vertex[x + 1][z + 1].Position;
//
//	Vector3 v12, v1p;
//	v12.x = pos2.x - pos1.x;
//	v12.y = pos2.y - pos1.y;
//	v12.z = pos2.z - pos1.z;
//
//	v1p.x = Position.x - pos1.x;
//	v1p.y = Position.y - pos1.y;
//	v1p.z = Position.z - pos1.z;
//
//	//外積
//	float cy = v12.z * v1p.x - v12.x * v1p.z;
//
//	float py;
//	Vector3 n;
//	if (cy > 0.0f)
//	{//左上ポリゴン
//		Vector3 v10;
//		v10.x = pos0.x - pos1.x;
//		v10.y = pos0.y - pos1.y;
//		v10.z = pos0.z - pos1.z;
//
//		//外積
//		n = Vector3::cross(v10, v12);
//	}
//	else
//	{//右下ポリゴン
//		Vector3 v13;
//		v13.x = pos3.x - pos1.x;
//		v13.y = pos3.y - pos1.y;
//		v13.z = pos3.z - pos1.z;
//
//		//外積
//		n = Vector3::cross(v12, v13);
//	}
//
//	//高さ調整
//	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;
//
//	return py;
//}