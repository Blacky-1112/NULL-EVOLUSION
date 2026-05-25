#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "titleCamera.h"

void TitleCamera::Init()
{
	m_Position = { 0.0f,8.0f,-3.0f };
    m_LookAt = { 0.0f,2.0f,0.0f };
    m_Time = 0.0f;
}

void TitleCamera::Uninit()
{
}

void TitleCamera::Update()
{
    float dt = Manager::GetDeltaTime();
    m_Time += dt;

    // ゆっくり揺れるカメラ演出
    m_Position.x = sinf(m_Time * 0.5f) * 0.5f;
    m_Position.y = 2.0f + sinf(m_Time * 0.8f) * 0.2f;
    m_Position.z = -8.0f + cosf(m_Time * 0.3f) * 0.3f;

    m_LookAt.x = sinf(m_Time * 0.3f) * 0.3f;

    if (m_Zoom)
    {
        m_Position.z += dt * 5.0f;
    }

    // View行列更新
    XMVECTOR eye = XMLoadFloat3((XMFLOAT3*)&m_Position);
    XMVECTOR target = XMLoadFloat3((XMFLOAT3*)&m_LookAt);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    m_View = XMMatrixLookAtLH(eye, target, up);
}

void TitleCamera::Draw()
{
    m_Projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

    Renderer::SetViewMatrix(m_View);
    Renderer::SetProjectionMatrix(m_Projection);
    Renderer::SetCameraPosition(m_Position);
}
