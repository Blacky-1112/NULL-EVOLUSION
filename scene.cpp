#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "fade.h"


void Scene::Init()
{

}


void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Uninit();
			delete gameObject;

		}
		m_GameObject[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Update();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		//ƒ‰ƒ€ƒ_Ž®
		m_GameObject[i].remove_if([](GameObject* object)
			{
				return object->Destroy();
			});
	}

}

void Scene::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Draw();

		}
	}
}

void Scene::FadeStart()
{

}

void Scene::SceneFadeOut()
{

}

void Scene::SceneFadeIn()
{

}

bool Scene::FadeOutFinishedCheck()
{
	return false;
}

bool Scene::FadeInFinishedCheck()
{
	return false;
}