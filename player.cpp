#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "scene.h"
#include "audio.h"
#include "seManager.h"
#include "cursor.h"

#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "modelRenderer.h"
#include "bullet.h"
#include "fannel.h"
#include "homing.h"
#include "item.h"
#include "box.h"
#include "bombCount.h"
#include "sparkle.h"
#include "shadow.h"
#include "shadowVolume.h"
#include "animationModel.h"


void Player::Init()
{
	m_AnimModel = new AnimationModel();
	m_AnimModel->Load("asset\\model\\Character.fbx");
	m_AnimModel->LoadAnimation("asset\\model\\Character_Run.fbx", "Run");
	m_AnimModel->LoadAnimation("asset\\model\\Character_Idle.fbx", "Idle");
	m_AnimModel->LoadAnimation("asset\\model\\Character_Jump.fbx", "Jump");
	m_AnimModel->LoadAnimation("asset\\model\\Character_Damage.fbx", "Damage");
	m_AnimModel->LoadAnimation("asset\\model\\Character_Dying.fbx", "Dying");

	m_AnimName = "Idle";
	m_AnimNameNext = "Idle";
	m_AnimBlend = 0.0f;


	m_Frame = 0;

	m_Input = new Input();

	m_Position = {0.0f,1.5f,0.0f};

	m_Scale = {1.0f, 1.0f, 1.0f};

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_ShadowVolume = new ShadowVolume();
	m_ShadowVolume->Init();
	m_ShadowVolume->SetScale(Vector3{ 0.6f, 1.0f, 0.6f });
}

