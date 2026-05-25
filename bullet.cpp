#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "modelRenderer.h"
#include "collision.h"


#include "bullet.h"
#include "aura.h"
#include "meshField.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "seManager.h"
#include "bulletEffect.h"


void BULLET::Init()
{
	m_ModelRenderer = new ModelRenderer();
	m_ModelRenderer->Load("asset\\model\\bullet.obj");

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void BULLET::Uninit()
{
	delete m_ModelRenderer;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BULLET::Update()
{
	if (!m_Enemy)
	{
		SetDestroy();
		return;
	}
	//敵の位置更新
	m_EndPos =
		m_Enemy->GetPosition() +
		Vector3(0, 1.0f, 0);

	Vector3 toEnemy = m_EndPos - m_Position;
	float dist = toEnemy.len();
	toEnemy.normalize();

	//時間更新(徐々に加速)

	m_Time += Manager::GetDeltaTime() * 0.8f;

	float t = m_Time;

	float accelT = t * t * t;

	//エルミート補間
	float h1 = 2 * accelT * accelT * accelT - 3 * accelT * accelT + 1;
	float h2 = -2 * accelT * accelT * accelT + 3 * accelT * accelT;
	float h3 = accelT * accelT * accelT - 2 * accelT * accelT + accelT;
	float h4 = accelT * accelT * accelT - accelT * accelT;

	m_Position = m_StartPos * h1 + m_EndPos * h2 + m_StartVec * h3 + m_EndVec * h4;

	//一応弾回転
	m_Rotation.y += 0.3f;
	m_Rotation.z += 0.15f;

	//Manager::GetScene()->AddGameObject<Explosion>(1)->SetPosition(m_Position);
	BulletEffect* effect = Manager::GetScene()->AddGameObject<BulletEffect>(1);
	m_Effect = effect;

	m_Effect->SetPosition(m_Position);
	m_Effect->SetColor({ 0.0f,0.0f,1.0f,1.0f });

	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();

	//敵との当たり判定
	if(Collision::HitChecker(m_Enemy, this))
	{
		SoundManager::PlaySE(SE::Hit);

		Aura* a = Manager::GetScene()->AddGameObject<Aura>(1);
		a->BurstHitBig(m_Enemy->GetPosition() + Vector3{ 0.0f, 1.2f, 0.0f });

		Manager::GetScene()->AddGameObject<Explosion>(0)->SetPosition(m_Enemy->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));

		pCamera->Shake({ 0.1f,0.7f,0.0f });
		m_Enemy->Damage();
		SetDestroy();
	}


	if (t >= 1.0f)
	{
		SetDestroy();
		return;
	}

}

void BULLET::Draw()
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

void BULLET::Shot(Vector3 playerPos, Vector3 playerForward, Enemy* target, float side)
{
	m_Enemy = target;
	if (!m_Enemy) { SetDestroy(); return; }

	Vector3 worldUp = Vector3(0, 1, 0);

	//基準方向
	Vector3 aimDir = (m_Enemy->GetPosition() + Vector3(0, 1, 0)) - playerPos;
	aimDir.normalize();

	// aimDirを基準に右を作る
	Vector3 right = Vector3::cross(worldUp, aimDir);
	if (right.len() < 0.0001f) right = Vector3(1, 0, 0); // 真上/真下対策
	right.normalize();

	// 敵方向の逆に出す
	Vector3 backOffset = aimDir * -2.0f;
	Vector3 upOffset = Vector3(0, 1.5f, 0);
	m_StartPos = playerPos + backOffset + upOffset;
	m_Position = m_StartPos;

	// 終点
	m_EndPos = m_Enemy->GetPosition() + Vector3(0, 1, 0);

	// 右後ろベクトル（sideで左右）
	m_StartVec =
		right * (8.0f * side)     // 左右
		- aimDir * 3.0f           // 後ろ成分
		+ worldUp * 4.0f;         // 上

	m_Time = 0.0f;
}
