#pragma once

#include <dinput.h>

class MDInput
{
public:
	static LPDIRECTINPUT8			m_pDI;
	static LPDIRECTINPUTDEVICE8		m_pDIDev;
	static char						m_keys[256];
	static void Initialize()
	{
		if (FAILED(DirectInput8Create(MDWindow::getSingleton().hInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void **)&m_pDI,
								NULL)))
		{
			MessageBox(0,"direct input :: failed :: direct input 8 create",0,0);
			return;
		}
		if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL)))
		{
			MessageBox(0,"direct input :: failed :: create device",0,0);
			return;
		}

		if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIKeyboard)))
		{
			MessageBox(0,"direct input :: failed :: set data format",0,0);
			return;
		}

		if (FAILED(m_pDIDev->SetCooperativeLevel(MDWindow::getSingleton().hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			MessageBox(0,"direct input :: failed :: set cooperative level",0,0);
			return;
		}

		if (FAILED(m_pDIDev->Acquire()))
		{
			MessageBox(0,"direct input :: failed :: acquire",0,0);
			return;
		}
		ZeroMemory(m_keys, 256 * sizeof(char));
	}
	static void Update()
	{
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
		{
			m_pDIDev->Acquire();
			m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys);
		}
	}
	static bool KeyDown(int key)
	{
		return (m_keys[key] & 0x80);	
	}
	static void CleanUp()
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
};
LPDIRECTINPUT8			MDInput::m_pDI;
LPDIRECTINPUTDEVICE8	MDInput::m_pDIDev;
char					MDInput::m_keys[256];
