#include "creature.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

creature::creature()
{
	name = "noname";

	HP = 100;
	MP = 100;

	STR = 10;
	CON = 10;
	DEX = 10;
	AGI = 10;
	MIND = 10;

	creatureType = 0;

	coins = 0;

	knowMagicMissile = false;
	knowBigThump = false;
	knowDoubleStrike = false;
}


creature::~creature()
{
}

void creature::GenerateMonster(int monType, location * loc)
{
	currentLoc = loc;

	switch (monType)
	{
	case 1: //ogre
		name = "an Ogre";
		HP = 150;
		STR = 12;
		CON = 12;
		DEX = 6;
		AGI = 6;
		MIND = 6;
		creatureType = 1;
		knowBigThump = true;
		break;
	case 2: //goblin
		name = "a Goblin";
		HP = 75;
		STR = 8;
		CON = 8;
		DEX = 12;
		AGI = 10;
		MIND = 6;
		creatureType = 2;
		knowDoubleStrike = true;
		break;
	case 3: //warlock
		name = "a Warlock";
		HP = 75;
		STR = 6;
		CON = 7;
		DEX = 9;
		AGI = 8;
		MIND = 12;
		creatureType = 3;
		knowMagicMissile = true;
		break;
	default: //orc
		name = "an Orc";
		HP = 100;
		STR = 10;
		CON = 10;
		DEX = 8;
		AGI = 8;
		MIND = 7;
		creatureType = 4;
		break;
	}

	
}

int creature::CreatePlayer(location * startPoint)
{
	string inString = "";
	int inInt = 0;

	currentLoc = startPoint;

	int loopCount = 0;//for preventing infinite loops

	//naming the player
	cout << "\nCreating a new character!";
	cout << "\nPlease enter a name for the character: ";
	cin >> inString;

	try
	{
		//check for name size


		//if infinite loop, throw error- otherwise set name
		if (loopCount >= 50)
		{
			throw 1;
		}
		else
		{
			name = inString;
		}
	}
	catch (int errNum)
	{
		if (errNum == 1)
		{
			cout << "\nERROR: Number of naming attempts exceeds 50. Aborting.\n";
			return 1;
		}
	}
	cout << "\nCharacter has been dubbed '" << name << "'.";

	//reset loop count
	loopCount = 0;

	//choosing character archetype
	cout << "\nNow then, do you wish to be a 1. Warrior, 2. Rogue, or 3. Mage? ";
	cin >> inInt;
	try
	{
		while (inInt < 1 || inInt > 3)
		{
			cout << "Please choose 1. Warrior, 2. Rogue, or 3. Mage: ";

			cin >> inInt;

			if (inInt < 1 || inInt > 3)
			{
				loopCount++;
			}
		}
		if (loopCount >= 50)
		{
			throw 1;
		}
	}
	catch (int errNum)
	{
		if (errNum == 1)
		{
			cout << "\nERROR: Number of attempts to choose character archetype exceeds 50. Aborting.\n";
			return 2;
		}
	}

	//set stats according to archetype
	switch (inInt)
	{
	case 1://Warrior
		STR += 2;
		CON += 2;
		AGI -= 2;
		MIND -= 2;
		knowBigThump = true;
		break;
	case 2://Rogue
		STR -= 2;
		CON -= 2;
		DEX += 2;
		AGI += 2;
		knowDoubleStrike = true;
		break;
	case 3://Mage
		STR -= 2;
		CON -= 2;
		MIND += 4;
		knowMagicMissile = true;
		break;
	default://shouldn't be used
		break;
	}

	return 0;
}

void creature::moveCreature(string input)
{
	if (input == "north" || input == "NORTH" || input == "n" || input == "N")
	{
		if (currentLoc->getNorth() || currentLoc->getY() < 9)
		{
			cout << "\nYou move north." << endl;
			currentLoc = currentLoc->getNorth();
		}
		else
		{
			cout << "\nSorry, but you cannot move further north!" << endl;
		}
	}
	else if (input == "south" || input == "SOUTH" || input == "s" || input == "S")
	{
		if (currentLoc->getSouth() || currentLoc->getY() > 1)
		{
			cout << "\nYou move south." << endl;
			currentLoc = currentLoc->getSouth();
		}
		else
		{
			cout << "\nSorry, but you cannot move further south!" << endl;
		}
	}
	else if (input == "west" || input == "WEST" || input == "w" || input == "W")
	{
		if (currentLoc->getWest() || currentLoc->getX() > 1)
		{
			cout << "\nYou move west." << endl;
			currentLoc = currentLoc->getWest();
		}
		else
		{
			cout << "\nSorry, but you cannot move further west!" << endl;
		}
	}
	else if (input == "east" || input == "EAST" || input == "e" || input == "E")
	{
		if (currentLoc->getEast() || currentLoc->getX() < 9)
		{
			cout << "\nYou move east." << endl;
			currentLoc = currentLoc->getEast();
		}
		else
		{
			cout << "\nSorry, but you cannot move further east!" << endl;
		}
	}
	else if (input == "random")
	{
		///implement randomized movement for monster
	}
	else
	{
		cout << "\nUnrecognized command. Try again." << endl;
	}

}

void creature::saveCharacter(fstream &outFile)
{
	outFile << currentLoc->getX() << endl;
	outFile << currentLoc->getY() << endl;
	outFile << name << endl;
	outFile << HP << endl;
	outFile << MP << endl;
	outFile << STR << endl;
	outFile << CON << endl;
	outFile << DEX << endl;
	outFile << AGI << endl;
	outFile << MIND << endl;
	outFile << coins << endl;

	outFile << knowBigThump << endl;
	outFile << knowDoubleStrike << endl;
	outFile << knowMagicMissile << endl;
}

void creature::loadCharacter(fstream &inFile, location   (&MAP)[10][10])
{
	string input;

	//obtain XY coords for location
	int inX;
	int inY;
	inFile >> inX;
	inFile >> inY;

	currentLoc = &MAP[inX][inY];

	//load name
	inFile >> input;
	name = input;

	//load stats
	inFile >> input;
	HP = atoi(input.c_str());

	inFile >> input;
	MP = atoi(input.c_str());

	inFile >> input;
	STR = atoi(input.c_str());

	inFile >> input;
	CON = atoi(input.c_str());

	inFile >> input;
	DEX = atoi(input.c_str());

	inFile >> input;
	AGI = atoi(input.c_str());

	inFile >> input;
	MIND = atoi(input.c_str());

	inFile >> input;
	coins = atoi(input.c_str());

	inFile >> knowBigThump;
	inFile >> knowDoubleStrike;
	inFile >> knowMagicMissile;

	//confirm load
	cout << "\nCharacter loaded." << endl;
	cout << "\nName: " << name;
	cout << "\nHP: " << HP;
	cout << "\nMP: " << MP;
	cout << "\ncoins: " << coins << endl;
}