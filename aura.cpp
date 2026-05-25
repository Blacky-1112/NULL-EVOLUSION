#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "aura.h"
#include "texture.h"


void Aura::Init()
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

	m_Scale = Vector3(0.4f, 0.4f, 0.4f);
}

void Aura::Uninit()
{
	//m_Texture->Release();			//プール化した意味がなくなる→画面遷移時にまとめて削除
	m_VertexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Aura::Update()
{
	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable)
		{
			m_Particle[i].Velocity.y += 0.0002f;
			m_Particle[i].Velocity.x *= 0.92f;
			m_Particle[i].Velocity.y *= 0.94f;
			m_Particle[i].Velocity.z *= 0.92f;


			m_Particle[i].Position += m_Particle[i].Velocity;

			m_Particle[i].Life--;

			//寿命割合
			float t = 0.0f;
			if (m_Particle[i].LifeMax > 0)
				t = 1.0f - (float)m_Particle[i].Life / (float)m_Particle[i].LifeMax;

			//最初はしっかり見えて、最後だけ消える
			float fadeStart = m_Particle[i].FadeStart;
			float amax = m_Particle[i].AlphaMax;
			if (t < fadeStart) m_Particle[i].alpha = 1.0f;
			else
			{
				float u = (t - fadeStart) / (1.0f - fadeStart); // 0→1
				m_Particle[i].alpha = 1.0f - u; // 1→0
			}

			if (m_Particle[i].Life <= 0)
				m_Particle[i].Enable = false;
		}
	}
}

void Aura::Draw()
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

	for (int i = 0; i < Particle_MAX; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//マテリアル設定
			MATERIAL material{};
			material.Diffuse = { m_Particle[i].Diffuse.y, m_Particle[i].Diffuse.z, m_Particle[i].Diffuse.x, m_Particle[i].alpha};
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

	Renderer::SetAddBlendEnable(false);
	Renderer::SetDepthEnable(true);
}

void Aura::Burst(const Vector3& pos, int num, float power)
{
	m_Position = pos;

	for (int n = 0; n < num; n++)
	{
		// 空きを探す
		int iFound = -1;
		for (int i = 0; i < Particle_MAX; i++)
		{
			if (!m_Particle[i].Enable) { iFound = i; break; }
		}
		if (iFound < 0) break;

		auto& p = m_Particle[iFound];
		p.Enable = true;
		p.LifeMax = 20 + (rand() % 15); // 20～34 くらい
		p.Life = p.LifeMax;
		p.FadeStart = 0.15f;
		p.AlphaMax = 1.0f;
		p.Position = pos;

		// ランダム方向（XZ強め、Yは少し）
		float rx = (rand() % 200 - 100) / 100.0f; // -1～1
		float ry = (rand() % 120) / 100.0f;       // 0～1.2（上方向少し）
		float rz = (rand() % 200 - 100) / 100.0f;

		Vector3 v(rx, ry * 0.6f, rz);
		if (v.length() > 0.0001f) v.normalize();

		// 初速：爆発は最初だけ強い
		float sp = power * (0.7f + (rand() % 60) / 100.0f); // 0.7～1.3倍
		p.Velocity = v * sp;

		// 色（今の赤系を活かす）
		p.Diffuse.x = 1.0f;
		p.Diffuse.y = 0.35f;
		p.Diffuse.z = 0.1f;

		p.alpha = 1.0f;
	}
}

static float Rand01()
{
	return (rand() % 1000) / 1000.0f;
}

static Vector3 RandDirXZ(float yMin, float yMax)
{
	float rx = Rand01() * 2.0f - 1.0f;
	float rz = Rand01() * 2.0f - 1.0f;
	float ry = yMin + (yMax - yMin) * Rand01();

	Vector3 v(rx, ry, rz);
	if (v.length() > 0.0001f) v.normalize();
	return v;
}

void Aura::BurstHitBig(const Vector3& pos)
{
	m_Position = pos;

	//火花
	const int sparks = 50;       // 派手さ
	const float spPow = 0.14f;   // 初速（環境に合わせて0.08～0.25くらい）

	for (int n = 0; n < sparks; n++)
	{
		int idx = -1;
		for (int i = 0; i < Particle_MAX; i++)
		{
			if (!m_Particle[i].Enable) { idx = i; break; }
		}
		if (idx < 0) break;

		auto& p = m_Particle[idx];
		p.Enable = true;
		p.LifeMax = 55;
		p.Life = p.LifeMax;
		p.FadeStart = 0.7f;
		p.AlphaMax - 1.0f;
		p.alpha = 1.0f;
		p.Position = pos;

		Vector3 dir = RandDirXZ(0.05f, 0.35f);
		float sp = spPow * (0.6f + Rand01() * 1.4f); // 0.6～2.0倍
		p.Velocity = dir * sp;

		// 火花っぽく
		p.Diffuse.x = 1.0f;
		p.Diffuse.y = 0.85f;
		p.Diffuse.z = 0.25f;

		p.alpha = 1.0f;
	}

	// 煙/熱
	const int smoke = 35;
	const float smPow = 0.05f;

	for (int n = 0; n < smoke; n++)
	{
		int idx = -1;
		for (int i = 0; i < Particle_MAX; i++)
		{
			if (!m_Particle[i].Enable) { idx = i; break; }
		}
		if (idx < 0) break;

		auto& p = m_Particle[idx];
		p.Enable = true;
		p.LifeMax = 55;
		p.Life = p.LifeMax;
		p.FadeStart = 0.7f;
		p.AlphaMax - 1.0f;
		p.alpha = 1.0f;
		p.Position = pos;

		Vector3 dir = RandDirXZ(0.10f, 0.60f);
		float sp = smPow * (0.7f + Rand01() * 0.8f);
		p.Velocity = dir * sp;

		// 煙っぽく
		p.Diffuse.x = 0.6f;
		p.Diffuse.y = 0.25f;
		p.Diffuse.z = 0.20f;

		p.alpha = 0.8f;
	}

	m_Scale = Vector3(0.6f, 0.6f, 0.6f);
}