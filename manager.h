#pragma once

class Manager
{
private:
	static class Scene* m_Scene;
	static class Scene* m_NextScene;
	static class Scene* m_FadeScene;

	static float m_DeltaTime;
	static long long m_PrevTime;
	static double m_Frequency;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}

	static float GetDeltaTime()
	{
		return m_DeltaTime;
	}
};