void Player::Uninit()
{
	m_ShadowVolume->Uninit();
	delete m_ShadowVolume;

	m_AnimModel->Uninit();
	delete m_AnimModel;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	if (m_AnimName.c_str() == "Jump")
	{
		m_AnimModel->Update(m_AnimName.c_str(), m_JumpFrame,
			m_AnimNameNext.c_str(), m_JumpFrame, m_AnimBlend);
	}
	else
	{
		m_AnimModel->Update(m_AnimName.c_str(), m_Frame,
			m_AnimNameNext.c_str(), m_Frame, m_AnimBlend);
	}

	m_Frame++;
	m_JumpFrame++;

	m_AnimBlend += 0.1f;
	if (m_AnimBlend > 1.0f)
	{
		m_AnimBlend = 1.0f;
	}

	float dt = Manager::GetDeltaTime();

	if (m_IsKnockback)
	{

		// ノックバック移動（地面方向のみ）
		m_Position += m_KnockVel * dt;

		// 地面に吸い付け
		MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();
		if (meshField && !isJump)
		{
			if (m_Position.y < meshField->GetHeight(m_Position))
				m_Position.y = meshField->GetHeight(m_Position);
		}

		// 影も追従
		m_ShadowVolume->SetPosition(m_Position + Vector3{ 0.0f,-0.45f,0.0f });

		m_Rotation.y += 0.35f; // くるくる回る

		// タイマー更新
		m_KnockTimer += dt;

		if (m_KnockTimer >= m_KnockDuration)
		{
			m_IsKnockback = false;

			// 操作復帰
			SetControllEnable(true);
		}
	}

	if (m_IsInvincible)
	{//被弾中
		// 無敵タイマー
		m_InvincibleTimer += dt;

		if (m_InvincibleTimer >= m_InvincibleDuration)
		{
			m_IsInvincible = false;
		}
	}


	if (m_IsKnockback)
	{
		if (m_AnimNameNext != "Damage")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Damage";
			m_AnimBlend = 0.0f;
		}
	}
	else if (isJump)
	{//ジャンプ
		if (m_AnimNameNext != "Jump")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Jump";
			m_AnimBlend = 0.0f;
		}
	}
	else if (m_Life <= 0)
	{//死んだとき
		if (m_AnimNameNext != "Dying")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Dying";
			m_Frame = 0.0f;
			m_AnimBlend = 0.0f;
		}
	}
	else if (isMove)
	{//動いてる
		if (m_AnimNameNext != "Run")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Run";
			m_AnimBlend = 0.0f;
		}
	}
	else
	{//その場で止まっている
		if (m_AnimNameNext != "Idle")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Idle";
			m_AnimBlend = 0.0f;
		}
	}


	if (!m_ControllEnable)return;

	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();

	Vector3 rotation = pCamera->GetRotation();

	Vector3 forward = pCamera->GetForward();
	Vector3 right = pCamera->GetRight();

	forward.y = 0.0f;
	right.y = 0.0f;

	forward.normalize();
	right.normalize();

	//移動
	if (Input::GetKeyPress('A'))
	{//→
		if (Input::GetKeyPress('W'))
		{
			m_Position += (-right + forward) * 0.075f;
			m_Rotation.y = rotation.y - XM_PIDIV4;
		}
		else if (Input::GetKeyPress('S'))
		{
			m_Position += (-right - forward) * 0.075f;
			m_Rotation.y = rotation.y - XM_PIDIV2 - XM_PIDIV4;
		}
		else
		{
			m_Position -= right * 0.1f;
			m_Rotation.y = rotation.y - XM_PIDIV2;
		}
		isMove = true;
	}
	else if (Input::GetKeyPress('D'))
	{//←
		if (Input::GetKeyPress('W'))
		{
			m_Position += (right + forward) * 0.075f;
			m_Rotation.y = rotation.y + XM_PIDIV4;
		}
		else if (Input::GetKeyPress('S'))
		{
			m_Position += (right - forward) * 0.075f;
			m_Rotation.y = rotation.y + XM_PIDIV2 + XM_PIDIV4;
		}
		else
		{
			m_Position += right * 0.1f;
			m_Rotation.y = rotation.y + XM_PIDIV2;
		}
		isMove = true;
	}
	else if (Input::GetKeyPress('W'))
	{//↑
		if (Input::GetKeyPress('A'))
		{
			m_Position += (forward - right) * 0.075f;
			m_Rotation.y = rotation.y - XM_PIDIV4;
		}
		else if (Input::GetKeyPress('D'))
		{
			m_Position += (forward + right) * 0.075f;
			m_Rotation.y = rotation.y + XM_PIDIV4;
		}
		else
		{
			m_Position += forward * 0.1f;
			m_Rotation.y = rotation.y;
		}
		isMove = true;
	}
	else if (Input::GetKeyPress('S'))
	{//↓
		if (Input::GetKeyPress('A'))
		{
			m_Position += (-forward - right) * 0.075f;
			m_Rotation.y = rotation.y + XM_PI - XM_PIDIV4;
		}
		else if (Input::GetKeyPress('D'))
		{
			m_Position += (-forward + right) * 0.075f;
			m_Rotation.y = rotation.y + XM_PI + XM_PIDIV4;
		}
		else
		{
			m_Position -= forward * 0.1f;
			m_Rotation.y = rotation.y + XM_PI;
		}
		isMove = true;
	}
	else
	{
		isMove = false;
	}

	//影の移動
	m_ShadowVolume->SetPosition(m_Position + Vector3{ 0.0f,-0.45f,0.0f });

	//メッシュフィールド高さ取得
	MeshField* meshField = Manager::GetScene()->GetGameObject<MeshField>();

	if (!isJump)
	{
		//重力
		m_Position += m_Gravity * 2;
		//地面より下に行かないように
		if (m_Position.y <= meshField->GetHeight(m_Position))
		{
			m_Position.y = meshField->GetHeight(m_Position);
		}
	}

	//ジャンプ処理
	if (Input::GetKeyTrigger(VK_SPACE) && !isJump)
	{
		isJump = true;
		jumpSpeed = 0.4f;
		m_JumpFrame = 0;
	}

	if (isJump)
	{
		jumpSpeed += m_Gravity.y;
		m_Position.y += jumpSpeed;

		if (m_Position.y <= meshField->GetHeight(m_Position))
		{
			m_Position.y = meshField->GetHeight(m_Position);
			isJump = false;
			jumpSpeed = 0.0f;
		}
	}

	// オーバーヒート: 冷却
	{
		float cool = m_IsOverheated ? m_OverheatCoolRate : m_CoolRate;

		m_Heat -= cool * dt;
		if (m_Heat < 0.0f) m_Heat = 0.0f;

		// オーバーヒート解除条件
		if (m_IsOverheated && m_Heat <= m_HeatMax * 0.10f)
		{
			SoundManager::PlaySE(SE::CoolDown);
			m_IsOverheated = false;
		}
	}

	// FireTimer更新
	if (m_FireTimer > 0.0f)
	{
		m_FireTimer -= dt;
		if (m_FireTimer < 0.0f) m_FireTimer = 0.0f;
	}

	//弾発射
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !m_IsOverheated && m_FireTimer <= 0.0f)
	{
		m_IsShot = true;
		Enemy* pEnemy = Manager::GetScene()->GetGameObject<Enemy>();

		Vector3 start = m_Position;
		Vector3 end = pEnemy->GetPosition();

		Vector3 toEnemy = end - start;
		float dist = toEnemy.length();

		if (dist < 8.0f)
		{
			//右後ろにふわっとしてから敵に飛ぶ弾
			BULLET* bulletR = Manager::GetScene()->AddGameObject<BULLET>(1);
			bulletR->Shot(start, GetForward(), pEnemy, 1);

			//左後ろにふわっとしてから敵に飛ぶ弾
			BULLET* bulletL = Manager::GetScene()->AddGameObject<BULLET>(1);
			bulletL->Shot(start, GetForward(), pEnemy, -1);
		}
		else
		{
			//右後ろにふわっとしてから敵に飛ぶ弾
			BULLET* bulletR = Manager::GetScene()->AddGameObject<BULLET>(1);
			bulletR->Shot(start, GetForward(), pEnemy, 1);
		}

		pCamera->Shake({ 0.0f,0.2f,0.0f });

		SoundManager::PlaySE(SE::Shot);

		// 次弾までの待ち
		m_FireTimer = m_FireInterval;

		// 発熱
		m_Heat += m_HeatPerShot;
		if (m_Heat >= m_HeatMax)
		{
			m_Heat = m_HeatMax;
			SoundManager::PlaySE(SE::OverHeat);
			m_IsOverheated = true;
		}
	}


	if (m_IsShot)
	{
		m_CoolTime++;
		if (m_CoolTime > 15.0f)
		{
			m_CoolTime = 0.0f;
			m_IsShot = false;
		}
	}
}
void Player::Draw()
{
	//影描画
	m_ShadowVolume->Draw();

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

	m_AnimModel->Draw();
}

