/*
	Marching Gangster cubes
	TSBK03
	Linköpings tekniska högskola
	Rövargänget:
	Gustav "Gubbsatan" Hallström
	Sebastian "Legolas" Alfredsson
	Viktor "Täbybronks" Kraft
*/

/* www.urbandictionary.com/define.php?term=MC
MC
Short for master of ceremonies or mic controller. Essentially a word for a rapper but the term is not limited to hip hop. Drum and Bass, Garage, Happy Hardcore, Ragga and old school rave all feature MCs, however with these genres, the MC generally MCs live while a DJ mixes the tunes, whereas hip hop MCs mainly do so on record
Navigator is a wicked MC
Biggie was a phat MC
I'm MCing at a house party tonight, you coming? 
*/

// Varför ser Gubbsatan ut som Loni Anderson?
// Varför ser Legolas ut som Jon Voight? 
// Varför ser Täby Bronx? 

#include "marching.h"

// ööh.. 
#define abs(x) (x > 0.0? x: -x)

int triSize;
float XMIN = -1.0f, XMAX = 1.0f, YMIN = -1.0f, YMAX=1.0f,  ZMIN = -1.0f, ZMAX=1.0f;


// öööh.. interpolation?  
float isolevel = 4.0;


// probably not needed or already included
vec3 p1,p2;
double valp1,valp2;

// Linear interpolation 
float interpolate(float p1, float p2, float valP1, float valP2)
{
	float p; 
	float mu;
 

	if (abs(isolevel-valP1) < 1.0f)
		return(p1);
	
	if (abs(isolevel-valP2) < 1.0f)
		return(p2);
	 
	if (abs(valp1-valP2) < 1.0f)
		return(p1);

	mu = (isolevel - valP1) / (valP2 - valP1);

	p = p1 + mu * (p2 - p1);
	

	return p; 

}


