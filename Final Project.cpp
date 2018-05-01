/*This is a simple RPG. The player has a character, whose base stats and special ability are determined by the
archetype (warrior, rogue, or mage) they chose when they start the game, which they use to
wander around a small map and fight monsters.

Both the player character and the monsters they encounter are objects of the "creature" 
class, using concepts from multiple chapters of the textbook. 

The location and navigation system uses a system of pointers and nodes, similar to lists and
other objects found in the Standard Template Library.

And finally, the game gives the player the option to save their character and load it in the future,
using an fstream object.*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "creature.h"
#include "location.h"

using namespace std;


/*To Do:
>implement saving & loading
*/
bool saveFound(fstream &inFile);
int runBattle(creature * player, creature * enemy);
int main()
{
	bool runProgram = true;
	int gameState = 1;
	string cameFrom = "";
	/*Game States:
	0 = Closing
	1 = Main Menu
	2 = Overworld
	3 = Battle*/

	//set up randomizer
	unsigned seed = time(0);
	srand(seed);

	int battleState = 1; 
	/*Battle States:
	0 = defeat
	1 = ongoing
	2 = victory
	3 = flee*/

	//error reporting
	int ERR_CODE = 0;

	//variables for player input
	int intInput = -1;
	string strInput = "";

	//for preventing unwanted infinite loops
	int loopControl = 0; 

	//save & load handling
	fstream saveFile;

	//creatures
	creature Player;
	creature Enemy;

	//map = 2d array of locations
	location Map[10][10];
	string quickDesc;
	int randomizer;
	int x, y;
	for (x = 0; x < 10; x++)
	{
		for (y = 0; y < 10; y++)
		{
			Map[x][y].setX(x);
			Map[x][y].setY(y);

			randomizer = rand() % 5;
			switch (randomizer)
			{
			case 1:
				quickDesc = "A wide-open field full of grass and wildflowers.";
				break;
			case 2:
				quickDesc = "Rolling hills covered in emerald green grass.";
				break;
			case 3: 
				quickDesc = "A shady stand of trees. A pleasant breeze causes the leaves to rustle.";
				break;
			case 4:
				quickDesc = "A swampy bog. It smells odd here.";
				break;
			default:
				quickDesc = "An empty patch of dirt.";
				break;
			}

			Map[x][y].setDesc(to_string(x) + ", " + to_string(y) + ": " + quickDesc);

			if (x > 0)
			{
				Map[x][y].setWest(&Map[x - 1][y]);
			}

			if (x < 9)
			{
				Map[x][y].setEast(&Map[x + 1][y]);
			}

			if (y > 0)
			{
				Map[x][y].setSouth(&Map[x][y - 1]);
			}

			if (y < 9)
			{
				Map[x][y].setNorth(&Map[x][y + 1]);
			}
		}
	}

	/*Core Program Loop*/
	while (runProgram)
	{
		cout << "\n----------------------------------" << endl;
		if (gameState == 1)//Main Menu
		{
			//display main menu
			cout << "MAIN MENU" << endl;
			cout << "1. Start Game\n2. Load Game\n3. Quit" << endl;
			cout << "Please enter the number corresponding to the option you wish to select: ";
			cin >> intInput;
			
			//check input
			switch (intInput)
			{
			case 1://new game
				ERR_CODE = Player.CreatePlayer(&Map[4][4]);
				if (ERR_CODE == 0)
				{
					gameState = 2;
					Enemy.GenerateMonster((rand() % 4), &Map[1][1]);
				}
				else
				{
					cout << "\nOne or more errors prevented the game from starting." << endl;
				}
				break;
			case 2://open saved game
				if (saveFound(saveFile))
				{
					saveFile.open("savegame.dat", ios::in);
					Player.loadCharacter(saveFile, Map);
					saveFile.close();

					gameState = 2;
					Enemy.GenerateMonster((rand() % 4), &Map[(rand() % 8) + 1][(rand() % 8) + 1]);
				}
				else
				{
					cout << "Save file not found!" << endl;
				}
				break;
			case 3://quit program
				gameState = 0;
				break;
			default://unrecognized command
				cout << "\nERROR: Bad Input Detected. Try again.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				intInput = 0;
				break;
			}

			
		}
		else if (gameState == 2)//Overworld
		{
			//show status
			cout << "\n" << Player.getName() << "'s Current HP: " << Player.getHP();
			cout << "\n" << Player.getName() << "'s Current MP: " << Player.getMP();
			cout << "\n" << Player.getName() << "'s Coins: " << Player.getCoins() << endl;

			//command reminder
			cout << "Current location: " << Player.getLocDesc() << endl;
			cout << "\nUsable commands are: north, south, west, east, rest, save, and quit.";
			//get command
			cout << "\nPlease enter a command: ";
			cin >> strInput;
			if (strInput == "Save" || strInput == "save" || strInput == "SAVE")
			{
				saveFile.open("savegame.dat", ios::out);
				Player.saveCharacter(saveFile);
				saveFile.close();

				cout << "\nGame saved!" << endl;
			}
			else if (strInput == "rest" || strInput == "REST" || strInput == "R" || strInput == "r")
			{
				cout << "\nYou take a moment to recover." << endl;
				Player.modHP(10 + (Player.getCON() / 3));
				Player.modMP(10 + (Player.getMIND() / 3));
			}
			else if (strInput == "QUIT" || strInput == "quit" || strInput == "Q" || strInput == "q" || strInput == "Quit")
			{
				cout << "\nAre you sure you want to quit to the main menu? y/n" << endl;
				cin >> strInput;

				while (!(strInput == "y" || strInput == "n" || strInput == "Y" || strInput == "N"))
				{
					cout << "\nBad input detected. Please enter only 'y' or 'n'." << endl;
					cin >> strInput;
					loopControl++;
					if (loopControl >= 50)
					{
						cout << "\nERROR: excessive number of loops. Exiting.\n";
						strInput = "y";
					}
				}
				loopControl = 0;

				if (strInput == "y" || strInput == "Y")
				{
					gameState = 1;
				}

			}
			else
			{
				cameFrom = strInput;
				Player.moveCreature(strInput);
			}

			//Check for collissions
			if (Player.getLoc() == Enemy.getLoc())
			{
				cout << "\nYou have encountered an enemy! Let the battle begin!" << endl;
				gameState = 3;
			}
		}
		else if (gameState == 3)//Battle
		{
			battleState = runBattle(&Player, &Enemy);

			if (battleState == 0)
			{
				gameState = 1;//return to main menu
			}
			else if (battleState == 1)
			{
				gameState = 3;//continue battle
			}
			else if (battleState == 2)
			{
				gameState = 2;
				Player.modCoins(10);
				cout << "\nYou gain 10 coins." << endl;
				battleState = 1;//resetting battlestate for next battle
				//Generate new monster
				Enemy.GenerateMonster((rand() % 4), &Map[(rand() % 8) + 1][(rand() % 8) + 1]);
			}
			else if (battleState = 3)
			{
				gameState = 2;//return to overworld
				battleState = 1;//resetting battlestate for next battle

				if (cameFrom == "NORTH" || cameFrom == "N" || cameFrom == "north" || cameFrom == "n")
				{
					Player.moveCreature("s");
				}
				else if (cameFrom == "SOUTH" || cameFrom == "S" || cameFrom == "south" || cameFrom == "s")
				{
					Player.moveCreature("n");
				}
				else if (cameFrom == "EAST" || cameFrom == "E" || cameFrom == "east" || cameFrom == "e")
				{
					Player.moveCreature("w");
				}
				else if (cameFrom == "WEST" || cameFrom == "W" || cameFrom == "west" || cameFrom == "w")
				{
					Player.moveCreature("e");
				}
			}
			
		}
		

		//check if quitting
		if (gameState == 0)
		{
			strInput = "";
			cout << "\nAre you sure you want to quit? y/n" << endl;
			cin >> strInput;
			
			while (!(strInput == "y" || strInput == "n" || strInput== "Y" || strInput == "N"))
			{
				cout << "\nBad input detected. Please enter only 'y' or 'n'." << endl;
				cin >> strInput;
				loopControl++;
				if (loopControl >= 50)
				{
					cout << "\nERROR: excessive number of loops. Exiting.\n";
					strInput = "y";
				}
			}
			loopControl = 0;

			if (strInput == "y" || strInput == "Y")
			{
				runProgram = false;
			}
			else if (strInput == "n" || strInput == "N")
			{
				gameState = 1;
			}
		}

		//cleanse inputs
		intInput = 0;
		strInput = "";
	}
	
	return 0;
}

