#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>

unsigned WINAPI recvProc(void *arg);
void ErrorHandling(char *message);

#define Recv_BUF_SIZE 65535
#define Send_BUF_SIZE 1234

SOCKET Socket;
SOCKADDR_IN SocketAddr;
WSADATA wsaData;
char RecvBuf[Recv_BUF_SIZE];
char SendBuf[Send_BUF_SIZE];
int main()
{
	HANDLE recvThread;
	WSAStartup(0x101, &wsaData);
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	//fill structure
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SocketAddr.sin_port = htons(12345);
	//bind the socket to the specified port
	connect(Socket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr));
	recvThread = (HANDLE)_beginthreadex(NULL, 0, recvProc, NULL, 0, NULL);
	while (1)
	{
		fflush(stdin);
		scanf_s("%s\n", SendBuf,Send_BUF_SIZE);
		send(Socket, SendBuf, strlen(SendBuf), 0);
	}
	closesocket(Socket);
	WSACleanup();
	return 0;
}

unsigned WINAPI recvProc(void *arg)
{
	int recvLen;
	int count = 0;
	while (1)
	{
		recvLen = recv(Socket, RecvBuf, Recv_BUF_SIZE, 0);
		if (recvLen < Recv_BUF_SIZE) 
		{
			RecvBuf[recvLen] = 0;
			printf("%d : %s\n", ++count, RecvBuf);
		}
	}
}
void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}