#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <sstream>

using namespace std;


const double ALPHA = 0.9;
const double GAMMA = 0.5;
const int EPOCHS = 10000;
double EXPLORE = 0.2;
const int MAX_LOCATIONS = 400;

const string INPUT_FILE = "input.dat";

void ReadFile(EnvironmentClass & ec);

LocRec EstablishStartingLocation(EnvironmentClass & ec);

bool MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it, bool algorithm);

void updateQLearnValues(RewardsRec currValues, int currRow, int currCol, int row, int col, Direction dir);

LocRec getDirectionPGreedy(EnvironmentClass & ec, LocRec curr);

double calculateQLearnValue(double lWeight, double qVal, double qMax, double reward);

double getMaxQ(QValueRec currState);

LocRec getDirectionGreedy(EnvironmentClass & ec, LocRec curr);

LocRec getNewLoc(LocRec temp, Direction dir);

double getSumQ(QValueRec currState);

int stringToInt(string s);

int main()
{
	LocRec currLoc;

	srand((time(NULL) * 9791) % 83);

	ofstream dout;

	dout.open("output.txt");

	vector<LocRec> path = vector<LocRec>();
	vector<LocRec>::iterator it = path.begin();

	for (int i = 0; i < EPOCHS; i++)
	{
		cout << "Epoch: " << i << endl;
		EnvironmentClass ec;

		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (!ec.GetLocationInformation(currLoc).isEscape)
		{	
			reward += ec.GetValueOnLocation(currLoc);	
			if(!MoveCurrentLocation(ec, currLoc, path, it, false))
				break;			
		}
		dout << "Reward: " << reward << endl;
		dout << ec.ToString(path);
		dout << endl << endl << endl;

		path.clear();
	}

	for (int i = 0; i < 1; i++)
	{
		EnvironmentClass ec;
		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{	reward += ec.GetValueOnLocation(currLoc);
			if(!MoveCurrentLocation(ec, currLoc, path, it, true))
				break;	
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

	do {
		ans.colX = (rand() % s) + 1;
		ans.rowY = (rand() % s) + 1;
	} while (ec.HasTroll(ans));

	return ans;
}


QValueRec qStates[MAX_ROOM_SIZE][MAX_ROOM_SIZE];

bool MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it, bool algorithm)
{	
	path.push_back(curr);

	if (ec.HasPony(curr))
		ec.FreePony(curr);
	else if (ec.HasTroll(curr))
		return false;

	Direction dir;

	LocRec temp;
	int count = 0;	
	if (!algorithm) {		
		do {
			temp = getDirectionPGreedy(ec, curr);
		} while (!ec.IsTileValid(temp));
		curr = temp;	
	} else {
		do {
			if (count > 1) {
				cout << "no max" << endl;
				temp = getNewLoc(curr, static_cast<Direction>(rand() % MAX_DIRECTIONS));
			} else
				temp = getDirectionGreedy(ec, curr);
			count++;
		} while (!ec.IsTileValid(temp));
	}
	curr = temp;
	return true;
}

LocRec getDirectionGreedy(EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
		qStates[curr.rowY - 1][curr.colX - 1].QNorth,
		qStates[curr.rowY - 1][curr.colX - 1].QSouth,
		qStates[curr.rowY - 1][curr.colX - 1].QWest,
		qStates[curr.rowY - 1][curr.colX - 1].QEast,
		qStates[curr.rowY - 1][curr.colX - 1].QNorthWest,
		qStates[curr.rowY - 1][curr.colX - 1].QNorthEast,
		qStates[curr.rowY - 1][curr.colX - 1].QSouthWest,
		qStates[curr.rowY - 1][curr.colX - 1].QSouthEast
	};

	cout << qStates[curr.rowY - 1][curr.colX - 1].QNorth << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QSouth << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QWest << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QEast << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QNorthWest << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QNorthEast << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QSouthWest << endl;
	cout << qStates[curr.rowY - 1][curr.colX - 1].QSouthEast << endl;

	int max = 0;
	for (int i = 0; i < MAX_DIRECTIONS; i++) {
		cout << "Max: " << max << endl;
		cout << "Values[i]: " << values[i] << endl;
		if (values[max] < values[i])
			max = i;
	}	
	return getNewLoc(curr, static_cast<Direction>(max));
}

