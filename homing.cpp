#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"


#include "homing.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "ballistic.h"
#include "score.h"
#include "audio.h"


void HOMING::Init()
{
	m_ModelRenderer = new ModelRenderer();
	//m_ModelRenderer->Load("asset\\model\\torus.obj");
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	m_Input = new Input();

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void HOMING::Uninit()
{
	delete m_ModelRenderer;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HOMING::Update()
{
	m_Time += 0.03f;

	float i = m_Time;

	m_Position = m_StartPos * (2.0f * i * i * i - 3.0f * i * i + 1.0f)
		+ m_EndPos * (-2.0f * i * i * i + 3.0f * i * i)
		+ m_StartVec * (i * i * i - 2.0f * i * i + i) * 1.5f
		+ m_EndVec * (i * i * i - i * i) * 1.5f;

	//Manager::GetScene()->AddGameObject<EXPLOSION>(1)->SetPosition(m_Position);
	Manager::GetScene()->AddGameObject<BALLISTIC>(1)->SetPosition(m_Position + Vector3(0.0f, -0.05f, 0.0f));

	auto pEnemies = Manager::GetScene()->GetGameObjects<Enemy>();

	for (auto enemy : pEnemies)
	{
		Vector3 d = enemy->GetPosition() - m_Position;
		float length = d.len();

		if (length < 1.0f)
		{
			Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(enemy->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));

			Score* pScore = Manager::GetScene()->GetGameObject<Score>();
			pScore->Add(10);

			enemy->SetDestroy();
			SetDestroy();
		}
	}

}

void HOMING::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y + XM_PI, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();
}
