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
	SOCKET hAcceptSocket, hRecvSocket;
	SOCKADDR_IN sendAddr, recvAddr;
	int sendAddrSize, strLen;
	char buf[BUF_SIZE];
	int result;

	fd_set read, except, readCopy, exceptCopy;
	struct timeval timeout;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	hAcceptSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hAcceptSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr(TargetIP);
	recvAddr.sin_port = htons(atoi(TargetPORT));

	if (bind(hAcceptSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hAcceptSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	sendAddrSize = sizeof(sendAddr);
	hRecvSocket = accept(hAcceptSocket, (SOCKADDR*)&sendAddr, &sendAddrSize);
	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(hRecvSocket, &read);
	FD_SET(hRecvSocket, &except);

	while (1)
	{
		readCopy = read;
		exceptCopy = except;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		result = select(0, &readCopy, 0, &exceptCopy, &timeout);

		if (result > 0)
		{
			if (FD_ISSET(hRecvSocket, &exceptCopy))
			{
				strLen = recv(hRecvSocket, buf, BUF_SIZE - 1, MSG_OOB);
				buf[strLen] = 0;
				printf("Urgent message: %s \n", buf);
			}
			if (FD_ISSET(hRecvSocket, &readCopy))
			{
				strLen = recv(hRecvSocket, buf, BUF_SIZE - 1, 0);
				if (strLen == 0)
				{
					break;
					closesocket(hRecvSocket);
				}
				else
				{
					buf[strLen] = 0;
					puts(buf);
				}
			}
		}
	}

	closesocket(hAcceptSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}