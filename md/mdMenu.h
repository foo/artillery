#include <list>

#pragma once

class MDMenuPosition
{
private:
	char* desc;
	bool printValue;
public:
	float value;
public:
	MDMenuPosition(char* desc_p, bool printV = false) : desc(desc_p), value(0), printValue(printV) {}
	char* GetDesc()
	{
		return desc;
	}
	const char* GetReal()
	{
		if (!printValue)
			return desc;

		char *chVal = mergeIt("%i",(int)value);
		char chA[255];
		strcpy(chA,chVal);
		char temp[255];
		strcpy(temp,desc);
		strcat(temp," ");
		strcat(temp,chA);
		return temp;
	}
};

class MDMenu
{
public:
	list<MDMenuPosition*> options;
	MDFont font;
	static const int fontSize = 32;
public:
	list<MDMenuPosition*>::iterator activeOption;
	MDVector position;
public:
	MDMenu() {}
	void Initialize()
	{
		font.Build("courier new",fontSize);
		position = MDVector(100,500);
		activeOption = options.begin();
	}
	void Add(MDMenuPosition *opt)
	{
		options.push_back(opt);
	}
	void Cycle()
	{
		if (MDInput::KeyDown(DIK_ESCAPE))
		{
			activeOption = options.end();
			activeOption--;
		}

		{
			const float time = 0.16;
			static float DS = time;
			DS += MDTimer::delta;
			if (DS > time)
			{
				if (MDInput::KeyDown(DIK_DOWN))
				{
					activeOption++;
					if (activeOption == options.end())
					{
						activeOption = options.begin();
					}
					DS = 0;
				}
				else if (MDInput::KeyDown(DIK_UP))
				{
					if (activeOption == options.begin())
					{
						activeOption = options.end();
					}
					activeOption--;
					DS = 0;
				}
			}
		}

		int yPos = 0;
		for (list<MDMenuPosition*>::iterator i = options.begin(); i != options.end(); ++i)
		{
			if (i == activeOption)
			{
				font.Color = MDColor(1,1,1);
			}
			else
			{
				font.Color = MDColor(1,0.2,0);
			}
			font.SetPos(position.x,position.y - yPos);
			font.Print((*i)->GetReal());
			yPos += fontSize;
		}
	}
};