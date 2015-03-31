#include <stdio.h>              
#include <winsock2.h>   
#pragma comment(lib, "ws2_32.lib")      
#include <vector>       
#include <conio.h>      

int serverLogic(SOCKET mySocket, sockaddr * connectionAddress);
int clientLogic(SOCKET mySocket, const sockaddr* connectionAddr);
int getReadStatus(const SOCKET a_socket);
int endBrokerSocket(SOCKET socket);
void finalWSACleanup();

int main()
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
	
	result = serverLogic(mySocket, (sockaddr*)&connectionAddress);
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

int serverLogic(SOCKET mySocket, sockaddr * connectionAddress)
{
	
	int result = bind(mySocket, connectionAddress, sizeof(sockaddr_in));
	if (result == SOCKET_ERROR)
	{
		printf("bind() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
	result = listen(mySocket, 10);
	if (result == SOCKET_ERROR)
	{
		printf("listen() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
	
	struct ClientSocket {
		SOCKET socket;
		sockaddr address;
	};
	
	std::vector<ClientSocket> clientSockets;
	
	std::vector<char> inputBuffer;
	
	int iterations = 0;
	while ((result = getReadStatus(mySocket)) != SOCKET_ERROR)
	{
		if (_kbhit() && _getch() == 27)  
		{
			break; 
		}
		if (result == 1)
		{
			printf("connecting client...");
			int client_size = sizeof(sockaddr_in);
			ClientSocket clientSocket;
			clientSocket.socket = accept(mySocket,
				(struct sockaddr*)&clientSocket.address, &client_size);
			if (clientSocket.socket == INVALID_SOCKET)
			{
				printf("accept() error %d\n", WSAGetLastError());
				return EXIT_FAILURE;
			}
			clientSockets.push_back(clientSocket);
			printf("\rconnected client %d\n", clientSockets.size());
		}
		else
		{
			printf("server: %d\r", iterations++);
			
			inputBuffer.clear();
			for (unsigned int i = 0; i < clientSockets.size(); ++i)
			{
				int status = getReadStatus(clientSockets[i].socket);
				if (status == 1)
				{
					if (clientSockets[i].socket == INVALID_SOCKET)
					{
						continue;
					}
					unsigned long howMuchInBuffer = 0;
					ioctlsocket(clientSockets[i].socket, FIONREAD,
						&howMuchInBuffer);
					if (howMuchInBuffer > 0)
					{
						unsigned long used = inputBuffer.size();
						inputBuffer.resize(used + howMuchInBuffer, 0);
						result = recv(clientSockets[i].socket,
							(char*)(&inputBuffer[used]), howMuchInBuffer, 0);
						
						
						
						if (result == SOCKET_ERROR)
						{
							printf("client %d recv() error %d\n", i,
								WSAGetLastError());
							return EXIT_FAILURE;
						}
					}
				}
				else if (status == SOCKET_ERROR)
				{
					printf("\rclient %d read error\n", i);
					int result = endBrokerSocket(clientSockets[i].socket);
					if (result == EXIT_FAILURE)
					{
						return EXIT_FAILURE;
					}
					clientSockets[i].socket = INVALID_SOCKET;
				}
			}
			if (inputBuffer.size() > 0)
			{
				
				printf("sending: \"");
				for (unsigned int i = 0; i < inputBuffer.size(); ++i)
				{
					printf("%c", inputBuffer[i]);
				}
				printf("\" %d bytes\n", inputBuffer.size());
				
				for (unsigned int i = 0; i < clientSockets.size(); ++i)
				{
					if (clientSockets[i].socket == INVALID_SOCKET)
					{
						continue;
					}
					
					
					
					result = send(clientSockets[i].socket,
						(const char*)&inputBuffer[0], inputBuffer.size(), 0);
					if (result == SOCKET_ERROR) 
					{
						printf("\rclient %d send error %d\n", i,
							WSAGetLastError());
						int result = endBrokerSocket(clientSockets[i].socket);
						if (result == EXIT_FAILURE)
						{
							return EXIT_FAILURE;
						}
						clientSockets[i].socket = INVALID_SOCKET;
					}
				}
			}
			
			if (clientSockets.size() > 0)
			{
				for (int i = (signed)clientSockets.size() - 1; i >= 0; --i)
				{
					if (clientSockets[i].socket == INVALID_SOCKET)
					{
						clientSockets.erase(clientSockets.begin() + i);
					}
				}
			}
		}
	}
	
	for (unsigned int i = 0; i < clientSockets.size(); ++i)
	{
		int result = endBrokerSocket(clientSockets[i].socket);
		if (result == EXIT_FAILURE)
		{
			return EXIT_FAILURE;
		}
		clientSockets[i].socket = INVALID_SOCKET;
	}
	return EXIT_SUCCESS;
}

int endBrokerSocket(SOCKET socket)
{
	int result = shutdown(socket, SD_BOTH);
	if (result != 0)
	{
		printf("socket shutdown() error %d\n", WSAGetLastError());
	}
	result = closesocket(socket);
	if (result != 0)
	{
		printf("socket closesocket() error %d\n", WSAGetLastError());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


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