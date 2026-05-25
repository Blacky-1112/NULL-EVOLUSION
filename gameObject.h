#pragma once
#include "input.h"
#include "vector3.h"

class GameObject
{
protected:
	bool m_Destroy = false;


	Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Rotation{ 0.0f, 0.0f, 0.0f };
	Vector3 m_Scale{ 1.0f, 1.0f, 1.0f };

	Input* input;
public:
	virtual void Init() {}	//âºëzä÷êî
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void SetDestroy()
	{
		m_Destroy = true;
	}

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPosition(Vector3 pos)
	{
		m_Position = pos;
	}
	Vector3 GetPosition()
	{
		return m_Position;
	}

	void SetRotation(Vector3 rot)
	{
		m_Rotation = rot;
	}
	Vector3 GetRotation()
	{
		return m_Rotation;
	}

	void SetScale(Vector3 scale)
	{
		m_Scale = scale;
	}
	Vector3 GetScale()
	{
		return m_Scale;
	}

	Vector3 GetRight()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);

		return right;

	}

	Vector3 GetUp()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 up;
		XMStoreFloat3((XMFLOAT3*)&up, matrix.r[1]);

		return up;

	}

	Vector3 GetForward()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);

		return forward;

	}

	float GetDistance(Vector3 Pos)
	{
		return (m_Position - Pos).length();
	}

	float GetZ(Vector3 pos, Vector3 For)
	{
		Vector3 direction = m_Position - pos;
		return Vector3::dot(direction, For);
	}
};