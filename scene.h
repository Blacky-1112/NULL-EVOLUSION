#pragma once

#include <list>
#include <vector>
#include "gameObject.h"
#include "enemybullet.h"

class Scene
{
private:
	std::list<GameObject*>m_GameObject[5];

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	virtual void FadeStart();
	virtual void SceneFadeOut();
	virtual void SceneFadeIn();
	virtual bool FadeOutFinishedCheck();
	virtual bool FadeInFinishedCheck();

	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template<typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto gameObject : m_GameObject[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
					return find;
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*>GetGameObjects()
	{
		std::vector<T*> finds;
		for (int i = 0; i < 3; i++)
		{
			for (auto gameObject : m_GameObject[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
					finds.push_back(find);
			}
		}
		return finds;
	}

	void OnEnemyDefeated()
	{
		auto bullets = GetGameObjects<EnemyBullet>();
		for (EnemyBullet* b : bullets)
		{
			b->SetDestroy();
		}
	}

};