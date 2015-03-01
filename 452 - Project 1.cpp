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
using namespace std;

static int shoulder = 0, elbow = 0, head = 0;
int randcount = 0;
GLfloat size = .1;
GLfloat mat[16];
GLfloat mat2[16];
GLfloat mat3[16];
/*
bool calc = false;
vector<double> v(16);
int cnt = 0;
*/

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void squareBrush(float x, float y)
{

	float p1[3] = {x +(.5*size), y+(.5*size), 0};
	float p2[3] = {x +(.5*size), y-(.5*size), 0};
	float p4[3] = {x -(.5*size), y+(.5*size), 0};
	float p3[3] = {x -(.5*size), y-(.5*size), 0};
	glBegin (GL_POLYGON);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
		glVertex3fv(p4);
	glEnd();
	glFlush();

}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);

   GLdouble MyMatrix[16];

   glMatrixMode(GL_MODELVIEW_MATRIX);

   glPushMatrix();

   //joint 1
   glTranslatef (0.0, -1.0, 0.0);
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 1.0, 0.0);
   glPushMatrix();
   glScalef (0.2, 1.5, 0);
   glGetFloatv(GL_MODELVIEW_MATRIX,mat);
   glutSolidCube (1.0);
   glPopMatrix();

   //joint 2
   
   glTranslatef (0.0, 0.6250, 0.0);//.6250
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

   //end effector
   glTranslatef (0.0, 0.20, 0.0);
   glTranslatef (0.0, 0.20, 0.0);
   glPushMatrix();
   glScalef (0.05, 0.05, 0);
   glutSolidCube (1.0);
   glPopMatrix();

   glPopMatrix();

   if (randcount == 1){
   squareBrush(mat[1]*4,mat[1]*4);
   }

	GLfloat sum[16];
	GLfloat sum_all[16];
   for (int i = 0; i < 4; i++){
	   for (int j = 0; j < 4; j++) {
		   sum[4*i+j] = 0;
		   for (int k = 0; k < 4; k++){
				sum[4*i+j] += mat[4*i+j] * mat2[4*i*j];
		   }
	   }
   }
    for (int i = 0; i < 4; i++){
	   for (int j = 0; j < 4; j++) {
		   sum_all[4*i+j] = 0;
		   for (int k = 0; k < 4; k++){
				sum_all[4*i+j] += sum[4*i+j] * mat3[4*i*j];
		   }
	   }
   }
	for (int i = 0; i < 16; i++){
		  printf("%f ", sum_all[i]);
	   }
	printf("\n");
  /* 
  // glGetDoublev(GL_MODELVIEW_MATRIX, MyMatrix);
   int i, j;
   
	//printf("x=%f, y=%f\n",c[0],c[1]);

   GLdouble b[] = {0, 2.5, 0, 1};
   GLdouble c[(sizeof(b)/sizeof(GLdouble))];
	GLdouble temp = 0;
	int count = 0; 
	for(j = 0; j < sizeof(mat)/sizeof(GLdouble); j++)
	{
		if(j != 0 && j%4 == 0)
		{
			c[count] = temp;
			temp = 0;
			count++;
		}
		temp += mat[j] * b[j%4];	
	}
	squareBrush(c[0],c[1]);
   
	for(i = 0; i < 16; i++)
	{
		if(i%4 == 0)
			printf("\n");
		printf("%f,", MyMatrix[i]);
	}
	printf("\n");
	*/
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
	  // calc = true;
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
		case 'q':
		  //squareBrush(mat[0],mat[1]);
		  randcount = 1;
		  break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

void but_cb( Fl_Widget* o, void*  ) {
   Fl_Button* b=(Fl_Button*)o;
   b->label("Good job"); //redraw not necessary
  
   b->resize(10,150,140,30); //redraw needed
   b->redraw();
}

int main(int argc, char** argv)
{
	Fl_Window win( 300,200,"Testing" );
    win.begin();
       Fl_Button but( 10, 150, 70, 30, "Click me" );
    but.callback( but_cb );
   // win.show();
  //  return Fl::run();

   //glutInit(&argc, argv);

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