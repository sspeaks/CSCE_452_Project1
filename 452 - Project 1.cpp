#include "stdafx.h"
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/gl.h>
#include <GL/glut.h>
#include <gl/GLU.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
#define PI 3.14159265358979323846

static int shoulder = 0, elbow = 0, head = 0;

GLfloat size = .1;
GLfloat mat[16];
GLfloat mat2[16];
GLfloat mat3[16];

bool calc = false;
bool inLoop = false;
vector<double> v(1, - 10000);

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void squareBrush(float x, float y)
{
	float p1[3] = {x +(.75*size), y+(.75*size), 0};
	float p2[3] = {x +(.75*size), y-(.75*size), 0};
	float p4[3] = {x -(.75*size), y+(.75*size), 0};
	float p3[3] = {x -(.75*size), y-(.75*size), 0};
	glBegin (GL_POLYGON);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
		glVertex3fv(p4);
	glEnd();
	glFlush();
}

void circle(double x, double y)
{
	int radius = 1;
	double i = 0;
	double changeInAngle = 0.01;
	glBegin(GL_POLYGON);
	for(i = 0; i < 2*PI; i += 0.01){
		glVertex3f((x+cos(i)*radius), (y+sin(i)*radius), 0.0);
	}
	glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   GLdouble MyMatrix[16];

   glMatrixMode(GL_MODELVIEW_MATRIX);

   glPushMatrix();

   //joint 1
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 1.0, 0.0);
   glPushMatrix();
   glScalef (0.2, 1.5, 0);
   glGetFloatv(GL_MODELVIEW_MATRIX,mat);
   glutSolidCube (1.0);
   glPopMatrix();

   //joint 2
   glTranslatef (0.0, 0.6250, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.6250, 0.0);
   glPushMatrix();
   glScalef (0.2, 1.0, 0);
   glGetFloatv(GL_MODELVIEW_MATRIX,mat2);
   glutSolidCube (1.0);
   glPopMatrix();

   //3
   glTranslatef (0.0, 0.43750, 0.0);
   glRotatef ((GLfloat) head, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.43750, 0.0);
   glPushMatrix();
   glScalef (0.2, 0.75, 0);
   glGetFloatv(GL_MODELVIEW_MATRIX,mat3);
   glutSolidCube (1.0);
   glPopMatrix();
   glPopMatrix();

	if(calc)
	{
		int spot = 0;
		while(v.at(spot) != -10000)
			spot++;
		if(v.size() - spot <= 2)
			v.resize(v.size()+2, -10000);
		v[spot] = mat[4]+mat2[4]+mat3[4];
		spot++;
		v[spot] = mat[5]+mat2[5]+mat3[5];
		
		calc = false;
	}
	squareBrush((mat[4]+mat2[4]+mat3[4]), (mat[5]+mat2[5]+mat3[5]));

	int count = 0;
	double x_1, y_1;
	while(v.at(count) != -10000)
	{
		x_1 = v.at(count);
		count++;
		y_1 = v.at(count);
		count++;
		squareBrush(x_1,y_1);
	}


   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
		case 'c':
		calc = true;
		 glutPostRedisplay();
		 break;
		case 's':
         shoulder = (shoulder + 1) % 360;
         glutPostRedisplay();
         break;
		case 'S':
         shoulder = (shoulder - 1) % 360;
         glutPostRedisplay();
         break;
		case 'e':
         elbow = (elbow + 1) % 360;
         glutPostRedisplay();
         break;
		case 'E':
         elbow = (elbow - 1) % 360;
         glutPostRedisplay();
         break;
		case 'h':
         head = (head + 1) % 360;
         glutPostRedisplay();
         break;
		case 'H':
         head = (head - 1) % 360;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}


int main(int argc, char** argv)
{
	Fl_Window win( 400,300,"Testing" );
    win.begin();

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);

   win.end();

   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   glutMainLoop();

   return Fl::run();
   return 0;
}