#include <gl/glut.h>
#include <gl/glui.h>
#include <vector>
#include <gl/GLU.h>
#include <cmath>
#include <stdlib.h>
#include <conio.h>      
#pragma comment(lib, "ws2_32.lib")      
#include <iostream>
#include <winsock2.h>   
#include <thread>
#include <sstream>

using namespace std;

int clientLogic(SOCKET mySocket, const sockaddr* connectionAddr);
int getReadStatus(const SOCKET a_socket);
int endBrokerSocket(SOCKET socket);
void finalWSACleanup();
int result;
SOCKET g_ocket;
bool setUp;
bool drawn = false;
int sleep_val;

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


int window_X_position;
int window_Y_position;


int window_width = 750;
int window_height = 750;


char *window_title = "Painting Robot Arm";


GLuint main_window;


GLUI * glui_subwindow;


int glui_subwindow_x, glui_subwindow_y;


int draw = 0;


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
void glui_callback14(int arg);
void glui_callback15(int arg);


int fullScreen = 0;

void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

void setupGLUI();


void centerOnScreen(void);

bool calc = false;
bool inLoop = false;
vector<double> v(1, -10000);

int getReadStatus(const SOCKET a_socket)
{
	
	static const timeval instantSpeedPlease = { 0, 0 };
	fd_set a = { 1, { a_socket } };
	
	int result = select(0, &a, 0, 0, &instantSpeedPlease);
	if (result == SOCKET_ERROR)
	{
		result = WSAGetLastError();
	}
	if (result != 0 && result != 1)
	{
		printf("select() error %d\n", result);
		return SOCKET_ERROR;
	}
	return result;
}

