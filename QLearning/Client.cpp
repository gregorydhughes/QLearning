#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <sstream>

using namespace std;

// learning rate
const double ALPHA = 0.75;

// discoutn factor
const double GAMMA = 0.5;

// learning runs
const int EPOCHS = 10000;

// Chance to explore
double EXPLORE = 0.2;

// Max board size
const int MAX_LOCATIONS = 400;

// the input file
const string INPUT_FILE = "in.dat";

// Agents memory banks
QValueRec qStates[MAX_ROOM_SIZE][MAX_ROOM_SIZE];

// Pathing to check for loops
vector<LocRec> prevLocs = vector<LocRec>();

// Parameters: ec - the enviroment the agent explores
// Post-Condition: builds the enviroment
void ReadFile(EnvironmentClass & ec);

// Post-Condition: initializes the agents q-learning memory banks
void initQStates();

// Parameters: ec - the enviroment the agent explores
// Returns: a LocRec of the starting space
LocRec EstablishStartingLocation(EnvironmentClass & ec);

// Parameters:        ec - the enviroment the agent explores
//	            curr - the current location of the agent
//	           &path - the path the agent has traveled so far
//             algorithm - a boolean for whether to use greedy of p greedy
// Returns: a boolean of whether the move was successful
bool MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, bool algorithm);

// Parameters:   ec - the enviroment the agent explores
//	       curr - the current location of the agent
// Returns: a LocRec for the next move
LocRec getDirectionPGreedy(EnvironmentClass & ec, LocRec curr);

// Parameters:   ec - the enviroment the agent explores
//	       curr - the current location of the agent
// Returns: a LocRec for the next possible move
LocRec getDirectionGreedy(EnvironmentClass & ec, LocRec curr);

// Parameters: temp - the LocRec to build the new move from
//		dir - the new direction to move
// Returns: a new LocRec to be tested
LocRec getNewLoc(LocRec temp, Direction dir);

// Parameters:        ec - the enviroment the agent explores
//	            curr - the current location of the agent
//	           &path - the path the agent has traveled so far
// Post-Condition: causes the agent to randomly move
void randomMove(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path);

// Parameters: currRewards - the current set of available rewards
// 		   currRow - the current row the agent is in
//		   currCol - the current column the agent is in
//		       row - the new row the agent will move to
//		       col - new new col the agent will move to
//	               dir - the direction the agent will be moving in
// Post-Condition: updates the q value between current state and next state
void updateQLearnValues(RewardsRec currRewards, int currRow, int currCol, int row, int col, Direction dir);

// Parameters: lWeigth - the weight the current update will have, decreases over times traveled
//		  qVal - the current q value
//		  qMax - the max q value of the next state
//		reward - the reward for traveling to the next state
// Returns: a double of the new value to add q
double calculateQLearnValue(double lWeight, double qVal, double qMax, double reward);

// Parameters: currState - the current state q values
// Returns: the sum of all the Qs
double getSumQ(QValueRec currState);

// Parameters: currState - the current state q values
// Returns: the max q of the state
double getMaxQ(QValueRec currState);

// Parameters:	check - agents current location
// Returns: returns a boolean for if the agent is looping
bool isLooping(LocRec check);

// Parameters: s - an int in string form
// Returns: a string in int form
int stringToInt(string s);


