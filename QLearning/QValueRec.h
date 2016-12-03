/*
	Author:		Garry Griggs & Greg Hughes
	Date:		September 11th, 2016
	Project:	Reflex Agent
	Filename:	TileRec.h
	Purpose:	This file contains all the details regarding vBot's room down to each tile
*/

#ifndef TILEREC_H
#define TILEREC_H

// Details pertaining to each of the tiles in the room
struct QValRec
{
	double north;				// Q-Value for moving North
	double northEast;			// Q-Value for moving North East
	double east;				// Q-Value for moving East
	double southEast;			// Q-Value for moving South East
	double south;				// Q-Value for moving South
	double southWest;			// Q-Value for moving South West
	double west;				// Q-Value for moving west
	double northWest;			// Q-Value for moving North West
};

#endif // !TILEREC_H