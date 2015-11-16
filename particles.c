// OpenGL 3 conversion 2013.
//gcc particles.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux


// Kolla om man kan ladda in lite fler tetraedrar från array typ.. ?  


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



#define kBallSize 0.9
#define boundRad 0.1

GLuint* vertexArrayObjID, vertexBufferObjID, shader;


GLuint* vertexArrayObjID, vertexBufferObjID, normalBuffer, normalArray, shader;
GLuint* tetraArray, tetraNormalArray, tetraVertexBuffer, tetraNormalBuffer, tetraIndexBuffer;



Point3D cam, point;

mat4 projectionMatrix;
mat4 viewMatrix, vm2;
mat4 tmpMatrix; 
const float XMIN = -0.5, XMAX = 0.5, YMIN = -0.5, YMAX=0.5;

//Plane strushish
GLfloat vertices[] = 
{
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

GLfloat normals[] = 
{
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



//regular (1,1,1), (1,−1,−1), (−1,1,−1), (−1,−1,1)
GLfloat tetraVertices[] = 
{
        -0.1f, -0.1f, 0.1f,
        0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
	0.1f, -0.1f, -0.1f
        
};

//Indices for tetrahedron
GLuint tetraIndicies[] = {
	//front	
	0,1,2, 
	//right
	3,1,0, 
	//left
	0,2,3,
	//back
	1,3,2
};

// Antalet tetraedrar
#define NO_OBJECTS 7

#define gravity 9.82

// Tetra struts
//*******************************************************************************************
typedef struct
{
	// Tror inte denna behövs..
	GLuint* tetraArr; 
	
	// Tetraederns massa
	GLfloat mass;

	// Om den ska synas eller inte 
	int alive;
	
	// Positioner
	 vec3 pos; 

	// Riktning		
	vec3 direction;

} Tetra;

// Array med alla tetras som ska ritas upp 
// Glöm inte ändra denna med
Tetra tetras[7];



// Kolla renderball för position
void drawTetra(int nr)
{
	
	// Använd tempmatrisen för förflyttning
	tmpMatrix = T(tetras[nr].pos.x, tetras[nr].pos.y, tetras[nr].pos.z); // position
	//tmpMatrix = T(0.5, 0.5, 0.5);	
	
	tmpMatrix = Mult(viewMatrix, tmpMatrix);
    	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, tmpMatrix.m);

	// Rita ut tetraeder? 
	glBindVertexArray(tetras[nr].tetraArr);
	//glBindVertexArray(tetraNormalArray);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	
}


//Education is an admirable thing, but it is well to remember from time to time 
//that nothing that is worth knowing can be taught.
// Oscar Wilde

// Uppdatera "fysiken" eller tetraweissarnas positioner
void uppdatePos()
{
	int i;
	for(i = 0; i < NO_OBJECTS; i++)
	{
		// Dålig konstant fall
		//tetras[i].pos.y -= gravity * 0.001; 
		
		// mer korrekt än förra men fel och går sjukt snabbt. 
		tetras[i].pos.y -= gravity * GLUT_ELAPSED_TIME * tetras[i].mass; 	
	}	

}

GLfloat tetraNormals[] = 
{
	//front        
	-0.58f, 0.58f, 0.58f,
	//right        
	0.58f, -0.58f, 0.58f,
	//left
        -0.58f, -0.58f, -0.58f,
	//back
	0.58f, 0.58f, -0.58f        
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


//Some cause happiness wherever they go; others whenever they go.
// Oscar Wilde 


// Drawing routine
void Display()
{
	// Uppdatera tetrornas pos och annat smött å gött 
	uppdatePos();

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

	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(shader);
	
	vm2 = viewMatrix;
	// Scale and place bunny since it is too small
	vm2 = Mult(vm2, T(0, -8.5, 0));
	vm2 = Mult(vm2, S(200,200,200));
	
	viewMatrix = lookAt(0,  0,  2,  0,  0,  0,  0,  1,  0);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	
	
	glBindVertexArray(vertexArrayObjID);
	glBindVertexArray(normalArray);

	// Rita ut planet	
	glDrawArrays(GL_TRIANGLES, 0, 12);// draw objectperspective


	int i; 
	// Rita ut tetraeder
	for(i = 0; i < NO_OBJECTS; i++) 
	{
		drawTetra(i); 
	}

	glDrawArrays(GL_TRIANGLES, 0, 12);// draw objectperspective
	
	GLfloat ang = 0.001f * glutGet(GLUT_ELAPSED_TIME);
	mat4 rot = ArbRotate(SetVector(1.0, 0.0, 0.0), ang);
	viewMatrix = Mult(viewMatrix, rot);

	glBindVertexArray(tetraArray);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//mat4 s = S(2.5, 2.5, 2.5);
	
	//glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	//renderBall();

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


	//zprInit(&viewMatrix, cam, point);
	
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
	
	// For rendering the tetrahedron
	//glGenVertexArrays(1, &tetraNormalArray);
	//glBindVertexArray(tetraNormalArray);	
	// Allocate Vertex Buffer Objects

	glGenVertexArrays(1, &tetraArray);
	glBindVertexArray(tetraArray);

	glGenVertexArrays(1, &tetraArray);
	glBindVertexArray(tetraArray);


	glGenBuffers(1, &tetraVertexBuffer);
	glGenBuffers(1, &tetraIndexBuffer);
	glGenBuffers(1, &tetraNormalBuffer);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, tetraVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetraVertices), tetraVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"), 3, GL_FLOAT,
	GL_FALSE, 0, 0); 

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, tetraNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetraNormals), tetraNormals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Normal"), 3, GL_FLOAT,
	GL_FALSE, 0,0); 
	

	int i; 
	// Initiera variabler för flera tetror
	for (i = 0; i < NO_OBJECTS; i++)
	{
		tetras[i].tetraArr = tetraArray; 

		//glGenVertexArrays(1, &tetras[i].tetraArr);
		//glBindVertexArray(tetras[i].tetraArr);
	
		tetras[i].mass = 0.00001;;
		tetras[i].pos = SetVector(-1.0 + 0.5 * (float)i, 0.5, 0.5);			
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetraIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetraIndicies)/*12*sizeof(GLuint)*/, tetraIndicies, GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetraIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetraIndicies), tetraIndicies, GL_STATIC_DRAW);
	
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
