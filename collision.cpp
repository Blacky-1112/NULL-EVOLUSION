#include "main.h"
#include "collision.h"
#include "manager.h"
#include "scene.h"
#include "meshField.h"


bool Collision::HitChecker(GameObject* a, GameObject* b)//aが当てたいオブジェクト
{
	if (!a || !b)
		return false;
	Vector3 aPos = a->GetPosition();
	Vector3 bPos = b->GetPosition();

	Vector3 direction = bPos - aPos;

	Vector3 axisX = a->GetRight().normalized();	//X方向分離軸
	float dotX = Vector3::dot(direction, axisX);//X分離軸方向距離

	Vector3 axisY = a->GetUp().normalized();	//Y方向分離軸
	float dotY = Vector3::dot(direction, axisY);//Y分離軸方向距離

	Vector3 axisZ = a->GetForward().normalized();	//Z方向分離軸
	float dotZ = Vector3::dot(direction, axisZ);//Z分離軸方向距離


	Vector3 aScale;
	if (a->GetScale().x < 0.1)
	{//エネミーーにかかわる当たり判定用(修正予定)
		aScale = a->GetScale() * 50;
	}
	else
	{//その他は問題ない(スケール全体を取ってしまっているから1/2を掛けるべきかも)
		aScale = a->GetScale();
	}

	if (-aScale.x < dotX && dotX < aScale.x &&
		-aScale.y < dotY && dotY < aScale.y &&
		-aScale.z < dotZ && dotZ < aScale.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision::HitBulletToPlayer(GameObject* a, GameObject* b)
{
	if (!a || !b)
		return false;

	Vector3 aScale = a->GetScale();
	Vector3 half = aScale * 0.5f;

	Vector3 aCenter =
		a->GetPosition()
		+ a->GetUp().normalized() * half.y;

	Vector3 bPos = b->GetPosition();

	Vector3 direction = bPos - aCenter;

	Vector3 axisX = a->GetRight().normalized();
	Vector3 axisY = a->GetUp().normalized();
	Vector3 axisZ = a->GetForward().normalized();

	float dotX = Vector3::dot(direction, axisX);
	float dotY = Vector3::dot(direction, axisY);
	float dotZ = Vector3::dot(direction, axisZ);

	float radius = b->GetScale().x * 0.2f;

	if (-half.x - radius < dotX && dotX < half.x + radius &&
		-half.y - radius < dotY && dotY < half.y + radius &&
		-half.z - radius < dotZ && dotZ < half.z + radius)
	{
		return true;
	}

	return false;
}

static float Clamp01(float t)
{
	if (t < 0.0f) return 0.0f;
	if (t > 1.0f) return 1.0f;
	return t;
}

static Vector3 ClosestPointOnSegment(const Vector3& a, const Vector3& b, const Vector3& p)
{
	Vector3 ab = b - a;
	float ab2 = Vector3::dot(ab, ab);

	// 限りなく0に近いなら0にする
	if (ab2 <= 1e-8f)
		return a;

	float t = Vector3::dot(p - a, ab) / ab2;
	t = Clamp01(t);
	return a + ab * t;
}

// 突進（移動）用 当たり判定
bool Collision::HitChargeToPlayer(
	const Vector3& enemyPrevPos,
	const Vector3& enemyNowPos,
	float enemyRadius,
	GameObject* player,
	float playerRadius)
{
	if (!player) return false;

	// プレイヤー中心
	Vector3 p = player->GetPosition();

	// 線分(敵の移動)に対する最近点
	Vector3 q = ClosestPointOnSegment(enemyPrevPos, enemyNowPos, p);

	// 最近点qとプレイヤーpの距離が、半径合計以内ならヒット
	Vector3 d = p - q;
	float dist2 = Vector3::dot(d, d);

	float r = enemyRadius + playerRadius;
	return dist2 <= r * r;
}
