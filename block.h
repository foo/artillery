#pragma once

class MDBlock : public MDEntity
{
public:
	static MDTextureTGA skin;

	MDBlock()
	{
		color = MDColor(1, 1, 1);
	}


	void Draw()
	{	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, skin.texID);       
		
		color.Set();
		MDFigure::DrawRectAnother(position,size);
		glDisable(GL_TEXTURE_2D);
	}

	void Update()
	{}

	void HandleCollision(MDEntity* e)
	{
		
	}
};

class MDTrapes : public MDEntity
{
public:
	static int num;
	float *heights;
	MDTrapes()
	{
		color = MDColor(0.5, 0.25, 0);
		size = MDVector(200,200);

		heights = new float[num];
		const float maxH = 200;

		for (int i = 0; i < num; ++i)
		{
			int a;
			a = int(maxH * sinf(0.0174 * i * 180/num));
			heights[i] = a;
		}
	}


	void Draw()
	{
		glBegin(GL_POLYGON);
		color.Set();
		MDFigure::DrawVertex(position);
		for(int x = 0; x < num; ++x)
		{	
			MDFigure::DrawVertex(position + MDVector(x * size.x/num, heights[x]));
		}
		MDFigure::DrawVertex(MDVector(position.x + size.x, position.y));
		glEnd();
	}

	void Update()
	{}

	void HandleCollision(MDEntity* e)
	{
		if(typeid(*e) == typeid(MDBullet))
		{
			float absPosX = e->position.x - position.x;
			int q = absPosX*num/size.x;
			if (e->position.y < heights[q] + position.y)
			{
				e->isDead = true;				
			}
		}
	}
};
MDTextureTGA MDBlock::skin;
int MDTrapes::num = 18;