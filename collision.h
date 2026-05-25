#pragma once
#include "GameObject.h"


namespace Collision
{
	bool HitChecker(GameObject* a, GameObject* b);

    bool HitBulletToPlayer(GameObject* a, GameObject* b);

	bool HitChargeToPlayer(
		const Vector3& enemyPrevPos,
		const Vector3& enemyNowPos,
		float enemyRadius,
		GameObject* player,
		float playerRadius);
}