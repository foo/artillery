#include "md/mdFigure.h"
#include "md/mdVector.h"
#include "md/mdSound.h"
#include "md/mdTexture.h"

list<MDEntity*> objects;

class MDBlock;
class MDPlayer;

#include "bullet.h"
#include "block.h"

MDPlayer *player2;
MDPlayer *player1;

MDSound onEnterSound;
MDSound onExitSound;
MDSound onTargetSound;

#include "player.h"
#include "airplane.h"


MDTextureTGA	splashM;
MDTextureTGA	endM;
MDTextureTGA	authorsM;
MDTextureTGA	menuM;
MDTextureTGA	backgroundM;
MDTextureTGA	helpM;

void LoadLight()
{
	settings.ReadFromFile("data/cfg.txt");
	MDTimer::Init();

	MDInput::Initialize();
	
	//MDMouse::Initialize(MDInput::pDI,MDWindow::getSingleton().hWnd);

	arial.Build("Arial",16);
	menuFont.Build("Arial",12);
	menuFont.Color = MDColor(0,1,0);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	splashM.LoadTGA("data/images/splash.tga");
	gameState = SPLASH_SCREEN;
}
void Unload()
{
	MDInput::CleanUp();
	settings.SaveToFile("data/cfg.txt");
}
void LoadHeavy()
{
	mainMenu.Add(new MDMenuPosition("play new game"));
	mainMenu.Add(new MDMenuPosition("authors"));
	mainMenu.Add(new MDMenuPosition("help"));
	mainMenu.Add(new MDMenuPosition("options"));
	mainMenu.Add(new MDMenuPosition("quit"));
	mainMenu.Initialize();

	MDMenuPosition *win = new MDMenuPosition("win score",true);
	win->value = settings.winScore;
	optionsMenu.Add(win);
	MDMenuPosition *explosion = new MDMenuPosition("explosion quality",true);
	explosion->value = MDExplosion::maxParticles;
	optionsMenu.Add(explosion);
	optionsMenu.Add(new MDMenuPosition("done"));
	optionsMenu.Initialize();

	MDSound::Initialize(MDWindow::getSingleton().hWnd);

	onEnterSound.Load("data/sounds/enter.wav");
	onExitSound.Load("data/sounds/exit.wav");
	onTargetSound.Load("data/sounds/target.wav");

	endM.LoadTGA("data/images/end.tga");
	authorsM.LoadTGA("data/images/authors.tga");
	menuM.LoadTGA("data/images/menu.tga");
	backgroundM.LoadTGA("data/images/background.tga");
	helpM.LoadTGA("data/images/help.tga");

	MDExplosion::sound.Load("data/sounds/explode.wav");

	MDBlock::skin.LoadTGA("data/images/brick.tga");
	MDExplosion::skin.LoadTGA("data/images/particle.tga");
	MDAirplane::skin.LoadTGA("data/images/plane.tga");
}

void Init()
{
	onEnterSound.Play();

	player1 = new MDPlayer;
	player1->SetPos(MDVector(111.5,100), 111.5);
	player1->size = MDVector(25,25);
	player1->scorePositionX = 10;
	player1->color = MDColor();
	player1->key.up = DIK_W;
	player1->key.down = DIK_S;
	player1->key.left = DIK_A;
	player1->key.right = DIK_D;
	player1->key.fire = DIK_LCONTROL;
	player1->texture.LoadTGA("data/images/player1.tga");
	objects.push_back(player1);

	player2 = new MDPlayer;
	player2->SetPos(MDVector(611.5,100),111.5);
	player2->size = MDVector(25,25);
	player2->scorePositionX = 650;
	player2->color = MDColor();
	player2->key.up = DIK_DOWN;
	player2->key.down = DIK_UP;
	player2->key.left = DIK_LEFT;
	player2->key.right = DIK_RIGHT;
	player2->key.fire = DIK_RCONTROL;
	player2->texture.LoadTGA("data/images/player2.tga");
	objects.push_back(player2);

	MDTrapes *t = new MDTrapes;
	t->position = MDVector(300,100);
	objects.push_back(t);

	MDBlock *block = new MDBlock;
	block->position = MDVector(0, 80 - 32);
	block->size = MDVector(800, 20 + 32);
	objects.push_back(block);

	{
		MDVector pos(0, 225);
		MDVector plus(0, 30);
		const int planesNum = 20;

		for (int i = 0; i < planesNum; ++i)
		{
			pos.y += plus.y;

			int asdf = rand() % 10 + 1;

			MDAirplane *a = new MDAirplane;

			if(asdf > 5) a->direction = 1;
			else		 a->direction = -1;

			a->Set( MDVector( rand() % 800 + 50, pos.y ) , rand() % 150 + 80 );
			objects.push_back(a);
		}
	}
}

void CleanUp()
{
	onExitSound.Play();
	list<MDEntity*>::iterator ii;
	for (ii = objects.begin(); ii != objects.end(); ++ii)
	{
		delete (*ii);
		ii = objects.erase(ii);
	}
}

void Play()
{
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,backgroundM.texID);
	MDFigure::DrawRect(MDVector(0, 0), MDVector(800, 600));
	glDisable(GL_TEXTURE_2D);

	list<MDEntity*>::iterator ii;
	for (ii = objects.begin(); ii != objects.end(); ++ii)
	{
/****************************************************/
// update all
		(*ii)->Update();
/****************************************************/
// check collisions
		list<MDEntity*>::iterator j = ii;
		++j;
		for (; j != objects.end(); ++j)
		{
			if ((*ii)->isCollisionWith((*j)))
			{
				(*ii)->HandleCollision((*j));
				(*j)->HandleCollision((*ii));
			}
		}
/***************************************************/
// if object is dead { delete him }
		if ((*ii)->isDead)
		{
			delete (*ii);
			ii = objects.erase(ii);
			continue;
		}
/****************************************************/
// draw all
		(*ii)->Draw();
/****************************************************/
	}

	if (player1->score >= settings.winScore)
	{
		MDMessage("First player (left) has just won!");
		//mb("Gracz pierwszy (po lewej) w³aœnie wygra³!");
		CleanUp();
		CleanUp();
		Init();
	}
	if (player2->score >= settings.winScore)
	{
		MDMessage("Second player (right) has just won!");
		//mb("Gracz drugi (po prawej) w³aœnie wygra³!");
		CleanUp();
		CleanUp();
		Init();
	}
}