#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h> // for iP_MULTICAST_TTL option

#define TTL 64
#define BUF_SIZE 30

void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	char *GroupIp = "127.0.0";
	char *GroupPort = "12345";
	WSADATA wsaData;
	SOCKET SendSocket;
	SOCKADDR_IN mulAddr;
	int timeLive = TTL;
	FILE *fp;
	char buf[BUF_SIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	SendSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (SendSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	memset(&mulAddr, 0, sizeof(mulAddr));
	mulAddr.sin_family = AF_INET;
	mulAddr.sin_addr.s_addr = inet_addr(GroupIp);
	mulAddr.sin_port = htons(atoi(GroupPort));

	setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&timeLive, sizeof(timeLive));
	if ((fp = fopen("news.txt", "r")) == NULL)
		ErrorHandling("fopen() error");
	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(SendSocket, buf, strlen(buf), 0, (SOCKADDR*)&mulAddr, sizeof(mulAddr));
		Sleep(2000);
	}
	closesocket(SendSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}