void Player::StartKnockback(const Vector3& dir, float speed, float duration)
{
	if (m_IsKnockback) return;

	Vector3 d = dir;
	d.y = 0.0f;
	if (d.length() < 0.0001f) return;
	d.normalize();

	// =========================
	// ★ランダムに斜め（-maxDeg ～ +maxDeg）
	// =========================
	const float maxDeg = 25.0f; // 15～35くらいで調整
	float u = (rand() % 1000) / 1000.0f;       // 0～1
	float deg = (u * 2.0f - 1.0f) * maxDeg;    // -maxDeg～+maxDeg
	float rad = deg * (XM_PI / 180.0f);

	float cs = cosf(rad);
	float sn = sinf(rad);

	// Y回転（XZ平面）
	Vector3 knockDir;
	knockDir.x = d.x * cs - d.z * sn;
	knockDir.y = 0.0f;
	knockDir.z = d.x * sn + d.z * cs;
	knockDir.normalize();

	// =========================
	// ノックバック開始
	// =========================
	m_IsKnockback = true;
	m_KnockTimer = 0.0f;
	m_KnockDuration = duration;

	m_KnockVel = knockDir * speed;

	// 操作不能
	SetControllEnable(false);

	//無敵
	StartInvincible(duration);
}

void Player::StartInvincible(float duration)
{
	m_IsInvincible = true;
	m_InvincibleTimer = 0.0f;
	m_InvincibleDuration = duration;
}
