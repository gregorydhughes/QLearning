#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <sstream>

using namespace std;

const double EXPLORE = 0.2;
const double ALPHA = 0.1;
const double GAMMA = 0.5;
const int EPOCHS = 10000;

const int MAX_LOCATIONS = 400;

const string INPUT_FILE = "input.dat";

string ALGORITHM = "";

void ReadFile(EnvironmentClass & ec);

LocRec EstablishStartingLocation(EnvironmentClass & ec);

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it);

void calculateQLearnValues(RewardsRec currValues, QValueRec *currState, QValueRec *nextState, Direction dir);

LocRec getDirectionPGreedy(QValueRec * currState, EnvironmentClass & ec, LocRec curr);

double calculateQLearnValue(double qVal, double qMax, double reward);

double getMaxQ(QValueRec *currState);

void initQStates();

LocRec getDirectionGreedy(QValueRec * currState, EnvironmentClass & ec, LocRec curr);

LocRec getNewLoc(LocRec temp, Direction dir);

double getSumQ(QValueRec *currState);

int stringToInt(string s);

int main()
{
	EnvironmentClass ec;

	ReadFile(ec);

	LocRec currLoc;
	initQStates();
	// for testing
	//srand(123);

	// For Running
	srand((time(NULL) * 9791) % 83);

	ofstream dout;

	dout.open("output.txt");

	vector<LocRec> path = vector<LocRec>();
	vector<LocRec>::iterator it = path.begin();

	//firstMove(ec, path, it);

	for (int i = 0; i < EPOCHS; i++)
	{
		cout << "Epoch: " << i << endl;
		EnvironmentClass ec;

		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{			
			MoveCurrentLocation(ec, currLoc, path, it);
			reward += ec.GetValueOnLocation(currLoc);
		}

		dout << "Reward: " << reward << endl;
		dout << ec.ToString(path);
		dout << endl << endl << endl;

		path.clear();
	}

	for (int i = 0; i < 1; i++)
	{
		EnvironmentClass ec;
		ALGORITHM = "GREEDY";
		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{	
			MoveCurrentLocation(ec, currLoc, path, it);
			reward += ec.GetValueOnLocation(currLoc);
			
		}

		dout << "Reward: " << reward << endl;
		dout << ec.ToString(path);
		cout << ec.ToString(path);
		dout << endl << endl << endl;

		path.clear();
	}
	dout.close();

	return 0;
}


int stringToInt(string s) {
    std::stringstream ss(s);
    int x;
    ss >> x;
    return x;
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
				temp.rowY = stringToInt(num);

				if (temp.rowY != -1 && temp.colX != -1)
					ec.SetObstructionOnLocation(temp);
			}
			else
				temp.colX = stringToInt(num);

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


QValueRec * qStates[MAX_ROOM_SIZE][MAX_ROOM_SIZE];

void initQStates() {
	for (int i = 0; i < MAX_ROOM_SIZE; i++) {
		for (int j = 0; j < MAX_ROOM_SIZE; j++) {
			qStates[i][j] = new QValueRec;
			qStates[i][j]->QNorth = 0.0;
			qStates[i][j]->QSouth = 0.0;
			qStates[i][j]->QWest = 0.0;
			qStates[i][j]->QEast = 0.0;
			qStates[i][j]->QNorthWest = 0.0;
			qStates[i][j]->QNorthEast = 0.0;
			qStates[i][j]->QSouthWest = 0.0;
			qStates[i][j]->QSouthEast = 0.0;
		}
	}
}

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it)
{	
	path.push_back(curr);

	if (ec.HasPony(curr))
		ec.FreePony(curr);

	Direction dir;	
	QValueRec * currState = qStates[curr.rowY][curr.colX];
	
	if (ALGORITHM.compare("GREEDY") == 0) {
		LocRec temp;
		do {
			temp = getDirectionPGreedy(currState, ec, curr);
		} while (!ec.IsTileValid(temp));
		curr = temp;		
	} else
		curr = getDirectionPGreedy(currState, ec, curr);
}

LocRec getDirectionGreedy(QValueRec * currState, EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
		currState->QNorth,
		currState->QSouth,
		currState->QWest,
		currState->QEast,
		currState->QNorthWest,
		currState->QNorthEast,
		currState->QSouthWest,
		currState->QSouthEast
	};

	int max = 0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)
		if (values[max] < values[i])
			max = i;

	return getNewLoc(curr, static_cast<Direction>(max));
}

