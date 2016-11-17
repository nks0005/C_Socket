#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	char *ServerAddress = "127.0.0.1";
	char *ServerPort = "12345";

	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	SOCKADDR_IN ServerAddr, ClientAddr;
	int clientAddrSize;

	FILE *fp;
	char buf[BUF_SIZE];
	int readCnt;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error!");

	fp = fopen("¼Ò½º.c", "rb");
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(ServerAddress);
	ServerAddr.sin_port = htons(atoi(ServerPort));

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	clientAddrSize = sizeof(ClientAddr);
	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &clientAddrSize);
	if (ClientSocket == SOCKET_ERROR)
		ErrorHandling("accept() error");

	while (1)
	{
		readCnt = fread(buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE)
		{
			send(ClientSocket, buf, readCnt, 0);
			break;
		}
		send(ClientSocket, buf, BUF_SIZE, 0);
	}

	shutdown(ClientSocket, SD_SEND);
	recv(ClientSocket, &buf, BUF_SIZE, 0);
	printf("Message from client: %s \n", buf);

	fclose(fp);
	closesocket(ClientSocket); closesocket(ServerSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}