int main()
{
	LocRec currLoc;
	initQStates();
	srand((time(NULL) * 9791) % 83);

	ofstream dout;

	dout.open("output.txt");

	vector<LocRec> path = vector<LocRec>();
	vector<LocRec>::iterator it = path.begin();

	for (int i = 0; i < EPOCHS; i++)
	{
		//cout << "Epoch: " << i << endl;
		EnvironmentClass ec;

		ReadFile(ec);

		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (!ec.GetLocationInformation(currLoc).isEscape)
		{	
			reward += ec.GetValueOnLocation(currLoc);	
			if(!MoveCurrentLocation(ec, currLoc, path, false))
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
		int count = 0;
		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{	
			if (count >= 50) {
				randomMove(ec, currLoc, path);
				count = 0;
			}
			reward += ec.GetValueOnLocation(currLoc);
			if(!MoveCurrentLocation(ec, currLoc, path, true))
				break;	
			count++;			
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

// Parameters: ec - the enviroment the agent explores
// Post-Condition: builds the enviroment
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

// Post-Condition: initializes the agents q-learning memory banks
void initQStates() {
	for (int i = 0; i < MAX_ROOM_SIZE; i++) {
		for (int j = 0; j < MAX_ROOM_SIZE; j++) {
			qStates[i][j].QNorth = 0.0;
			qStates[i][j].QSouth = 0.0;
			qStates[i][j].QWest = 0.0;
			qStates[i][j].QEast = 0.0;
			qStates[i][j].QNorthWest = 0.0;
			qStates[i][j].QNorthEast = 0.0;
			qStates[i][j].QSouthWest = 0.0;
			qStates[i][j].QSouthEast = 0.0;
			// Initialize weigths
			qStates[i][j].weigthNorth = 1.0;
			qStates[i][j].weigthSouth = 1.0;
			qStates[i][j].weigthWest = 1.0;
			qStates[i][j].weigthEast = 1.0;
			qStates[i][j].weigthNorthWest = 1.0;
			qStates[i][j].weigthNorthEast = 1.0;
			qStates[i][j].weigthSouthWest = 1.0;
			qStates[i][j].weigthSouthEast = 1.0;
		}
	}
}

// Parameters: ec - the enviroment the agent explores
// Returns: a LocRec of the starting space
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

// Parameters:        ec - the enviroment the agent explores
//	            curr - the current location of the agent
//	           &path - the path the agent has traveled so far
//             algorithm - a boolean for whether to use greedy of p greedy
// Returns: a boolean of whether the move was successful
bool MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, bool algorithm)
{	
	path.push_back(curr);

	if (ec.HasPony(curr))
		ec.FreePony(curr);
	else if (ec.HasTroll(curr))
		return false;

	Direction dir;

	LocRec temp;
	if (!algorithm) {		
		do {
			temp = getDirectionPGreedy(ec, curr);
		} while (!ec.IsTileValid(temp));
		curr = temp;	
	} else {
		do {
			temp = getDirectionGreedy(ec, curr);
		} while (!ec.IsTileValid(temp));

		if (!isLooping(temp)) {
			prevLocs.push_back(temp);
		} else {
			do {
				temp = getNewLoc(curr, static_cast<Direction>(rand() % MAX_DIRECTIONS));
			} while (!ec.IsTileValid(temp));
			prevLocs.clear();
			prevLocs.push_back(temp);
		}
	}
	curr = temp;
	return true;
}

// Parameters:   ec - the enviroment the agent explores
//	       curr - the current location of the agent
// Returns: a LocRec for the next move
LocRec getDirectionPGreedy(EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
						qStates[curr.rowY][curr.colX].QNorth,
					    qStates[curr.rowY][curr.colX].QSouth,
						qStates[curr.rowY][curr.colX].QWest,
						qStates[curr.rowY][curr.colX].QEast,
						qStates[curr.rowY][curr.colX].QNorthWest,
						qStates[curr.rowY][curr.colX].QNorthEast,
						qStates[curr.rowY][curr.colX].QSouthWest,
						qStates[curr.rowY][curr.colX].QSouthEast
	};

	RewardsRec currRewards = ec.ReturnNeighboringQValues(curr);
	LocRec temp;
	Direction dir;
	double checkProb = ((double)rand() / (RAND_MAX));
	
	if (getSumQ(qStates[curr.rowY][curr.colX]) == 0.0 || EXPLORE > checkProb) {
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
				prob = values[action] / getSumQ(qStates[curr.rowY][curr.colX]);
				checkProb = ((double)rand() / (RAND_MAX));
				if (prob > checkProb) {
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

// Parameters:   ec - the enviroment the agent explores
//	       curr - the current location of the agent
// Returns: a LocRec for the next possible move
LocRec getDirectionGreedy(EnvironmentClass & ec, LocRec curr) {
	double values[MAX_DIRECTIONS] = {
		qStates[curr.rowY][curr.colX].QNorth,
		qStates[curr.rowY][curr.colX].QSouth,
		qStates[curr.rowY][curr.colX].QWest,
		qStates[curr.rowY][curr.colX].QEast,
		qStates[curr.rowY][curr.colX].QNorthWest,
		qStates[curr.rowY][curr.colX].QNorthEast,
		qStates[curr.rowY][curr.colX].QSouthWest,
		qStates[curr.rowY][curr.colX].QSouthEast
	};
	if (getSumQ(qStates[curr.rowY][curr.colX]) <= 0.0)
		return getNewLoc(curr, static_cast<Direction>(rand() % MAX_DIRECTIONS));

	int max = 0;
	for (int i = 0; i < MAX_DIRECTIONS; i++)	
		if (values[max] < values[i])
			max = i;
	return getNewLoc(curr, static_cast<Direction>(max));
}

// Parameters: temp - the LocRec to build the new move from
//		dir - the new direction to move
// Returns: a new LocRec to be tested
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

// Parameters:        ec - the enviroment the agent explores
//	            curr - the current location of the agent
//	           &path - the path the agent has traveled so far
// Post-Condition: causes the agent to randomly move
void randomMove(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path) {	
	path.push_back(curr);
	LocRec temp;
	do {
		temp = getNewLoc(curr, static_cast<Direction>(rand() % MAX_DIRECTIONS));
	} while (!ec.IsTileValid(temp));
	curr = temp;
}

// Parameters: currRewards - the current set of available rewards
// 		   currRow - the current row the agent is in
//		   currCol - the current column the agent is in
//		       row - the new row the agent will move to
//		       col - new new col the agent will move to
//	               dir - the direction the agent will be moving in
// Post-Condition: updates the q value between current state and next state
void updateQLearnValues(RewardsRec currRewards, int currRow, int currCol, int nextRow, int nextCol, Direction dir) {
	double maxQ = getMaxQ(qStates[nextRow][nextCol]);
	switch (dir) {
	case TRUE_NORTH:
		qStates[currRow][currCol].QNorth += calculateQLearnValue(qStates[currRow][currCol].weigthNorth, qStates[currRow][currCol].QNorth, maxQ, currRewards.rNorth);
		if (qStates[currRow][currCol].weigthNorth > 0.0)
			qStates[currRow][currCol].weigthNorth *= .99;
		break;
	case TRUE_SOUTH:
		qStates[currRow][currCol].QSouth += calculateQLearnValue(qStates[currRow][currCol].weigthSouth, qStates[currRow][currCol].QSouth, maxQ, currRewards.rSouth);
		if (qStates[currRow][currCol].weigthSouth > 0.0)
			qStates[currRow][currCol].weigthSouth *= .99;
		break;
	case TRUE_WEST:
		qStates[currRow][currCol].QWest += calculateQLearnValue(qStates[currRow][currCol].weigthWest, qStates[currRow][currCol].QWest, maxQ, currRewards.rWest);
		if (qStates[currRow][currCol].weigthWest > 0.0)
			qStates[currRow][currCol].weigthWest *= .99;
		break;
	case TRUE_EAST:
		qStates[currRow][currCol].QEast += calculateQLearnValue(qStates[currRow][currCol].weigthEast, qStates[currRow][currCol].QEast, maxQ, currRewards.rEast);
		if (qStates[currRow][currCol].weigthEast > 0.0)
			qStates[currRow][currCol].weigthEast *= .99;
		break;
	case NORTH_WEST:
		qStates[currRow][currCol].QNorthWest += calculateQLearnValue(qStates[currRow][currCol].weigthNorthWest, qStates[currRow][currCol].QNorthWest, maxQ, currRewards.rNorthWest);
		if (qStates[currRow][currCol].weigthNorthWest > 0.0)
			qStates[currRow][currCol].weigthNorthWest *= .99;
		break;
	case NORTH_EAST:
		qStates[currRow][currCol].QNorthEast += calculateQLearnValue(qStates[currRow][currCol].weigthNorthEast, qStates[currRow][currCol].QNorthEast, maxQ, currRewards.rNorthEast);
		if (qStates[currRow][currCol].weigthNorthEast > 0.0)
			qStates[currRow][currCol].weigthNorthEast *= .99;
		break;
	case SOUTH_WEST:
		qStates[currRow][currCol].QSouthWest += calculateQLearnValue(qStates[currRow][currCol].weigthSouthWest, qStates[currRow][currCol].QSouthWest, maxQ, currRewards.rSouthWest);
		if (qStates[currRow][currCol].weigthSouthWest > 0.0)
			qStates[currRow][currCol].weigthSouthWest *= .99;
		break;
	case SOUTH_EAST:
		qStates[currRow][currCol].QSouthEast += calculateQLearnValue(qStates[currRow][currCol].weigthSouthEast, qStates[currRow][currCol].QSouthEast, maxQ, currRewards.rSouthEast);
		if (qStates[currRow][currCol].weigthSouthEast > 0.0)
			qStates[currRow][currCol].weigthSouthEast *= .99;
		break;
	}
}

// Parameters: lWeigth - the weight the current update will have, decreases over times traveled
//		  qVal - the current q value
//		  qMax - the max q value of the next state
//		reward - the reward for traveling to the next state
// Returns: a double of the new value to add q
double calculateQLearnValue(double lWeight, double qVal, double qMax, double reward) {
	double qUpdate = (ALPHA * lWeight) * (reward + (GAMMA * qMax) - qVal);	
	if (reward < 0.0)
		qUpdate -= 0.01;
	return qUpdate;
}

// Parameters: currState - the current state q values
// Returns: the sum of all the Qs
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

// Parameters: currState - the current state q values
// Returns: the max q of the state
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

// Parameters:	check - agents current location
// Returns: returns a boolean for if the agent is looping
bool isLooping(LocRec check) {
	for (int i = 0; i < prevLocs.size(); i++)
		if (prevLocs[i] == check)
			return true;
	return false;	
}

// Parameters: s - an int in string form
// Returns: a string in int form
int stringToInt(string s) {
    std::stringstream ss(s);
    int x;
    ss >> x;
    return x;
}
