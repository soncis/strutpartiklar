// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux

// fixa lite sfärer täby :) 

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


#define kBallSize 0.1

GLuint* vertexArrayObjID, vertexBufferObjID, shader;

Point3D cam, point;

mat4 projectionMatrix;
mat4 viewMatrix;
/*GLfloat viewMatrix[] = { 0.7f, 0.0f, -0.7f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f,
0.7f, 0.0f, 0.7f, 0.0f,
0.0f, 0.0f, 0.0f, 1.0f };*/

GLfloat vertices[] = {
        // Left bottom triangle
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        // Right top triangle
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
};


// En bolljävel
//*******************************************************************************************
typedef struct
{
  GLuint tex;
  GLfloat mass;

  vec3 X, P, L; // position, linear momentum, angular momentum
  mat4 R; // Rotation

  vec3 F, T; // accumulated force and torque

//  mat4 J, Ji; We could have these but we can live without them for spheres.
  vec3 omega; // Angular momentum
  vec3 v; // Change in velocity

} Ball;

Model *sphere;
Ball ball[1];

void renderBall()
{
	DrawModel(sphere, shader, "in_Position", "in_Normal", NULL);
	
}

//*******************************************************************************************

// Drawing routine
void Display()
{
	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw the triangle

	glBindVertexArray(vertexArrayObjID);// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);// draw object	

	//glBindVertexArray(vertexArrayObjID);// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 6);// draw object
	
	//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, NULL);
	
	// also draw a ball
	//renderBall();
	//ball[0].X = ScalarMult(ball[0].X, 2); 	


	glFlush();
	
	glutSwapBuffers();
}

void Reshape(int h, int w)
{

	glViewport(0, 0, w, h);
    	GLfloat ratio = (GLfloat) w / (GLfloat) h;
   	projectionMatrix = perspective(90, ratio, 0.1, 1000);
    	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

void Init()
{

	// GL inits
	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = loadShaders("shader.vert", "shader.frag");

	//************************************************************
	// Balls balls balls balls
	sphere = LoadModelPlus("sphere.obj");
	
	ball[0].mass = 1.0;
	ball[0].R = IdentityMatrix();
	
	ball[0].X = SetVector(1.0, 1.0, 1.0);
	ball[0].P = SetVector(0, 0, 0);
	
	//************************************************************

	printError("init shader");
	
	cam = SetVector(0, -1.5, 0.5);
	point = SetVector(0, 0, 0);


	zprInit(&viewMatrix, cam, point);
	
	glUseProgram(shader);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000); // It would be silly to upload an uninitialized matrix
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects

	glGenBuffers(1, &vertexBufferObjID);
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),
	3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(shader, "in_Position"));
}



void Idle()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");

	glutDisplayFunc(Display);

	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	
	Init();
	
	glutMainLoop();
	exit(0);
}




