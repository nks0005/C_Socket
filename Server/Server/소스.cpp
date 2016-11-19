#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 100

void ErrorHandling(char *message);
void SetPort(int *port);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	SOCKADDR_IN ServerAddr, ClientAddr;
	int ClientAddrSize = sizeof(ClientAddr);
	int recvLen;

	char message[BUF_SIZE];
	int Port = 0;

	SetPort(&Port);
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(Port);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(ServerSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientAddrSize);

	send(ClientSocket, "You can send message!", strlen("You can send message!"), 0);
	recvLen = recv(ClientSocket, message, BUF_SIZE, 0);
	if (recvLen < BUF_SIZE)
		message[recvLen - 1] = '\0';
	else
		message[BUF_SIZE - 1] = '\0';

	printf("���� �޽��� �Դϴ�.\n ? : %s\n", message);
	closesocket(ServerSocket);
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

void SetPort(int *port)
{
	printf("�Ҵ��� ��Ʈ �ּҸ� �������ּ���. [0 - 65535]\n");
	scanf_s("%d", port);
	if (*port < 0 || *port>65535)
		ErrorHandling("��Ʈ �ּҸ� �߸� �����Ͽ����ϴ�.");
	else
		return;
}