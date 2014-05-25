#pragma once

class MDEntity
{
public:
	MDVector position;
	MDVector size;
	MDColor color;
	bool isDead;

	MDEntity()
	{
		isDead = false;
	}

	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void HandleCollision(MDEntity *e) = 0;

	bool isCollisionWith(MDEntity *c)
	{
		if (
			position.x + size.x > c->position.x &&
			c->position.x + c->size.x > position.x &&
			
			position.y + size.y > c->position.y &&
			c->position.y + c->size.y > position.y
			)
		{
			return true;
		}
		return false;
	}
};