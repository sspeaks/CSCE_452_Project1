#include "stdafx.h"
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <Fl/Gl.h>
#include <GL/glut.h>
#include <gl/GLU.h>

static int shoulder = 0, elbow = 0, head = 0;


void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
	//joint 1
   glClear (GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glPushMatrix();
   glScalef (0.2, 1.5, 0);
   glutSolidCube (1.0);
   glPopMatrix();

   //joint 2
   glTranslatef (0.0, 0.6250, 0.0);//.6250
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.6250, 0.0);
   glPushMatrix();
   glScalef (0.2, 1.0, 0);
   glutSolidCube (1.0);
   glPopMatrix();

   //3
	
   glTranslatef (0.0, 0.43750, 0.0);
   glRotatef ((GLfloat) head, 0.0, 0.0, 1.0);
   glTranslatef (0.0, 0.43750, 0.0);
   glPushMatrix();
   glScalef (0.2, 0.75, 0);
   glutSolidCube (1.0);
   glPopMatrix();
   
   glPopMatrix();
   glutSwapBuffers();
}

void MyMouse (int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	}
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
	Fl_Window *window = new Fl_Window(300,180);
	window ->show(argc, argv);
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   glutMouseFunc(MyMouse);
   glutMainLoop();
   return 0;
}