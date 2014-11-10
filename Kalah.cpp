#include "Kalah.h"

GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
//	GLfloat mat_diffuse[] = { 0.8, 0.5, 0.8, 1.0 };
GLfloat mat_diffuse1[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat mat_diffuse2[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

Kalah::Kalah(void)
{
	mouseHouseID = -1;

	//
	mouseClickState = false;

	// initial house center
	float currentHouseCenter[14][2] ={{-3.310, 0.505}, // store 0
									  {-2.450,-1.235},
									  {-1.490,-1.235},
									  {-0.517,-1.235},
									  { 0.467,-1.235},
									  { 1.435,-1.235},
									  { 2.405,-1.235},
									  { 3.330,-0.515},	   // store 7
									  { 2.405, 1.275},
									  { 1.435, 1.275},
									  { 0.467, 1.275},
									  {-0.517, 1.275},
									  {-1.490, 1.275},
									  {-2.450, 1.275}};
	memcpy(houseCenter,currentHouseCenter,14*2*sizeof(float));

	gameState = 1;			// start
}


Kalah::~Kalah(void)
{
}


AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		printf("read image success!\n");
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}


void Kalah::initResource()
{
	bool status = false;

	AUX_RGBImageRec *textureImage[6];
	memset(textureImage,0,sizeof(void*)*1);

	if ((textureImage[0]=LoadBMP("data/green_marble_background.bmp")) && (textureImage[1]=LoadBMP("data/white_seed.bmp")) 
		&& (textureImage[2] = LoadBMP("data/background.bmp")) && (textureImage[3] = LoadBMP("data/Font.bmp"))
		&& (textureImage[4] = LoadBMP("data/win.bmp")) && (textureImage[5] =LoadBMP("data/lose.bmp")))
	{
		status = true;
		int i;
		glGenTextures(6,&texture[0]);

		for (i = 0; i<6; i++)
		{
			glBindTexture(GL_TEXTURE_2D, texture[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImage[i]->sizeX, textureImage[i]->sizeY, 0, 
				GL_RGB, GL_UNSIGNED_BYTE, textureImage[i]->data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		}

		for (i = 0; i<6; i++)
		{
			if (textureImage[i])
			{	
				if (textureImage[i]->data)
				{
					free(textureImage[i]);
				}
			}
		}
	}
	//return status;
}

void Kalah::initSeeds()
{
	srand(time(NULL));

	float centers[14][2]={{-3.53,-1.05},		//house
						  {-2.70,-0.5},
						  {-1.70,-0.5},
						  {-0.70,-0.5},
						  { 0.30,-0.5},
						  { 1.25,-0.5},
						  { 2.20,-0.5},
						  { 3.13,1.07},			//house
						  { 2.20, 0.5},
						  { 1.25, 0.5},
						  { 0.30, 0.5},
						  {-0.70, 0.5},
						  {-1.70, 0.5},
						  {-2.70, 0.5}};
	int i;
	// initial 14 houses or stores
	// add 3 seeds to every stores
	for (i = 0; i<14; i++)
	{
		sh[i].pX = centers[i][0];
		sh[i].pY = centers[i][1];

		if (i!=0 && i!=7)
		{
			Seed tempSeed[SEED_PER_HOUSE];
			int j;
			for (j = 0; j<SEED_PER_HOUSE; j++)
			{
				tempSeed[j].color[0] = rand()%256/255.0;
				tempSeed[j].color[1] = rand()%256/255.0;
				tempSeed[j].color[2] = rand()%256/255.0;

				tempSeed[j].position = i;

				sh[i].seeds.push_back(tempSeed[j]);
			}

		}
	}

}

void Kalah::render()
{
	renderBoard();
	renderHouse();
	if (gameState == 2 || gameState == 3)
		renderResult();
}
void Kalah::renderBoard()
{
	// draw background
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glNormal3f(0.0,0.0,1.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0,0.0); glVertex3f(-4.0,-3.0,-2.0);
	glTexCoord2d(1.0,0.0); glVertex3f( 4.0,-3.0,-2.0);
	glTexCoord2d(1.0,1.0); glVertex3f( 4.0, 3.0,-2.0);
	glTexCoord2d(0.0,1.0); glVertex3f(-4.0, 3.0,-2.0);
	glEnd();

}

void Kalah::renderOneHouse(int id)
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
	glPushMatrix();
	glTranslatef(houseCenter[id][0],houseCenter[id][1],0.0);
	glBegin(GL_QUADS);
	glVertex3f(-0.5,-1.0,0.0);
	glVertex3f( 0.5,-1.0,0.0);
	glVertex3f( 0.5, 1.0,0.0);
	glVertex3f(-0.5, 1.0,0.0);
	glEnd();
	glPopMatrix();

	int tempSize = sh[id].seeds.size();				// calculate next id
	int it=1;
	int tempPosition = (id+1)%14;
	while(it<=tempSize)
	{
		tempPosition = tempPosition%14;
		if (tempPosition !=0)
		{
			it++;
		}
		tempPosition++;
	}
	int nextID = (tempPosition-1)%14;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);

	if (nextID == 0 || nextID == 7)
	{
		glPushMatrix();
		glTranslatef(houseCenter[nextID][0],houseCenter[nextID][1],0.0);
		glBegin(GL_QUADS);
		glVertex3f(-0.35,-1.73,0.0);
		glVertex3f( 0.35,-1.73,0.0);
		glVertex3f( 0.35, 1.73,0.0);
		glVertex3f(-0.35, 1.73,0.0);
		glEnd();
		glPopMatrix();
	}
	else
	{
		glPushMatrix();

		glTranslatef(houseCenter[nextID][0],houseCenter[nextID][1],0.0);
		glBegin(GL_QUADS);
		glVertex3f(-0.5,-1.0,0.0);
		glVertex3f( 0.5,-1.0,0.0);
		glVertex3f( 0.5, 1.0,0.0);
		glVertex3f(-0.5, 1.0,0.0);
		glEnd();
		glPopMatrix();
	}
}

void Kalah::renderHouse()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glLoadIdentity();
//	printf("houseID %d\n",mouseHouseID);
	// render house is click and the destination house
	if (mouseHouseID != -1)
	{
		// draw the mouse position
		renderOneHouse(mouseHouseID);
	}


	// traverse every house
	int i;
	for (i = 0; i<14; i++)
	{
		float pFactor;

		std::list<Seed>::iterator it;
		it = sh[i].seeds.begin();
		if (i != 0 && i!= 7)		// draw the houses
		{
			pFactor = i>7?1:-1;	// check the house position is up or down

			int count = 0;
			glPushMatrix();
			glTranslatef(sh[i].pX,sh[i].pY,0.0);
			while(it!=sh[i].seeds.end())
			{
				glPushMatrix();
				glTranslatef(0.5*(count%2),pFactor*0.3*(count/2),0.0);

				GLfloat tempColor[] = {it->color[0],it->color[1],it->color[2]};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, tempColor);

				glutSolidSphere(0.1,30,30);
				glPopMatrix();
				count++;	
				it++;
			}
			glPopMatrix();
		}
		else					// draw the 2 stores
		{
			pFactor = i==0?1:-1;
			int count = 0;
			glPushMatrix();
			glTranslatef(sh[i].pX,sh[i].pY,0.0);
			while(it != sh[i].seeds.end())
			{
				glPushMatrix();

				glTranslatef(0.35*(count%2),pFactor*0.3*(count/2),0.0);
				GLfloat tempColor[] = {it->color[0],it->color[1],it->color[2]};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, tempColor);

				glutSolidSphere(0.1,30,30);

				glPopMatrix();
				
				count++;
				it++;
			}
			glPopMatrix();
		}
	}
}
void Kalah::processClick(float x,float y,bool& mCS,bool single)
{
	int checkInHouse = 0;
	// down 4 houses
	if (y<-0.27 && y>-2.20)
	{
		if (x>-2.9&& x<2.9)
		{
			checkInHouse = 1;
			if (x<0.025)
			{
				// case 1
				if (x<-1.97)
				{	
					mouseHouseID = 1;
					
				}
				if (x>=-1.97 && x<-1.01)
				{
					mouseHouseID = 2;
					
				}
				if (x>= -1.01)
				{
					mouseHouseID = 3;
					
				}
			}
			else
			{
				if (x<0.96)
				{
					mouseHouseID = 4;
				
				}
				if (x>=0.96 && x<1.91)
				{
					mouseHouseID = 5;
					
				}
				if (x>=1.91)
				{
					mouseHouseID = 6;
				}
			}
		}
	}

	if (!single)
	{
		// up 4 houses
		if (y> 0.35 && y< 2.20)
		{
			if (x>-2.9 && x<2.9)
			{
				checkInHouse = 1;
				if (x<0.025)
				{
					// case 1
					if (x<-1.97)
					{	
						mouseHouseID = 13;
					}
					if (x>=-1.97 && x<-1.01)
					{
						mouseHouseID = 12;
					}
					if (x>= -1.01)
					{
						mouseHouseID = 11;
					}
				}
				else
				{
					if (x<0.96)
					{
						mouseHouseID = 10;
					}
					if (x>=0.96 && x<1.91)
					{
						mouseHouseID = 9;
					}
					if (x>=1.91)
					{
						mouseHouseID = 8;
					}
				}
			}
		}
	}
	if (!checkInHouse)
		mouseHouseID = -1;
	else
	{
		if (mCS)			//	1. if mouse click then move seeds;
		{	
			int tempStatePlayer = moveSeeds(mouseHouseID,0,7);

			if (tempStatePlayer == 2 )
			{
				gameState = 3;
				printf("you lose!\n");
				return;
			}
			if (tempStatePlayer == 3)
			{
				gameState = 2;
				printf("you win!\n");
				return;
			}

			if (tempStatePlayer == 0)	// check if have another move
			{
				if (single)		//	2. AI's turn
				{
					do
					{
						processAI();
					}
					while( moveSeeds(ai.nextMove,7,0)==1);
				}
			}
		}
	}
	mCS = false;
}

