#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	char *address = "127.0.0.1";
	char *port = "1234";

	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAddr;

	for(int k=0; k<5; k++)
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = inet_addr(address);
		servAddr.sin_port = htons(atoi(port));

		connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));

		while (1)
		{
			fputs("Input message(Q to quit): ", stdout);
			fgets(message, BUF_SIZE, stdin);

			if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
				break;

			send(hSocket, message, strlen(message), 0);
			strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
			message[strLen] = 0;
			printf("Message from server: %s", message);
		}
		closesocket(hSocket);
		WSACleanup();
	}
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}