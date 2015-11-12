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



#define kBallSize 0.1

GLuint* vertexArrayObjID, vertexBufferObjID, shader;


GLuint* vertexArrayObjID, vertexBufferObjID, normalBuffer, normalArray, shader;
Model *model1;

Point3D cam, point;

mat4 projectionMatrix;
mat4 viewMatrix, vm2;
const float XMIN = -0.5, XMAX = 0.5, YMIN = -0.5, YMAX=0.5;

GLfloat vertices[] = {
        // Left bottom triangle
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        // Right top triangle
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
	//back lower triangle
	-1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
	//back upper triangle
	-1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
};

GLfloat normals[] = {
        // Left bottom triangle
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // Right top triangle
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
	//back lower triangle
	0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
	//back upper triangle
	0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
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



//http://www.3dgep.com/understanding-the-view-matrix/   
/*void Look(vec3 eyepos, vec3 target, vec3 up){
	vec3 zaxis = Normalize(VectorSub(eyepos, target));  //The z axis is pointing outwards
	vec3 xaxis = Normalize(CrossProduct(up, zaxis));  //xaxis is pointing to the right
	vec3 yaxis = CrossProduct(zaxis, xaxis);  //y axis is pointing up
	
	//mat4 orientation = SetMat4(xaxis.x,yaxis.x,zaxis.x,0.0f,xaxis.y,yaxis.y,zaxis.y,0.0f,xaxis.z,yaxis.z,zaxis.z,0.0f,0.0f,0.0f,0.0f,1.0f);
  	mat4 ori;
	ori.m = SetMat4(0.0f, 1.0f, 2.0f, 3.0f,4.0f, 5.0f, 6.0f, 7.0f,8.0f, 9.0f, 10.0f,  1.0f, 12.0f, 13.0f, 14.0f, 15.0f);
}
*/

// Drawing routine
void Display()
{
	
	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw the triangle


	//glBindVertexArray(vertexArrayObjID);// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 6);// draw object	

	//glBindVertexArray(vertexArrayObjID);// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 6);// draw object
	
	//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, NULL);
	
	// also draw a ball
		



	//glBindVertexArray(vertexArrayObjID);// Select VAO
	
	glUseProgram(shader);
	
	vm2 = viewMatrix;
	// Scale and place bunny since it is too small
	vm2 = Mult(vm2, T(0, -8.5, 0));
	vm2 = Mult(vm2, S(200,200,200));
	
	viewMatrix = lookAt(0,  0,  2,  0,  0,  -1,  0,  1,  0);

	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindVertexArray(vertexArrayObjID);
	glBindVertexArray(normalArray);
	glDrawArrays(GL_TRIANGLES, 0, 12);// draw objectperspective
	//DrawModel(model1, shader, "in_Position", "in_Normal", NULL);
	
	renderBall();
	ball[0].X = ScalarMult(ball[0].X, 2); 

	glFlush();
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(5, &OnTimer, value);
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
	glDisable(GL_CULL_FACE);
	printError("GL inits");

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
	


	printError("init shader");
	

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	//Normal buffer
	glGenVertexArrays(1, &normalArray);
	glBindVertexArray(normalArray);	

	// Allocate Vertex Buffer Objects	
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &normalBuffer);
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);							//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!!
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),
	3, GL_FLOAT, GL_FALSE, 0, 0); 

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Normal"),
	3, GL_FLOAT, GL_FALSE, 0, 0); 

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


