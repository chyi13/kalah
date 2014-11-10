#pragma once
#ifndef KALAH_HEAD
#define KALAH_HEAD

#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/GLAUX.H>		// Header File For The Glaux Library

#include <list>
#include <time.h>
#include "Minimax.h"

#define SEED_PER_HOUSE 3

struct Seed
{
	int position;
	float color[3];

};
struct StoreHouse
{
	std::list<Seed> seeds;
	float pX;
	float pY;
};

class Kalah
{
public:
	Kalah(void);
	~Kalah(void);

	void initResource();

	void initSeeds();

	void render();

	void processClick(float x,float y,bool& mCS, bool single);		// single mark indicates play with AI or true oppo

private:
	void renderBoard();

	void renderHouse();
	
	void renderOneHouse(int id);				// render specific id house

	int moveSeeds(int tempMoveID, int forbid, int allow);	// move seed as an operation ,return next state

	//////////////////////////////////////////////
	void getBoard();							// change seeds node to a simple array
	void processAI();

	//////////////////////////////////////////////
	void renderResult();
private:
	int gameState;

	GLuint texture[6]; 

	StoreHouse sh[14];

	int mouseHouseID;	// record which house is current mouse position

	float houseCenter[14][2];

	bool mouseClickState;

	//////////////////////////////
	Minimax ai;
	int board[14];
};

#endif