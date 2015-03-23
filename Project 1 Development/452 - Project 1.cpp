#include "stdafx.h"
#include <stdlib.h>
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
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

   GLdouble MyMatrix[16];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   //joint 1
   glColor3f(1.0f, 1.0f, 1.0f);
   glTranslatef(0.0f, 0.0f, -7.0f);  // Move  into the screen
   
   glPushMatrix();
   glTranslatef(0.0f, -1.0f, 0.0f);
   glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);
   
   glTranslatef(0.0f, +1.0f, 0.0f);
   
   
   
   //joint 2
   
   glPushMatrix();
   glTranslatef(0.0f, 1.66f, 0.0f);
   glTranslatef(0.0f, -.66f, 0.0f);
   glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
   
   glTranslatef(0.0f, .66f, 0.0f);
  
   
   
   
   
   //3
   glPushMatrix();
	
   glTranslatef (0.0f, 1.06f, 0.0f);
   glTranslatef(0.0f, -.4f, 0.0f);
   glRotatef ((GLfloat) head, 0.0, 0.0, 1.0);
   
   glTranslatef(0.0f, +.4f, 0.0f);
   glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
   glScalef (0.1, 0.4f, .1f);
   glutSolidSphere (1.0,15,15);
   
   
  
   
   glPopMatrix();
   glGetFloatv(GL_MODELVIEW_MATRIX, mat2);
   glScalef(.1f, 0.66f, 0.1f);
   glutSolidSphere(1.0, 15, 15);
  
   
   glPopMatrix();
   glGetFloatv(GL_MODELVIEW_MATRIX, mat);
   glScalef(.1f, 1.0f, 0.1f);
   glutSolidSphere(1.0, 15, 15);
   
   
   glPopMatrix();

	if(calc)
	{
		int spot = 0;
		while(v.at(spot) != -10000)
			spot++;
		if(v.size() - spot <= 2)
			v.resize(v.size()+2, -10000);
		v[spot] = 2.0f*mat[4] + .66f * 2 * mat2[4] + .4 * 2 * mat3[4];
		spot++;
		v[spot] = 2.0f*mat[5] + .66f*2* mat2[5] + .4*2 *mat3[5];
		
		calc = false;
	}

	int count = 0;
	double x_1, y_1;
	while(v.at(count) != -10000)
	{
		x_1 = v.at(count);
		count++;
		y_1 = v.at(count);
		count++;
		glPushMatrix();
		glTranslatef(0.0f, -1.0f, 0);
		glTranslatef(x_1, y_1, 0);
		glScalef(0.1f, 0.1f, 0.1f);
		glutSolidSphere(1.0, 15, 15);
		glPopMatrix();
		
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
	glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(800, 600);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("OpenGL Test");
  
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   init();
   
   glutKeyboardFunc(keyboard);

   glutMainLoop();

   return 0;
}