int Kalah::moveSeeds(int tempMoveID, int forbid, int allow)
{
	////////
	// move seed version 2
	int returnState = 0;				// return default state 0

	if (sh[tempMoveID].seeds.size() == 0)
		returnState = -1;				// if there is no seed return -1

	int tempPosition = (tempMoveID+1)%14;
	int length = sh[tempMoveID].seeds.size();
	std::list<Seed>::iterator it = sh[tempMoveID].seeds.begin();

	while(it != sh[tempMoveID].seeds.end())
	{
		tempPosition = tempPosition%14;
		if(tempPosition != forbid)				// skip the opponent's store
		{
			Seed tempSeed = *it;
			sh[tempPosition].seeds.push_back(tempSeed);
			it = sh[tempMoveID].seeds.erase(it);
		}
		tempPosition++;
	}
	//	check if the last seed is in the store
	tempPosition = (tempPosition-1)%14;
	if (tempPosition == allow)
		returnState = 1;				//   return one more step state  1

	// eat South or Player
	if (allow == 7 && tempPosition>=1 && tempPosition<=6)
	{
		if (sh[tempPosition].seeds.size() == 1 && sh[14-tempPosition].seeds.size()!=0)
		{
			// 1. opponent's all seeds (14-tempPosition)
			it = sh[14-tempPosition].seeds.begin();
			while(it != sh[14-tempPosition].seeds.end())
			{
				sh[allow].seeds.push_back(*it);
				it = sh[14-tempPosition].seeds.erase(it);
			}
			// 2. current last seed
			it = sh[tempPosition].seeds.begin();
			sh[7].seeds.push_back(*it);
			sh[tempPosition].seeds.erase(it);
		}
	}

	if (allow == 0 && tempPosition>=8 && tempPosition<=13)
	{
		if (sh[tempPosition].seeds.size() == 1 && sh[14-tempPosition].seeds.size()!=0)
		{
			// 1. opponent's all seeds (14-tempPosition)
			it = sh[14-tempPosition].seeds.begin();
			while(it != sh[14-tempPosition].seeds.end())
			{
				sh[allow].seeds.push_back(*it);
				it = sh[14-tempPosition].seeds.erase(it);
			}
			// 2. current last seed
			it = sh[tempPosition].seeds.begin();
			sh[allow].seeds.push_back(*it);
			sh[tempPosition].seeds.erase(it);
		}
	}
									
	int iit;								// check if reach an end that either side has no seeds
	getBoard();
	bool checkSeedSouth = false;
	for (iit = 1;iit<=6;iit++)
	{
		if (board[iit]!=0)
			checkSeedSouth = true;
	}
	if (!checkSeedSouth) 	returnState = 4;

	
	bool checkSeedNorth = false;
	for (iit = 8;iit<=13;iit++)
	{
		if (board[iit]!=0)
			checkSeedNorth  = true;
	}
	if (!checkSeedNorth ) 	returnState = 5;


	if (!checkSeedSouth || !checkSeedNorth)
	{
		for (iit = 1;iit<=6;iit++)					// eat south
		{
			if (sh[iit].seeds.size()!=0)
			{
				it = sh[iit].seeds.begin();
				while(it!=sh[iit].seeds.end())
				{
					sh[7].seeds.push_back(*it);
					it = sh[iit].seeds.erase(it);
				}
			}
		}
		for (iit = 8;iit<=13;iit++)					// eat north
		{
			if (sh[iit].seeds.size()!=0)
			{
				it = sh[iit].seeds.begin();
				while(it!=sh[iit].seeds.end())
				{
					sh[0].seeds.push_back(*it);
					it = sh[iit].seeds.erase(it);
				}
			}
		}
	}

	////////////////// 
	if (sh[0].seeds.size() >= SEED_NUMBER*6 || sh[7].seeds.size() >= SEED_NUMBER*6)
	{
		if (sh[0].seeds.size() > sh[7].seeds.size())
			returnState = 2;							 // robot or opponent win! 2
		else
			returnState = 3;							 // player win !  3
	}
	glutPostRedisplay();
	return returnState;
}

