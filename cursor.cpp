#include "main.h"
#include "cursor.h"  
#include "texture.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "player.h"

POINT pt;

void Cursor::Init()  
{  
	//マウス初期位置
	m_CursorLockPos = { 0.0f, 0.0f, 0.0f};
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	if (GetCursorPos(&pt))
	{
		SetPosition(Vector3{ static_cast<float>(pt.x), static_cast<float>(pt.y), 0.0f});
	}

	//マウスを取得できない場合
	if (!GetCursorPos(&pt))
	{
		//エラーメッセージ
		MessageBox(nullptr, "Cursor position could not be retrieved.", "Error", MB_OK | MB_ICONERROR);
	}

	m_Scale = { 50.0f, 50.0f , 0.0f}; //カーソルのスケールを初期化

	VERTEX_3D vertex[4];

	vertex[0].Position = { GetPosition().x - m_Scale.x, GetPosition().y - m_Scale.y, 0.0f };
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { GetPosition().x + m_Scale.x, GetPosition().y - m_Scale.y, 0.0f };
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { GetPosition().x - m_Scale.x, GetPosition().y + m_Scale.y, 0.0f };
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { GetPosition().x + m_Scale.x, GetPosition().y + m_Scale.y, 0.0f };
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = { 1.0f, 1.0f };


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\loading.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(),
		image.GetImageCount(), metadata, &m_Texture);
	assert(&m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	//マウス初期位置変更後再移動
	m_CursorLockPos = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f , 0.0f};
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	//マウスカーソル非表示
	ShowCursor(FALSE);


	////カーソル用テクスチャの分割数を設定
	//cursorInstance.m_TextureSplit = { 1, 1 };
}

void Cursor::Uninit()  
{  
	////カーソル情報解放
	//if (cursorInstance.m_VertexBuffer) 
	//{
	//	cursorInstance.m_VertexBuffer->Release();
	//	cursorInstance.m_VertexBuffer = nullptr;
	//}

	m_Texture->Release();

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}  

void Cursor::Update()  
{  
	
	//クリックを取得
    
    //左クリックの処理
    if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && !m_RightButtonUse)
    {  
		m_LeftButtonUse = true;

	}
    //右クリックの処理
    else if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !m_LeftButtonUse)
    {
        m_RightButtonUse = true;

	}
	else
	{
		m_LeftButtonUse = false;
		m_RightButtonUse = false;


	}

	if ((!m_RightButtonUse || !m_LeftButtonUse))
	{
		m_CursorLockPos = m_Position;
	}


	//カーソル情報更新
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector3{ static_cast<float>(pt.x), static_cast<float>(pt.y), 0.0f});
	}

	////マウス位置固定
	//if (cursorInstance.m_MouseLock)
	//{
	//	SetCursorPos(cursorInstance.m_MouseLockPos.x, cursorInstance.m_MouseLockPos.y);
	//}

	////マウス固定切り替え
	//if (Input::GetKeyTrigger(VK_MENU))
	//{
	//	if (cursorInstance.m_MouseLock)
	//	{
	//		cursorInstance.m_MouseLock = false;
	//		
	//	}
	//	else
	//	{
	//		cursorInstance.m_MouseLock = true;
	//	}
	//}
	
	//カーソルの位置を更新
}  

void Cursor::Draw()
{  
	SetPosition(Vector3(0.0f, 0.0f, 0.0f)); //カーソルの位置をリセット

	//カーソル情報更新
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector3{ static_cast<float>(pt.x), static_cast<float>(pt.y), 0.0f});
	}

    //マウスの位置に代わりとなるものを描画

	//D3D11_MAPPED_SUBRESOURCE msr;
	//Renderer::GetDeviceContext()->Map(cursorInstance.m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, 0.0f);
	//trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);


	if (m_LeftButtonUse)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
	else if (m_RightButtonUse)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
	else
	{

	}

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Renderer::SetDepthEnable(false);

	//カーソル描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	//Renderer::SetDepthEnable(true);

}
