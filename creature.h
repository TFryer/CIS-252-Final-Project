#pragma once

#include <string>
#include "location.h"

using namespace std;

class creature
{
public:
	creature();
	~creature();

	//special abilities
	bool knowMagicMissile;
	bool knowBigThump;
	bool knowDoubleStrike;

	int creatureType; /*Creature Type:
				 0 = player
				 1 = Ogre
				 2 = Goblin
				 3 = Warlock
				 4 = Orc*/
private:
	location * currentLoc;

	string name;//name of creature

	int HP; //health
	int MP; //mana

	int STR; //For impact of melee attacks
	int CON; //For HP and regen
	int DEX; //For accuracy of attacks
	int AGI; //For avoiding attacks
	int MIND; //For magic attack power

	int coins; //Reward for winning battles


	
public:
	//save & load functions
	void saveCharacter(fstream &outFile);
	void loadCharacter(fstream &inFile, location (&MAP)[10][10]);

	//creature initialization
	void GenerateMonster(int monType, location * loc);
	int CreatePlayer(location * startPoint);

	//overworld movement and description
	string getLocDesc() { return currentLoc->getDesc(); }
	void moveCreature(string input);

	//stat mutators
	void modHP(int mod) { HP += mod; }
	void modMP(int mod) { MP += mod; }

	void modSTR(int mod) { STR += mod; }
	void modCON(int mod) { CON += mod; }
	void modDEX(int mod) { DEX += mod; }
	void modAGI(int mod) { AGI += mod; }
	void modMIND(int mod) { MIND += mod; }

	void modCoins(int mod) { coins += mod; }
	//stat accessors
	string getName() { return name; }

	int getHP() { return HP; }
	int getMP() { return MP; }

	int getSTR() { return STR; }
	int getCON() { return CON; }
	int getDEX() { return DEX; }
	int getAGI() { return AGI; }
	int getMIND() { return MIND; }

	int getCoins(){ return coins; }

	location * getLoc(){ return currentLoc; }

	//stat setters
	void setName(string input) { name = input; }

	void setHP(int mod) { HP = mod; }
	void setMP(int mod) { MP = mod; }

	void setSTR(int mod) { STR = mod; }
	void setCON(int mod) { CON = mod; }
	void setDEX(int mod) { DEX = mod; }
	void setAGI(int mod) { AGI = mod; }
	void setMIND(int mod) { MIND = mod; }

	void setCoins(int mod) { coins = mod; }
};

