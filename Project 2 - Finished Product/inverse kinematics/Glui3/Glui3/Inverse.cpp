/****************************************************************************

Subwindow Control Scheme.

-----------------------------------------------------------------------

Trevor Barnett

****************************************************************************/

//*************************************************************************
//
//  File Name: GLUI SubWindow Template
//  Author   : Ali Baderedine
//  
//  Description:
//  
//*************************************************************************

#include <gl/glut.h>
#include <gl/glui.h>
#include <vector>
#include <gl/GLU.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>

using namespace std;
#define PI 3.14159265358979323846
int refreshMills = 15;

static int shoulder = 0, elbow = 0, head = 0;
static int goalS = 0, goalE = 0, goalH = 0;

GLfloat size = .1;
GLfloat mat[16];
GLfloat mat2[16];
GLfloat mat3[16];

GLfloat drawdot_x = 0.0;
GLfloat drawdot_y = 4.12;

//  define the window position on screen
int window_X_position;
int window_Y_position;

//  variables representing the window size
int window_width = 750;
int window_height = 750;

//  variable representing the window title
char *window_title = "Painting Robot Arm";

//  The id of the main window
GLuint main_window;

//  pointer to a GLUI window
GLUI * glui_subwindow;

//  The x and y coordinates of the glui window
int glui_subwindow_x, glui_subwindow_y;

//  Declare live variables (related to GLUI)
int draw = 0;

//  Declare callbacks related to GLUI
void glui_callback(int arg);
void glui_callback2(int arg);
void glui_callback3(int arg);
void glui_callback4(int arg);
void glui_callback5(int arg);
void glui_callback6(int arg);
void glui_callback7(int arg);
void glui_callback8(int arg);
void glui_callback9(int arg);
void glui_callback10(int arg);
void glui_callback11(int arg);
void glui_callback12(int arg);
void glui_callback13(int arg);

//  Tells whether to display the window full screen or not
int fullScreen = 0;

void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

void setupGLUI();

//  This function centers your window on the screen
void centerOnScreen(void);

