#pragma once
#include <iostream>
#include <memory>

#include "renderer.h"
#include "gameObject.h"

class Cursor : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	bool	m_ButtonUse = false;	//ボタン使用中

	Vector3 m_CursorLockPos;		//カーソル位置固定座標

	POINT pt;

	bool m_RightButtonUse;
	bool m_LeftButtonUse;
public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	bool GetRightMouse()
	{
		return m_RightButtonUse;
	}

	bool GetLeftMouse()
	{
		return m_LeftButtonUse;
	}
};

