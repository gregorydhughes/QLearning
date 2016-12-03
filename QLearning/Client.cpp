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

	for (int i = 0; i < EPOCHS; i++)
	{
		int reward = 0;

		vector<LocRec>::iterator it = path.begin();

		currLoc = EstablishStartingLocation(ec);

		while (ec.GetLocationInformation(currLoc).isEscape == false)
		{
			//dout << ec.ToString(currLoc);
			MoveCurrentLocation(ec, currLoc, path, it);
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

void MoveCurrentLocation(EnvironmentClass & ec, LocRec & curr, vector<LocRec> & path, vector<LocRec>::iterator & it)
{
	path.push_back(curr);

	LocRec temp = curr;

	do
	{
		temp = curr;

		QValRec currValues = ec.ReturnNeighboringQValues(temp);


		
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

void calculateQLearnValues(QValRec currValues) {
	currValues.north = calculateQLearnValue(currValues.north);
	currValues.south = calculateQLearnValue(currValues.south);
	currValues.east = calculateQLearnValue(currValues.east);
	currValues.west = calculateQLearnValue(currValues.west);
	currValues.southEast = calculateQLearnValue(currValues.southEast);
	currValues.southWest = calculateQLearnValue(currValues.southWest);
	currValues.norhtEast = calculateQLearnValue(currValues.norhtEast);
	currValues.northWest = calculateQLearnValue(currValues.northWest);
}

double calculateQLearnValue(double dir) {
	


}