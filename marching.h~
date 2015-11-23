/* www.urbandictionary.com/define.php?term=MC
MC
Short for master of ceremonies or mic controller. Essentially a word for a rapper but the term is not limited to hip hop. Drum and Bass, Garage, Happy Hardcore, Ragga and old school rave all feature MCs, however with these genres, the MC generally MCs live while a DJ mixes the tunes, whereas hip hop MCs mainly do so on record
Navigator is a wicked MC
Biggie was a phat MC
I'm MCing at a house party tonight, you coming? 
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "VectorUtils3.h"

#define kBallSize 0.3
#define boundRad 0.3
#define cellSize 1.0
// Antalet tetraedrar
#define NO_OBJECTS 1
#define gravity 9.82
#define threshold 1
#define nrCells 8
#define DIM 2

typedef struct Tetra
{
	// Tror inte denna behövs..
	GLuint* tetraArr; 
	
	// Tetraederns massa
	GLfloat mass;

	// Om den ska synas eller inte 
	int alive;
	
	// Positioner
	 vec3 pos; 

	// Hastigheten		
	vec3 vel;

} Tetra;

typedef struct Cell 
{

	// inne/ute
	int state;
	
	// antal partiklar
	int nrParts;

}Cell;

typedef struct MCube
{
	// Positioner
	 vec3 pos; 

	//cube Storlek	SKRIV INTE ULFS	
	//float size;

} MCube;

//Denna håller reda på hur stort utrymme det allokerade minnet tar. Används till bufferdata
extern int triSize; 

extern float XMIN , XMAX , YMIN , YMAX,  ZMIN, ZMAX;



//http://paulbourke.net/geometry/polygonise/
extern int triTable[256][16]; 


Cell cell[nrCells];
int cubeCorners[8];
MCube mc;
