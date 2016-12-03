/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	EnvironmentClass.cpp
	Purpose:	This file contains the implementation for the EnvironmentClass Object
*/

#include "EnvironmentClass.h"


EnvironmentClass::EnvironmentClass()
{
	environmentSize = MAX_ROOM_SIZE;
	BuildRoom(MAX_ROOM_SIZE);
}

EnvironmentClass::EnvironmentClass(int size, double a, double g)
{
	environmentSize = size;
	BuildRoom(size);
}

void EnvironmentClass::BuildRoom(int size)
{
	for (int i = 0; i < ROOM_SIZE_WITH_BORDERS; i++)
	{
		for (int j = 0; j < ROOM_SIZE_WITH_BORDERS; j++)
		{
			room[i][j].isValid = false;
			room[i][j].hasPony = false;
			room[i][j].hasTroll = false;
			room[i][j].isEscape = false;
			room[i][j].isObstructed = false;
		}
	}

	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			room[i][j].isValid = true;
		}
	}
}

TileRec EnvironmentClass::GetLocationInformation(LocRec loc)
{
	return room[loc.rowY][loc.colX];
}

int EnvironmentClass::GetValueOnLocation(LocRec loc)
{
	if (room[loc.rowY][loc.colX].hasPony)
		return REWARD_FOR_PONY;
	else if (room[loc.rowY][loc.colX].hasTroll)
		return REWARD_FOR_TROLL;
	else if (room[loc.rowY][loc.colX].isEscape)
		return REWARD_FOR_ESCAPE;
	else
		return REWARD_FOR_MOVE;
}

void EnvironmentClass::FreePony(LocRec loc)
{
	room[loc.rowY][loc.colX].hasPony = false;
}

void EnvironmentClass::SetPonyOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].hasPony = true;
}

void EnvironmentClass::SetTrollOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].hasTroll = true;
}

void EnvironmentClass::SetEscapeOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].isEscape = true;
}

void EnvironmentClass::SetObstructionOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].isObstructed = true;
}

bool EnvironmentClass::IsTileValid(LocRec loc)
{
	return room[loc.rowY][loc.colX].isValid;
}

int EnvironmentClass::GetRoomSize()
{
	return environmentSize;
}

int EnvironmentClass::ReturnReward(LocRec loc)
{
	if (room[loc.rowY][loc.colX].hasPony)
		return REWARD_FOR_PONY;
	else if (room[loc.rowY][loc.colX].hasTroll)
		return REWARD_FOR_TROLL;
	else if (room[loc.rowY][loc.colX].isEscape)
		return REWARD_FOR_ESCAPE;
	else
		return REWARD_FOR_MOVE;
}

QValRec EnvironmentClass::ReturnNeighboringQValues(LocRec loc)
{
	QValRec ans;

	// check north
	LocRec north, northEast, northWest, east, west, southEast, southWest, south;

	north.colX = loc.colX;
	north.rowY = loc.rowY + 1;

	northEast.rowY = loc.rowY + 1;
	northEast.colX = loc.colX + 1;

	east.colX = loc.colX + 1;
	east.rowY = loc.rowY;

	southEast.colX = loc.colX + 1;
	southEast.rowY = loc.rowY - 1;

	south.colX = loc.colX;
	south.rowY = loc.rowY - 1;

	southWest.rowY = loc.rowY - 1;
	southWest.colX = loc.colX - 1;

	west.colX = loc.colX - 1;
	west.rowY = loc.rowY;
	
	northWest.rowY = loc.rowY + 1;
	northWest.colX = loc.colX - 1;
	
	ans.north = ReturnReward(north);
	ans.northEast = ReturnReward(northEast);
	ans.east = ReturnReward(east);
	ans.southEast = ReturnReward(southEast);
	ans.south = ReturnReward(south);
	ans.southWest = ReturnReward(southWest);
	ans.west = ReturnReward(west);

	return ans;
}

std::string EnvironmentClass::ToString(LocRec currLoc)
{
	std::string ans = "";

	// Print top wall
	ans += GetTopAndBottomWalls()+ "\n";

	for (int i = environmentSize; i > 0; i--)
	{
		ans += "|";

		for (int j = 1; j <= environmentSize; j++)
		{
			if (room[i][j].hasPony)
				ans = ans + "P";

			else if (room[i][j].hasTroll)
				ans = ans + "T";

			else if (room[i][j].isEscape)
				ans = ans + "E";

			else if (currLoc.rowY == i && currLoc.colX == j)
					ans = ans + "B";

			else
				ans = ans + " ";

			// add space after each
			if (j != environmentSize)
				ans = ans + " ";
		}

		ans = ans + "|\n";

		if (i != 1)
		{
			for (int j = 0; j <= environmentSize; j++)
			{
				ans = ans + "+";

				if (j != environmentSize)
					ans = ans + " ";
			}

			ans = ans + "\n";
		}
	}

	// Print bottom wall
	ans = ans + GetTopAndBottomWalls() + "\n";

	return ans;
}

std::string EnvironmentClass::ToString(vector<LocRec> path)
{
	std::string ans = "";

	// Print top wall
	ans += GetTopAndBottomWalls() + "\n";

	for (int i = environmentSize; i > 0; i--)
	{
		ans += "|";

		for (int j = 1; j <= environmentSize; j++)
		{
			LocRec temp;

			temp.colX = j;
			temp.rowY = i;

			if (LocationInVector(temp, path))
				ans = ans + "X";
		
			else if (room[i][j].hasPony)
				ans = ans + "P";

			else if (room[i][j].hasTroll)
				ans = ans + "T";

			else if (room[i][j].isEscape)
				ans = ans + "E";

			else
				ans = ans + " ";

			// add space after each
			if (j != environmentSize)
				ans = ans + " ";
		}

		ans = ans + "|\n";

		if (i != 1)
		{
			for (int j = 0; j <= environmentSize; j++)
			{
				ans = ans + "+";

				if (j != environmentSize)
					ans = ans + " ";
			}

			ans = ans + "\n";
		}
	}

	// Print bottom wall
	ans = ans + GetTopAndBottomWalls() + "\n";

	return ans;
}

std::string EnvironmentClass::GetTopAndBottomWalls()
{
	std::string ans = "";

	for (int i = 0; i < environmentSize; i++)
		ans = ans + "+-";

	ans = ans + "+";

	return ans;
}

bool EnvironmentClass::LocationInVector(LocRec pos, vector<LocRec> path)
{
	for (int i = 0; i < path.size(); i++)
		if (pos == path[i])
			return true;

	return false;
}