#include "location.h"


location::location()
{
	xCoord = 0;
	yCoord = 0;

	westPTR = nullptr;
	eastPTR = nullptr;
	northPTR = nullptr;
	southPTR = nullptr;

	description = "empty desc";
}


location::~location()
{
}

string location::getDesc()
{
	string output;

	output = description + "\nCurrent available directions for travel are: ";

	if (westPTR != nullptr)
	{
		output += "\nWest";//for debugging: - " + to_string(westPTR->getX()) + ", " + to_string(westPTR->getY());
	}
	if (eastPTR != nullptr)
	{
		output += "\nEast";//for debugging: - " + to_string(eastPTR->getX()) + ", " + to_string(eastPTR->getY());
	}
	if (northPTR != nullptr)
	{
		output += "\nNorth";//for debugging: - " + to_string(northPTR->getX()) + ", " + to_string(northPTR->getY());
	}
	if (southPTR != nullptr)
	{
		output += "\nSouth";//for debugging: - " + to_string(southPTR->getX()) + ", " + to_string(southPTR->getY());
	}

	return output;
}