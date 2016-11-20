/*
	Author:		Garry Griggs & Gregory Hughes
	Date:		9/11/2016
	Project:	ReflexAgent
	FileName:	PerceptRec.h
	Purpose:	This file contains the definitions for all percepts for the vBot
*/

#ifndef PERCEPTREC_H
#define PERCEPTREC_H

const int REWARD_FOR_PONY = 10;
const int REWARD_FOR_ESCAPE = 15;
const int REWARD_FOR_MOVE = 2;
const int REWARD_FOR_TROLL = 15;

// Record to hold the percepts on a location
struct PerceptRec
{
	int touch;		// Predicates if the agent has touched furniture, 1 if yes, 0 otherwise
	int dUnder;		// Predicates if the agent is on top of dirt, 1 if yes, 0 otherwise
	int dNorth;		// Predicates if the agent is south of dirt, 1 if yes, 0 otherwise
	int dSouth;		// Predicates if the agent is north of dirt, 1 if yes, 0 otherwise
	int dWest;		// Predicates if the agent is east of dirt, 1 if yes, 0 otherwise
	int dEast;		// Predicates if the agent is west of dirt, 1 if yes, 0 otherwise
	int gUnder;		// Predicates if the agent in on top of the goal tile, 1 if yes, 0 otherwise
	int gNorth;		// Predicates if the agent is south of the goal tile, 1 if yes, 0 otherwise
	int gSouth;		// Predicates if the agent is in north of the goal, 1 if yes, 0 otherwise
	int gWest;		// Predicates if the agent is east of the goal, 1 if yes, 0 otherwise
	int gEast;		// Predicates if the agent is west of the goal, 1 if yes, 0 otherwise

};

#endif // !PERCEPTREC_H
