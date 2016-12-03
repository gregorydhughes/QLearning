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
	double rNorth;				// Q-Value for moving North
	double rNorthEast;			// Q-Value for moving North East
	double rEast;				// Q-Value for moving East
	double rSouthEast;			// Q-Value for moving South East
	double rSouth;				// Q-Value for moving South
	double rSouthWest;			// Q-Value for moving South West
	double rWest;				// Q-Value for moving rWest
	double rNorthWest;			// Q-Value for moving North West

	double qNorth;				// Q-Value for moving North
	double qNorthEast;			// Q-Value for moving North East
	double qEast;				// Q-Value for moving East
	double qSouthEast;			// Q-Value for moving South East
	double qSouth;				// Q-Value for moving South
	double qSouthWest;			// Q-Value for moving South West
	double qWest;				// Q-Value for moving rWest
	double qNorthWest;			// Q-Value for moving North West
};

#endif // !TILEREC_H