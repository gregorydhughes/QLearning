/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	EnvironmentClass.h
	Purpose:	This file contains the definitions for the EnvironmentClass Object
*/

#include "LocRec.h"
#include "TileRec.h"
#include "QValueRec.h"
#include "ExceptionClass.h"
#include <string>
#include <vector>

const int MAX_ROOM_SIZE = 20;	// The Maximum size of the room
const int BORDER_SIZE = 1;		// The size of the border
								
// The Maximum size of the room with borders
const int ROOM_SIZE_WITH_BORDERS = MAX_ROOM_SIZE + BORDER_SIZE + BORDER_SIZE;

using namespace std;


class EnvironmentClass
{
public:
	// DC - Default constuctor
	EnvironmentClass();
	
	// NDC - Non-Default Constructor
	EnvironmentClass(int size, double a, double g);

	// Returns the percepts to the vBot on location
	TileRec GetLocationInformation(LocRec loc);

	// Returns the value for a move on location
	int GetValueOnLocation(LocRec loc);

	// Predicates if there is a pony on location
	bool HasPony(LocRec loc);

	// Predicates if there is a troll on location
	bool HasTroll(LocRec loc);

	// vacuums a tile
	void FreePony(LocRec loc);

	// Sets a tile's dirt value
	void SetPonyOnLocation(LocRec loc);

	// Sets a tile's furniture value
	void SetTrollOnLocation(LocRec loc);

	// Sets a tile's goal value
	void SetEscapeOnLocation(LocRec loc);

	// Sets a tile's Obstruction value
	void SetObstructionOnLocation(LocRec loc);

	// Checks if location is valid
	bool IsTileValid(LocRec loc);

	// Returns the size (length and width) of the room
	int GetRoomSize();
	
	// check reward value of location
	int ReturnReward(LocRec loc);

	// Creates a list of neighboring rewards
	RewardsRec ReturnNeighboringQValues(LocRec loc);

	// Prints board with the burgalars locaiton
	std::string ToString(LocRec currLoc);

	// Prints board with the burgalars path
	std::string ToString(vector<LocRec> path);

private:
	// Fills the 2-d array of the room for both the DC and the NDC
	void BuildRoom(int size);

	// Returns a string representation of the northern and southern walls.
	std::string GetTopAndBottomWalls();

	// Predicates whether pos is in the path
	bool LocationInVector(LocRec pos, vector<LocRec> path);
	
	// The Maximum room size as a record
	typedef TileRec RoomRec[ROOM_SIZE_WITH_BORDERS][ROOM_SIZE_WITH_BORDERS];
	
	int environmentSize;	// The lenght and width of the Environment
	RoomRec room;			// The room
	double alpha;			// Learning rate
	double gamma;			// Discount factor
};


