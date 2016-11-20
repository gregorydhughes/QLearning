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
struct TileDetails
{
	bool hasPony;				// Predicates if a tile has a pony on location
	bool hasTroll;				// Predicates if a tile has a troll on location
	bool isEscape;				// Predicates if the location is a goal location
	bool isValid;				// Predicates if a location is valid
	bool isObstructed;			// Predicates if the location is obstructed
	bool currLoc;				// Predicates if location is the current location of the burglar
};

#endif // !TILEREC_H