bool calc = false;
bool inLoop = false;
vector<double> v(1, -10000);

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
	float p1[3] = { x + (.75*size), y + (.75*size), 0 };
	float p2[3] = { x + (.75*size), y - (.75*size), 0 };
	float p4[3] = { x - (.75*size), y + (.75*size), 0 };
	float p3[3] = { x - (.75*size), y - (.75*size), 0 };
	glBegin(GL_POLYGON);
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
	for (i = 0; i < 2 * PI; i += 0.01){
		glVertex3f((x + cos(i)*radius), (y + sin(i)*radius), 0.0);
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

	glTranslatef(0.0f, 1.06f, 0.0f);
	glTranslatef(0.0f, -.4f, 0.0f);
	glRotatef((GLfloat)head, 0.0, 0.0, 1.0);

	glTranslatef(0.0f, +.4f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat3);
	glScalef(0.1, 0.4f, .1f);
	glutSolidSphere(1.0, 15, 15);




	glPopMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat2);
	glScalef(.1f, 0.66f, 0.1f);
	glutSolidSphere(1.0, 15, 15);


	glPopMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	glScalef(.1f, 1.0f, 0.1f);
	glutSolidSphere(1.0, 15, 15);


	glPopMatrix();

	if (calc)
	{
		int spot = 0;
		while (v.at(spot) != -10000)
			spot++;
		if (v.size() - spot <= 2)
			v.resize(v.size() + 2, -10000);
		v[spot] = 2.0f*mat[4] + .66f * 2 * mat2[4] + .4 * 2 * mat3[4];
		//cout << "Drawn at " << 2.0f*mat[4] + .66f * 2 * mat2[4] + .4 * 2 * mat3[4] << " " << 2.0f*mat[5] + .66f * 2 * mat2[5] + .4 * 2 * mat3[5] << endl;
		spot++;
		v[spot] = 2.0f*mat[5] + .66f * 2 * mat2[5] + .4 * 2 * mat3[5];

		calc = false;
	}

	int count = 0;
	double x_1, y_1;
	while (v.at(count) != -10000)
	{
		x_1 = v.at(count);
		count++;
		y_1 = v.at(count);
		count++;
		glColor3f(0.5f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(0.0f, -1.0f, 0);
		glTranslatef(x_1, y_1, 0);
		glScalef(0.1f, 0.1f, 0.1f);
		glutSolidSphere(1.0, 15, 15);
		glPopMatrix();

	}

	glColor3f(.7f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0);
	glTranslatef(drawdot_x, drawdot_y, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();

	glutSwapBuffers();
}




double getDistance(float x, float y, double angle1, double angle2, double angle3)
{

	double xdist = pow(2 * cos(angle1*PI / 180) + 1.32 * cos(angle2*PI / 180) + 0.8 * cos(angle3*PI / 180) - x, 2);

	double ydist = pow(2 * sin(angle1*PI / 180) + 1.32 * sin(angle2*PI / 180) + 0.8 * sin(angle3*PI / 180) - y, 2);

	return sqrt(xdist + ydist);
}

void heuristicInverseK(float x, float y)
{
	double angle1 = shoulder + 90;
	double angle2 = angle1 + elbow;
	double angle3 = angle2 + head;
	double minimumDistance;
	double lastDistance = -1;

	minimumDistance = getDistance(x, y, angle1, angle2, angle3);

	double curDist = minimumDistance;
	while (minimumDistance != lastDistance)
	{
		lastDistance = minimumDistance;

		while (curDist == minimumDistance)
		{
			if ((curDist = getDistance(x, y, angle1, angle2, angle3 + 1)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle3 += 1;
			}
			else if ((curDist = getDistance(x, y, angle1, angle2, angle3 - 1)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle3 -= 1;
			}
		}
		curDist = minimumDistance;
		while (curDist == minimumDistance)
		{
			if ((curDist = getDistance(x, y, angle1, angle2 + 1, angle3)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle2 += 1;
			}
			else if ((curDist = getDistance(x, y, angle1, angle2 - 1, angle3)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle2 -= 1;
			}
		}
		curDist = minimumDistance;
		while (curDist == minimumDistance)
		{
			if ((curDist = getDistance(x, y, angle1 + 1, angle2, angle3)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle1 += 1;
			}
			else if ((curDist = getDistance(x, y, angle1 - 1, angle2, angle3)) < minimumDistance)
			{
				minimumDistance = curDist;
				angle1 -= 1;
			}
		}
	}
	shoulder = (int)(angle1 - 90) % 360;
	elbow = (int)(angle2 - angle1) % 360;
	head = (int)(angle3 - angle2) % 360;


}

void processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		heuristicInverseK(drawdot_x, drawdot_y);
	}

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		calc = true;
		glutPostRedisplay();
		break;
	case'v':
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		break;
	case 's':
		shoulder = (shoulder + 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'S':
		shoulder = (shoulder - 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'e':
		elbow = (elbow + 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'E':
		elbow = (elbow - 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'h':
		head = (head + 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'H':
		head = (head - 1) % 360;
		drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
		drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
		glutPostRedisplay();
		break;
	case 'i':
		drawdot_y += .05;
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		break;
	case'j':
		drawdot_x -= .05;
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		break;
	case'k':
		drawdot_y -= .05;
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		break;
	case 'l':
		drawdot_x += .05;
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

void timer(int value) {

	glutPostRedisplay();
	glutTimerFunc(refreshMills, timer, 0);
}



void setupGLUI()
{
	//  Register keyboard, mouse, special, and reshape callbacks of your graphics window
	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutMouseFunc(processMouse);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutTimerFunc(0, timer, 0);

	GLUI_Master.set_glutIdleFunc(NULL);

	// ***** Define glui subwindow (s) *****/

	//  create glui sun window
	glui_subwindow = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

	//  Let each GLUI window you've created know where its main graphics window is
	glui_subwindow->set_main_gfx_window(main_window);

	//  Add controls to the GLUI subwindow
	glui_subwindow->add_button("Joint 1 Left", 0, glui_callback);
	glui_subwindow->add_button("Joint 1 Right", 1, glui_callback2);
	glui_subwindow->add_button("Joint 2 Left", 2, glui_callback3);
	glui_subwindow->add_button("Joint 2 Right", 3, glui_callback4);
	glui_subwindow->add_button("Joint 3 Left", 4, glui_callback5);
	glui_subwindow->add_button("Joint 3 Right", 5, glui_callback6);
	glui_subwindow->add_button("Paint", 6, glui_callback7);
	glui_subwindow->add_button(" +X ", 8, glui_callback9);
	glui_subwindow->add_button(" -X ", 9, glui_callback10);
	glui_subwindow->add_button(" +Y ", 10, glui_callback11);
	glui_subwindow->add_button(" -Y ", 11, glui_callback12);
	glui_subwindow->add_button("seek", 12, glui_callback13);
	glui_subwindow->add_button(" QUIT ", 7, glui_callback8);

	// ***** End of Define glui subwindow (s) *****/
}

void glui_callback(int arg)
{
	shoulder = (shoulder + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback2(int arg)
{
	shoulder = (shoulder - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback3(int arg)
{
	elbow = (elbow + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback4(int arg)
{
	elbow = (elbow - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback5(int arg)
{
	head = (head + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback6(int arg)
{
	head = (head - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
}
void glui_callback7(int arg)
{
	calc = true;
	glutPostRedisplay();
}
void glui_callback8(int arg)
{
	exit(1);
}
void glui_callback9(int arg)
{
	drawdot_x += .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
}
void glui_callback10(int arg)
{
	drawdot_x -= .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
}
void glui_callback11(int arg)
{
	drawdot_y += .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
}
void glui_callback12(int arg)
{
	drawdot_y -= .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
}
void glui_callback13(int arg){
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
}




void centerOnScreen(void)
{
	//  Set the window position such that the window becomes centered
	window_X_position = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
	window_Y_position = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	main_window = glutCreateWindow(window_title);


	init();

	glutTimerFunc(0, timer, 0);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(processMouse);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	setupGLUI();

	glutMainLoop();

	return 0;
}
/* Broken animation
if (goalS != shoulder)
{
if (goalS >= 0 && shoulder >= 0)
{
if (shoulder - goalS < shoulder)
{
if ((shoulder - goalS) % 180 >= 1)
{
shoulder = (shoulder - 1) % 360;
}
else
shoulder = (shoulder + 1) % 360;
}
else
{
if ((goalS - shoulder) % 180 >= 1)
shoulder = (shoulder + 1) % 360;
else
shoulder = (shoulder - 1) % 360;
}
}
if (goalS >= 0 && shoulder < 0)
{
if (abs((shoulder - goalS)) % 180 >= 1)
{
shoulder = (shoulder - 1) % 360;
}
else
shoulder = (shoulder + 1) % 360;
}
if (goalS < 0 && shoulder >= 0)
{
if (abs((shoulder - goalS)) % 180 >= 1)
{
shoulder = (shoulder + 1) % 360;
}
else
shoulder = (shoulder - 1) % 360;
}
if (goalS < 0 && shoulder < 0)
{
if (shoulder - goalS < 0)
{
if (abs((shoulder - goalS)) % 180 >= 1)
{
shoulder = (shoulder - 1) % 360;
}
else
shoulder = (shoulder + 1) % 360;
}
else
{
if (abs(goalS - shoulder) % 180 > 1)
shoulder = (shoulder + 1) % 360;
else
shoulder = (shoulder - 1) % 360;
}
}
}
if (goalE != elbow)
{
if (goalE >= 0 && elbow >= 0)
{
if (elbow - goalE < elbow)
{
if ((elbow - goalE) % 180 >= 1)
{
elbow = (elbow - 1) % 360;
}
else
elbow = (elbow + 1) % 360;
}
else
{
if ((goalE - elbow) % 180 >= 1)
elbow = (elbow + 1) % 360;
else
elbow = (elbow - 1) % 360;
}
}
if (goalE >= 0 && elbow < 0)
{
if (abs((elbow - goalE)) % 180 >= 1)
{
elbow = (elbow - 1) % 360;
}
else
elbow = (elbow + 1) % 360;
}
if (goalE < 0 && elbow >= 0)
{
if (abs((elbow - goalE)) % 180 >= 1)
{
elbow = (elbow + 1) % 360;
}
else
elbow = (elbow - 1) % 360;
}
if (goalE < 0 && elbow < 0)
{
if (elbow - goalE < 0)
{
if (abs((elbow - goalE)) % 180 >= 1)
{
elbow = (elbow - 1) % 360;
}
else
elbow = (elbow + 1) % 360;
}
else
{
if (abs(goalE - elbow) % 180 > 1)
elbow = (elbow + 1) % 360;
else
elbow = (elbow - 1) % 360;
}
}
}
if (goalH != head)
{
if (goalH >= 0 && head >= 0)
{
if (head - goalH < head)
{
if ((head - goalH) % 180 >= 1)
{
head = (head - 1) % 360;
}
else
head = (head + 1) % 360;
}
else
{
if ((goalH - head) % 180 >= 1)
head = (head + 1) % 360;
else
head = (head - 1) % 360;
}
}
if (goalH >= 0 && head < 0)
{
if (abs((head - goalH)) % 180 >= 1)
{
head = (head - 1) % 360;
}
else
head = (head + 1) % 360;
}
if (goalH < 0 && head >= 0)
{
if (abs((head - goalH)) % 180 >= 1)
{
head = (head - 1) % 360;
}
else
head = (head + 1) % 360;
}
if (goalH < 0 && head < 0)
{
if (head - goalH < 0)
{
if (abs((head - goalH)) % 180 >= 1)
{
head = (head - 1) % 360;
}
else
head = (head + 1) % 360;
}
else
{
if (abs(goalH - head) % 180 > 1)
head = (head + 1) % 360;
else
head = (head - 1) % 360;
}
}
}*/
