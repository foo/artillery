#include "md/mdVector.h"

#pragma once

class MDColor
{
public:
	float r,g,b, alpha;

	MDColor(const float &rr = 1, const float &gg = 1, const float &bb = 1, const float &aa = 1) : r(rr), b(bb), g(gg), alpha(aa) {}

	void Set() const
	{
		glColor4f(r,g,b,alpha);
	}

	void Random()
	{
		r = float(rand()%100)/100;
		g = float(rand()%100)/100;
		b = float(rand()%100)/100;
	}
};

class MDFigure
{
public:
	static void DrawRect(const MDVector &pos,const MDVector &size)
	{
		glPushMatrix();
			glTranslatef(pos.x,pos.y,0);
			glScalef(size.x,size.y,0);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex2f(0,0);
				glTexCoord2f(1,0); glVertex2f(1,0);
				glTexCoord2f(1,1); glVertex2f(1,1);
				glTexCoord2f(0,1); glVertex2f(0,1);
			glEnd();
		glPopMatrix();
	}

	static void DrawOval(const MDVector &pos, const float &size)
	{
		static GLUquadricObj *sph = gluNewQuadric();
		gluQuadricTexture(sph, GL_TRUE);

		glPushMatrix();
		glTranslatef(pos.x,pos.y,0);
		gluDisk(sph,0, size, 16, 10);
		glPopMatrix();
	}

	static void DrawPoly(const MDVector pos[], const int &size = 0)
	{
		glBegin(GL_POLYGON);
			for(int i = 0; i < size; ++i)
			{
				glVertex2f(pos[i].x, pos[i].y);
			}
		glEnd();
	}

	static void DrawRectAnother(const MDVector &pos,const MDVector &size)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0,0); glVertex2f(pos.x, pos.y);
			glTexCoord2f(1 * size.x/32,0); glVertex2f(pos.x + size.x, pos.y);
			glTexCoord2f(1 * size.x/32,1 * size.y/32); glVertex2f(pos.x  + size.x, pos.y + size.y);
			glTexCoord2f(0,1 * size.y/32); glVertex2f(pos.x, pos.y + size.y);
		glEnd();
	}

	static void DrawVertex(const MDVector &v)
	{
		glVertex2f(v.x, v.y);
	}
};