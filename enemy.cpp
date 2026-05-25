#include "main.h"
#include "manager.h"
#include "camera.h"
#include "renderer.h"
#include "easing.h"
#include "modelRenderer.h"
#include "animationModel.h"

#include "enemy.h"
#include "meshField.h"
#include "enemybullet.h"
#include "player.h"
#include "audio.h"
#include "aura.h"
#include "seManager.h"
#include "collision.h"


void Enemy::Init()
{
	m_AnimModel = new AnimationModel();

	m_AnimModel->Load("asset\\model\\Monster.fbx");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Idle.fbx", "Idle");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Run.fbx", "Run");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Dying.fbx", "Dying");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Attack.fbx", "Attack");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Charge.fbx", "Charge");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_Punch.fbx", "Punch");
	m_AnimModel->LoadAnimation("asset\\model\\Monster_JumpAttack.fbx", "JumpAttack");

	m_AnimName = "Idle";
	m_AnimNameNext = "Idle";
	m_AnimBlend = 0.0f;

	//シェーダー読み込み
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Scale = { 0.03f, 0.03f, 0.03f };

	m_Phase = PHASE_1;
}

void Enemy::Uninit()
{
	delete m_AnimModel;

	SetDestroy();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Enemy::Update()
{
	Camera* pCamera = Manager::GetScene()->GetGameObject<Camera>();
	if (m_State == ENEMY_DYING && pCamera->isFinished())
	{
		UpdateDying();
	}

	if (m_State == ENEMY_DYING)
	{
		if (m_AnimNameNext != "Dying")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Dying";
			m_AnimFrame = 0.0f;
			m_AnimBlend = 0.0f;
		}

		if (m_AnimFrame > 140.0f)
		{
			m_AnimFrame = 140.0f;
		}
	}
	m_AnimModel->Update(m_AnimName.c_str(), m_AnimFrame,
		m_AnimNameNext.c_str(), m_AnimFrame, m_AnimBlend);

	m_AnimFrame++;

	m_AnimBlend += 0.1f;
	if (m_AnimBlend > 1.0f)
	{
		m_AnimBlend = 1.0f;
	}

	//敵が存在しているなら下の処理を行う
	if (m_State != ENEMY_ALIVE) return;

	m_Frame++;

	//プレイヤーとの向き取得
	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
	Vector3 direction = pPlayer->GetPosition() - m_Position;
	direction.normalize();

	//向き補正
	bool faceToPlayer = false;
	float faceRate = 0.03f;

	// 例：登場演出中（落下～カメラ演出中）は今の追尾
	if (!m_IsSpawn || !m_SpawnFinished)
	{
		faceToPlayer = true;
		faceRate = 0.03f; // 今の気持ち良い値
	}
	else
	{
		// 戦闘中はAI/攻撃種別で制御
		if (m_AI == ENEMY_CHASE)
		{
			faceToPlayer = true;// 追いかけ中は向いてOK
			faceRate = 0.03f;
		}

		// 攻撃中は向かない
		if (m_AI == ENEMY_ATTACK)
		{
			faceToPlayer = false;
		}

		// 突進中は向きを固定
		if (m_ChargeState != CHARGE_NONE)
		{
			faceToPlayer = false;
		}
	}

	if (faceToPlayer)
		UpdateFacingToPlayer(faceRate);

	m_ShakeX = ((rand() % 100) / 100.0f - 0.5f) * 0.04f;
	m_ShakeY = ((rand() % 100) / 100.0f - 0.5f) * 0.04f;

	//フェーズ管理
	if (m_Life > 70)
	{
		m_Phase = PHASE_1;
	}
	else if (m_Life > 50)
	{
		m_Phase = PHASE_2;
	}
	else if (m_Life > 30)
	{
		m_Phase = PHASE_3;
	}
	else
	{
		m_Phase = PHASE_4;
	}


	if (pPlayer->GetControllEnable())
	{
		UpdateAI();
	}
}

void Enemy::Draw()
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
	XMMATRIX shakeMat = XMMatrixTranslation(m_ShakeX, m_ShakeY, 0.0f);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * shakeMat * trans;

	Renderer::SetWorldMatrix(world);

	//m_ModelRenderer->Draw();
	m_AnimModel->Draw();
}

void Enemy::BeginSpawn()
{
	m_SpawnFinished = false;
	m_IsSpawn = false;
	m_SpawnTime = 0.0f;
	m_FallDuration = 1.2f;

	m_StartY = 20.0f;

	Vector3 pos = GetPosition();
	pos.y = m_StartY;
	SetPosition(pos);

	//メッシュフィールド高さ取得
	MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
	m_EndY = mesh->GetHeight(pos);
}

