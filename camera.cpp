#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cursor.h"
#include "player.h"


void Camera::Init()
{
	m_Position = { 0.0f,4.0f,-3.0f };
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	float dt = Manager::GetDeltaTime();

	switch (m_Mode)
	{
	case CameraMode::NORMAL:
		Update_Normal(dt);
		break;
	case CameraMode::ENEMY_INTRO:
		Update_EnemyIntro(dt);
		break;
	case CameraMode::ENEMY_FINISH:
		Update_EnemyFinish(dt);
		break;
	case CameraMode::RETURN_TO_PLAYER:
		Update_ReturnToPlayer(dt);
		break;
	default: break;
	}
}

void Camera::Draw()
{
	m_Projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(m_Projection);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_View = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),
		XMLoadFloat3((XMFLOAT3*)&m_Target),
		XMLoadFloat3(&up));

	Renderer::SetViewMatrix(m_View);

	Renderer::SetCameraPosition(m_Position);
}

void Camera::Update_Normal(float dt)
{
	Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();

	if (pPlayer)
	{
		m_Target = pPlayer->GetPosition() + Vector3(0.0f, 1.0f, 0.0f);

		//カメラシェイク
		m_Target += m_ShakeVector * sinf(m_ShakeTime);

		//揺れる速さ
		m_ShakeTime += 1.5f;
		m_ShakeVector *= 0.9f;

		//カメラシェイク持続
		if (m_EnableContinuousShake)
		{
			m_ContinuousShakeTime += 0.1f;

			m_Target.x += sinf(m_ContinuousShakeTime * 2.1f) * m_ContinuousShake.x;
			m_Target.y += cosf(m_ContinuousShakeTime * 1.7f) * m_ContinuousShake.y;
			m_Target.z += sinf(m_ContinuousShakeTime * 2.5f) * m_ContinuousShake.z;
		}

		m_Position = m_Target + Vector3(-sinf(m_Rotation.y), sinf(m_Rotation.x), -cosf(m_Rotation.y)) * 5.0f;
	}


	if (pPlayer->GetControllEnable())
	{

		POINT mouse = Input::GetMouseMove();
		const float sensitivity = 0.003f;

		//横回転
		m_Rotation.y += mouse.x * sensitivity;
		//縦回転
		m_Rotation.x += mouse.y * sensitivity;

		if (m_Rotation.x > MAX_PITCH) 
		{
			m_Rotation.x = MAX_PITCH;
		}

		if (m_Rotation.x < MIN_PITCH)
		{
			m_Rotation.x = MIN_PITCH;
		}
	}
}

void Camera::Update_EnemyIntro(float dt)
{
	if (m_EnemyTarget == nullptr)return;

	m_IntroTimer += dt;
	m_Target = m_EnemyTarget->GetPosition();
	m_Target.y += 1.0f;

	//カメラシェイク
	m_Target += m_ShakeVector * sinf(m_ShakeTime);

	//揺れる速さ
	m_ShakeTime += 1.5f;

	m_ShakeVector *= 0.9f;

	m_Position = m_Target + Vector3{ 0.0f,3.0f,-8.0f };

	if (m_IntroTimer >= m_IntroDuration)
	{
		m_Mode = CameraMode::RETURN_TO_PLAYER;

		m_ReturnTime = 0.0f;
		m_StartPos = m_Position;
		m_StartTarget = m_Target;
	}
}

void Camera::Update_EnemyFinish(float dt)
{
	if (!m_EnemyTarget) return;

	m_FinishTimer += dt;
	float t = m_FinishTimer / m_FinishDuration;
	t = std::min(t, 1.0f);

	Vector3 enemyPos = m_EnemyTarget->GetPosition();
	enemyPos.y += 1.0f;

	float radius = Lerp(6.0f, 3.0f, t);
	float angle = t * XM_PI * 0.75f;

	m_Position.x = enemyPos.x + sinf(angle) * radius;
	m_Position.z = enemyPos.z + cosf(angle) * radius;
	m_Position.y = enemyPos.y + 2.0f;

	m_Target = enemyPos;

	m_Target += m_ShakeVector * sinf(m_ShakeTime);
	m_ShakeTime += 1.5f;
	m_ShakeVector *= 0.9f;
	if (t >= 1.0f)
	{
		m_EnemyFinished = true;
	}
}

