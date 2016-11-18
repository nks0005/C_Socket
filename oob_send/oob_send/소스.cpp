#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	char *TargetIP = "127.0.0.1";
	char *TargetPORT = "12345";

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&sendAddr, 0, sizeof(sendAddr));
	sendAddr.sin_family = AF_INET;
	sendAddr.sin_addr.s_addr = inet_addr(TargetIP);
	sendAddr.sin_port = htons(atoi(TargetPORT));

	if (connect(hSocket, (SOCKADDR*)&sendAddr, sizeof(sendAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	send(hSocket, "123", 3, 0);
	send(hSocket, "4", 1, MSG_OOB);
	send(hSocket, "567", 3, 0);
	send(hSocket, "890", 3, MSG_OOB);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}