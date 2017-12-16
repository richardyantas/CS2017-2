// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT

#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#define NUMVTX (8)
#define NUMVTR (20)
#define NUMVTV (8)
static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;	

/*
	
Ejercicio 1.1 Programación OpenGL en C

Intercambie, en el programa C que puede ser encontrado en el sitio de internet del curso (Google Classroom), 
el bloque con comandos glVertex con un bloque que genere las iniciales de su nombre con Quadtrips.
 Utilice además una fuente de luz direccional, así como características de material ambiental y difusa, para iluminar 
 sus iniciales. Recuerde especificar las normales e inicializar los estados de OpenGL correctamente.
Además, piense en el efecto que tendría intercambiar los comandos glRotate y glTranslate en la función drawgraphix.

*/


/*
Ejercicio 1.2 OpenGL 3

 Modificar el código usando OpenGL 3. Para esto usen como base el tutorial descrito en
 http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ 


*/

GLfloat v[NUMVTX][3]={{-1,0,1},{-1,0,-1},
                      {0,-1,1},{0,-1,-1},
                      {1,1,1},{1,1,-1}};

GLfloat LETRA_R[NUMVTR][3] = {{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},
						 {0.0f,6.0f,0.0f},{1.0f,5.0f,0.0f},
						 {2.0f,5.0f,0.0f},{2.0f,6.0f,0.0f},
						 {3.0f,4.0f,0.0f},{4.0f,4.0f,0.0f},
						 {2.0f,3.0f,0.0f},{3.0f,3.0f,0.0f},
						 {2.0f,3.0f,0.0f},{3.0f,2.0f,0.0f},
						 {2.0f,3.0f,0.0f},{4.0f,0.0f,0.0f},
						 {2.0f,3.0f,0.0f},{3.0f,0.0f,0.0f},
						 {2.0f,3.0f,0.0f},{2.0f,2.0f,0.0f},
						 {1.0f,3.0f,0.0f},{1.0f,2.0f,0.0f},
				 };

GLfloat LETRA_V[NUMVTV][3] = { {0.0f,6.0f,0.0f},{1.0f,6.0f,0.0f},
						 {3.0f,0.0f,0.0f},{3.0f,2.0f,0.0f},
						 {4.0f,0.0f,0.0f},{4.0f,2.0f,0.0f},
						 {6.0f,6.0f,0.0f},{7.0f,6.0f,0.0f},					
						};

GLfloat COLOR[NUMVTR][3] = { {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
						 {1,0,0},{1,0,0},
				       };


GLfloat c[NUMVTX][3]={{1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0}};

void init(void) 
{
	/*
   for (int i = 0; i < NUMVTR; ++i)
   {
   		for (int j = 0; j < 2; ++j)
   		{
   			LETRA_R[i][j] = (LETRA_R[i][j]-20)/5;
   		}
		
   }*/

   for (int i = 0; i < NUMVTV; ++i)
   {
   		for (int j = 0; j < 2; ++j)
   		{
   			LETRA_V[i][j] = (LETRA_V[i][j])/5;
   		}		
   }
   
   for (int i = 0; i < NUMVTV; ++i)LETRA_V[i][0] = (LETRA_V[i][0]);		
   

   for (int i = 0; i < NUMVTR; ++i)
   {
   		for (int j = 0; j < 2; ++j)
   		{
   			LETRA_R[i][j] = (LETRA_R[i][j])/5;
   		}		
   }
   
   for (int i = 0; i < NUMVTR; ++i)LETRA_R[i][0] = (LETRA_R[i][0]-2);	


   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glutSolidSphere (1.0, 20, 16);
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


void drawgraphix()
{
	int i;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	glTranslatef(0.0f,0.0f,-3.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);

	glBegin(GL_QUAD_STRIP);

	
	for (i=0; i<NUMVTR; i++) 
	{
		glColor3fv(COLOR[i]);
		glVertex3fv(LETRA_R[i]);
	}

	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTV; i++) 
	{
		glColor3fv(COLOR[i]);
		glVertex3fv(LETRA_V[i]);
	}

	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		drawgraphix();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

int main(int argc,char **argv)
{
   winwidth=winheight=512;

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   glutCreateWindow("Practica 1 - Ejercicio 1.1");


   // Lighting part
   init();

   glutDisplayFunc(drawgraphix);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyboardfunc);
   glutIdleFunc(idlefunc);

   glutMainLoop();

   return(0);
}