void Enemy::UpdateSpawn(float dt)
{
	if (m_SpawnFinished)return;

	m_SpawnTime += dt;

	float t = m_SpawnTime / m_FallDuration;
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	float eased = Easing::EaseInCubic(t);

	float y = Easing::Lerp(m_StartY, m_EndY, eased);

	Vector3 pos = GetPosition();
	pos.y = y;
	SetPosition(pos);

	if (t >= 1.0f)
	{
		m_SpawnFinished = true;
		m_IsSpawn = true;
		m_State = ENEMY_ALIVE;

		Camera* cam = Manager::GetScene()->GetGameObject<Camera>();
		if (cam)
		{
			cam->StartEnemyIntro(this);
		}
	}
}

void Enemy::OnDefeated()
{
	if (m_State != ENEMY_ALIVE) return;

	m_State = ENEMY_DYING;
	m_DeadTimer = 0.0f;

	m_IsSpawn = false;

	Scene* scene = Manager::GetScene();
	if (scene)
	{
		scene->OnEnemyDefeated();
	}

	Camera* cam = Manager::GetScene()->GetGameObject<Camera>();
	if (cam)
	{
		cam->StartEnemyFinish(this);
	}
}

bool Enemy::IsDeadFinished() const
{
	return m_State == ENEMY_DEAD;
}

void Enemy::UpdateDying()
{
	if (m_AnimFrame < 140)
		return;
	float dt = Manager::GetDeltaTime();
	m_DeadTimer += dt;

	float t = m_DeadTimer / m_DeadDuration;
	t = std::min(t, 1.0f);

	float scale = Easing::EaseInCubic(1.0 - t);
	m_Scale = Vector3{ 0.03f, 0.03f, 0.03f } *scale;

	if (t >= 1.0f)
	{
		m_State = ENEMY_DEAD;
	}
}

void Enemy::UpdateAI()
{
	ApplyAIChange();

	switch (m_AI)
	{
	case ENEMY_IDLE:
		UpdateIdle();
		break;
	case ENEMY_CHASE:
		UpdateChase();
		break;
	case ENEMY_ATTACK:
		UpdateAttack();
		break;
	}
}

void Enemy::ChangeAI(EnemyAI next)
{
	if (m_AINext == next) return;
	m_AINext = next;
	m_AIChanged = true;
}

void Enemy::ApplyAIChange()
{
	if (!m_AIChanged) return;
	m_AIChanged = false;

	m_AI = m_AINext;

	// ===== Enter処理 =====
	switch (m_AI)
	{
	case ENEMY_IDLE:
		// 何もしない or 待機用タイマーリセット等
		break;

	case ENEMY_CHASE:
		// 追跡開始時の処理（必要なら）
		break;

	case ENEMY_ATTACK:
		// 攻撃開始
		// アニメーション切替
		if (m_Phase == PHASE_4 && m_AnimNameNext != "Attack")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Attack";
			m_AnimFrame = 0.0f;
			m_AnimBlend = 0.0f;
		}
		else if (m_AnimNameNext != "Punch")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Punch";
			m_AnimFrame = 0.0f;
			m_AnimBlend = 0.0f;
		}
		break;
	}
}

void Enemy::UpdateIdle()
{
	if (m_AnimNameNext != "Idle")
	{
		m_AnimName = m_AnimNameNext;
		m_AnimNameNext = "Idle";
		m_AnimBlend = 0.0f;
	}

	float dist = GetPlayerDistance();

	// 追跡開始
	if (dist < m_ChaseRange)
	{
		ChangeAI(ENEMY_CHASE);
	}
}

void Enemy::UpdateChase()
{
	if (m_AnimNameNext != "Run")
	{
		m_AnimName = m_AnimNameNext;
		m_AnimNameNext = "Run";
		m_AnimBlend = 0.0f;
	}
	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
	if (!pPlayer) return;

	Vector3 toPlayer = pPlayer->GetPosition() - m_Position;
	float dist = toPlayer.length();

	// 向き
	Vector3 dir = toPlayer;
	dir.normalize();

	// 移動
	if (dist > m_AttackRange)
	{
		m_Position += dir * m_MoveSpeed;

		MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
		m_Position.y = mesh->GetHeight(m_Position);
	}
	else
	{
		ChangeAI(ENEMY_ATTACK);
	}

	// 離れすぎたらIDLEへ
	if (dist > m_ChaseRange * 1.2f)
	{
		ChangeAI(ENEMY_IDLE);
	}
}

