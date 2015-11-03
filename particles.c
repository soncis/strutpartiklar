
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

mat4 projectionMatrix;
mat4 viewMatrix;
// Drawing routine
void Display()
{
	GLfloat vertices[4][3] = {
	{-0.5,0.0,-0.5},
	{0.5,0.0,-0.5},
	{0.5,0.0,0.5},
	{-0.5,0.5,0.5}
	};
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

void Init()
{
	shader = loadShaders("shader.vert", "shader.frag");

	GLfloat rotationMatrix[] = {0.7f, -0.7f, 0.0f, 0.0f,
		0.7f, 0.7f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };


	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
}

void display(void)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Struta Hårt");
	
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);


int main(int argc, const char *argv[])
{
	glutInit();
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init ();
	glutMainLoop();
}







