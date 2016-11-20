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

PerceptRec EnvironmentClass::GetLocationInformation(LocRec loc)
{
	PerceptRec ans;

	// Check Touch
	if (room[loc.row][loc.col].hasTroll || !room[loc.row][loc.col].isValid)
		ans.touch = 1;
	else
		ans.touch = 0;

	// Check under
	if (room[loc.row][loc.col].hasPony)
		ans.dUnder = 1;
	else
		ans.dUnder = 0;

	if (room[loc.row][loc.col].isEscape)
		ans.gUnder = 1;
	else
		ans.gUnder = 0;

	// Check front
	if (room[loc.row + 1][loc.col].hasPony)
		ans.dNorth = 1;
	else
		ans.dNorth = 0;

	if (room[loc.row + 1][loc.col].isEscape)
		ans.gNorth = 1;
	else
		ans.gNorth = 0;

	// Check Back
	if (room[loc.row - 1][loc.col].hasPony)
		ans.dSouth = 1;
	else
		ans.dSouth = 0;

	if (room[loc.row - 1][loc.col].isEscape)
		ans.gSouth = 1;
	else
		ans.gSouth = 0;

	// Check Left
	if (room[loc.row][loc.col - 1].hasPony)
		ans.dWest = 1;
	else
		ans.dWest = 0;

	if (room[loc.row][loc.col - 1].isEscape)
		ans.gWest = 1;
	else
		ans.gWest = 0;

	// Check Right
	if (room[loc.row][loc.col + 1].hasPony)
		ans.dEast = 1;
	else
		ans.dEast = 0;

	if (room[loc.row][loc.col + 1].isEscape)
		ans.gEast = 1;
	else
		ans.gEast = 0;

	return ans;
}

void EnvironmentClass::FreePony(LocRec loc)
{
	room[loc.row][loc.col].hasPony = false;
}

void EnvironmentClass::SetPonyOnLocation(LocRec loc)
{
	room[loc.row][loc.col].hasPony = true;
}

void EnvironmentClass::SetTrollOnLocation(LocRec loc)
{
	room[loc.row][loc.col].hasTroll = true;
}

void EnvironmentClass::SetEscapeOnLocation(LocRec loc)
{
	room[loc.row][loc.col].isEscape = true;
}

void EnvironmentClass::SetObstructionOnLocation(LocRec loc)
{
	room[loc.row][loc.col].isObstructed = true;
}

int EnvironmentClass::GetRoomSize()
{
	return environmentSize;
}

int EnvironmentClass::ReturnReward(LocRec loc)
{
	if (room[loc.row][loc.col].hasPony)
		return REWARD_FOR_PONY;
	else if (room[loc.row][loc.col].hasTroll)
		return REWARD_FOR_TROLL;
	else if (room[loc.row][loc.col].isEscape)
		return REWARD_FOR_ESCAPE;
	else
		return REWARD_FOR_MOVE;
}

std::string EnvironmentClass::GetRoomString(LocRec currLoc, Direction dir)
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
				ans = ans + "#";

			else if (room[i][j].hasTroll)
				ans = ans + "X";

			else if (room[i][j].isEscape)
				ans = ans + "$";

			else if (currLoc.row == i && currLoc.col == j)
			{
				switch (dir)
				{
				case TRUE_NORTH:
				case TRUE_SOUTH:
				case TRUE_EAST:
				case TRUE_WEST:
				case NORTH_EAST:
				case NORTH_WEST:
				case SOUTH_EAST:
				case SOUTH_WEST:
				default:
					ans = ans + "B";
					break;
				}

			}
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