#pragma once
#ifndef MINIMAX_H
#define MINIMAX_H

#include<vector>

#define SOUTH 0
#define NORTH 1
#define MAX_DEPTH 9
#define SEED_NUMBER 3
struct Node
{
	int pos[14];				// board 0-14
	Node* father;
	Node* child[6];
	int value;					// evaluate value
	int nextturn;				// next turn South or North
	int nextmove;				// next move position
};


class Minimax
{
public:
	Minimax(void);
	~Minimax(void);
	int nextMove;
public:
	int move(int* tPos,int start,int length,int forbid,int allow);
	int evaluate(Node* tempFather,Node* temp);
	void minimax(Node* root,int depth,int low,int high);
};
#endif

