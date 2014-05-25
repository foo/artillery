#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <list>
#include <typeinfo>
#include <fstream>
#include <string>

using namespace std;

#include "md/mdWindow.h"
#include "md/mdTimer.h"
#include "md/mdInput.h"
#include "md/mdFont.h"
#include "md/mdMenu.h"
#include "md/mdSound.h" 
#include "md/mdParticle.h"
#include "md/mdEntity.h"
	
MDFont arial;
MDFont menuFont;
MDMenu mainMenu;
MDMenu optionsMenu;

enum MDGameState
{
	SPLASH_SCREEN,
	END_SCREEN,
	MAIN_MENU,
	PLAY,
	AUTHORS,
	OPTIONS,
	HELP
};

MDGameState gameState;

class MDGameSettings
{
public:
	int winScore;

	MDGameSettings()
	{
		winScore = 3;
	}

	void SaveToFile(char* filename)
	{
		ofstream cfg(filename);
		cfg << "winScore " << winScore << endl;
		cfg << "explosionQuality " << MDExplosion::maxParticles << endl;
	}

	void ReadFromFile(char* filename)
	{
		ifstream cfg(filename);
		string description;
		float value = 0;

		while(cfg >> description >> value)
		{
			if(description == "winScore") winScore = value;
			if(description == "explosionQuality") MDExplosion::maxParticles = value;
		}
	}
};

MDGameSettings settings;

#include "play.h"

bool GameCycle();
void LoadLight();
void Unload();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MDWindow::getSingleton().Register(hInstance);
	MDWindow::getSingleton().Create("openGL game", 800, 600, 32, true);
	LoadLight();
	MDWindow::getSingleton().Run(GameCycle);
	Unload();
	MDWindow::getSingleton().Destroy();
}

bool GameCycle()
{
	MDTimer::Update();
	MDInput::Update();

	static float frames = 0;

	const MDVector imageSize(512,512);

	switch (gameState)
	{
	case SPLASH_SCREEN:
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,splashM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		if (++frames == 2)
		{
			LoadHeavy();
		}
		if (MDInput::KeyDown(DIK_RETURN))
		{
			gameState = MAIN_MENU;
			frames = 0;
			// don`t count loading time
			MDTimer::Update();
		}
		if (frames > 2)
		{
			menuFont.SetPos(50, 25);
			menuFont.Print("splash screen | press enter to continue");
		}
		break;

	case END_SCREEN:
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,endM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		frames += MDTimer::delta;
		if (frames > 0.4)
		{
			if (MDInput::KeyDown(DIK_RETURN))
			{
				return false;
			}
		}
		menuFont.SetPos(50, 25);
		menuFont.Print("end screen | press enter to quit");
		break;

	case AUTHORS:
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,authorsM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		frames += MDTimer::delta;
		if (frames > 0.4)
		{
			if (MDInput::KeyDown(DIK_RETURN))
			{
				gameState = MAIN_MENU;
				frames = 0;
			}
		}
		menuFont.SetPos(50, 25);
		menuFont.Print("authors | press enter to return");
		break;

	case HELP:
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,helpM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		frames += MDTimer::delta;
		if (frames > 0.4)
		{
			if (MDInput::KeyDown(DIK_RETURN))
			{
				gameState = MAIN_MENU;
				frames = 0;
			}
		}
		menuFont.SetPos(50, 25);
		menuFont.Print("help | press enter to return");
		break;

	case MAIN_MENU:
		frames += MDTimer::delta;
		if (frames > 0.4)
		{
			if (MDInput::KeyDown(DIK_RETURN))
			{
				if ((*mainMenu.activeOption)->GetDesc() == "quit")
				{
					gameState = END_SCREEN;
					frames = 0;
				}
				else if ((*mainMenu.activeOption)->GetDesc() == "play new game")
				{
					gameState = PLAY;
					Init();
				}
				else if ((*mainMenu.activeOption)->GetDesc() == "authors")
				{
					gameState = AUTHORS;
					frames = 0;
				}
				else if ((*mainMenu.activeOption)->GetDesc() == "help")
				{
					gameState = HELP;
					frames = 0;
				}
				else if ((*mainMenu.activeOption)->GetDesc() == "options")
				{
					gameState = OPTIONS;
					optionsMenu.activeOption = optionsMenu.options.begin();
					frames = 0;
				}
			}
		}
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,menuM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		mainMenu.Cycle();
		menuFont.SetPos(50, 25);
		menuFont.Print("main menu | press enter");
		break;

	case OPTIONS:
		frames += MDTimer::delta;
		if (frames > 0.4)
		{
			if (MDInput::KeyDown(DIK_RETURN))
			{
				if ((*optionsMenu.activeOption)->GetDesc() == "done")
				{
					gameState = MAIN_MENU;
					frames = 0;
				}
			}
			if (MDInput::KeyDown(DIK_NUMPADPLUS) || MDInput::KeyDown(DIK_EQUALS) || MDInput::KeyDown(DIK_RIGHT))
			{
				if ((*optionsMenu.activeOption)->GetDesc() == "win score")
				{
					(*optionsMenu.activeOption)->value = ++settings.winScore;
					frames = 0;
				}
				if ((*optionsMenu.activeOption)->GetDesc() == "explosion quality")
				{
					MDExplosion::maxParticles += 10;
					if (MDExplosion::maxParticles >= 2000)
					{
						MDExplosion::maxParticles = 2000;
					}
					(*optionsMenu.activeOption)->value = (MDExplosion::maxParticles);
					frames = 0;
				}
			}
			if (MDInput::KeyDown(DIK_NUMPADMINUS) || MDInput::KeyDown(DIK_MINUS) || MDInput::KeyDown(DIK_LEFT))
			{
				if ((*optionsMenu.activeOption)->GetDesc() == "win score")
				{
					--settings.winScore;
					if (settings.winScore <= 1)
					{
						settings.winScore = 1;
					}
					(*optionsMenu.activeOption)->value = settings.winScore;
					frames = 0;
				}
				if ((*optionsMenu.activeOption)->GetDesc() == "explosion quality")
				{
					MDExplosion::maxParticles -= 10;
					if (MDExplosion::maxParticles <= 200)
					{
						MDExplosion::maxParticles = 200;
					}
					(*optionsMenu.activeOption)->value = MDExplosion::maxParticles;
					frames = 0;
				}
			}
		}

		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,menuM.texID);
		MDFigure::DrawRect(
			MDVector(
				(800 - imageSize.x) / 2,
				(600 - imageSize.y) / 2
				),
			imageSize
			);
		glDisable(GL_TEXTURE_2D);
		optionsMenu.Cycle();
		menuFont.SetPos(50, 25);
		menuFont.Print("options | press plus or minus to change the value");
		break;

	case PLAY:
		if (MDInput::KeyDown(DIK_ESCAPE))
		{
			gameState = MAIN_MENU;
			CleanUp();
			CleanUp();
		}
		Play();
		break;
	}

	return true;
}