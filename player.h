#include "md/mdEntity.h"

#pragma once

class MDKey
{
public:
	int left;
	int right;
	int up;
	int down;
	int fire;
};

class MDPlayer : public MDEntity
{
public:
	MDTextureTGA	texture;
	MDKey			key;
	float			speed;
	float			angle;
	float			rotateSpeed;
	float			rangeLeft;
	float			rangeRight;
	float			lufaLength;
	float			force;
	int				score;
	float			scorePositionX;
public:
	MDPlayer()
	{
		position = MDVector(100,100);
		size = MDVector(50,50);
		color = MDColor(1,1,1);
		speed = 1.5f;
		rotateSpeed = 3;
		angle = 90;
		lufaLength = 35;
		force = 0;
		scorePositionX = 0;
		score = 0;
	}

	void Draw()
	{
		color.Set();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.texID);
		MDFigure::DrawRect(position,size);
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_LINES);
			glVertex2f(
				position.x + size.x/2,
				position.y + size.y/2
				);
			glVertex2f(
				position.x + size.x/2 + lufaLength * cosf(angle * 0.0174f),
				position.y + size.y/2 + lufaLength * sinf(angle * 0.0174f)
				);
		glEnd();

		menuFont.SetPos(scorePositionX, 10);
		menuFont.Print("Score: %i", score);
	}

	void Update()
	{
		if (MDInput::KeyDown(key.up)) angle += 60 * MDTimer::delta;
		if (MDInput::KeyDown(key.down)) angle -= 60 * MDTimer::delta;
		if (MDInput::KeyDown(key.left))
		{
			if(position.x > rangeLeft)
				position.x -= speed * 60 * MDTimer::delta;
		}
		if (MDInput::KeyDown(key.right)) 
		{
			if(position.x < rangeRight)
				position.x += speed * 60 * MDTimer::delta;
		}

		const float maxForce = 20;
		if(MDInput::KeyDown(key.fire))
		{
			force += MDTimer::delta * 60 * 0.25;
			if (force > maxForce) force = maxForce;

			const float rate = 4;
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			glVertex2f(
				position.x + size.x/2 + (cosf(angle * 0.0174) * (lufaLength + force*rate)),
				position.y + size.y/2 + (sinf(angle * 0.0174) * (lufaLength + force*rate))
				);
			glVertex2f(
				position.x + size.x/2 + (cosf(angle * 0.0174) * (lufaLength + 1)),
				position.y + size.y/2 + (sinf(angle * 0.0174) * (lufaLength + 1))
				);
			glEnd();
		}
		else if (force != 0)
		{
			MDBullet *b = new MDBullet;
			b->position.x = position.x + size.x/2 + (cosf(angle * 0.0174) * 35) - b->size.x/2;
			b->position.y = position.y + size.y/2 + (sinf(angle * 0.0174) * 35) - b->size.y/2;
			b->angle = angle;
			b->force = force;
			b->upEnergy = force * sinf(angle * 0.0174);
			objects.push_back(b);
			force = 0;
		}
	}

	void HandleCollision(MDEntity *e)
	{
		if(typeid(*e) == typeid(MDBullet))
		{
			e->isDead = true;
			GetSecondPlayer()->score++;
			onTargetSound.Play();
		}
	}

	void SetPos(const MDVector &newPosition, const float &range)
	{
		position = newPosition;
		rangeLeft =  position.x - range;
		rangeRight = position.x + range + size.x;
	}

	MDPlayer* GetSecondPlayer() const
	{
		if (this == player1)
			return player2;
		return player1;
	}

};