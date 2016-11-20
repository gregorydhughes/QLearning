/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	LocRec.h
	Purpose:	This file contains the definitions for each of the locations and directions within the room
*/

#ifndef LOCREC_H
#define LOCREC_H

const int MAX_DIRECTIONS = 8;

// establishes the direction of the vBot
enum Direction
{
	TRUE_NORTH,		// vBot is looking North
	TRUE_SOUTH,		// vBot is looking South
	TRUE_EAST,		// vBot is looking East
	TRUE_WEST,		// vBot is looking West
	NORTH_EAST,		// vBot is looking North East
	NORTH_WEST,		// vBot is looking North West
	SOUTH_EAST,		// vBot is looking South East
	SOUTH_WEST		// vBot is looking South West
};

// Structure to hold a 2-d location in room(2d array)
struct LocRec
{
	int rowY;		// Row location
	int colX;		// Col location

	bool operator==(const LocRec & rtOp) const
	{
		if (rowY == rtOp.rowY && colX == rtOp.colX)
			return true;

		return false;
	}
};

#endif // !LOCREC_H
