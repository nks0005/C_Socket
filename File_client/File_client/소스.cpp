#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	argv[1] = "127.0.0.1";
	argv[2] = "1234";

	WSADATA wsaData;
	SOCKET hSocket;
	FILE *fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error!");

	fp = fopen("receive.dat", "wb");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));

	while((readCnt = recv(hSocket,buf,sizeof(buf),0))!=0)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("Receive file data");
	send(hSocket, "Hello", 5, 0);
	fclose(fp);
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