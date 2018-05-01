#pragma once

#include <string>

using namespace std;

class location
{
public:
	location();
	~location();

	location * westPTR;
	location * eastPTR;
	location * southPTR;
	location * northPTR;

	//setters
	void setX(int num) { xCoord = num; }
	void setY(int num) { yCoord = num; }

	void setDesc(string desc) { description = desc; }

	void setWest(location * input){ westPTR = input; }
	void setEast(location * input){ eastPTR = input; }
	void setNorth(location * input){ northPTR = input; }
	void setSouth(location * input){ southPTR = input; }

	//getters
	int getX(){ return xCoord; }
	int getY(){ return yCoord; }

	location * getWest(){ return westPTR; }
	location * getEast(){ return eastPTR; }
	location * getSouth(){ return southPTR; }
	location * getNorth(){ return northPTR; }

	string getDesc();

private:
	int xCoord;
	int yCoord;

	string description;
};

