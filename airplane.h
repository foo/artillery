#pragma once

class MDAirplane : public MDEntity
{
public:
	int direction;
	static MDTextureTGA skin;
	float left;
	float right;

	MDAirplane()
	{
		direction = 1;
		size = MDVector(64,32);
		color = MDColor(1,1,1);
	}

	void Draw()
	{
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
		glBindTexture(GL_TEXTURE_2D, skin.texID);
			color.Set();
			if(direction == 1)
			{
				glBegin(GL_QUADS);
					glTexCoord2f(0,0);glVertex2f(position.x, position.y);
					glTexCoord2f(1,0);glVertex2f(position.x + size.x, position.y);
					glTexCoord2f(1,1);glVertex2f(position.x + size.x, position.y + size.y);
					glTexCoord2f(0,1);glVertex2f(position.x, position.y + size.y);
				glEnd();
			} else
			{
				glBegin(GL_QUADS);
					glTexCoord2f(1,0);glVertex2f(position.x, position.y);
					glTexCoord2f(0,0);glVertex2f(position.x + size.x, position.y);
					glTexCoord2f(0,1);glVertex2f(position.x + size.x, position.y + size.y);
					glTexCoord2f(1,1);glVertex2f(position.x, position.y + size.y);
				glEnd();
			}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void Update()
	{
		if(direction == 1)
		{
			if(position.x < right)
			{
				//position.x = right;
				position.x += MDTimer::delta * 60;
			} else 
				direction = -1;
		} else 
		{
			if(position.x > left)
			{
				//position.x = left;
				position.x -= MDTimer::delta * 60;
			} else 
				direction = 1;
		}

	}

	void HandleCollision(MDEntity *e)
	{
		if (typeid(*e) == typeid(MDBullet))
		{
			e->isDead = true;
			isDead = true;

			MDExplosion *p = new MDExplosion;
			p->maxParticles = 750;
			p->timeToDie = 1.5;
			p->position = position;
			p->size = size;
			objects.push_back(p);
		}
	}
	
	void Set(const MDVector &newPos, const float &range = 100)
	{
		position = newPos;
		left = position.x - range;
		right = position.x + size.x + range;
	}
};
MDTextureTGA MDAirplane::skin;