LocRec getDirectionPGreedy(QValueRec * currState, EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
						currState->QNorth,
					    currState->QSouth,
						currState->QWest,
						currState->QEast,
						currState->QNorthWest,
						currState->QNorthEast,
						currState->QSouthWest,
						currState->QSouthEast
	};

	RewardsRec currRewards = ec.ReturnNeighboringQValues(curr);
	LocRec temp;
	Direction dir;
	double checkProb = ((double)rand() / (RAND_MAX));
	
	if (getSumQ(currState) == 0.0 || EXPLORE > checkProb) {
		dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);
		temp = getNewLoc(curr, dir);

		do {
			dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);
			temp = getNewLoc(curr, dir);
		} while (!ec.IsTileValid(temp));

		calculateQLearnValues(currRewards, currState, qStates[temp.rowY][temp.colX], dir);
		return temp;

	} else {
		double prob = 0.0;			
		int action = rand() % MAX_DIRECTIONS;	
		
		for (int i = 0; i < MAX_DIRECTIONS; i++) {			
			dir = static_cast<Direction>(action);

			temp = getNewLoc(curr, dir);
			if (ec.IsTileValid(temp)) {
				prob = values[action] / getSumQ(currState);
				checkProb = ((double)rand() / (RAND_MAX));
				if (prob > checkProb && prob > 0.0) {
					calculateQLearnValues(currRewards, currState, qStates[temp.rowY][temp.colX], dir);
					return temp;
				}				
				if (++action == MAX_DIRECTIONS)
					action = 0;				
			}
		}
	}

	do {
		dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);
		temp = getNewLoc(curr, dir);
	} while (!ec.IsTileValid(temp));
	calculateQLearnValues(currRewards, currState, qStates[temp.rowY][temp.colX], dir);	
	return temp;
}

LocRec getNewLoc(LocRec temp, Direction dir) {
	switch (dir) {
	case TRUE_NORTH:
		temp.rowY += 1;
		break;
	case TRUE_SOUTH:
		temp.rowY -= 1;
		break;
	case TRUE_WEST:
		temp.colX -= 1;
		break;
	case TRUE_EAST:
		temp.colX += 1;
		break;
	case NORTH_WEST:
		temp.colX -= 1;
		temp.rowY += 1;
		break;
	case NORTH_EAST:
		temp.colX += 1;
		temp.rowY += 1;
		break;
	case SOUTH_WEST:
		temp.colX -= 1;
		temp.rowY -= 1;
		break;
	case SOUTH_EAST:
		temp.colX += 1;
		temp.rowY -= 1;
		break;
	}
	return temp;
}

void calculateQLearnValues(RewardsRec currRewards, QValueRec *maxState, QValueRec *nextState, Direction dir) {
	double maxQ = getMaxQ(nextState);
	switch (dir) {
	case TRUE_NORTH:
		maxState->QNorth += calculateQLearnValue(maxState->QNorth, maxQ, currRewards.rNorth);
		break;
	case TRUE_SOUTH:
		maxState->QSouth += calculateQLearnValue(maxState->QSouth, maxQ, currRewards.rSouth);
		break;
	case TRUE_WEST:
		maxState->QWest += calculateQLearnValue(maxState->QWest, maxQ, currRewards.rWest);
		break;
	case TRUE_EAST:
		maxState->QEast += calculateQLearnValue(maxState->QEast, maxQ, currRewards.rEast);
		break;	
	case NORTH_WEST:
		maxState->QNorthWest += calculateQLearnValue(maxState->QNorthWest, maxQ, currRewards.rNorthWest);
		break;
	case NORTH_EAST:
		maxState->QNorthEast += calculateQLearnValue(maxState->QNorthEast, maxQ, currRewards.rNorthEast);
		break;
	case SOUTH_WEST:
		maxState->QSouthWest += calculateQLearnValue(maxState->QSouthWest, maxQ, currRewards.rSouthWest);
		break;
	case SOUTH_EAST:
		maxState->QSouthEast += calculateQLearnValue(maxState->QSouthEast, maxQ, currRewards.rSouthEast);
		break;	
	}	
}

double getSumQ(QValueRec *currState) {
	double values[MAX_DIRECTIONS] = {
		currState->QNorth,
		currState->QSouth,
		currState->QWest,
		currState->QEast,
		currState->QNorthWest,
		currState->QNorthEast,
		currState->QSouthWest,
		currState->QSouthEast
	};

	double sum = 0.0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)
		sum += values[i];

	return sum;
}

double getMaxQ(QValueRec *currState) {
	double values[MAX_DIRECTIONS] = {
		currState->QNorth,
		currState->QSouth,
		currState->QWest,
		currState->QEast,
		currState->QNorthWest,
		currState->QNorthEast,
		currState->QSouthWest,
		currState->QSouthEast
	};

	double maxQ = 0.0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)
		if (maxQ < values[i])
			maxQ = values[i];
	return maxQ;
}

double calculateQLearnValue(double qVal, double qMax, double reward) {
	return ALPHA * (reward + (GAMMA * qMax) - qVal) + .01;
}
