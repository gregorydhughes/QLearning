/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	TileRec.h
	Purpose:	This file contains the definitions for all percepts for the vBot
*/

#ifndef PERCEPTREC_H
#define PERCEPTREC_H

const int REWARD_FOR_PONY = 10;
const int REWARD_FOR_ESCAPE = 15;
const int REWARD_FOR_MOVE = 2;
const int REWARD_FOR_TROLL = 15;

// Record to hold the percepts on a location
struct TileRec
{
	bool hasPony;				// Predicates if a tile has a pony on location
	bool hasTroll;				// Predicates if a tile has a troll on location
	bool isEscape;				// Predicates if the location is a goal location
	bool isValid;				// Predicates if a location is valid
	bool isObstructed;			// Predicates if the location is obstructed
	//bool currLoc;				// Predicates if location is the current location of the burglar

};

#endif // !PERCEPTREC_H
