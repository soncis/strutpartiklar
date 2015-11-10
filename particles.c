// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux


// fixa lite sfärer täby :) 

#include <stdlib.h>


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		#include <stdio.h>
		#include <GL/gl.h>
		#include "MicroGlut.h"
//		#include <GL/glut.h>
	#endif
#endif

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "zpr.h"



#define MAX_DROPS 50000
#define GRAVITY -0.0005

#ifdef WIN32
//to correct ASCI deviations in Microsoft VC++ 6.0

#define M_PI (3.1415926535897932384626433832795)

double drand48()
{	return (rand()%10000)/10000.0; }

//end of corrections
#endif


typedef struct {
int alive;
GLfloat xpos, ypos;
GLfloat xdir, ydir;
GLfloat mass;
} Particle;

Particle water[MAX_DROPS];
int NumDrops;

void draw_waterfall(void)
{
int i;
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0.0, 0.5, 1.0);
glBegin(GL_POINTS);
for ( i=0 ; i<NumDrops ; i++ )
if (water[i].alive) {
glVertex2f(water[i].xpos, water[i].ypos);
}
glEnd();
glFlush();
glutSwapBuffers();
}

void time_step(void)
{
int i;
for ( i=0 ; i<NumDrops ; i++ ) {
if (water[i].alive) {
// set up an object to hit
if (water[i].ypos + GRAVITY*water[i].mass < -0.75) {
// bounce it off of the "floor"
water[i].ydir = -water[i].ydir;
} else {
// let gravity do its thing
water[i].ydir += GRAVITY * water[i].mass;
}
water[i].xpos += water[i].xdir;
water[i].ypos += water[i].ydir;
if (water[i].ypos < -1.0 || water[i].xpos > 1.0)
water[i].alive = 0;
}
} 
}

void drop_generator(void)
{
int i,newdrops = drand48()*60;

if (NumDrops + newdrops > MAX_DROPS)
newdrops = MAX_DROPS - NumDrops;

for ( i=NumDrops ; i<NumDrops+newdrops ; i++ ) {
water[i].alive = 1; 
water[i].xpos = -0.8 + 0.01*drand48();
water[i].ypos = 0.8 + 0.01*drand48();
water[i].xdir = 0.0075 + 0.0025*drand48();
water[i].ydir = 0.0;
water[i].mass = 0.5 + 0.5*drand48();
}
NumDrops += newdrops;
}

void display(void)
{

drop_generator();
draw_waterfall();
time_step();

}

void keyboard(unsigned char key, int x, int y)
{
switch (key) { 
case 27: exit(0); break;
}
}

void reshape (int w, int h)
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION); 
glLoadIdentity();
if (w <= h)
glOrtho(-1.0, 1.0,
-1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w,
-1.0, 1.0);
else
glOrtho(-1.0*(GLfloat)w/(GLfloat)h, 1.0*(GLfloat)w/(GLfloat)h,
-1.0, 1.0,
-1.0, 1.0);
glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
glutPostRedisplay();
}

int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
glutInitWindowSize (700, 700);
glutInitWindowPosition(0, 0);
glutCreateWindow ("Waterfall");

glClearColor (0.0, 0.0, 0.0, 0.0);
glPointSize(2.0);

glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutIdleFunc(idle);
glutKeyboardFunc(keyboard);
glutMainLoop();

return 1; 
}