void Kalah::getBoard()
{
	int i;
	for (i= 0;i<14;i++)
	{
		board[i] = sh[i].seeds.size();
//		printf("%d ",board[i]);
	}

//	printf("\n");
}

void Kalah::processAI()
{
	Node* root = new Node;
	getBoard();
	memcpy(root->pos,board,sizeof(int)*14);

	root->father = NULL;
	root->nextturn = NORTH;
	ai.minimax(root,0,-10000,10000);
	ai.nextMove = root->nextmove;
	printf("ai next move %d\n",ai.nextMove);
}

void Kalah::renderResult()
{
	// draw background
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
	if (gameState == 2)				// win
		glBindTexture(GL_TEXTURE_2D,texture[4]);
	if (gameState == 3)				// lose
		glBindTexture(GL_TEXTURE_2D,texture[5]);

	glNormal3f(0.0,0.0,1.0f);
	glBegin(GL_QUADS);
	
	glTexCoord2d(0.0,0.0); glVertex3f(-2.0,-1.5,-1.0);
	glTexCoord2d(1.0,0.0); glVertex3f( 2.0,-1.5,-1.0);
	glTexCoord2d(1.0,1.0); glVertex3f( 2.0, 1.5,-1.0);
	glTexCoord2d(0.0,1.0); glVertex3f(-2.0, 1.5,-1.0);
	glEnd();
}