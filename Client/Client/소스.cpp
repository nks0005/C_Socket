#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 100

void SetIp(char *Ip);
void SetPort(int *Port);
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET ClientSocket;
	SOCKADDR_IN ClientAddr;

	char message[BUF_SIZE];
	char Send_Message[BUF_SIZE];
	char Ip[15];
	int Port = 0;
	int RecvLen = 0;

	SetIp(Ip);
	SetPort(&Port);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	ClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&ClientAddr, 0, sizeof(ClientAddr));
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_addr.s_addr = inet_addr(Ip);
	ClientAddr.sin_port = htons(Port);

	if (connect(ClientSocket, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	RecvLen = recv(ClientSocket, message, BUF_SIZE - 1, 0);
	if (RecvLen < BUF_SIZE)
		message[RecvLen] = '\0';
	printf("���� �޽��� �Դϴ�.\n? : %s\n", message);
	
	printf("���� �޽����� �Է����ּ���.\n? : ");
	getchar();
	fgets(Send_Message, BUF_SIZE, stdin);
	
	send(ClientSocket, Send_Message, strlen(Send_Message), 0);
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

void SetIp(char *Ip)
{
	printf("������ Ÿ���� ������ �ּҸ� �������ּ���. [x.x.x.x]\n");
	fgets(Ip, 15, stdin);

	return;
}
void SetPort(int *port)
{
	printf("������ Ÿ���� ��Ʈ �ּҸ� �������ּ���. [0 - 65535]\n");
	scanf_s("%d", port);
	if (*port < 0 || *port>65535)
		ErrorHandling("��Ʈ �ּҸ� �߸� �����Ͽ����ϴ�.");
	else
		return;
}