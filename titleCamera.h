#pragma once
#include "main.h"
#include "gameObject.h"

class TitleCamera : public GameObject
{
private:
	XMMATRIX m_View;
	XMMATRIX m_Projection;

	Vector3 m_LookAt;
	float m_Time;

	bool m_Zoom = false;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	bool SetZoom()
	{
		return m_Zoom = true;
	}

	XMMATRIX GetCameraView() {
		return m_View;
	}
};