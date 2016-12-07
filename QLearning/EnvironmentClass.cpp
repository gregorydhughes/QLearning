/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	EnvironmentClass.cpp
	Purpose:	This file contains the implementation for the EnvironmentClass Object
*/

#include "EnvironmentClass.h"

// DC - Default constuctor
EnvironmentClass::EnvironmentClass()
{
	environmentSize = MAX_ROOM_SIZE;
	BuildRoom(MAX_ROOM_SIZE);
}

// NDC - Non-Default Constructor
EnvironmentClass::EnvironmentClass(int size, double a, double g)
{
	environmentSize = size;
	BuildRoom(size);
}

// Fills the 2-d array of the room for both the DC and the NDC
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

// Returns the percepts to the vBot on location
TileRec EnvironmentClass::GetLocationInformation(LocRec loc)
{
	return room[loc.rowY][loc.colX];
}

// Returns the value for a move on location
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

// Predicates if there is a pony on location
bool EnvironmentClass::HasPony(LocRec loc)
{
	if (room[loc.rowY][loc.colX].hasPony)
		return true;
	return false;
}

// Predicates if there is a troll on location
bool EnvironmentClass::HasTroll(LocRec loc)
{
	if (room[loc.rowY][loc.colX].hasTroll)
		return true;
	return false;
}

// vacuums a tile
void EnvironmentClass::FreePony(LocRec loc)
{
	room[loc.rowY][loc.colX].hasPony = false;
}

// Sets a tile's dirt value
void EnvironmentClass::SetPonyOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].hasPony = true;
}

// Sets a tile's furniture value
void EnvironmentClass::SetTrollOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].hasTroll = true;
}

// Sets a tile's goal value
void EnvironmentClass::SetEscapeOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].isEscape = true;
}

// Sets a tile's Obstruction value
void EnvironmentClass::SetObstructionOnLocation(LocRec loc)
{
	room[loc.rowY][loc.colX].isObstructed = true;
}

// Checks if location is valid
bool EnvironmentClass::IsTileValid(LocRec loc)
{
	return room[loc.rowY][loc.colX].isValid;
}

// Returns the size (length and width) of the room
int EnvironmentClass::GetRoomSize()
{
	return environmentSize;
}

// check reward value of location
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

// Creates a list of neighboring rewards
RewardsRec EnvironmentClass::ReturnNeighboringQValues(LocRec loc)
{
	RewardsRec ans;

	// check rNorth
	LocRec rNorth, rNorthEast, rNorthWest, rEast, rWest, rSouthEast, rSouthWest, rSouth;

	rNorth.colX = loc.colX;
	rNorth.rowY = loc.rowY + 1;

	rNorthEast.rowY = loc.rowY + 1;
	rNorthEast.colX = loc.colX + 1;

	rEast.colX = loc.colX + 1;
	rEast.rowY = loc.rowY;

	rSouthEast.colX = loc.colX + 1;
	rSouthEast.rowY = loc.rowY - 1;

	rSouth.colX = loc.colX;
	rSouth.rowY = loc.rowY - 1;

	rSouthWest.rowY = loc.rowY - 1;
	rSouthWest.colX = loc.colX - 1;

	rWest.colX = loc.colX - 1;
	rWest.rowY = loc.rowY;
	
	rNorthWest.rowY = loc.rowY + 1;
	rNorthWest.colX = loc.colX - 1;
	
	ans.rNorth = ReturnReward(rNorth);
	ans.rNorthEast = ReturnReward(rNorthEast);
	ans.rEast = ReturnReward(rEast);
	ans.rSouthEast = ReturnReward(rSouthEast);
	ans.rSouth = ReturnReward(rSouth);
	ans.rSouthWest = ReturnReward(rSouthWest);
	ans.rWest = ReturnReward(rWest);
	ans.rNorthWest = ReturnReward(rNorthWest);

	return ans;
}

// Prints board with the burgalars location
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

// Prints board with the burgalars path
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

// Returns a string representation of the northern and southern walls.
std::string EnvironmentClass::GetTopAndBottomWalls()
{
	std::string ans = "";

	for (int i = 0; i < environmentSize; i++)
		ans = ans + "+-";

	ans = ans + "+";

	return ans;
}

// Predicates whether pos is in the path
bool EnvironmentClass::LocationInVector(LocRec pos, vector<LocRec> path)
{
	for (int i = 0; i < path.size(); i++)
		if (pos == path[i])
			return true;

	return false;
}