void Enemy::UpdateAttack()
{
	float dt = Manager::GetDeltaTime();

	// フェーズごとに攻撃間隔を決定
	switch (m_Phase)
	{
	case PHASE_1:
		m_AttackInterval = 1.0f;
		break;
	case PHASE_2:
		m_AttackInterval = 1.0f;
		break;
	case PHASE_3:
		m_AttackInterval = 1.0f;
		break;
	case PHASE_4:
		m_AttackInterval = 3.0f;
		break;
	}

	// クールダウン減算
	m_AttackCD -= dt;

	if (m_Phase == PHASE_3)
	{
		if (m_ChargeState == CHARGE_NONE)
		{
			StartCharge();
		}
		UpdateCharge(dt); // 下で作る
		return;           // 突進中は他の攻撃をしない
	}
	if (m_Phase == PHASE_1 || m_Phase == PHASE_2)
	{
		UpdateFacingToPlayer(1.0f);
	}

	// 0以下なら攻撃実行してCDを戻す
	if (m_AttackCD <= 0.0f)
	{
		switch (m_Phase)
		{
		case PHASE_1: 
			UpdatePhase1(); 
			break;
		case PHASE_2: 
			UpdatePhase2(); 
			break;
		case PHASE_3: 
			UpdatePhase3();
			break;
		case PHASE_4: 
			UpdatePhase4(); 
			UpdateFacingToPlayer(1.0f); 
			break;
		}
		m_AttackCD += m_AttackInterval;
	}

	// 行動遷移(距離で判断)
	float dist = GetPlayerDistance();
	if (dist > m_AttackExitRange && dist < m_ChaseRange) ChangeAI(ENEMY_CHASE);
	else if (dist >= m_ChaseRange) ChangeAI(ENEMY_IDLE);
}

float Enemy::GetPlayerDistance() const
{
	Player* p = Manager::GetScene()->GetGameObject<Player>();
	if (!p) return 99999.0f;

	Vector3 d = p->GetPosition() - m_Position;
	return d.length();
}

void Enemy::UpdatePhase1()
{
	{
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetType(BULLET_WAVE);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,1.0f,0.0f });
		eBullet->SetVelocity((GetForward() + GetRight()) * 0.1f);
	}
	{
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetType(BULLET_WAVE);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,1.0f,0.0f });
		eBullet->SetVelocity(GetForward() * 0.1f);
	}
	{
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetType(BULLET_WAVE);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,1.0f,0.0f });
		eBullet->SetVelocity((GetForward() - GetRight()) * 0.1f);
	}

	SoundManager::PlaySE(SE::EnemyShot);
}

void Enemy::UpdatePhase3()
{
	{
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,2.0f,0.0f });

		eBullet->SetScale(Vector3{ 4.0f,4.0f,4.0f });
		eBullet->SetVelocity(GetForward() * 0.12f);

		SoundManager::PlaySE(SE::EnemyShot);
	}
}

void Enemy::UpdatePhase2()
{
	{//正面
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,0.5f,0.0f });
		eBullet->SetType(BULLET_BEAM);
		eBullet->SetVelocity(GetForward() * 0.3f);
	}
	{//右斜め
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,0.5f,0.0f });
		eBullet->SetType(BULLET_BEAM);
		eBullet->SetVelocity((GetForward() + GetRight() / 2) * 0.3f);
	}
	{//右斜め前
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,0.5f,0.0f });
		eBullet->SetType(BULLET_BEAM);
		eBullet->SetVelocity((GetForward() + GetRight()) * 0.3f);
	}
	{//左斜め
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,0.5f,0.0f });
		eBullet->SetType(BULLET_BEAM);
		eBullet->SetVelocity((GetForward() - GetRight() / 2) * 0.3f);
	}
	{//左斜め前
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetPosition(m_Position + Vector3{ 0.0f,0.5f,0.0f });
		eBullet->SetType(BULLET_BEAM);
		eBullet->SetVelocity((GetForward() - GetRight()) * 0.3f);
	}
	SoundManager::PlaySE(SE::Beam);
}

void Enemy::UpdatePhase4()
{

	for (int i = 0; i < 10; i++)
	{
		EnemyBullet* eBullet = Manager::GetScene()->AddGameObject<EnemyBullet>(1);
		eBullet->SetScale({ 10.0f,10.0f,10.0f });
		eBullet->ShotMeteor(m_Position, 15.0f);
	}
	SoundManager::PlaySE(SE::Meteor);
}

