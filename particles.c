/*
	Marching Gangster cubes
	TSBK03
	Linköpings tekniska högskola
	Rövargänget:
	Gustav "Gubbsatan" Hallström
	Sebastian "Legolas" Alfredsson
	Viktor "Täbybronks" Kraft
*/


// OpenGL 3 conversion 2013.
//gcc particles.c marching.c common/*.c common/Linux/MicroGlut.c -lGL -lX11 -lm -o particles -I common -I common/Linux


//#include <stdlib.h>


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
#include "marching.h"


#define boundRad 0.3
//#define cellSize 1.0
// Antalet tetraedrar
//#define NO_OBJECTS 100
#define gravity 9.82
//#define threshold 1
//#define nrCells 8
//#define DIM 2

GLuint* vertexArrayObjID, vertexBufferObjID, shader;


GLuint* vertexArrayObjID, vertexBufferObjID, normalBuffer, normalArray, shader;
GLuint* tetraArray, tetraNormalArray, tetraVertexBuffer, tetraNormalBuffer, tetraIndexBuffer;
GLuint* mTriArray, mNormArray, mTriBuffer, mNormBuffer;

//Denna håller reda på hur stort utrymme det allokerade minnet tar. Används till bufferdata
//int triSize; 

Point3D cam, point;

mat4 projectionMatrix;
mat4 viewMatrix, vm2;
mat4 tmpMatrix; 
float X1 = -1.0f, X2 = 1.0f, Y1 = -1.0f, Y2=1.0f,  Z1 = -1.0f, Z2=1.0f;

// Array med alla tetras som ska ritas upp 
// Glöm inte ändra denna med
Tetra tetras[NO_OBJECTS];

// En array av celler medelst innehåll
//Cell cell[nrCells];
//int cubeCorners[8];
//MCube mc;

// För keyboard-grejjen om marching
// ska vara aktiverad eller inte
int marchOn = 0; 

// Fps grejjor
int frameCount = 0; 
float currentTime = 0; 
float prevTime = 0; 
float FPS = 0; 

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
//http://cdn.thedailybeast.com/content/dailybeast/articles/2013/01/03/oscar-wilde-s-american-tour/jcr:content/image.img.2000.jpg/1357250876652.cached.jpg
// Always borrow money from a pessimist, he wont expect it back.
// Oscar Wilde
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
GLuint tetraIndicies[] = 
{
	//front	
	0,1,2, 
	//right
	3,1,0, 
	//left
	0,2,3,
	//back
	1,3,2
};

GLfloat *mTris;
GLfloat *mNorms;

// Every saint has a past and every sinner has a future
// Oscar Wilde