LocRec getDirectionPGreedy(EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
						qStates[curr.rowY - 1][curr.colX - 1].QNorth,
					    qStates[curr.rowY - 1][curr.colX - 1].QSouth,
						qStates[curr.rowY - 1][curr.colX - 1].QWest,
						qStates[curr.rowY - 1][curr.colX - 1].QEast,
						qStates[curr.rowY - 1][curr.colX - 1].QNorthWest,
						qStates[curr.rowY - 1][curr.colX - 1].QNorthEast,
						qStates[curr.rowY - 1][curr.colX - 1].QSouthWest,
						qStates[curr.rowY - 1][curr.colX - 1].QSouthEast
	};

	RewardsRec currRewards = ec.ReturnNeighboringQValues(curr);
	LocRec temp;
	Direction dir;
	double checkProb = ((double)rand() / (RAND_MAX));
	
	if (getSumQ(qStates[curr.rowY - 1][curr.colX - 1]) == 0.0 || EXPLORE > checkProb) {
		dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);
		temp = getNewLoc(curr, dir);

		do {
			dir = static_cast<Direction>(rand() % MAX_DIRECTIONS);
			temp = getNewLoc(curr, dir);
		} while (!ec.IsTileValid(temp));

		updateQLearnValues(currRewards, curr.rowY, curr.colX, temp.rowY, temp.colX, dir);
		return temp;

	} else {
		double prob = 0.0;			
		int action = rand() % MAX_DIRECTIONS;	
		
		for (int i = 0; i < MAX_DIRECTIONS; i++) {			
			dir = static_cast<Direction>(action);

			temp = getNewLoc(curr, dir);
			if (ec.IsTileValid(temp)) {
				prob = values[action] / getSumQ(qStates[curr.rowY - 1][curr.colX - 1]);
				checkProb = ((double)rand() / (RAND_MAX));
				if (prob > checkProb && prob > 0.0) {
					updateQLearnValues(currRewards, curr.rowY, curr.colX, temp.rowY, temp.colX, dir);
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
	updateQLearnValues(currRewards, curr.rowY, curr.colX, temp.rowY, temp.colX, dir);
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

void updateQLearnValues(RewardsRec currRewards, int currRow, int currCol, int nextRow, int nextCol, Direction dir) {
	double maxQ = getMaxQ(qStates[nextRow - 1][nextCol - 1]);
	switch (dir) {
	case TRUE_NORTH:
		qStates[currRow - 1][currCol - 1 - 1].QNorth = calculateQLearnValue(qStates[currRow - 1][currCol - 1 - 1].weigthNorth, qStates[currRow - 1][currCol - 1 - 1].QNorth, maxQ, currRewards.rNorth);
		if (qStates[currRow - 1][currCol - 1 - 1].weigthNorth > 0.0)
			qStates[currRow - 1][currCol - 1].weigthNorth -= .001;
		break;
	case TRUE_SOUTH:
		qStates[currRow - 1][currCol - 1].QSouth += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthSouth, qStates[currRow - 1][currCol - 1].QSouth, maxQ, currRewards.rSouth);
		if (qStates[currRow - 1][currCol - 1].weigthSouth > 0.0)
			qStates[currRow - 1][currCol - 1].weigthSouth -= .001;
		break;
	case TRUE_WEST:
		qStates[currRow - 1][currCol - 1].QWest += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthWest, qStates[currRow - 1][currCol - 1].QWest, maxQ, currRewards.rWest);
		if (qStates[currRow - 1][currCol - 1].weigthWest > 0.0)
			qStates[currRow - 1][currCol - 1].weigthWest -= .001;
		break;
	case TRUE_EAST:
		qStates[currRow - 1][currCol - 1].QEast += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthEast, qStates[currRow - 1][currCol - 1].QEast, maxQ, currRewards.rEast);
		if (qStates[currRow - 1][currCol - 1].weigthEast > 0.0)
			qStates[currRow - 1][currCol - 1].weigthEast -= .001;
		break;
	case NORTH_WEST:
		qStates[currRow - 1][currCol - 1].QNorthWest += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthNorthWest, qStates[currRow - 1][currCol - 1].QNorthWest, maxQ, currRewards.rNorthWest);
		if (qStates[currRow - 1][currCol - 1].weigthNorthWest > 0.0)
			qStates[currRow - 1][currCol - 1].weigthNorthWest -= .001;
		break;
	case NORTH_EAST:
		qStates[currRow - 1][currCol - 1].QNorthEast += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthNorthEast, qStates[currRow - 1][currCol - 1].QNorthEast, maxQ, currRewards.rNorthEast);
		if (qStates[currRow - 1][currCol - 1].weigthNorthEast > 0.0)
			qStates[currRow - 1][currCol - 1].weigthNorthEast -= .001;
		break;
	case SOUTH_WEST:
		qStates[currRow - 1][currCol - 1].QSouthWest += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthSouthWest, qStates[currRow - 1][currCol - 1].QSouthWest, maxQ, currRewards.rSouthWest);
		if (qStates[currRow - 1][currCol - 1].weigthSouthWest > 0.0)
			qStates[currRow - 1][currCol - 1].weigthSouthWest -= .001;
		break;
	case SOUTH_EAST:
		qStates[currRow - 1][currCol - 1].QSouthEast += calculateQLearnValue(qStates[currRow - 1][currCol - 1].weigthSouthEast, qStates[currRow - 1][currCol - 1].QSouthEast, maxQ, currRewards.rSouthEast);
		if (qStates[currRow - 1][currCol - 1].weigthSouthEast > 0.0)
			qStates[currRow - 1][currCol - 1].weigthSouthEast -= .001;
		break;
	}
}

double getSumQ(QValueRec currState) {
	double values[MAX_DIRECTIONS] = {
		currState.QNorth,
		currState.QSouth,
		currState.QWest,
		currState.QEast,
		currState.QNorthWest,
		currState.QNorthEast,
		currState.QSouthWest,
		currState.QSouthEast
	};

	double sum = 0.0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)
		sum += values[i];

	return sum;
}

double getMaxQ(QValueRec currState) {
	double values[MAX_DIRECTIONS] = {
		currState.QNorth,
		currState.QSouth,
		currState.QWest,
		currState.QEast,
		currState.QNorthWest,
		currState.QNorthEast,
		currState.QSouthWest,
		currState.QSouthEast
	};

	double maxQ = 0.0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)
		if (maxQ < values[i])
			maxQ = values[i];
	return maxQ;
}

double calculateQLearnValue(double lWeight, double qVal, double qMax, double reward) {
	double updatedQ = (ALPHA * lWeight) * (reward + (GAMMA * qMax) - qVal);
	return updatedQ;
}
