#pragma once

class MDBullet : public MDEntity
{
public:
	float force;
	float angle;
	float upEnergy;
public:
	MDBullet()
	{
		size = MDVector(5,5);
		color = MDColor(1,1,0);
	}

	void Draw()
	{
		color.Set();
		MDFigure::DrawRect(position,size);
	}
	void Update()
	{
		const float gForce = 0.4;
		position.x += force * cosf(angle * 0.0174) * 60 * MDTimer::delta;
		position.y += upEnergy * 60 * MDTimer::delta;
		upEnergy -= 60 * MDTimer::delta * gForce;
	}
	void HandleCollision(MDEntity *e)
	{
		if(typeid(*e) == typeid(MDBlock))
		{
			isDead = true;
		}
	}
};