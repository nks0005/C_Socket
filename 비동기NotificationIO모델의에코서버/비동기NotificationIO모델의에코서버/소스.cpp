#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 100

void CompressSockets(SOCKET hSockArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idex, int total);
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServerSocket, hClientSocket;
	SOCKADDR_IN hServerAddr, hClientAddr;

	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int NumOfClientSocket = 0;
	int strLen, i;
	int posInfo, startIdx;
	int ClientAddrLen;
	char msg[BUF_SIZE];

	if (argc != 2)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR)
		ErrorHandling("WSAStartup() error");

	hServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&hServerAddr, 0, sizeof(hServerAddr));
	hServerAddr.sin_family = AF_INET;
	hServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	hServerAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServerSocket, (SOCKADDR*)&hServerAddr, sizeof(hServerAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServerSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	newEvent = WSACreateEvent();
	if (WSAEventSelect(hServerSocket, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");

	hSockArr[NumOfClientSocket] = hServerSocket;
	hEventArr[NumOfClientSocket] = newEvent;
	NumOfClientSocket++;
	
	/*
		Ŭ���̾�Ʈ�� ���� ��û ������ ���� ���� ����(������ ����)�� �����ϰ� �ִ�.
		�׸���� �̸� ������� �̺�Ʈ FD_ACCEPT�� ���ؼ� WSAEventSelect �Լ��� ȣ���ϰ� �ִ�.

		hSockArr[NumOfClientSocket] = hServerSocket;
		hEventArr[NumOfClientSocket] = newEvent;
		�̴� WSAEventSelect �Լ� ȣ���� ���ؼ� ����Ǵ� ���ϰ� Event ������Ʈ�� �ڵ������� ���� �迭 hSockArr�� hEventArr�� �����ϴ� �ڵ��̴�.
			��, hSockArr[n]�� ����� ���ϰ� ����� Event ���꺤Ʈ�� hEventArr[n]�� ����Ǿ� �ִ�.
	*/

	while(1)
	{
		posInfo = WSAWaitForMultipleEvents(NumOfClientSocket, hEventArr, FALSE, WSA_INFINITE, FALSE);
		startIdx = posInfo - WSA_WAIT_EVENT_0;
		
		for(i=startIdx;i<NumOfClientSocket;i++)
		{
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			if(netEvents.lNetworkEvents&FD_ACCEPT) // ���� ��û ��
			{
				if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					puts("Accept Error");
					break;
				}
				