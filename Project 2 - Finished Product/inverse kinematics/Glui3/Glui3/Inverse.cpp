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
#include <conio.h>      // for kbhit() and getch()
#pragma comment(lib, "ws2_32.lib")      // links WinSock2 with MS Visual Studio
#include <iostream>
#include <winsock2.h>   // the networking library
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

int getReadStatus(const SOCKET a_socket)
{
	// zero seconds, zero milliseconds. max time select call allowd to block
	static const timeval instantSpeedPlease = { 0, 0 };
	fd_set a = { 1, { a_socket } };
	/*
	select returns the number of ready socket handles in the fd_set structure, zero if the time limit expired, or SOCKET_ERROR if an error occurred. WSAGetLastError can be used to retrieve a specific error code.
	*/
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

void finalWSACleanup() // callback used to clean up sockets
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
	// initialize the socket's address
	memset(&connectionAddress, 0, sizeof(sockaddr_in)); // initialize to zero
	connectionAddress.sin_family = AF_INET;
	// host-to-network-short: big-endian conversion of a 16 byte value
	connectionAddress.sin_port = htons(myPort);
	// initialize the server socket
	int result;
	WSADATA wsaData; // gets populated w/ info explaining this sockets implementation
	// load Winsock 2.0 DLL. initiates use of the Winsock DLL by a process
	if ((result = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0)
	{
		printf("WSAStartup() error %d\n", result);
		return EXIT_FAILURE;
	}
	atexit(finalWSACleanup); // add callback to trigger when program ends. cleans up sockets
	// create the main socket, either client or server
	SOCKET mySocket = socket(
		/*
		AF_UNSPEC: 0     The address family is unspecified.
		AF_INET: 2       The Internet Protocol version 4 (IPv4) address family.
		AF_IPX: 6        The IPX/SPX address family. This address family is only supported if the NWLink IPX/SPX NetBIOS Compatible Transport protocol is installed. This address family is not supported on Windows Vista and later.
		AF_APPLETALK: 17 The AppleTalk address family. This address family is only supported if the AppleTalk protocol is installed. This address family is not supported on Windows Vista and later.
		AF_NETBIOS: 17   The NetBIOS address family. This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
		AF_INET6: 23     The Internet Protocol version 6 (IPv6) address family.
		AF_IRDA: 26      The Infrared Data Association (IrDA) address family. This address family is only supported if the computer has an infrared port and driver installed.
		AF_BTH: 32       The Bluetooth address family. This address family is supported on Windows XP with SP2 or later if the computer has a Bluetooth adapter and driver installed.
		*/
		AF_INET,
		/*
		SOCK_STREAM: 1    A socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) for the Internet address family (AF_INET or AF_INET6).
		SOCK_DGRAM: 2     A socket type that supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length. This socket type uses the User Datagram Protocol (UDP) for the Internet address family (AF_INET or AF_INET6).
		SOCK_RAW: 3       A socket type that provides a raw socket that allows an application to manipulate the next upper-layer protocol header. To manipulate the IPv4 header, the IP_HDRINCL socket option must be set on the socket. To manipulate the IPv6 header, the IPV6_HDRINCL socket option must be set on the socket.
		SOCK_RDM: 4       A socket type that provides a reliable message datagram. An example of this type is the Pragmatic General Multicast (PGM) multicast protocol implementation in Windows, often referred to as reliable multicast programming. This type is only supported if the Reliable Multicast Protocol is installed.
		SOCK_SEQPACKET: 5 A socket type that provides a pseudo-stream packet based on datagrams.
		*/
		SOCK_STREAM,
		/*
		BTHPROTO_RFCOMM: 3 The Bluetooth Radio Frequency Communications (Bluetooth RFCOMM) protocol. This is a possible value when the af parameter is AF_BTH and the type parameter is SOCK_STREAM. This protocol is supported on Windows XP with SP2 or later.
		IPPROTO_TCP: 6     The Transmission Control Protocol (TCP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_STREAM.
		IPPROTO_UDP: 17    The User Datagram Protocol (UDP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_DGRAM.
		IPPROTO_RM: 113    The PGM protocol for reliable multicast. This is a possible value when the af parameter is AF_INET and the type parameter is SOCK_RDM. On the Windows SDK released for Windows Vista and later, this value is also called IPPROTO_PGM. This protocol is only supported if the Reliable Multicast Protocol is installed.
		*/
		IPPROTO_TCP);
	if (mySocket == INVALID_SOCKET)
	{
		printf("socket() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
	// 1 to set non-blocking, 0 to set re-usable
	unsigned long argp = 1;
	// for complete info on this method, check out http://msdn.microsoft.com/en-us/library/ms740476(VS.85).aspx
	result = setsockopt(mySocket,
		SOL_SOCKET,
		/*
		SO_REUSEADDR: 4 Allows the socket to be bound to an address that is already in use. For more information, see bind. Not applicable on ATM sockets.
		*/
		SO_REUSEADDR,
		(char *)&argp, sizeof(argp));
	if (result != 0)
	{
		printf("setsockopt() error %d\n", result);
		return EXIT_FAILURE;
	}
	// 1 to set non-blocking, 0 to set blocking
	argp = 1;
	// attempt to setup the socket as non-blocking
	if (ioctlsocket(mySocket,
		/*
		FIONBIO:    The *argp parameter is a pointer to an unsigned long value. Set *argp to a nonzero value if the nonblocking mode should be enabled, or zero if the nonblocking mode should be disabled. When a socket is created, it operates in blocking mode by default (nonblocking mode is disabled). This is consistent with BSD sockets.
		FIONREAD:   Use to determine the amount of data pending in the network's input buffer that can be read from socket s. The argp parameter points to an unsigned long value in which ioctlsocket stores the result. FIONREAD returns the amount of data that can be read in a single call to the recv function, which may not be the same as the total amount of data queued on the socket. If s is message oriented (for example, type SOCK_DGRAM), FIONREAD still returns the amount of pending data in the network buffer, however, the amount that can actually be read in a single call to the recv function is limited to the data size written in the send or sendto function call.
		SIOCATMARK: Use to determine if all out of band (OOB) data has been read. (See Windows Sockets 1.1 Blocking Routines and EINPROGRESS for a discussion on OOB data.) This applies only to a stream oriented socket (for example, type SOCK_STREAM) that has been configured for in-line reception of any OOB data (SO_OOBINLINE). On sockets with the SO_OOBINLINE socket option set, SIOCATMARK always returns TRUE and the OOB data is returned to the user as normal data.
		*/
		FIONBIO,
		&argp) == SOCKET_ERROR)
	{
		printf("ioctlsocket() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
		// connect to the server
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
		result = closesocket(mySocket); // server closes, it doesn't shut down
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
	// client loop
	int iterations = 0;
	bool sendit = false;
	char userTextField[1024];
	userTextField[0] = '\0';
	int userTextFieldCursor = 0;
	int userInput;
	while (getReadStatus(mySocket) != SOCKET_ERROR)
	{
		// if user types, remember what's being typed, send with enter.
		/*if (_kbhit())
		{
			userInput = _getch();
			sendit = userInput == '\n' || userInput == '\r';
			if (!sendit)
			{
				putchar(userInput);
				userTextField[userTextFieldCursor++] = userInput;
				userTextField[userTextFieldCursor] = '\0';
				if (userTextFieldCursor >= sizeof(userTextField)-1)
				{
					sendit = true;
				}
			}
			if (sendit)
			{
				// NOTE: for multi-byte values sent across the network
				// htonl or htons should be used to convert 4-byte and
				// 2-byte values respectively.
				result = send(mySocket,
					(const char *)userTextField, userTextFieldCursor, 0);
				if (result == SOCKET_ERROR)
				{
					printf("client send() error %d\n", WSAGetLastError());
					return EXIT_FAILURE;
				}
				userTextFieldCursor = 0;
				userTextField[userTextFieldCursor] = '\0';
			}
		}*/
		// receive data from the server, if there is any
		if (getReadStatus(mySocket) == 1)
		{
			unsigned long howMuchInBuffer = 0;
			unsigned long numBytesRead = 0;
			printf("received: \"");
			stringstream input;
			do
			{
				ioctlsocket(mySocket, FIONREAD, &howMuchInBuffer);
				// 4 bytes at a time out of the socket, stored in userInput
				int result = recv(mySocket,
					(char*)(&userInput), sizeof(userInput), 0);
				// NOTE: for multi-byte values sent across the network
				// ntohl or ntohs should be used to convert 4-byte and
				// 2-byte values respectively.
				if (result == SOCKET_ERROR)
				{
					printf("client recv() error %d\n", WSAGetLastError());
					return EXIT_FAILURE;
				}
				for (int i = 0; i < result; ++i)
				{
					//printf("%c", ((char*)(&userInput))[i]);
					input << ((char*)(&userInput))[i];
					numBytesRead++;
				}
				howMuchInBuffer -= result;
			} while (howMuchInBuffer > 0);
			printf("\" %d bytes%c", numBytesRead,
				((numBytesRead != 0) ? '\n' : '\r'));
			int t;
			input >> shoulder;
			input >> elbow;
			input >> head;
			input >> t;
			if (t == 1)
				calc = true;
			drawdot_x = cos((shoulder + 90)*PI / 180) * 2 + cos((elbow + shoulder + 90)*PI / 180) * 1.32 + cos((head + elbow + shoulder + 90)*PI / 180) * .8;
			drawdot_y = sin((shoulder + 90)*PI / 180) * 2 + sin((elbow + shoulder + 90)*PI / 180) * 1.32 + sin((head + elbow + shoulder + 90)*PI / 180) * .8;
			glutPostRedisplay();
		//	input >> t;
			//printf("s = %d, e = %d, h = %d, t = %d\n", s, e, h, t);
		}
		else
		{
			printf("client: \"%s\" %d\r", userTextField, iterations++);
		}
	}
	return EXIT_SUCCESS;
}
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
	bool drawn = false;
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

	stringstream ss;
	ss << "\0" << shoulder << " " << elbow << " " << head << " " << drawn;
	string send_s = ss.str();
	char userTextField[1024];
	memcpy(userTextField, send_s.c_str(), send_s.size() * sizeof(char));
	int userTextFieldCursor = send_s.size();
	result = send(g_ocket, (const char *)userTextField, userTextFieldCursor, 0);
	if (result == SOCKET_ERROR)
	{
		printf("client send() error %d\n", WSAGetLastError());
	}
	userTextFieldCursor = 0;
	userTextField[userTextFieldCursor] = '\0';
	drawn = false;
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

	thread t1(clientInit);

	glutMainLoop();

	t1.join();


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
