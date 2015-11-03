// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif


#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "zpr.h"


GLuint* vertexArrayObjID, shader;

void OnTimer(int value);

mat4 projectionMatrix;
mat4 viewMatrix;

//----------------------Globals-------------------------------------------------
Point3D cam, point;
//Model *model1;
//FBOstruct *fbo1, *fbo2, *fbo3;
//GLuint phongshader = 0, plaintextureshader = 0, truncshader = 0, filtershader = 0, mixshader =0;
//int switch_ = 1; 

//-------------------------------------------------------------------------------------


void Init()
{
	dumpInfo();  // shader info
	
	// GL inits
	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shaders
	shader = loadShaders("shader.vert", "shader.frag");
	
	printError("init shader");

	// load the model
	//model1 = LoadModelPlus("teapot.obj");

	cam = SetVector(0, 5, 15);
	point = SetVector(0, 1, 0);

	glutTimerFunc(5, &OnTimer, 0);

	zprInit(&viewMatrix, cam, point);	

	GLfloat rotationMatrix[] = 
	{
		0.7f, -0.7f, 0.0f, 0.0f,
		0.7f, 0.7f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(5, &OnTimer, value);
}

// Drawing routine
void Display()
{
	GLfloat vertices[4][3] = 
	{
		{-0.5, 0.0, -0.5},
		{0.5, 0.0, -0.5},
		{0.5, 0.0, 0.5},
		{-0.5, 0.5, 0.5}
	};

	// Activate shader program
	glUseProgram(shader);

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	// Allocate Vertex Buffer Objects
	//glGenBuffers(1, &vertexBufferObjID);
	glBindVertexArray(vertexArrayObjID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);

	glutSwapBuffers();
}

void Reshape(int h, int w)
{
	glViewport(0, 0, h, w);
	GLfloat ratio = (GLfloat) w / (GLfloat) h;
	projectionMatrix = perspective(90, ratio, 1.0, 1000);
}

// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");
	
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	Init();
	
	glutMainLoop();
	return 0;
}
