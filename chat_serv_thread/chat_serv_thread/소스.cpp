#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI HandleClnt(void *arg);
void SendMsg(char *msg, int len);
void ErrorHandling(char *message);

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main(int argc, char *argv[])
{
	char *TargetIP = "127.0.0.1";
	char *TargetPORT = "12345";

	WSADATA wsaData;
	SOCKET hServerSocket, hClientSocket;
	SOCKADDR_IN ServerAddr, ClientAddr;

	int ClientAddrSize;
	HANDLE hThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");

	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hServerSocket = socket(PF_INET, SOCK_STREAM, 0) == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(TargetIP);
	ServerAddr.sin_port = htons(atoi(TargetPORT));

	if (bind(hServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServerSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	while (1)
	{
		ClientAddrSize = sizeof(ClientAddr);
		hClientSocket = accept(hServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrSize);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClientSocket;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClientSocket, 0, NULL);
		printf("Connected client IP: %s \n", inet_ntoa(ClientAddr.sin_addr));
	}
	closesocket(hServerSocket);
	WSACleanup();
	return 0;
}

unsigned WINAPI HandleClnt(void *arg)
{
	SOCKET hClientSocket = *((SOCKET*)arg);
	int strLen = 0, i;
	char msg[BUF_SIZE];

	while ((strLen = recv(hClientSocket, msg, sizeof(msg), 0)) != 0)
		SendMsg(msg, strLen);

	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++) // remove disconnected client
	{
		if (hClientSocket == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClientSocket);
	return 0;
}

void SendMsg(char *msg, int len) // send to all
{
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
	ReleaseMutex(hMutex);
}

void ErrorHandling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}