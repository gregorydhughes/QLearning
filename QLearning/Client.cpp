#include <iostream>
#include <fstream>
#include <string>
#include "EnvironmentClass.h"

using namespace std;

const double ALPHA = 0.75;
const double GAMMA = 0.5;
const int EPOCHS = 10000;

const int MAX_LOCATIONS = 400;

const string INPUT_FILE = "input.dat";

void ReadFile(EnvironmentClass & ec);

int main()
{
	EnvironmentClass ec;

	ReadFile(ec);



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
	din >> temp.col >> temp.row;

	ec = EnvironmentClass(n, ALPHA, GAMMA);

	ec.SetEscapeOnLocation(temp);


	// get line three from file
	for (int i = 0; i < p; i++)
	{
		din >> temp.col >> temp.row;

		ec.SetPonyOnLocation(temp);
	}


	//get line 4 from file
	string tempStr = din.getline;

	int obstructionArr [MAX_LOCATIONS];

	string num = "";

	int numbersFound = 0;

	for (int i = 0; i < tempStr.length; i++)
	{
		if (tempStr[i] == ' ')
		{
			numbersFound++;

			if (numbersFound % 2 == 0)
			{
				temp.row = stoi(num);

				if (temp.row != -1 && temp.col != -1)
					ec.SetObstructionOnLocation(temp);
			}
			else
				temp.col = stoi(num);

			num = "";
		}
		else
			num += tempStr[i];
	}


	// get line 5 from file
	for (int i = 0; i < t; i++)
	{
		din >> temp.col >> temp.row;

		ec.SetTrollOnLocation(temp);
	}
	
}