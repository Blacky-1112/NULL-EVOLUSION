#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"


#include "fannel.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "ballistic.h"
#include "score.h"
#include "audio.h"


void FANNEL::Init()
{
	m_ModelRenderer = new ModelRenderer();
	//m_ModelRenderer->Load("asset\\model\\torus.obj");
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	m_Input = new Input();

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void FANNEL::Uninit()
{
	delete m_ModelRenderer;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void FANNEL::Update()
{
	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
	if (pPlayer != nullptr)
	{
		m_Target = pPlayer->GetPosition() + Vector3(0.0f, 1.0f, 0.0f);
		//m_Target += GetRight() * 0.5f;
	}

	m_Frame++;
	m_Time += 0.2f;

	m_Position = m_Target + Vector3(-sinf(m_Time), 0.0f, -cosf(m_Time)) * 2.0f;

	auto pEnemies = Manager::GetScene()->GetGameObjects<Enemy>();

	for (auto enemy : pEnemies)
	{
		Vector3 d = enemy->GetPosition() - m_Position;
		float length = d.len();

		if (length < 1.0f)
		{

			Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));

			Score* pScore = Manager::GetScene()->GetGameObject<Score>();
			pScore->Add(5);

			enemy->SetDestroy();
			SetDestroy();
		}
	}

	if (m_Frame >= 60)
	{
		SetDestroy();
	}
}

void FANNEL::Draw()
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

	m_ModelRenderer->Draw();
}