void finalWSACleanup() 
{
	int result = WSACleanup();
	if (result != 0)
	{
		printf("WSACleanup() error %d\n", result);
	}
}
int clientInit()
{
	sockaddr_in connectionAddress;
	unsigned short myPort = 1337;
	
	memset(&connectionAddress, 0, sizeof(sockaddr_in)); 
	connectionAddress.sin_family = AF_INET;
	
	connectionAddress.sin_port = htons(myPort);
	
	int result;
	WSADATA wsaData; 
	
	if ((result = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0)
	{
		printf("WSAStartup() error %d\n", result);
		return EXIT_FAILURE;
	}
	atexit(finalWSACleanup); 
	
	SOCKET mySocket = socket(
		
		AF_INET,
		
		SOCK_STREAM,
		
		IPPROTO_TCP);
	if (mySocket == INVALID_SOCKET)
	{
		printf("socket() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
	
	unsigned long argp = 1;
	
	result = setsockopt(mySocket,
		SOL_SOCKET,
		
		SO_REUSEADDR,
		(char *)&argp, sizeof(argp));
	if (result != 0)
	{
		printf("setsockopt() error %d\n", result);
		return EXIT_FAILURE;
	}
	
	argp = 1;
	
	if (ioctlsocket(mySocket,
		
		FIONBIO,
		&argp) == SOCKET_ERROR)
	{
		printf("ioctlsocket() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
		
		char * targetIP = "127.0.0.1";
		unsigned long raw_ip_nbo = inet_addr(targetIP);
		if (raw_ip_nbo == INADDR_NONE)
		{
			printf("inet_addr() error \"%s\"\n", targetIP);
			return EXIT_FAILURE;
		}
		connectionAddress.sin_addr.s_addr = raw_ip_nbo;
		result = clientLogic(mySocket, (const sockaddr*)&connectionAddress);
	if (result == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	if (mySocket != INVALID_SOCKET)
	{
		result = closesocket(mySocket); 
		if (result != 0)
		{
			printf("closesocket() error %d\n", WSAGetLastError());
			return EXIT_FAILURE;
		}
		mySocket = INVALID_SOCKET;
	}
	return EXIT_SUCCESS;
}
int clientLogic(SOCKET mySocket, const sockaddr* connectionAddress)
{
	g_ocket = mySocket;
	result = connect(mySocket, connectionAddress, sizeof(sockaddr_in));
	if (result == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		switch (errorCode)
		{
		case WSAEISCONN:        printf("already connected!\n"); break;
		case WSAEWOULDBLOCK:
		case WSAEALREADY:
			break;
		default:
			printf("client connect() error %d\n", errorCode);
			setUp = false;
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("connected\n");
		setUp = true;
	}
	
	int iterations = 0;
	bool sendit = false;
	char userTextField[1024];
	userTextField[0] = '\0';
	int userTextFieldCursor = 0;
	int userInput;
	while (getReadStatus(mySocket) != SOCKET_ERROR)
	{
		
		
		
		if (getReadStatus(mySocket) == 1)
		{
			unsigned long howMuchInBuffer = 0;
			unsigned long numBytesRead = 0;
			
			stringstream input;
			do
			{
				ioctlsocket(mySocket, FIONREAD, &howMuchInBuffer);
				
				int result = recv(mySocket,
					(char*)(&userInput), sizeof(userInput), 0);
				
				
				
				if (result == SOCKET_ERROR)
				{
					printf("client recv() error %d\n", WSAGetLastError());
					return EXIT_FAILURE;
				}
				for (int i = 0; i < result; ++i)
				{
					
					input << ((char*)(&userInput))[i];
					numBytesRead++;
				}
				howMuchInBuffer -= result;
			} while (howMuchInBuffer > 0);
			
				
			int t;
			input >> shoulder;
			input >> elbow;
			input >> head;
			input >> t;
			input >> drawdot_x;
			input >> drawdot_y;
			if (t == 1)
				calc = true;
		
			
			glutPostRedisplay();
		
			
		}
		else
		{
			
		}
	}
	return EXIT_SUCCESS;
}
void init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);                   
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);    
	glShadeModel(GL_SMOOTH);   
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
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

	
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -7.0f);  

	glPushMatrix();
	glTranslatef(0.0f, -1.0f, 0.0f);
	glRotatef((GLfloat)shoulder, 0.0f, 0.0f, 1.0f);

	glTranslatef(0.0f, +1.0f, 0.0f);



	

	glPushMatrix();
	glTranslatef(0.0f, 1.66f, 0.0f);
	glTranslatef(0.0f, -.66f, 0.0f);
	glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);

	glTranslatef(0.0f, .66f, 0.0f);





	
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
void send_function()
{
	stringstream ss;
	ss << "\0" << shoulder << " " << elbow << " " << head << " " << drawn << " " << drawdot_x << " " <<  drawdot_y;
	string send_s = ss.str();
	char userTextField[1024];
	memcpy(userTextField, send_s.c_str(), send_s.size() * sizeof(char));
	int userTextFieldCursor = send_s.size();
	_sleep(sleep_val);
	result = send(g_ocket, (const char *)userTextField, userTextFieldCursor, 0);
	if (result == SOCKET_ERROR)
	{
		printf("client send() error %d\n", WSAGetLastError());
	}
	userTextFieldCursor = 0;
	userTextField[userTextFieldCursor] = '\0';
	drawn = false;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		
		calc = true;
		glutPostRedisplay();
		drawn = true;
		break;
	case'v':
		
		heuristicInverseK(drawdot_x, drawdot_y);
		glutPostRedisplay();
		drawn = true;
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
	send_function();
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
	
	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutMouseFunc(processMouse);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutTimerFunc(0, timer, 0);

	GLUI_Master.set_glutIdleFunc(NULL);

	

	
	glui_subwindow = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

	
	glui_subwindow->set_main_gfx_window(main_window);

	
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
	glui_subwindow->add_button("Delay", 13, glui_callback14);
	glui_subwindow->add_button("No Delay", 14, glui_callback15);
	glui_subwindow->add_button(" QUIT ", 7, glui_callback8);


	
}

void glui_callback(int arg)
{
	
	shoulder = (shoulder + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();

}
void glui_callback2(int arg)
{
	
	shoulder = (shoulder - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();

}
void glui_callback3(int arg)
{
	
	elbow = (elbow + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();
}
void glui_callback4(int arg)
{
	
	elbow = (elbow - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();
}
void glui_callback5(int arg)
{
	
	head = (head + 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();
}
void glui_callback6(int arg)
{
	
	head = (head - 1) % 360;
	drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
	drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
	glutPostRedisplay();
	send_function();
}
void glui_callback7(int arg)
{
	
	calc = true;
	drawn = true;
	glutPostRedisplay();
	send_function();

}
void glui_callback8(int arg)
{
	
	exit(1);
	send_function();
}
void glui_callback9(int arg)
{
	
	drawdot_x += .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
	send_function();
}
void glui_callback10(int arg)
{
	
	drawdot_x -= .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
	send_function();
}
void glui_callback11(int arg)
{
	
	drawdot_y += .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
	send_function();
}
void glui_callback12(int arg)
{
	
	drawdot_y -= .05;
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
	send_function();
}
void glui_callback13(int arg){
	
	heuristicInverseK(drawdot_x, drawdot_y);
	glutPostRedisplay();
	send_function();
}
void glui_callback14(int arg){
	sleep_val = 2000;
}
void glui_callback15(int arg){
	sleep_val = 0;
}




void centerOnScreen(void)
{
	
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

	thread t1(clientInit);

	glutMainLoop();

	t1.join();


	return 0;
}

