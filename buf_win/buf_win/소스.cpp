#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char *message);
void ShowSocketBufSize(SOCKET sock);

int main(int argc, char *message)
{
	WSADATA wsaData;
	SOCKET hSocket;
	int sndBuf, rcvBuf, state;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	ShowSocketBufSize(hSocket);

	sndBuf = 1024 * 3, rcvBuf = 1024 * 3;
	state = setsockopt(hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf));
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error!");

	state = setsockopt(hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf));
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error!");

	ShowSocketBufSize(hSocket);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ShowSocketBufSize(SOCKET sock)
{
	int sndBuf, rcvBuf, state, len;

	len = sizeof(sndBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, &len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	len = sizeof(rcvBuf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, &len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");

	printf("Input buffer size: %d \n", rcvBuf);
	printf("Output buffer size: %d \n", sndBuf);
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}