bool saveFound(fstream &inFile)
{
	inFile.open("savegame.dat", ios::in);
	if (inFile.fail())
	{
		inFile.close();
		return false;
	}
	else
	{
		inFile.close();
		return true;
	}
}

int runBattle(creature * player, creature * enemy)
{
	/*Return Explanation:
	0 = Defeat
	1 = Ongoing
	2 = Victory
	3 = Fleeing*/

	string input = "";
	bool goodInput = false;

	int roll = 0;

	int strikeTimes = 1; //number of attacks per action- mainly used for rogue ability
	int loopCount = 0;
	int hitCount = 0;

	int attack = 0;
	int defense = 0;
	int damage = 0;
	int soak = 0;

	cout << "\nYou are in a battle against " << enemy->getName() << "!" << endl;
	//show status
	cout << "\n" << player->getName() << "'s Current HP: " << player->getHP();
	cout << "\n" << player->getName() << "'s Current MP: " << player->getMP() << endl;

	//Player Action - menu & input
	while (!goodInput)
	{
		cout << "\nHere are your options (use the number or letter at the start of the line): " << endl;
		cout << "1. Melee Attack" << endl;
		cout << "2. Rest" << endl;
		cout << "3. Flee" << endl;
		if (player->knowBigThump)
		{
			cout << "B. Big Thump (costs 10 MP)" << endl;
		}
		if (player->knowDoubleStrike)
		{
			cout << "D. Double Strike (costs 10 MP)" << endl;
		}
		if (player->knowMagicMissile)
		{
			cout << "M. Magic Missile (costs 20 MP)" << endl;
		}

		cin >> input;

		if (input == "1")//basic melee attack
		{
			goodInput = true;
			roll = (rand() % 11);
			attack = player->getDEX() + roll;
			roll = (rand() % 11);
			defense = enemy->getAGI() + roll;
			if (attack > defense)
			{
				soak = enemy->getCON() / 3;
				damage = player->getSTR() / 3 + (attack - defense);

				damage -= soak;

				if (damage <= 1)
				{
					damage = 1;
				}

				cout << "\nYou strike the enemy, doing " << damage << " damage." << endl;

				enemy->modHP(-damage);
			}
			else
			{
				cout << "\nYou take a swing at the enemy, but miss!" << endl;
			}
		}
		else if (input == "2")//rest and recover
		{
			goodInput = true;
			cout << "\nYou take a moment to catch your breath." << endl;

			player->modHP(5+(player->getCON()/3));
			player->modMP(5+(player->getMIND()/3));

		}
		else if (input == "3")//attempt to flee the battle
		{
			goodInput = true;
			roll = rand() % 11;
			attack = enemy->getDEX() + roll;
			roll = rand() % 11;
			defense = player->getAGI() + roll;

			if (defense >= attack)
			{
				cout << "\nYou successfully fled back from the direction you came from." << endl;
				return 3;
			}
			else
			{
				cout << "\nYou failed to escape! The enemy was too quick!" << endl;
			}
		}
		else if (input == "B" || input == "b")//warrior special attack
		{
			if (player->getMP() >= 10)
			{
				goodInput = true;

				player->modMP(-10);

				roll = (rand() % 11);
				attack = player->getDEX() + roll;
				roll = (rand() % 11);
				defense = enemy->getAGI() + roll;
				if (attack > defense)
				{
					soak = enemy->getCON() / 3;
					damage = ((player->getSTR() / 3) * 2) + (attack - defense) + 5;

					damage -= soak;

					cout << "\nYou hit the enemy, doing " << damage << " damage!" << endl;

					enemy->modHP(-damage);
				}
				else
				{
					cout << "\nYou take a swing at the enemy, but miss!" << endl;
				}
			}
			else
			{
				cout << "\nYou do not have enough MP!" << endl;
			}
			
		}
		else if (input == "D" || input == "d")//rogue special attack
		{
			if (player->getMP() >= 10)
			{
				goodInput = true;

				player->modMP(-10);

				strikeTimes = 2;

				while (loopCount < strikeTimes)
				{
					roll = (rand() % 11);
					attack = player->getDEX() + roll;
					roll = (rand() % 11);
					defense = enemy->getAGI() + roll;
					if (attack > defense)
					{
						soak = enemy->getCON() / 3;
						damage = player->getSTR() / 4 + (attack - defense);

						damage -= soak;

						enemy->modHP(-damage);
						cout << "\nYou hit for " << damage << " damage!";

						hitCount++;
					}
					else
					{
						cout << "\nYou miss!" << endl;
					}

					loopCount++;
				}
			}
			else
			{
				cout << "\nYou do not have enough MP!" << endl;
			}
			
		}
		else if (input == "M" || input == "m")//mage special attack
		{
			if (player->getMP() >= 20)
			{
				player->modMP(-20);

				goodInput = true;
				roll = (rand() % 11);
				attack = player->getMIND() + roll;
				roll = (rand() % 11);
				defense = enemy->getMIND() + roll;
				if (attack > defense)
				{
					damage = player->getMIND() / 2 + (attack - defense);

					damage -= soak;

					cout << "\nYou blast your enemy with a swirling bolt of magical energy, doing " << damage << "damage!"
						<< endl;

					enemy->modHP(-damage);
				}
				else
				{
					cout << "\nYour arcane projectiles miss their target! Drat!" << endl;
				}
			}
			else
			{
				cout << "\nYou do not have enough MP!" << endl;
			}
			
		}
		else//bad input
		{
			goodInput = false;
			cout << "\nBad input detected." << endl;
		}
	}
	

	//check to see if enemy defeated
	if (enemy->getHP() <= 0)
	{
		cout << "\nYou have defeated " << enemy->getName() << "!" << endl;
		return 2;
	}

	//Enemy Action
	roll = (rand() % 10);
	if (roll > 3 || (enemy->getHP() > 100 && enemy->getMP() > 20))//attack
	{
		if (roll > 7 && enemy->getMP() > 20)//special attack
		{
			if (enemy->creatureType == 1)//ogre
			{
				enemy->modMP(-10);
				roll = (rand() % 11);
				attack = enemy->getDEX() + roll - 2;
				roll = (rand() % 11);
				defense = player->getAGI() + roll;

				if (attack > defense)
				{
					soak = player->getCON() / 3;
					damage = enemy->getSTR() / 2 + (attack - defense) + 5;

					damage -= soak;

					if (damage <= 1)
					{
						damage = 4;
					}

					cout << "\nThe enemy slams you with its huge club, doing " << damage << " damage!" << endl;

					player->modHP(-damage);
				}
				else
				{
					cout << "\nThe enemy takes a swing at you, but misses!" << endl;
				}
			}
			else if (enemy->creatureType == 2)//goblin
			{
				enemy->modMP(-10);
				hitCount = 0;
				strikeTimes = 5;
				loopCount = 0;
				cout << "\nThe enemy swings its knives wildly at you!" << endl;
				damage = 0;
				while (loopCount < strikeTimes)
				{
					roll = (rand() % 11);
					attack = enemy->getDEX() + roll;
					roll = (rand() % 11);
					defense = player->getAGI() + roll;
					if (attack > defense)
					{

						damage += 2;

						cout << "\nHit!";

						hitCount++;
					}
					else
					{
						cout << "\nMiss!";
					}
					loopCount++;
				}
				cout << "\nThe enemy hits you " << hitCount << " times, doing " << damage << " damage!" << endl;
				player->modHP(-damage);
			}
			else if (enemy->creatureType == 3)//warlock
			{
				enemy->modMP(-20);
				cout << "\nThe enemy hurls a fireball at you!" << endl;
				roll = (rand() % 11);
				attack = enemy->getDEX() + enemy->getMIND() + roll;
				roll = (rand() % 11);
				defense = player->getAGI() + player->getMIND() + roll;
				if (attack > defense)
				{
					damage = enemy->getMIND() / 2 + (attack - defense);

					damage -= soak;

					if (damage <= 1)
					{
						damage = 4;
					}

					cout << "\nYou get blasted for " << damage << " damage!" << endl;

					player->modHP(-damage);
				}
				else
				{
					cout << "\nFortunately, it misses you!" << endl;
				}
			}
			else//default attack
			{
				roll = (rand() % 11);
				attack = enemy->getDEX() + roll;
				roll = (rand() % 11);
				defense = player->getAGI() + roll;
				if (attack > defense)
				{
					soak = player->getCON() / 3;
					damage = enemy->getSTR() / 3 + (attack - defense);

					damage -= soak;

					if (damage <= 1)
					{
						damage = 1;
					}

					cout << "\nThe enemy hits you, doing " << damage << " damage." << endl;

					player->modHP(-damage);
				}
				else
				{
					cout << "\nThe enemy takes a swing at you, but misses!" << endl;
				}
			}
		}
		else
		{
			roll = (rand() % 11);
			attack = enemy->getDEX() + roll;
			roll = (rand() % 11);
			defense = player->getAGI() + roll;
			if (attack > defense)
			{
				soak = player->getCON() / 3;
				damage = enemy->getSTR() / 3 + (attack - defense);

				damage -= soak;

				if (damage <= 1)
				{
					damage = 1;
				}

				cout << "\nThe enemy hits you, doing " << damage << " damage." << endl;

				player->modHP(-damage);
			}
			else
			{
				cout << "\nThe enemy takes a swing at you, but misses!" << endl;
			}
		}
	}
	else//rest
	{
		cout << "\nThe enemy appears to be catching its breath." << endl;
		enemy->modHP((enemy->getCON() / 3));
		enemy->modMP((enemy->getMIND() / 2));
	}

	//End-Round Stat Checks
	if (player->getHP() <= 0)
	{
		cout << "\nYou have been defeated!" << endl;
		return 0;
	}
	else
	{
		return 1;
	}
}