void Enemy::StartCharge()
{
	m_ChargeState = CHARGE_WINDUP;
	m_ChargeTimer = 0.0f;
	m_ChargeHitDone = false;

	// 溜め開始時点のプレイヤー方向で固定（追尾しすぎない）
	Player* p = Manager::GetScene()->GetGameObject<Player>();
	if (p)
	{
		Vector3 dir = p->GetPosition() - m_Position;
		dir.y = 0.0f;
		if (dir.length() > 0.0001f) dir.normalize();
		m_ChargeDir = dir;
	}
	else
	{
		m_ChargeDir = GetForward();
	}

	m_ChargeStart = m_Position;

	// アニメ
	if (m_AnimNameNext != "Charge")
	{
		m_AnimName = m_AnimNameNext;
		m_AnimNameNext = "Charge";
		m_AnimFrame = 0.0f;
		m_AnimBlend = 0.0f;
	}
}

void Enemy::UpdateCharge(float dt)
{
	auto SnapToGround = [&]()
		{
			MeshField* mesh = Manager::GetScene()->GetGameObject<MeshField>();
			if (mesh) m_Position.y = mesh->GetHeight(m_Position);
		};

	m_ChargeTimer += dt;

	switch (m_ChargeState)
	{
	case CHARGE_WINDUP:
		// 溜め中は動かない（向きだけ固定方向へ）
	{
		float targetRot = atan2f(m_ChargeDir.x, m_ChargeDir.z);
		m_Rotation.y = Easing::EaseLerp(m_Rotation.y, targetRot, 0.15f);
	}

	if (m_ChargeTimer >= m_ChargeWindup)
	{
		m_ChargeState = CHARGE_DASH;
		m_ChargeTimer = 0.0f;

		// 走りアニメに切替
		if (m_AnimNameNext != "Charge")
		{
			m_AnimName = m_AnimNameNext;
			m_AnimNameNext = "Charge";
			m_AnimFrame = 0.0f;
			m_AnimBlend = 0.0f;
		}
	}
	break;

	case CHARGE_DASH:
		prev = m_Position; // 移動前保存

		// 方向固定で前進
		m_Position += m_ChargeDir * (m_ChargeSpeed * dt);
		SnapToGround();

		// 当たり判定
		if (!m_ChargeHitDone)
		{
			Player* player = Manager::GetScene()->GetGameObject<Player>();
			if (player)
			{
				float enemyR = 1.2f;
				float playerR = player->GetScale().x * 0.6f;

				if (Collision::HitChargeToPlayer(prev, m_Position, enemyR, player, playerR))
				{
					if (!player->IsInvincible())
					{
						Aura* a = Manager::GetScene()->AddGameObject<Aura>(1);
						a->Burst(m_Position + Vector3{ 0, 1.0f, 0 }, 35, 0.08f);
						player->Damage();
						player->StartKnockback(m_ChargeDir, 12.0f, 0.25f);
						SoundManager::PlaySE(SE::Damage);
					}
					m_ChargeHitDone = true;
				}
			}
		}

		// 距離/時間で終了 → 硬直へ
		moved = m_Position - m_ChargeStart;
		moved.y = 0.0f;

		if (moved.length() >= m_ChargeMaxDist || m_ChargeTimer >= m_ChargeDuration)
		{
			m_ChargeState = CHARGE_RECOVER;
			m_ChargeTimer = 0.0f;

			if (m_AnimNameNext != "Idle")
			{
				m_AnimName = m_AnimNameNext;
				m_AnimNameNext = "Idle";
				m_AnimFrame = 0.0f;
				m_AnimBlend = 0.0f;
			}
		}
	break;

	case CHARGE_RECOVER:
		// 硬直中は動かない
		if (m_ChargeTimer >= m_ChargeRecover)
		{
			// 突進終了
			m_ChargeState = CHARGE_NONE;

			// 攻撃CDを戻してから次行動へ
			m_AttackCD = 0.6f;
		}
		break;
	}
}

void Enemy::UpdateFacingToPlayer(float lerpRate)
{
	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
	if (!pPlayer) return;

	Vector3 direction = pPlayer->GetPosition() - m_Position;
	direction.y = 0.0f;                 // 地面上だけで回す（重要）
	if (direction.length() < 0.0001f) return;
	direction.normalize();

	float targetRot = atan2f(direction.x, direction.z);
	m_Rotation.y = Easing::EaseLerp(m_Rotation.y, targetRot, lerpRate);
}
