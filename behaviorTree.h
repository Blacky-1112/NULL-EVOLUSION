#pragma once
#include <vector>

enum BEHAVIOR_RESULT
{
	BEHAVIOR_RESULT_SUCCESS,
	BEHAVIOR_RESULT_FAIL,
	BEHAVIOR_RESULT_CONTINUE
};

class BehaviorNode
{
protected:
	std::vector<BehaviorNode*> m_Child;
	int m_Index = 0;
public:
	virtual BEHAVIOR_RESULT Update() = 0;
};

class BehaviorSequence : public BehaviorNode
{
public:
	BEHAVIOR_RESULT Update()
	{
		BEHAVIOR_RESULT result = m_Child[m_Index]->Update();

		switch (result)
		{
			case BEHAVIOR_RESULT_SUCCESS:
				m_Index++;
				if (m_Child.size() <= m_Index)
				{
					m_Index = 0;
					return BEHAVIOR_RESULT_SUCCESS;
				}
				return BEHAVIOR_RESULT_CONTINUE;

			case BEHAVIOR_RESULT_FAIL:
				m_Index = 0;
				return BEHAVIOR_RESULT_FAIL;

			case BEHAVIOR_RESULT_CONTINUE:
				return BEHAVIOR_RESULT_CONTINUE;

			default: break;
		}
	}
};

class BehaviorSelecter : public BehaviorNode
{
public:
	BEHAVIOR_RESULT Update()
	{
		BEHAVIOR_RESULT result = m_Child[m_Index]->Update();

		switch (result)
		{
		case BEHAVIOR_RESULT_SUCCESS:
			m_Index = 0;
			return BEHAVIOR_RESULT_SUCCESS;

		case BEHAVIOR_RESULT_FAIL:
			m_Index++;
			if (m_Child.size() <= m_Index)
			{
				m_Index = 0;
				return BEHAVIOR_RESULT_FAIL;
			}
			return BEHAVIOR_RESULT_CONTINUE;

		case BEHAVIOR_RESULT_CONTINUE:
			return BEHAVIOR_RESULT_CONTINUE;

		default: break;
		}
	}
};

class BehaviorWait : public BehaviorNode
{
public:
	BEHAVIOR_RESULT Update()
	{
		Player* pPlayer = Manager::GetScene()->GetGameObject<Player>();
		if (player != nullptr)
		{
			Vector3 direction = pPlayer->GetPosition() - BaseEnemy->m_Position;

			if (direction.length() < 10.0f)
			{
				return BEHAVIOR_RESULT_SUCCESS;
			}

			return BEHAVIOR_RESULT_CONTINUE;
		}
	}
};

void Enemy::Init()
{
	m_Root = new BehaviorSequence();

	m_Root->m_Child[0] = new BehaviorWait();
	m_Root->m_Child[1] = new BehaviorMove();

	m_Root->m_Child[2] = new BehaviorSelecter();
	m_Root->m_Child[2]->m_Child[0] = new BehaviorShot();
	m_Root->m_Child[2]->m_Child[1] = new BehaviorPunch();
}

void Enemy::Update()
{
	m_Root->Update();
}