#include "md/mdFigure.h"
#include "md/mdTexture.h"
#include <stdio.h>

#pragma once

class MDFont
{
protected:
	unsigned int listBase;
	float x,y;
public:
	MDColor Color;
public:
	MDFont() {}
	MDFont(char *name, int size)
	{
		Build(name, size);
	}
	virtual ~MDFont() { glDeleteLists(listBase, 96); }

	void SetPos(const float &xx, const float &yy)
	{
		x = xx;
		y = yy;
	}
	void SetPos(const MDVector& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void Build(char *fontName, int fontSize)
	{
		HFONT hFont;
		listBase = glGenLists(96);
		if (stricmp(fontName, "symbol") == 0)
		{
			hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, SYMBOL_CHARSET, 
								OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
								FF_DONTCARE | DEFAULT_PITCH, fontName);
		}
		else
		{
			hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
								OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
								FF_DONTCARE | DEFAULT_PITCH, fontName);
		}

		SelectObject(MDWindow::getSingleton().hDC, hFont);
		wglUseFontBitmaps(MDWindow::getSingleton().hDC, 32, 96, listBase);
	}
	void Print(const char *str, ...)
	{
		char text[256];
		va_list args;
		va_start(args, str);
			vsprintf(text, str, args);
		va_end(args);

		glPushMatrix();
		glLoadIdentity();
		Color.Set();
		glTranslatef(0.0f, 0.0f, -1.0f);
		glRasterPos2f(x,y);
		glPushAttrib(GL_LIST_BIT);
			glListBase(listBase - 32);
			glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
		glPopMatrix();
	}
};

void mb(const char *str = "", ...)
{
	char text[256];
	va_list args;

	va_start(args, str);
		vsprintf(text, str, args);
	va_end(args);

	MessageBox(MDWindow::getSingleton().hWnd,text,"info",MB_OK | MB_ICONINFORMATION);
}
char* mergeIt(const char *str = "", ...)
{
	char text[256];
	va_list args;

	va_start(args, str);
		vsprintf(text, str, args);
	va_end(args);

	return text;
}

void MDMessage(char *what)
{
	const MDVector size(280, 75);
	const MDVector pos = MDVector(400 - size.x / 2, 300 - size.y / 2);
	const MDColor color(1,1,1);
	const float msgFontSize = 20;
	static MDFont msgFont("Arial", msgFontSize);
	static MDTextureTGA tt = MDTextureTGA("data/images/message.tga");

	color.Set();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tt.texID);
	MDFigure::DrawRect(pos, size);
	glDisable(GL_TEXTURE_2D);
	
	msgFont.Color = MDColor(0,0,0);
	msgFont.SetPos(pos.x, pos.y + size.y/2);
	msgFont.Print(what);
	glFlush();
	SwapBuffers(MDWindow::getSingleton().hDC);

	while (!MDInput::KeyDown(DIK_RETURN))
	{
		MDInput::Update();
	}
	MDTimer::Update();
}