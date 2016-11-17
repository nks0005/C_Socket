#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char *argv[])
{
	char *TargetIP = "127.0.0.1";
	char *TargetPORT = "12345";
	WSADATA wsaData;
	SOCKET ClientSocket;
	SOCKADDR_IN ClientAddr;

	FILE *fp;
	char buf[BUF_SIZE];
	int readCnt;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	ClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&ClientAddr, 0, sizeof(ClientAddr));
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_addr.s_addr = inet_addr(TargetIP);
	ClientAddr.sin_port = htons(atoi(TargetPORT));
	
	fp = fopen("receive.dat", "wb");
	connect(ClientSocket, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr));

	while ((readCnt = recv(ClientSocket, buf, BUF_SIZE, 0)) != 0)
		fwrite(buf, 1, readCnt, fp);

	puts("Received file data");
	send(ClientSocket, "Thank You", 10, 0);
	fclose(fp);
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}