#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"
#include <cstdlib>
#include <ctime>

using namespace std;

const double ALPHA = 0.75;
const double GAMMA = 0.5;
const int EPOCHS = 1; // = 10000;

const int MAX_LOCATIONS = 400;

const string INPUT_FILE = "input.dat";

void ReadFile(EnvironmentClass & ec);

LocRec EstablishStartingLocation(EnvironmentClass & ec);

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it);

void calculateQLearnValues(RewardsRec currValues, QValueRec &currState);

Direction getDirection(QValueRec currState);

double calculateQLearnValue(double qVal, double qMax, double reward);

void MoveCurrentLocationFirst(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it);

void firstMove(EnvironmentClass & ec, vector<LocRec> & path, vector<LocRec>::iterator & it);

int main()
{
	EnvironmentClass ec;

	ReadFile(ec);

	LocRec currLoc;

	// for testing
	srand(123);

	// For Running
	// srand((time(NULL) * 9791) % 83);

	ofstream dout;

	dout.open("output.txt");

	vector<LocRec> path = vector<LocRec>();
	vector<LocRec>::iterator it = path.begin();

	firstMove(ec, path, it);

	for (int i = 1; i < EPOCHS; i++)
	{
		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{
			//dout << ec.ToString(currLoc);
			MoveCurrentLocation(ec, currLoc, path, it);
			cout << ec.ToString(path);
			system("pause");
			reward += ec.GetValueOnLocation(currLoc);
		}

		dout << "Reward: " << reward << endl;

		dout << ec.ToString(path);

		dout << endl << endl << endl;

		path.clear();
	}

	dout.close();

	return 0;
}


void ReadFile(EnvironmentClass & ec)
{
	ifstream din;

	din.open(INPUT_FILE.c_str());

	int n, t, p;

	LocRec temp;

	// Get line one from file.
	din >> n >> t >> p;

	// get line two from file
	din >> temp.colX >> temp.rowY;

	ec = EnvironmentClass(n, ALPHA, GAMMA);

	ec.SetEscapeOnLocation(temp);


	// get line three from file
	for (int i = 0; i < p; i++)
	{
		din >> temp.colX >> temp.rowY;

		ec.SetPonyOnLocation(temp);
	}


	//get line 4 from file
	string tempStr;

	getline(din, tempStr);

	int obstructionArr[MAX_LOCATIONS];

	string num = "";

	int numbersFound = 0;

	for (int i = 0; i < tempStr.length(); i++)
	{
		if (tempStr[i] == ' ')
		{
			numbersFound++;

			if (numbersFound % 2 == 0)
			{
				temp.rowY = stoi(num);

				if (temp.rowY != -1 && temp.colX != -1)
					ec.SetObstructionOnLocation(temp);
			}
			else
				temp.colX = stoi(num);

			num = "";
		}
		else
			num += tempStr[i];
	}


	// get line 5 from file
	for (int i = 0; i < t; i++)
	{
		din >> temp.colX >> temp.rowY;

		ec.SetTrollOnLocation(temp);
	}

}

LocRec EstablishStartingLocation(EnvironmentClass & ec)
{
	int s = ec.GetRoomSize();

	LocRec ans;

	ans.colX = (rand() % s) + 1;

	ans.rowY = (rand() % s) + 1;

	return ans;
}

void firstMove(EnvironmentClass & ec, vector<LocRec> & path, vector<LocRec>::iterator & it) {
	int reward = 0;

	LocRec currLoc = EstablishStartingLocation(ec);

	while (ec.GetLocationInformation(currLoc).isEscape == false)
	{
		//dout << ec.ToString(currLoc);
		MoveCurrentLocationFirst(ec, currLoc, path, it);
		cout << ec.ToString(path);
		system("pause");
		reward += ec.GetValueOnLocation(currLoc);
	}

	path.clear();

}

void MoveCurrentLocationFirst(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it) {
	path.push_back(curr);

	LocRec temp = curr;

	do {
		temp = curr;
		
		// get a new direction
		Direction dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);

		switch (dir)
		{
		case TRUE_NORTH:
			temp.rowY += 1;
			break;
		case TRUE_SOUTH:
			temp.rowY -= 1;
			break;
		case TRUE_EAST:
			temp.colX += 1;
			break;
		case TRUE_WEST:
			temp.colX -= 1;
			break;
		case NORTH_EAST:
			temp.colX += 1;
			temp.rowY += 1;
			break;
		case NORTH_WEST:
			temp.colX -= 1;
			temp.rowY += 1;
			break;
		case SOUTH_EAST:
			temp.colX += 1;
			temp.rowY -= 1;
			break;
		case SOUTH_WEST:
			temp.colX -= 1;
			temp.rowY -= 1;
			break;
		}
	} while (!ec.IsTileValid(temp));

	curr = temp;
}

