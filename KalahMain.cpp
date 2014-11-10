#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include <gl/gl.h>			// Header File For The OpenGL32 Library

int WIDTH = 640, HEIGHT = 480;

#include "Kalah.h"

Kalah mKalah;

bool mouseClickState = false;

void init()
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	
	// light
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.0, 0.0, 3.0, 0.0 };
	GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat local_view[] = { 0.0 };

//	glClearColor(0.0, 0.1, 0.1, 0.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	mKalah.initResource();
	mKalah.initSeeds();

}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	
	mKalah.render();

	glutSwapBuffers();
//	glFlush();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//
	glOrtho(-4.0,4.0,-3.0,3.0,-10.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

void mouseClick(int button,int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		{
			switch(state)
			{
			case GLUT_DOWN:
				mouseClickState = true;

				break;
			case GLUT_UP:
			//	mouseClickState = false;
				break;
			}
		}
	}
}

void processMousePassiveMotion(int x,int y)
{
	float halfWidth = WIDTH/2, halfHeight = HEIGHT/2;
	float tempX = (x-halfWidth)/halfWidth * 4;
	float tempY =-(y-halfHeight)/halfHeight * 3;

	if (tempX<3.75 && tempX>-3.68 && tempY<2.21&& tempY>-2.25)
	{
	//	printf("x =%f  y=%f\n",tempX,tempY);
		mKalah.processClick(tempX,tempY,mouseClickState,true);
		glutPostRedisplay();
	}
	else
		mouseClickState = false;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(processMousePassiveMotion);
	
	glutMainLoop();
	return 0; 
}