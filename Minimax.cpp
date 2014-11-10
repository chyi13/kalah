#include "Minimax.h"


Minimax::Minimax(void)
{
}

Minimax::~Minimax(void)
{
}

int Minimax::move(int* tPos,int start,int length,int forbid,int allow)
{
	int returnState = 0;

	int it = 1;
	int tempPosition = (start + it)%14; 

	tPos[start] = 0;

	while(it<=length)					// put seed in every house
	{
		tempPosition = tempPosition%14;
		if (tempPosition!= forbid)
		{
			tPos[tempPosition]++;
			it++;
		}
		tempPosition++;
	}	
												// calculate bounus state
	tempPosition = (tempPosition-1)%14;			// tempPosition is last position 

	if (tempPosition == allow)					// if the last seed is in the house return 1
		returnState = 1;
	if (allow == 7 && tempPosition>=1 && tempPosition<=6)
	{
		if (tPos[tempPosition] == 1 && tPos[14-tempPosition] != 0)			// South Move if the last seed is in an empty house
		{
			tPos[allow] = tPos[allow] + 1 + tPos[14-tempPosition];
			tPos[tempPosition] = 0;
			tPos[14-tempPosition] = 0;
		//	returnState = 2;					// return 2
		}
	}
	if (allow == 0 && tempPosition>=8 && tempPosition<=13)
	{
		if (tPos[tempPosition] == 1 && tPos[14-tempPosition] != 0)			// North Move if the last seed is in an empty house 
		{
			tPos[allow] = tPos[allow] + 1 + tPos[14-tempPosition];
			tPos[tempPosition] = 0;
			tPos[14-tempPosition] = 0;
		//	returnState = 3;					// return 3
		}
	}

	// check if reach an end that either side has no seeds
	bool checkSeedSouth = false;
	for (it = 1;it<=6;it++)
	{
		if (tPos[it]!=0)
			checkSeedSouth = true;
	}
	if (!checkSeedSouth) 	returnState = 4;			// return 4

	bool checkSeedNorth = false;
	for (it = 8;it<=13;it++)
	{
		if (tPos[it]!=0)
			checkSeedNorth  = true;
	}
	if (!checkSeedNorth ) 	returnState = 5;			// return 5

	if  (!checkSeedSouth || !checkSeedNorth)
	{
		for (it = 1;it<=6;it++)
		{
			tPos[7]+=tPos[it];
			tPos[it] = 0;
		}

		for (it = 8;it<=13;it++)
		{
			tPos[0]+=tPos[it];
			tPos[it] = 0;
		}
	}

	return returnState;							//else return 0;
}
int Minimax::evaluate(Node* tempFather,Node* temp)		// compare the house difference bewteen father and temp
{
	int value;
	if (tempFather->nextturn == SOUTH)
		value = temp->pos[7] - temp->pos[0];	// max +
	else
		value = temp->pos[7] - temp->pos[0];	// min -
	
	temp->value = value;
	
	return value;
}
void Minimax::minimax(Node* root,int depth,int low, int high)
{
	if (root->pos[0]>=SEED_NUMBER*6||root->pos[7]>=SEED_NUMBER*6)
	{
		if (root->pos[0] > root->pos[7])
			root->value = -100;
		else
			root->value = 100;
	}
	else
	{
		if (depth == MAX_DEPTH)
			evaluate(root->father,root);
		else
		{
			int position;

			if (root->nextturn == SOUTH)					// south turn == MAX
			{
				int bestScore = -1;
				int bestMove = -1;
				int tempCount = 0;
				for(position = 1;position<=6 && bestScore>low;position++,tempCount++)	// position = 1,2,3,4,5,6
				{
					if (root->pos[position] != 0)			// if pos has no seed ,skip
					{
						Node* tempNode = new Node;			// new node
						tempNode->father = root;
						root->child[tempCount] = tempNode;
						memcpy(tempNode->pos,root->pos,sizeof(int)*14);

						int tempNextTurn = move(tempNode->pos,position,root->pos[position],0,7);
						if (tempNextTurn == 1)
						{
							tempNode->nextturn = SOUTH;
						}
						if (tempNextTurn == 0)
							tempNode->nextturn = NORTH;

						int tempBestScore =0;
						if (tempNextTurn == 4 || tempNextTurn == 5)
							minimax(tempNode,MAX_DEPTH,low,high);	// recursive1
						else
							minimax(tempNode,depth+1,low,high);	// recursive2

						if (tempNode->value >bestScore)				// record best score and best move
						{
							bestScore = tempNode->value;			//
							bestMove = position;

							if (bestScore > high)
								high = bestScore;
						}
					}
				}
				root->value = bestScore;
				root->nextmove = bestMove;
			}
			else											// north turn == MIN
			{
				int bestScore = 1000;
				int bestMove = -1;
				int tempCount = 0;
				for (position = 8;position<=13 && bestScore <high;position++,tempCount++)	// position = 8,9,10,11,12,13
				{
					if (root->pos[position]!=0)
					{
						Node* tempNode = new Node;
						tempNode->father = root;
						root->child[tempCount] = tempNode;
						memcpy(tempNode->pos,root->pos,sizeof(int)*14);

						int tempNextTurn = move(tempNode->pos,position,root->pos[position],7,0);
						if (tempNextTurn == 1)
							tempNode->nextturn = NORTH;
						if (tempNextTurn == 0)
							tempNode->nextturn = SOUTH;

						int tempBestScore =0;
						if (tempNextTurn == 4 || tempNextTurn == 5)
							minimax(tempNode,MAX_DEPTH,low,high);	// recursive1
						else
							minimax(tempNode,depth+1,low,high);	// recursive2
						if (tempNode->value< bestScore)
						{
							bestScore = tempNode->value;
							bestMove = position;
							if (bestScore < low)
								low = bestScore;
						}
					}
				}
				root->value = bestScore;
				root->nextmove = bestMove;
			}
		}
	}
}