QValueRec qStates[MAX_ROOM_SIZE][MAX_ROOM_SIZE];

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it)
{
	path.push_back(curr);

	LocRec temp = curr;

	do {
		temp = curr;

		RewardsRec currRewards = ec.ReturnNeighboringQValues(temp);
		QValueRec currState = qStates[temp.rowY][temp.colX];
		
		calculateQLearnValues(currRewards, currState);

		// get a new direction
		Direction dir = getDirection(currState);

		switch (dir)
		{
		case TRUE_NORTH:
			temp.rowY += 1;
			break;
		case TRUE_SOUTH:
			temp.rowY -= 1;
			break;
		case TRUE_EAST:
			temp.colX += 1;
			break;
		case TRUE_WEST:
			temp.colX -= 1;
			break;
		case NORTH_EAST:
			temp.colX += 1;
			temp.rowY += 1;
			break;
		case NORTH_WEST:
			temp.colX -= 1;
			temp.rowY += 1;
			break;
		case SOUTH_EAST:
			temp.colX += 1;
			temp.rowY -= 1;
			break;
		case SOUTH_WEST:
			temp.colX -= 1;
			temp.rowY -= 1;
			break;
		}		
	} while (!ec.IsTileValid(temp));

	curr = temp;
}

Direction getDirection(QValueRec currState) {
	double values[8] = {
						currState.QNorth,
					    currState.QSouth,
						currState.QWest,
						currState.QEast,
						currState.QNorthWest,
						currState.QNorthEast,
						currState.QSouthWest,
						currState.QSouthEast
	};
	
	int location = 0;
	for (int i = 0; i < 8; i++)
		if (values[location] < values[i])
			location = i;

	if (values[location] == 0.0)
		return static_cast<Direction>(rand() % MAX_DIRECTIONS);

	switch (location) {
	case 0: return TRUE_NORTH;		
		break;
	case 1: return TRUE_SOUTH;
		break;
	case 2: return TRUE_WEST;		
		break;
	case 3: return TRUE_EAST;
		break;
	case 4: return NORTH_WEST;
		break;
	case 5: return NORTH_EAST;
		break;
	case 6: return SOUTH_WEST;
		break;
	case 7: return SOUTH_EAST;
		break;
	}	
}

void calculateQLearnValues(RewardsRec currValues, QValueRec &currState) {
	currState.QNorth = calculateQLearnValue(currState.QNorth, currState.QNorthMax, currValues.rNorth);
	if (currState.QNorth > currState.QNorthMax)
		currState.QNorthMax = currState.QNorth;

	currState.QSouth = calculateQLearnValue(currState.QSouth, currState.QSouthMax, currValues.rSouth);
	if (currState.QSouth > currState.QSouthMax)
		currState.QSouthMax = currState.QSouth;

	currState.QEast = calculateQLearnValue(currState.QEast, currState.QEastMax, currValues.rEast);
	if (currState.QEast > currState.QEastMax)
		currState.QEastMax = currState.QEast;

	currState.QWest = calculateQLearnValue(currState.QWest, currState.QWestMax, currValues.rWest);
	if (currState.QWest > currState.QWestMax)
		currState.QWestMax = currState.QWest;

	currState.QSouthEast = calculateQLearnValue(currState.QSouthEast, currState.QSouthEastMax, currValues.rSouthEast);
	if (currState.QSouthEast > currState.QSouthEastMax)
		currState.QSouthEastMax = currState.QSouth;

	currState.QSouthWest = calculateQLearnValue(currState.QSouthWest, currState.QSouthWestMax, currValues.rSouthWest);
	if (currState.QSouthWest > currState.QSouthWestMax)
		currState.QSouthWestMax = currState.QSouthWest;

	currState.QNorthEast = calculateQLearnValue(currState.QNorthEast, currState.QNorthEastMax, currValues.rNorthEast);
	if (currState.QNorthEast > currState.QNorthEastMax)
		currState.QNorthEastMax = currState.QNorthEast;

	currState.QNorthWest = calculateQLearnValue(currState.QNorthWest, currState.QNorthWestMax, currValues.rNorthWest);
	if (currState.QNorthWest > currState.QNorthWestMax)
		currState.QNorthWestMax = currState.QNorthWest;
}

double calculateQLearnValue(double qVal, double qMax, double reward) {
	return qVal + ALPHA * (reward + (GAMMA * qMax) - qVal);
}