#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void *msg);
unsigned WINAPI RecvMsg(void *msg);
void ErrorHandling(char *msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
	char *TargetIP = "127.0.0.1";
	char *TargetPORT = "12345";
	char *Name = "¾ÈÇüºó";

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	HANDLE hSendThread, hRecvThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	sprintf(name, "[%s]", Name);
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(TargetIP);
	servAddr.sin_port = htons(atoi(TargetPORT));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	hSendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSocket, 0, NULL);
	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSocket, 0, NULL);

	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

unsigned WINAPI SendMsg(void *asg) // send thread main
{
	SOCKET hSocket = *((SOCKET*)asg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSocket);
			exit(0);
		}
		sprintf(nameMsg, "%s %s", name, msg);
		send(hSocket, nameMsg, strlen(nameMsg), 0);
	}
	return 0;
}

unsigned WINAPI RecvMsg(void *arg) // read thread main
{
	SOCKET hSocket = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	int strLen;
	while (1)
	{
		strLen = recv(hSocket, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (strLen == -1)
			return -1;
		nameMsg[strLen] = 0;
		fputs(nameMsg, stdout);
	}
	return 0;
}

void ErrorHandling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}