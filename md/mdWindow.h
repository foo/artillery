#pragma once

#include <gl/gl.h>	
#include <gl/glu.h>

class MDWindow
{
public:
	HDC			hDC;
	HWND		hWnd;
	HINSTANCE	hInstance;
	HPALETTE	hPalette;
	HGLRC		hRC;
	bool		done;
	bool		fullScreen;
private:
	void SetupPixelFormat()
	{
		int nPixelFormat;					// indeks formatu pikseli

		static PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
			1,								// domy�lna wersja
			PFD_DRAW_TO_WINDOW |			// grafika w oknie
			PFD_SUPPORT_OPENGL |			// grafika OpenGL 
			PFD_DOUBLEBUFFER,				// podw�jne buforowanie
			PFD_TYPE_RGBA,					// tryb kolor�w RGBA 
			32,								// 32-bitowy opis kolor�w
			0, 0, 0, 0, 0, 0,				// nie specyfikuje bit�w kolor�w
			0,								// bez buforu alfa
			0,								// nie specyfikuje bitu przesuni�cia
			0,								// bez bufora akumulacji
			0, 0, 0, 0,						// ignoruje bity akumulacji
			16,								// 16-bit bufor z
			0,								// bez bufora powielania
			0,								// bez bufor�w pomocniczych
			PFD_MAIN_PLANE,					// g��wna p�aszczyzna rysowania
			0,								// zarezerwowane
			0, 0, 0 };						// ingoruje maski warstw

		nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 

		SetPixelFormat(hDC, nPixelFormat, &pfd);		// okre�la format pikseli dla danego kontekstu urz�dzenia
	}
	friend LRESULT CALLBACK WndProcOGL(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	MDWindow()
	{
		
	}
public:
	void Register(HINSTANCE hInst)
	{
		hInstance = hInst;
		WNDCLASS windowClass;		// the window class

		windowClass.style		 = 0;
		windowClass.lpfnWndProc	 = WndProcOGL;
		windowClass.cbClsExtra	 = 0;
		windowClass.cbWndExtra	 = 0;
		windowClass.hInstance	 = hInstance;
		windowClass.hIcon		 = 0;
		windowClass.hCursor		 = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName	 = NULL;
		windowClass.lpszClassName = "Engine";

		if (!RegisterClass(&windowClass))
		{
			MessageBox(0,"Nie mo�na zarejestrowa� klasy",0,0);
		}
	}
	void Destroy()
	{
		if (fullScreen)
		{
			ChangeDisplaySettings(NULL,0);					// przywraca pulpit
			ShowCursor(TRUE);						// i wska�nik myszy
		}
	}
	void Create(const char *szName,const int &width,const int &height,const int &bits = 32, const bool fs = true)
	{
		DWORD	   dwExStyle;
		DWORD	   dwStyle;
		RECT	   windowRect;
		windowRect.left=(long)0;
		windowRect.right=(long)width;					
		windowRect.top=(long)0;						
		windowRect.bottom=(long)height;	
		done = false;

		fullScreen = fs;

		if (fullScreen)								// tryb pe�noekranowy?
		{
			DEVMODE dmScreenSettings;					// tryb urz�dzenia
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);	
			dmScreenSettings.dmPelsWidth = width;			// szeroko�� ekranu
			dmScreenSettings.dmPelsHeight = height;			// wysoko�� ekranu
			dmScreenSettings.dmBitsPerPel = bits;				// bit�w na piksel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			// 
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				// prze��czenie trybu nie powiod�o si�, z powrotem tryb okienkowy
				MessageBox(NULL, "Prze��czenie trybu wyswietlania nie powiod�o si�", NULL, MB_OK);
				fullScreen=FALSE;	
			}
		}

		if (fullScreen)								// tryb pe�noekranowy?
		{
			dwExStyle=WS_EX_APPWINDOW;					// rozszerzony styl okna
			dwStyle=WS_POPUP;						// styl okna
			ShowCursor(FALSE);						// ukrywa kursor myszy
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// definicja klasy okna
			dwStyle=WS_OVERLAPPEDWINDOW;					// styl okna
		}

		AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// koryguje rozmiar okna

		// tworzy okno
		hWnd = CreateWindowEx(NULL,									// rozszerzony styl okna
							"Engine",			// nazwa klasy
							szName,
							dwStyle | WS_CLIPCHILDREN |
							WS_CLIPSIBLINGS,
							0, 0,								// wsp�rz�dne x,y
							windowRect.right - windowRect.left,
							windowRect.bottom - windowRect.top, // szeroko��, wysoko��
							NULL,									// uchwyt okna nadrz�dnego
							NULL,									// uchwyt menu
							hInstance,							// instancja aplikacji
							NULL);								// bez dodatkowych parametr�w

		if (!hWnd)
		{
			MessageBox(NULL, "Tworzenie okna si� nie powiod�o", NULL, MB_OK);
			return;
		}

		ShowWindow(hWnd, SW_SHOW);			// wy�wietla okno
		UpdateWindow(hWnd);
	}
	void Run(bool (*cycle)())
	{
		MSG msg;
		while (!done)
		{
			PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE);

			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);										
				glLoadIdentity();
	
				if ((*cycle)() == false)
				{
					done = true;
				}

				glFlush();
				SwapBuffers(hDC);

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	static MDWindow& getSingleton()
	{
		static MDWindow singleton;
		return singleton;
	}
	void quit()
	{
		done = true;
	}
};
LRESULT APIENTRY WndProcOGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int width = 800;
	static int height = 600;
	switch(uMsg)
	{
		case WM_CREATE:
			MDWindow::getSingleton().hDC = GetDC(hWnd);	
			MDWindow::getSingleton().SetupPixelFormat();	
			MDWindow::getSingleton().hRC = wglCreateContext(MDWindow::getSingleton().hDC);
			wglMakeCurrent(MDWindow::getSingleton().hDC, MDWindow::getSingleton().hRC);

			return 0;
			break;

		case WM_CLOSE:
			wglMakeCurrent(MDWindow::getSingleton().hDC, NULL);
			wglDeleteContext(MDWindow::getSingleton().hRC);
			PostQuitMessage(0);

			return 0;
			break;

		case WM_SIZE:
			height = HIWORD(lParam);
			width = LOWORD(lParam);
			if (height==0)
			{
				height=1;					
			}

			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, width, 0, height);
			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();

			return 0;
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}