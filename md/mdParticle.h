#include "md/mdTexture.h"
#include "md/mdEntity.h"

#pragma once

class MDParticle
{
public:
	MDVector position;
	float size;
	MDColor color;
	float timeToDie;
	MDParticle()
	{
		timeToDie = float(rand() % 100)/100;
		color.r = 1;
		color.b = color.g = float(rand()%100)/100;
		color.b -= 0.2;
		size = float(rand()%100)/100 * 13 + 7;
	}
};

class MDExplosion : public MDEntity
{
public:
	list<MDParticle*> particles;
	static int maxParticles;
	float timeToDie;
public:
	static MDTextureTGA skin;
	static MDSound sound;

	MDExplosion()
	{
		timeToDie = 2;
		sound.Play();
	}

	void Draw()
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D,skin.texID);
		for (list<MDParticle*> :: iterator i = particles.begin(); i != particles.end(); ++i)
		{
			(*i)->color.Set();
			MDFigure::DrawRect(
				(*i)->position,
				MDVector(
					((*i)->size),
					((*i)->size)
					)
				);
		}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void Update()
	{
		timeToDie -= MDTimer::delta;
		if(timeToDie >= 0.0f)
		{
			for (int i = 0; i < 5 * 60 * MDTimer::delta; ++i)
				Emit();
		}

		for (list<MDParticle*> :: iterator i = particles.begin(); i != particles.end(); ++i)
		{
			(*i)->timeToDie -= MDTimer::delta;
			if ((*i)->timeToDie <= 0)
			{
				delete (*i);
				i = particles.erase(i);
			}
		}
		if (particles.size() <= 0) isDead = true;
	}

	void HandleCollision(MDEntity *e)
	{
	}

	void Emit()
	{
		MDParticle *p = new MDParticle;
		p->position.x = position.x + (rand() % (int)size.x);  
		p->position.y = position.y + (rand() % (int)size.y);  
		particles.push_back(p);
	}
};
MDTextureTGA MDExplosion::skin;
MDSound MDExplosion::sound;
int MDExplosion::maxParticles = 1000;