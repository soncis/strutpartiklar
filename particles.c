
// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "GL_utilities.h"
#include <math.h>

// Lgg till egna globaler hr efter behov.
float deltaT = 30000;
float oldTime;
bool huntFood = false;
float maxDist = 300.0;
void SpriteBehavior() // Din kod!
{
// Lgg till din labbkod hr. Det gr bra att ndra var som helst i
// koden i vrigt, men mycket kan samlas hr. Du kan utg frn den
// globala listroten, gSpriteRoot, fr att kontrollera alla sprites
// hastigheter och positioner, eller arbeta frn egna globaler.
}

// Drawing routine
void Display()
{
	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
}

void Init()
{
	

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");
	
	glutDisplayFunc(Display);
	//glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);

	Init();
	
	glutMainLoop();
	return 0;
}

