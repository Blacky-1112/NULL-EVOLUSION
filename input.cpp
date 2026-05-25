
#include "main.h"
#include "input.h"


BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];


void Input::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

	/*POINT center = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
	ClientToScreen(GetActiveWindow(), &center);
	SetCursorPos(center.x, center.y);*/

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

POINT Input::GetMouseMove()
{
	POINT now;
	GetCursorPos(&now);

	POINT center = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
	ClientToScreen(GetActiveWindow(), &center);

	POINT delta;
	delta.x = now.x - center.x;
	delta.y = now.y - center.y;

	SetCursorPos(center.x, center.y);
	return delta;
}
