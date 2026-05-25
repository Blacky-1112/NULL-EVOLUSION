#include "main.h"
#include "manager.h"
#include "camera.h"
#include "collision.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"

#include "meshField.h"
#include "enemybullet.h"
#include "bulletEffect.h"
#include "meteorShadow.h"
#include "playerhpUI.h"
#include "player.h"
#include "explosion.h"
#include "aura.h"
#include "seManager.h"


void EnemyBullet::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Type = BULLET_STRAIGHT;
	m_Speed = 0.1f;
	m_RotateSpeed = 0.03f;
}

void EnemyBullet::Uninit()
{
	delete m_ModelRenderer;

	if (m_Shadow)
	{
		m_Shadow->SetDestroy();
		m_Shadow = nullptr;
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void EnemyBullet::Update()
{
	switch (m_Type)
	{
	case BULLET_STRAIGHT:
		UpdateStraight();
		break;

	case BULLET_HOMING:
		UpdateHoming();
		break;

	case BULLET_WAVE:
		UpdateWave();
		break;

	case BULLET_BIG:
		UpdateBig();
		break;
	case BULLET_BEAM:
		UpdateBeam();
		break;
	case BULLET_METEOR:
		UpdateMeteor();
		break;
	}

	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
	BulletEffect* effect = Manager::GetScene()->AddGameObject<BulletEffect>(1);
	m_Effect = effect;

	m_Effect->SetPosition(m_Position);
	m_Effect->SetScale(m_Scale);
	m_Effect->SetColor({1.0f,0.0f,0.0f,1.0f});

	if (Collision::HitBulletToPlayer(pPlayer, this))
	{
		Aura* a = Manager::GetScene()->AddGameObject<Aura>(1);
		a->Burst(m_Position, 35, 0.08f);

		SoundManager::PlaySE(SE::Damage);
		pPlayer->Damage();
		pPlayer->StartInvincible(1.0f);
		SetDestroy();
	}

	m_Time++;

	if (m_Time > 180 && m_Type != BULLET_METEOR)
	{
		SetDestroy();
	}
}

void EnemyBullet::Draw()
{
	auto ctx = Renderer::GetDeviceContext();

	ctx->IASetInputLayout(m_VertexLayout);
	ctx->VSSetShader(m_VertexShader, NULL, 0);
	ctx->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	m_ModelRenderer->Draw();

	if (m_Type == BULLET_BEAM)
	{
		ctx->RSSetState(nullptr);
	}
}

void EnemyBullet::UpdateStraight()
{
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position += m_Velocity;
	m_Position.y = mesh->GetHeight(m_Position) + 1.0f;
}

void EnemyBullet::UpdateHoming()
{
	Player* p = Manager::GetScene()->GetGameObject<Player>();
	Vector3 dir = p->GetPosition() - m_Position;
	dir.normalize();

	// 向きを少しずつ変える
	m_Velocity = Vector3::Lerp(m_Velocity, dir * m_Speed, m_RotateSpeed);

	m_Position += m_Velocity;
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position.y = mesh->GetHeight(m_Position) + 1.0f;
}

void EnemyBullet::UpdateWave()
{
	m_Frame++;
	float wave = sinf(m_Frame * 0.3f) * 0.5f;
	Vector3 side(-m_Velocity.z, 0.0f, m_Velocity.x);

	m_Position += m_Velocity + side * wave;
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position.y = mesh->GetHeight(m_Position) + 1.0f;
}

void EnemyBullet::UpdateBig()
{
	m_Position += m_Velocity;
	m_Scale = Vector3{ 2.5f,2.5f,2.5f };
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position.y = mesh->GetHeight(m_Position) + 1.0f;
}

void EnemyBullet::UpdateBeam()
{
	// 前進
	m_Position += m_Velocity;
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_Position.y = mesh->GetHeight(m_Position) + 1.0f;

	// 向きを速度に合わせる
	Vector3 dir = m_Velocity;
	dir.normalize();

	m_Rotation.y = atan2f(dir.x, dir.z);

	// ビームっぽいスケール
	float length = 8.0f;

	m_Scale = Vector3(
		1.5f,   // 太さ
		1.5f,
		length  // ← 前方向に長い
	);
}

void EnemyBullet::UpdateMeteor()
{
	//出現後の溜め
	if (m_WaitTime > 0)
	{
		m_WaitTime--;
		m_Rotation.y += 0.02f;  // 回転演出
		return;
	}

	//時間進行
	m_CurrentTime++;

	//ゴゴゴってカメラが揺れる感じ
	Camera* cam = Manager::GetScene()->GetGameObject<Camera>();
	cam->StartContinuousShake(Vector3(0.1f, 0.1f, 0.1f));

	float t = m_CurrentTime / m_TotalFallTime;

	if (m_Shadow)
	{
		float scale = 1.0f + t * 3.0f;
		float MaxScale = m_Scale.x * 1.2f;
		if (scale >= MaxScale)
		{
			scale = MaxScale;
		}

		m_Shadow->SetPosition(Vector3(
			m_Position.x,
			m_TargetY + 0.05f,
			m_Position.z
		));

		m_Shadow->SetScale(Vector3(scale, 1.0f, scale));
	}

	if (t > 1.0f)
		t = 1.0f;

	float heightRate = 1.0f - (t * t * t);

	//落下
	m_Position.y = m_TargetY + heightRate * m_StartHeight;

	//揺れている演出
	m_Position.x += m_Drift.x;
	m_Position.z += m_Drift.z;

	// 回転演出
	m_Rotation.x += 0.05f;
	m_Rotation.y += 0.03f;

	//着弾
	if (t >= 1.0f)
	{
		//ゴゴゴ停止
		cam->StopContinuousShake();

		// インパクトシェイク
		cam->Shake(Vector3(0.5f, 0.5f, 0.5f));
		if (m_Shadow)
			m_Shadow->SetDestroy();

		Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(m_Position);
		SoundManager::PlaySE(SE::MeteorBurn);

		SetDestroy();
	}
}

void EnemyBullet::ShotMeteor(Vector3 centerPos, float range)
{
	//位置ランダム
	float rx = ((rand() % 200) / 100.0f - 1.0f) * range;
	float rz = ((rand() % 200) / 100.0f - 1.0f) * range;

	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();

	//落下開始位置指定
	m_StartHeight = 20.0f + (rand() % 100) / 10.0f;

	//停滞する時間
	m_WaitTime = (rand() % 30 + 30);

	//敵を中心に一定範囲のどこか
	m_Position = centerPos + Vector3(rx, m_StartHeight, rz);
	m_TargetY = mesh->GetHeight(m_Position);

	//落下しながら揺れるように
	m_Drift.x = ((rand() % 100) / 100.0f - 0.5f) * 0.05f;
	m_Drift.z = ((rand() % 100) / 100.0f - 0.5f) * 0.05f;

	//警告円表示
	MeteorShadow* shadow =
		Manager::GetScene()->AddGameObject<MeteorShadow>(1);

	m_Shadow = shadow;

	m_Shadow->SetPosition(Vector3(
		m_Position.x,
		m_TargetY + 0.05f,
		m_Position.z
	));

	m_Shadow->SetScale(Vector3(1.0f, 1.0f, 1.0f));

	m_CurrentTime = 0.0f;
	m_Type = BULLET_METEOR;
}
