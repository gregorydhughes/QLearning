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
struct RewardsRec
{
	double rNorth;				// Reward Value for moving North
	double rNorthEast;			// Reward Value for moving North East
	double rEast;				// Reward Value for moving East
	double rSouthEast;			// Reward Value for moving South East
	double rSouth;				// Reward Value for moving South
	double rSouthWest;			// Reward Value for moving South West
	double rWest;				// Reward Value for moving rWest
	double rNorthWest;			// Reward Value for moving North West
};


struct QValueRec
{
	// The curret Q Values
	double QNorth;				
	double QNorthEast;			
	double QEast;				
	double QSouthEast;			
	double QSouth;				
	double QSouthWest;			
	double QWest;				
	double QNorthWest;
	// The weight determiner
	double weigthNorth;
	double weigthNorthEast;
	double weigthEast;
	double weigthSouthEast;
	double weigthSouth;
	double weigthSouthWest;
	double weigthWest;
	double weigthNorthWest;
};

#endif // !TILEREC_H