#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;

const double ALPHA = 0.5;
const double GAMMA = 0.9;
const int EPOCHS = 1;

const int MAX_LOCATIONS = 400;

const string INPUT_FILE = "input.dat";

string ALGORITHM = "";

void ReadFile(EnvironmentClass & ec);

LocRec EstablishStartingLocation(EnvironmentClass & ec);

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it);

void calculateQLearnValues(RewardsRec currValues, QValueRec *currState, Direction dir);

Direction getDirectionPGreedy(QValueRec * currState);

double calculateQLearnValue(double qVal, double qMax, double reward);

double getMaxQ(QValueRec *currState);

void initQStates();

Direction getDirectionGreedy(QValueRec * currState, LocRec temp);

int main()
{
	EnvironmentClass ec;

	ReadFile(ec);

	LocRec currLoc;
	initQStates();
	// for testing
	srand(123);

	// For Running
	// srand((time(NULL) * 9791) % 83);

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
			cout << ec.ToString(currLoc);
		}

		dout << "Reward: " << reward << endl;
		cout << "Reward: " << reward << endl;
		dout << ec.ToString(path);
		//cout << ec.ToString(path);
		//system("pause");
		dout << endl << endl << endl;

		path.clear();
	}

	for (int i = 0; i < 1; i++)
	{
		system("pause");
		EnvironmentClass ec;
		ALGORITHM = "GREEDY";
		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{
			//dout << ec.ToString(currLoc);
			
			MoveCurrentLocation(ec, currLoc, path, it);
			reward += ec.GetValueOnLocation(currLoc);
			cout << ec.ToString(path);
		}

		dout << "Reward: " << reward << endl;
		cout << "Reward: " << reward << endl;
		dout << ec.ToString(path);
		cout << ec.ToString(path);
		//system("pause");
		dout << endl << endl << endl;

		path.clear();
	}

	system("pause");
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
		cout << endl;
	}
}

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it)
{
	path.push_back(curr);

	if (ec.HasPony(curr))
		ec.FreePony(curr);

	Direction dir;
	RewardsRec currRewards = ec.ReturnNeighboringQValues(curr);
	QValueRec * currState = qStates[curr.rowY - 1][curr.colX - 1];
	
	LocRec temp;

	do {
		temp = curr;
		// get a new direction
		if (ALGORITHM.compare("GREEDY") == 0) {			
			dir = getDirectionGreedy(currState, temp);
		} else
			dir = getDirectionPGreedy(currState);

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
	calculateQLearnValues(currRewards, currState, dir);

	curr = temp;
}

Direction getDirectionGreedy(QValueRec * currState, LocRec temp) {
	double values[8] = {
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
	double sum = 0;

	for (int i = 0; i < 8; i++) {
		sum += values[i];
		if (values[max] < values[i])
			max = i;
	}
	
	switch (max) {
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

Direction getDirectionPGreedy(QValueRec * currState) {
	double values[8] = {
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
	for (int i = 0; i < 8; i++)
		sum += values[i];

	if (sum == 0.0)
		return static_cast<Direction>(rand() % MAX_DIRECTIONS);
	
	double prob = 0.0;
	while (prob == 0.0)
		prob = fmod((double)rand(), sum);

	int result;	

	double runningSum = 0.0;	
	for (int i = 0; i < 8; i++) {
		if (prob > runningSum && prob <= (runningSum + values[i])) {
			result = i;
			break;
		}
		runningSum += values[i];
	}

	switch (result) {
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

void calculateQLearnValues(RewardsRec currValues, QValueRec *currState, Direction dir) {

	double maxQ = getMaxQ(currState);

	
	switch (dir) {
	case TRUE_NORTH:
		currState->QNorth = calculateQLearnValue(currState->QNorth, maxQ, currValues.rNorth);
		break;
	case TRUE_SOUTH:
		currState->QSouth = calculateQLearnValue(currState->QSouth, maxQ, currValues.rSouth);
		break;
	case TRUE_EAST:
		currState->QEast = calculateQLearnValue(currState->QEast, maxQ, currValues.rEast);
		break;
	case TRUE_WEST:
		currState->QWest = calculateQLearnValue(currState->QWest, maxQ, currValues.rWest);
		break;
	case NORTH_EAST:
		currState->QSouthEast = calculateQLearnValue(currState->QSouthEast, maxQ, currValues.rSouthEast);
		break;
	case NORTH_WEST:
		currState->QSouthWest = calculateQLearnValue(currState->QSouthWest, maxQ, currValues.rSouthWest);
		break;
	case SOUTH_EAST:
		currState->QNorthEast = calculateQLearnValue(currState->QNorthEast, maxQ, currValues.rNorthEast);
		break;
	case SOUTH_WEST:
		currState->QNorthWest = calculateQLearnValue(currState->QNorthWest, maxQ, currValues.rNorthWest);
		break;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << "South: " << qStates[i][j]->QSouth << " ";
		}
		cout << endl;
	}

}

double getMaxQ(QValueRec *currState) {
	double values[8] = {
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
	for (int i = 0; i < 8; i++)
		if (maxQ < values[i])
			maxQ = values[i];
	return maxQ;
}

double calculateQLearnValue(double qVal, double qMax, double reward) {
	return qVal + ALPHA * (reward + (GAMMA * qMax) - qVal);		
}