void March(GLfloat **mTris, GLfloat **mNorms,Tetra *tetras)
{
	mc.pos = SetVector(XMIN, YMIN, ZMIN);
	int i;
	*mTris = malloc(0);
	*mNorms = malloc(0);
	// loop through all cells and calculate number of particles
	for(i = 0; i < DIM*DIM*DIM; i++)
	{ 
		cell[i].nrParts = 0;
		cell[i].state = 0;
		int j;		
		for(j = 0; j < NO_OBJECTS; j++)
		{
			// Check if particle is inside this cell
			//OBS! Will now register both cells on cell border
			if(tetras[j].pos.x >= mc.pos.x 
			&& tetras[j].pos.x <= (mc.pos.x + cellSize) 
			&& tetras[j].pos.y >= mc.pos.y 
			&& tetras[j].pos.y <= (mc.pos.y + cellSize)
			&& tetras[j].pos.z >= mc.pos.z 
			&& tetras[j].pos.z <= (mc.pos.z + cellSize))
			{
				// If particle is inside cell add to this cells particle count
				cell[i].nrParts++;	
			}
				
		}
		
		if(cell[i].nrParts >= isolevel)
		{
			// If threshold is reached set state to 1!!!!!!!!!!!	
			cell[i].state = 1;
		}
		/*
		Step through cells using the position and check against the borders.
		When XMAX position is used, step up in y direction and reset xpos.
		*/
		mc.pos.x += cellSize;
		if(mc.pos.x == XMAX)
                {
			mc.pos.x = XMIN;
			mc.pos.y += cellSize;

			if(mc.pos.y == YMAX)
                        {
				mc.pos.y = YMIN;
				mc.pos.z += cellSize;
			}
		}
	}
	int m, j, x, y, z, Case;
        y=z=0;
        x=-1;
        vec3 edge[12];
        int count = 0;

	for(j=0; j< DIM * DIM * DIM; j++)
        {
                //detta gör vi för att inte alla hörnpunkter ska få värdet 1
                for(m=0; m<8; m++)
                        cubeCorners[m] = 0;
                x++;
                if(x == DIM)
                {
                        y++;
                        x=0;
                }
                if(y == DIM)
                {
                        z++;
                        x=y=0;
                }
                
		// nollställ density
		for(m = 0; m < 24; m++)
			density[m] = 0; 

                //Kolla mot kuber som ligger "vägg i vägg" med vår kub
                // Kolla även denstiy så den får rätt antal partiklar mot varje hörn
		if(x!=0 && cell[j-1].state == 1)
                {
			cubeCorners[0] = 1;
			density[0] += cell[j-1].nrParts;

                        cubeCorners[3] = 8;
			density[9] += cell[j-1].nrParts;

                        cubeCorners[4] = 16;
			density[12] += cell[j-1].nrParts;

                        cubeCorners[7] = 128;
			density[21] += cell[j-1].nrParts;
		}
                if(x!=DIM-1 && cell[j+1].state == 1)
                {
                        cubeCorners[1] = 2;
			density[3] += cell[j+1].nrParts;

                        cubeCorners[2] = 4;
			density[6] += cell[j+1].nrParts;		
	
                        cubeCorners[5] = 32;
			density[15] += cell[j+1].nrParts;

                        cubeCorners[6] = 64;
			density[18] += cell[j+1].nrParts;
                }
                if(y!=0 && cell[j-DIM].state == 1)
		{
                        cubeCorners[0] = 1;
			density[1] += cell[j-DIM].nrParts;

                        cubeCorners[1] = 2;
			density[4] += cell[j-DIM].nrParts;

                        cubeCorners[2] = 4;
			density[7] += cell[j-DIM].nrParts;     
                        
			cubeCorners[3] = 8;
			density[10] += cell[j-DIM].nrParts;
                }
                if(y!=DIM-1 && cell[j+DIM].state == 1)
                {
                        cubeCorners[4] = 16;
			density[13] += cell[j+DIM].nrParts;

                        cubeCorners[5] = 32;
			density[16] += cell[j+DIM].nrParts;

                        cubeCorners[6] = 64;
			density[19] += cell[j+DIM].nrParts;

                        cubeCorners[7] = 128;
			density[22] += cell[j+DIM].nrParts;
                }
                if(z!=0 && cell[j-DIM*DIM].state == 1)
		{
                        cubeCorners[0] = 1;
			density[2] += cell[j-DIM*DIM].nrParts;

                        cubeCorners[1] = 2;
			density[5] += cell[j-DIM*DIM].nrParts;

                        cubeCorners[4] = 16;
			density[14] += cell[j-DIM*DIM].nrParts;

                        cubeCorners[5] = 32;
			density[17] += cell[j-DIM*DIM].nrParts;
                }       
                if(z!=DIM-1 && cell[j+DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;
			density[8] += cell[j+DIM*DIM].nrParts;
     
                        cubeCorners[3] = 8;
			density[11] += cell[j+DIM*DIM].nrParts;

                        cubeCorners[6] = 64;
			density[20] += cell[j+DIM*DIM].nrParts;

                        cubeCorners[7] = 128;
			density[23] += cell[j+DIM*DIM].nrParts;

                }
                
                //Kolla kuber som delar en edge med vår kub
                if(x!=0 && y!=0 && cell[j-DIM - 1].state == 1)
                {
                        cubeCorners[0] = 1;
			density[0] += cell[j-DIM-1].nrParts;
			density[1] += cell[j-DIM-1].nrParts;

                        cubeCorners[3] = 8;
			density[9] += cell[j-DIM-1].nrParts;
			density[10] += cell[j-DIM-1].nrParts;
                }
                if(x!=DIM-1 && y!=0 && cell[j-DIM + 1].state == 1)
                {
                        cubeCorners[1] = 2;
			density[3] += cell[j-DIM+1].nrParts;
			density[4] += cell[j-DIM+1].nrParts;

                        cubeCorners[2] = 4;
			density[6] += cell[j-DIM+1].nrParts;
			density[7] += cell[j-DIM+1].nrParts;
                }
                if(y!=0 && z!=0 && cell[j-DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
			density[1] += cell[j-DIM - DIM*DIM].nrParts;
			density[2] += cell[j-DIM - DIM*DIM].nrParts;

                        cubeCorners[1] = 2;
			density[4] += cell[j-DIM - DIM*DIM].nrParts;
			density[5] += cell[j-DIM - DIM*DIM].nrParts;

                }
                if(y!=0 && z!=DIM-1 && cell[j-DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;
			density[7] += cell[j-DIM + DIM*DIM].nrParts;
			density[8] += cell[j-DIM + DIM*DIM].nrParts;

                        cubeCorners[3] = 8;
			density[10] += cell[j-DIM + DIM*DIM].nrParts;
			density[11] += cell[j-DIM + DIM*DIM].nrParts;

                }
                if(x!=0 && y!=DIM-1 && cell[j+DIM - 1].state == 1)
                {
                        cubeCorners[4] = 16;
			density[12] += cell[j+DIM - 1].nrParts;
			density[13] += cell[j+DIM - 1].nrParts;

                        cubeCorners[7] = 128;
			density[21] += cell[j+DIM - 1].nrParts;
			density[22] += cell[j+DIM - 1].nrParts;
                }
                if(x!=DIM-1 && y!=DIM-1 && cell[j+DIM + 1].state == 1)
                {
                        cubeCorners[5] = 32;
			density[15] += cell[j+DIM + 1].nrParts;
			density[16] += cell[j+DIM + 1].nrParts;

                        cubeCorners[6] = 64;
			density[18] += cell[j+DIM + 1].nrParts;
			density[19] += cell[j+DIM + 1].nrParts;
                }
                if(y!=DIM-1 && z!=0 && cell[j+DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[4] = 16;
			density[13] += cell[j+DIM - DIM*DIM].nrParts;
			density[14] += cell[j+DIM - DIM*DIM].nrParts;

                        cubeCorners[5] = 32;
			density[16] += cell[j+DIM - DIM*DIM].nrParts;
			density[17] += cell[j+DIM - DIM*DIM].nrParts;
                }
                if(y!=DIM-1 && z!=DIM-1 && cell[j+DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
			density[19] += cell[j+DIM + DIM*DIM].nrParts;
			density[20] += cell[j+DIM + DIM*DIM].nrParts;

                        cubeCorners[7] = 128;
			density[22] += cell[j+DIM + DIM*DIM].nrParts;
			density[23] += cell[j+DIM + DIM*DIM].nrParts;
                }    
                if(x!=0 && z!=0 && cell[j-1 - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
			density[0] += cell[j-1 - DIM*DIM].nrParts;
			density[2] += cell[j-1 - DIM*DIM].nrParts;

                        cubeCorners[4] = 16;
			density[12] += cell[j-1 - DIM*DIM].nrParts;
			density[14] += cell[j-1 - DIM*DIM].nrParts;
                }
                if(x!=DIM-1 && z!=0 && cell[j+1 - DIM*DIM].state == 1)
                {
                        cubeCorners[1] = 2;
			density[3] += cell[j+1 - DIM*DIM].nrParts;
			density[5] += cell[j+1 - DIM*DIM].nrParts;

                        cubeCorners[5] = 32;
			density[15] += cell[j+1 - DIM*DIM].nrParts;
			density[17] += cell[j+1 - DIM*DIM].nrParts;
                }
                if(x!=0 && z!=DIM-1 && cell[j-1 + DIM*DIM].state == 1)
                {
                        cubeCorners[7] = 128;
			density[21] += cell[j-1 + DIM*DIM].nrParts;
			density[23] += cell[j-1 + DIM*DIM].nrParts;

                        cubeCorners[3] = 8;
			density[9] += cell[j-1 + DIM*DIM].nrParts;
			density[11] += cell[j-1 + DIM*DIM].nrParts;
                }    
                if(x!=DIM-1 && z!=DIM-1 && cell[j+1 + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
			density[18] += cell[j+1 + DIM*DIM].nrParts;
			density[20] += cell[j+1 + DIM*DIM].nrParts;

                        cubeCorners[2] = 4;
			density[6] += cell[j+1 + DIM*DIM].nrParts;
			density[8] += cell[j+1 + DIM*DIM].nrParts;
                }  
 
                //Kollar kuber som delar hörnpunkt med vår kub
                if(x!=0 && y!=0 && z!=0 && cell[j-1 - DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[0] = 1;
			density[0] += cell[j-1 - DIM - DIM*DIM].nrParts;
			density[1] += cell[j-1 - DIM - DIM*DIM].nrParts;
			density[2] += cell[j-1 - DIM - DIM*DIM].nrParts;

                } 
                if(x!=DIM-1 && y!=0 && z!=0 && cell[j+1 - DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[1] = 2;
			density[3] += cell[j+1 - DIM - DIM*DIM].nrParts;
			density[4] += cell[j+1 - DIM - DIM*DIM].nrParts;
			density[5] += cell[j+1 - DIM - DIM*DIM].nrParts;
                }  
                if(x!=0 && y!=0 && z!=DIM-1 && cell[j-1 - DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[3] = 8;
			density[9] += cell[j-1 - DIM + DIM*DIM].nrParts;
			density[10] += cell[j-1 - DIM + DIM*DIM].nrParts;
			density[11] += cell[j-1 - DIM + DIM*DIM].nrParts;
                } 
                if(x!=DIM-1 && y!=0 && z!=DIM-1 && cell[j+1 - DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[2] = 4;
			density[6] += cell[j+1 - DIM + DIM*DIM].nrParts;
			density[7] += cell[j+1 - DIM + DIM*DIM].nrParts;
			density[8] += cell[j+1 - DIM + DIM*DIM].nrParts;
                }
                if(x!=0 && y!=DIM-1 && z!=0 && cell[j-1 + DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[4] = 16;
			density[12] += cell[j-1 + DIM - DIM*DIM].nrParts;
			density[13] += cell[j-1 + DIM - DIM*DIM].nrParts;
			density[14] += cell[j-1 + DIM - DIM*DIM].nrParts;
                } 
                if(x!=DIM-1 && y!=DIM-1 && z!=0 && cell[j+1 + DIM - DIM*DIM].state == 1)
                {
                        cubeCorners[5] = 32;
			density[15] += cell[j+1 + DIM - DIM*DIM].nrParts;
			density[16] += cell[j+1 + DIM - DIM*DIM].nrParts;
			density[17] += cell[j+1 + DIM - DIM*DIM].nrParts;
                }  
                if(x!=0 && y!=DIM-1 && z!=DIM-1 && cell[j-1 + DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[7] = 128;
			density[21] += cell[j-1 + DIM + DIM*DIM].nrParts;
			density[22] += cell[j-1 + DIM + DIM*DIM].nrParts;
			density[23] += cell[j-1 + DIM + DIM*DIM].nrParts;
                } 
                if(x!=DIM-1 && y!=DIM-1 && z!=DIM-1 && cell[j+1 + DIM + DIM*DIM].state == 1)
                {
                        cubeCorners[6] = 64;
			density[18] += cell[j+1 + DIM + DIM*DIM].nrParts;
			density[19] += cell[j+1 + DIM + DIM*DIM].nrParts;
			density[20] += cell[j+1 + DIM + DIM*DIM].nrParts;
                }
                

                int k;
                Case = 0;
                for(k = 0; k<8; k++)
		{
                        Case += cubeCorners[k];
                }

		
                //vart ligger edgesen i världen

		// Ersätta detta med en loop ? 
		
		edge[0] = SetVector( interpolate(-1.0f+x*cellSize, -1.0f+x*cellSize+cellSize, density[0], density[3])
		 , -1.0f+y*cellSize, -1.0f+z*cellSize);

                edge[1] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize, 
		interpolate(-1.0f+z*cellSize, -1.0f+z*cellSize + cellSize, density[5], density[8]));

                edge[2] = SetVector( interpolate(-1.0f+x*cellSize, -1.0f+x*cellSize + cellSize, density[9], density[6])
		, -1.0f+y*cellSize, -1.0f+(z+1)*cellSize);

                edge[3] = SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize, 
		interpolate(-1.0f+z*cellSize, -1.0f+z*cellSize + cellSize, density[2], density[11]));
                
		//toppen av cuben
                edge[4] = SetVector(interpolate(-1.0f+x*cellSize, -1.0f+x*cellSize + cellSize, density[12], density[15])
		, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize);

                edge[5] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+(y+1)*cellSize, 
		interpolate(-1.0f+z*cellSize, -1.0f+z*cellSize + cellSize, density[17], density[20]));


                edge[6] = SetVector(interpolate(-1.0f+x*cellSize, -1.0f+x*cellSize + cellSize, density[21], density[18])
		, -1.0f+(y+1)*cellSize, -1.0f+(z+1)*cellSize);

                edge[7] = SetVector(-1.0f+x*cellSize, -1.0f+(y+1)*cellSize,
		interpolate(-1.0f+z*cellSize, -1.0f+z*cellSize+cellSize, density[14], density[23]));
                
		//sidorna av cuben
                edge[8] = SetVector(-1.0f+x*cellSize, 
		interpolate(-1.0f+y*cellSize, -1.0f+y*cellSize+cellSize, density[1], density[13])
		, -1.0f+z*cellSize);


                edge[9] = SetVector(-1.0f+(x+1)*cellSize, 
		interpolate(-1.0f+y*cellSize, -1.0f+y*cellSize+cellSize, density[4], density[16])
		, -1.0f+z*cellSize);
	
                edge[10] = SetVector(-1.0f+(x+1)*cellSize, 
		interpolate(-1.0f+y*cellSize, -1.0f+y*cellSize+cellSize, density[7], density[19])
		, -1.0f+(z+1)*cellSize);

                edge[11] = SetVector(-1.0f+x*cellSize,
		interpolate(-1.0f+y*cellSize, -1.0f+y*cellSize+cellSize, density[10], density[22])
		, -1.0f+(z+1)*cellSize); 


                //botten av cuben
        	/* Backup       
		edge[0] = SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+y*cellSize, -1.0f+z*cellSize);
                edge[1] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                edge[2] = SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+y*cellSize, -1.0f+(z+1)*cellSize);
                edge[3] = SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                
		//toppen av cuben
                edge[4] = SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize);
                edge[5] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                edge[6] = SetVector(-1.0f+x*cellSize+cellSize/2.0f, -1.0f+(y+1)*cellSize, -1.0f+(z+1)*cellSize);
                edge[7] = SetVector(-1.0f+x*cellSize, -1.0f+(y+1)*cellSize, -1.0f+z*cellSize+cellSize/2.0f);
                
		//sidorna av cuben
                edge[8] = SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+z*cellSize);
                edge[9] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+z*cellSize);
                edge[10] = SetVector(-1.0f+(x+1)*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+(z+1)*cellSize);
                edge[11] = SetVector(-1.0f+x*cellSize, -1.0f+y*cellSize+cellSize/2.0f, -1.0f+(z+1)*cellSize);            
                */

                //hämta vilket case
                int tmp;
                
                for(k = 0; k<16; k++)
                {
                        tmp = triTable[Case][k];
                        if(tmp != -1)
                        {	
				(*mTris) = realloc((*mTris),(count+3) * sizeof(GLfloat)); /* give the pointer some memory */
				/*
				if ( temp != NULL ) 
				{
					mTris = temp;
	
				} 
				else 
				{
					free(mTris);
					printf("%s", "FFFEEEEEEELL!!!!");				
				}
				*/		

                                (*mTris)[count] = edge[tmp].x;
                                count++;
                                (*mTris)[count] = edge[tmp].y;
                                count++;                                
                                (*mTris)[count] = edge[tmp].z;
                                count++;
                        }
			else
                        	k=15;
                        
                }
		
	}
	
	int normalCount = 0;
	vec3 normal, u, v, v1, v2;

	(*mNorms) = realloc((*mNorms),count * sizeof(GLfloat));

	while(normalCount < count){
		v1 = SetVector((*mTris)[normalCount], (*mTris)[normalCount+1], (*mTris)[normalCount+2]);
		v2 = SetVector((*mTris)[normalCount+3], (*mTris)[normalCount+4], (*mTris)[normalCount+5]);
		u = Normalize(VectorSub(v2, v1));
		
		v2 = SetVector((*mTris)[normalCount+6], (*mTris)[normalCount+7], (*mTris)[normalCount+8]);
		v = Normalize(VectorSub(v2, v1));

		normal = Normalize(CrossProduct(u, v));
		
		(*mNorms)[normalCount] = normal.x;
		(*mNorms)[normalCount+1] = normal.y;
		(*mNorms)[normalCount+2] = normal.z;

		(*mNorms)[normalCount+3] = normal.x;
		(*mNorms)[normalCount+4] = normal.y;
		(*mNorms)[normalCount+5] = normal.z;

		(*mNorms)[normalCount+6] = normal.x;
		(*mNorms)[normalCount+7] = normal.y;
		(*mNorms)[normalCount+8] = normal.z;	
		/*printf("%f, ", (*mNorms)[normalCount]);
		printf("%f, ", (*mNorms)[normalCount+1]);
		printf("%f, ", (*mNorms)[normalCount+2]);
		printf("%s", "\n");*/
		normalCount+=9;
	}
	//Ge triSize storleken på det allokerade minnet

	triSize = (count) * sizeof(GLfloat); 
	//return mTris;
	//printf("%i", triSize);
}	

/* 	// Interpolation i marching cubes...

	http://www3.cs.stonybrook.edu/~mueller/teaching/cse564/marchingCubes.pdf
	
	iso = isovalue
	u = distance between corner and interpolated point (typ.. avstandet mellan hornet och där triangelns horn är.) 
	v1, v2 = obtained by central differencing https://en.wikipedia.org/wiki/Central_differencing_scheme  
	g1, g2 = gradient vectors	


	iso = v1 * ( 1 - u ) + v2 * u
	u = (v1 - iso) / (v1 - v2)

	interpolate vertex color:
	
	c1 = u * c2 + ( 1 - u ) * c1

	interpolate vertex normal: 

	n1 = u * g2 + ( 1 - u ) * g1
=======
	triSize = count * sizeof(GLfloat); 
	//return mTris;
	//printf("%i, ", triSize);
}	
//http://www.paulbourke.net/geometry/polygonise/
// Linear interpolation 
vec3 interpolate(vec3 p1, vec3 p2, double valP1, double valP2)
{
	//Point to be calculated
	vec3 p; 
	double mu;
 
	//If the value of point 1 is the same as the isovalue
	if (abs(isolevel-valP1) < 0.00001)   
		return(p1);  
	//If the value of point 2 is the same as the isovalue
	if (abs(isolevel-valP2) < 0.00001)  
		return(p2);
	
	if (abs(valp1-valP2) < 0.00001)
		return(p1);
	//Calculate weight
	mu = (isolevel - valP1) / (valP2 - valP1); 

	//Will depend on if the weighting is in x, y or z
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);
>>>>>>> Strut

	// Remove Ambiguities Using the Asymptotic Decider Method 
	// ??????????????????????????????????????????????????????	

	Salpha = (B00 - B01) / (B00 + B11 - B01 - B10)
	Talpha = (B00 - B01) / (B00 + B11 - B01 - B10)

	- a surface created by bilinear interpolation

	function (1-s, s) [B00, B01; B10, B11] (1-t, t)

	Give rise to 2 hypoerbolas B(s,t) = alpha (isovalue) 

	-Ambiguity: both hyperbolas intersect domain (0,0) and (1,1)

	-Resolve ambiguity by comparing B(Salpha, Talpha) with alpha 

*/


//http://paulbourke.net/geometry/polygonise/
int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