void calcBounce(int nr){
	vec3 normal = SetVector(0,0,0);
	if(tetras[nr].pos.x >= X2)
	{
		normal = SetVector(-1.0f,0.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
		
	if(tetras[nr].pos.x <= X1)
	{
		normal = SetVector(1.0f,0.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}

	if(tetras[nr].pos.y >= Y2)
	{
		normal = SetVector(0.0f,-1.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
	
	if(tetras[nr].pos.y <= Y1)
	{
		normal = SetVector(0.0f,1.0f,0.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
	
	if(tetras[nr].pos.z >= Z2)
	{
		normal = SetVector(0.0f,0.0f,-1.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}

	if(tetras[nr].pos.z <= Z1)
	{
		normal = SetVector(0.0f,0.0f,1.0f);
		tetras[nr].vel = VectorSub(tetras[nr].vel, ScalarMult(normal, 2.0f*DotProduct(tetras[nr].vel, normal)));
	}
}


void drawTetra(int nr)
{
	calcBounce(nr);
	tetras[nr].pos = VectorAdd(tetras[nr].pos, tetras[nr].vel);
	tmpMatrix = T(tetras[nr].pos.x, tetras[nr].pos.y, tetras[nr].pos.z); // position
	tmpMatrix = Mult(viewMatrix, tmpMatrix);
    	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, tmpMatrix.m);
	
	// Rita ut tetraeder? 
	//glBindVertexArray(tetraArray);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	
}


//Education is an admirable thing, but it is well to remember from time to time 
//that nothing that is worth knowing can be taught.
// Oscar Wilde

// Uppdatera "fysiken" eller tetraweissarnas positioner
/*void uppdatePos(int nr)
{
	int i;
	for(i = 0; i < NO_OBJECTS; i++)
	{
		// Dålig konstant fall
		//tetras[i].pos.y -= gravity * 0.001; 
		
		// mer korrekt än förra men fel och går sjukt snabbt. 
		tetras[i].pos.y -= gravity * GLUT_ELAPSED_TIME * tetras[i].mass; 	
	}	
}*/


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
//Some cause happiness wherever they go; others whenever they go.
// Oscar Wilde 


// Drawing routine
void Display()
{
	char buf[100] = {0};
	snprintf(buf, 100, "Sreums strutpartiklar, FPS : %.1f", FPS);

	glutSetWindowTitle(buf);	
	
	// Uppdatera tetrornas pos och annat smött å gött 

	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUseProgram(shader);
	
	
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
	
	GLfloat ang = 0.001f * glutGet(GLUT_ELAPSED_TIME);
	mat4 rot = ArbRotate(SetVector(1.0, 0.0, 0.0), ang);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);

        March(&mTris, &mNorms,tetras); //Här kommer vi att få olika antal trianglar vilket innebär att vi måste kunna allokera en ny mängd minne
        // VBO for vertex data	
	glGenVertexArrays(1, &mTriArray);
	glBindVertexArray(mTriArray);
	
	glGenVertexArrays(1, &mNormArray);
	glBindVertexArray(mNormArray);

	// Allocate Vertex Buffer Objects	
	glGenBuffers(1, &mTriBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTriBuffer);
	glBufferData(GL_ARRAY_BUFFER, triSize, mTris, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);							//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!!
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Position"),
	3, GL_FLOAT, GL_FALSE, 0, 0);

	
	

	// Allocate Vertex Buffer Objects	
	glGenBuffers(1, &mNormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mNormBuffer);
	glBufferData(GL_ARRAY_BUFFER, triSize, mNorms, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);							//OBS DETTA SKA ANVÄNDAS. ANNARS BLIR DET SÄMST!!!
	glVertexAttribPointer(glGetAttribLocation(shader, "in_Normal"),
	3, GL_FLOAT, GL_FALSE, 0, 0);
	
        glBindVertexArray(mTriArray); 
	glBindVertexArray(mNormArray);     
        glDrawArrays(GL_TRIANGLES, 0, (triSize/sizeof(GLfloat)/3));// draw objectperspective
	
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

	//Allokera lite minne till vår pointer	
	//mTris = malloc(0); 
	//mNorms = malloc(0); 

	// GL inits
	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	shader = loadShaders("shader.vert", "shader.frag");
	
	glUseProgram(shader);
	projectionMatrix = perspective(90, 1.0, 0.1, 1000); // It would be silly to upload an uninitialized matrix
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "modelviewMatrix"), 1, GL_TRUE, viewMatrix.m);
	
	//Eazy E boyz in the hood
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
	
	// Allocate Vertex Buffer Objects

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
		
		tetras[i].mass = 0.00001;

		//tetras[i].pos = SetVector(-0.9 + 0.01 * (float)i, -0.5f + 0.01 * (float)i, -0.5f + 0.01 * (float)i);
		tetras[i].pos = SetVector(0.0f, 0.0f, 0.0f);
		//tetras[i].pos = SetVector(glutGet(GLUT_ELAPSED_TIME)*0.0022f* sin(i*(3.14f/8.0f)),glutGet(GLUT_ELAPSED_TIME)*0.002f * sin(i*(3.14f/4.0f)),glutGet(GLUT_ELAPSED_TIME)*0.0023f * sin(i*(3.14f/2.0f)));
		//tetras[i].vel = SetVector(0.0001f* (float)i,0.0003f * (float)i,0.0002f * (float)i);
		tetras[i].vel = SetVector(glutGet(GLUT_ELAPSED_TIME)*0.00001f* sin(i*(3.14f/8.0f)),glutGet(GLUT_ELAPSED_TIME)*0.00001f * sin(i*(3.14f/4.0f)),glutGet(GLUT_ELAPSED_TIME)*0.00001f * sin(i*(3.14f/2.0f))); 	
		//tetras[i].vel = SetVector(0.005f, 0.0f, 0.0f);

		/*tetras[i].pos = SetVector(-0.9 + 0.01 * (float)i, -0.5f + 0.01 * (float)i, -0.5f + 0.01 * (float)i);
		//tetras[i].pos = SetVector(0.0f, 0.0f, 0.0f);
		//tetras[i].vel = SetVector(0.0,0.0,0.0);
		tetras[i].vel = SetVector(glutGet(GLUT_ELAPSED_TIME)*0.00001f* sin(i*(3.14f/8.0f)),glutGet(GLUT_ELAPSED_TIME)*0.00001f * sin(i*(3.14f/4.0f)),glutGet(GLUT_ELAPSED_TIME)*0.00001f * sin(i*(3.14f/2.0f))); */	

		
		//tetras[i].pos.y -= gravity * GLUT_ELAPSED_TIME * tetras[i].mass;		
		
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetraIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetraIndicies), tetraIndicies, GL_STATIC_DRAW);

	
      

	// Så vi kan rotera scenen 
	cam = SetVector(0, 0, 1);
    	point = SetVector(0, 0, 0);
    	zprInit(&viewMatrix, cam, point);
	
}

// Experience is just the name we give to our misstakes.
// Oscar Wilde

// Räkna ut FPS snodd från
//http://mycodelog.com/2010/04/16/fps/
void calculateFPS()
{
    //  Increase frame count
    frameCount++;
 
    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);
 
    //  Calculate time passed
    int timeInterval = currentTime - prevTime;
 
    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        FPS = frameCount / (timeInterval / 1000.0f);
 
        //  Set time
        prevTime = currentTime;
 glGenVertexArrays(1, &normalArray);
	glBindVertexArray(normalArray);	
        //  Reset frame count
        frameCount = 0;
    }
}

// funkar inte... än
void Keyboardfunk(unsigned char key, int x, int y)
{	

	printf("success");
	if(key == 27)
	{
		printf("%s", "Marching cubes on");
			
	}
	/*switch (key)
	{
		case 'o':
			printf("%s", "Marching cubes on");
			marchOn = 1;			
			break;

		case 'p':
			printf("%s", "Marching cubes off");
			marchOn = 0;
			break;

		case 0x1b:
			exit(0);
	}*/
}

void Idle()
{
	glutPostRedisplay();
	
	//  Calculate FPS
    	calculateFPS();
}


//I don't want to go to heaven. None of my friends are there.
// Oscar Wilde






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
	
	// Tangäntbård
	glutKeyboardFunc(Keyboardfunk);

	Init();
	
	

	glutMainLoop();
	exit(0);
}