void Camera::Update_ReturnToPlayer(float dt)
{
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	if (!player) return;

	m_ReturnTime += dt;
	float t = m_ReturnTime / m_ReturnDuration;
	t = std::min(t, 1.0f);

	t = t * t * (3 - 2 * t);

	Vector3 targetPlayer = player->GetPosition() + Vector3(0.0f, 1.0f, 0.0f);
	Vector3 playerCameraPos =
		targetPlayer + Vector3(-sinf(m_Rotation.y),
			sinf(m_Rotation.x),
			-cosf(m_Rotation.y)) * 5.0f;

	m_Target = Vector3::Lerp(m_StartTarget, targetPlayer, t);
	m_Position = Vector3::Lerp(m_StartPos, playerCameraPos, t);

	if (t >= 1.0f)
	{
		Vector3 dir = (m_Position - m_Target);
		dir.normalize();

		m_Rotation.y = atan2f(-dir.x, -dir.z);
		m_Rotation.x = asinf(dir.y);

		m_EnemyTarget = nullptr;
		m_Mode = CameraMode::NORMAL;
	}
}

void Camera::Shake(Vector3 ShakeVector)
{
	m_ShakeVector = ShakeVector;
	m_ShakeTime = 0.0f;
}

void Camera::StartContinuousShake(Vector3 power)
{
	m_EnableContinuousShake = true;
	m_ContinuousShake = power;
}

void Camera::StopContinuousShake()
{
	m_EnableContinuousShake = false;
}

//視錐台カリング
bool Camera::CheckView(Vector3 Pos, float size)
{
	XMMATRIX vp;
	vp = m_View * m_Projection;

	XMMATRIX invVp;
	invVp = XMMatrixInverse(nullptr, vp);//逆行列

	XMFLOAT3 vpos[4];
	vpos[0] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vpos[1] = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vpos[2] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vpos[3] = XMFLOAT3(1.0f, 1.0f, 1.0f);

	XMVECTOR vposv[4];
	vposv[0] = XMLoadFloat3(&vpos[0]);
	vposv[1] = XMLoadFloat3(&vpos[1]);
	vposv[2] = XMLoadFloat3(&vpos[2]);
	vposv[3] = XMLoadFloat3(&vpos[3]);

	XMVECTOR wposv[4];
	wposv[0] = XMVector3TransformCoord(vposv[0], invVp);
	wposv[1] = XMVector3TransformCoord(vposv[1], invVp);
	wposv[2] = XMVector3TransformCoord(vposv[2], invVp);
	wposv[3] = XMVector3TransformCoord(vposv[3], invVp);

	XMFLOAT3 wpos[4];
	XMStoreFloat3(&wpos[0], wposv[0]);
	XMStoreFloat3(&wpos[1], wposv[1]);
	XMStoreFloat3(&wpos[2], wposv[2]);
	XMStoreFloat3(&wpos[3], wposv[3]);

	Vector3 v;
	v = Pos - m_Position;

	Vector3 wp[4];
	wp[0] = Vector3(wpos[0].x, wpos[0].y, wpos[0].z);
	wp[1] = Vector3(wpos[1].x, wpos[1].y, wpos[1].z);
	wp[2] = Vector3(wpos[2].x, wpos[2].y, wpos[2].z);
	wp[3] = Vector3(wpos[3].x, wpos[3].y, wpos[3].z);

	//左面
	{
		Vector3 v1, v2;
		v1 = wp[0] - m_Position;
		v2 = wp[2] - m_Position;
		
		//外積
		Vector3 n;
		n = Vector3::cross(v1, v2);
		n.normalize();//正規化

		//内積
		float d;
		d = Vector3::dot(n, v);//面からの距離

		if (d < -size)
			return false;
	}

	//右面
	{
		Vector3 v1, v2;
		v1 = wp[3] - m_Position;
		v2 = wp[1] - m_Position;

		//外積
		Vector3 n;
		n = Vector3::cross(v1, v2);
		n.normalize();//正規化

		//内積
		float d;
		d = Vector3::dot(n, v);

		if (d < -size)
			return false;
	}

	return true;
}

void Camera::StartEnemyIntro(GameObject* enemy)
{
	m_Mode = CameraMode::ENEMY_INTRO;
	m_EnemyTarget = enemy;

	m_IntroTimer = 0.0f;

	Shake(Vector3{ 0.2f,0.2f,0.2f });
}

void Camera::StartEnemyFinish(GameObject* enemy)
{
	m_Mode = CameraMode::ENEMY_FINISH;
	m_EnemyTarget = enemy;

	m_FinishTimer = 0.0f;

	m_FinishStartPos = m_Position;
	m_FinishStartTarget = m_Target;

	Shake(Vector3{ 0.3f,0.3f,0.3f });
}
