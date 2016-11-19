#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char *message);

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	char *Port = "1235";

	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	SOCKADDR_IN ServerAddr, ClientAddr;
	int ClientAddrSize = sizeof(ClientAddr);
	int option = 1;

	char Message[BUF_SIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
		ErrorHandling("Socket() error");

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(atoi(Port));
	
	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrSize);

	send(ClientSocket, "Are you Read?", strlen("Are you Read?"), 0);
	recv(ClientSocket, Message, BUF_SIZE, 0);

	printf("%s \n", Message);

	closesocket(ServerSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}