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
		클라이언트의 연결 요청 수락을 위한 서버 소켓(리스닝 소켓)을 생성하고 있다.
		그리고는 이를 대상으로 이벤트 FD_ACCEPT에 대해서 WSAEventSelect 함수를 호출하고 있다.

		hSockArr[NumOfClientSocket] = hServerSocket;
		hEventArr[NumOfClientSocket] = newEvent;
		이는 WSAEventSelect 함수 호출을 통해서 연결되는 소켓과 Event 오브젝트의 핸들정보를 각각 배열 hSockArr과 hEventArr에 저장하는 코드이다.
			즉, hSockArr[n]에 저장된 소켓과 연결된 Event 오브벡트는 hEventArr[n]에 저장되어 있다.
	*/

	while(1)
	{
		posInfo = WSAWaitForMultipleEvents(NumOfClientSocket, hEventArr, FALSE, WSA_INFINITE, FALSE);
		startIdx = posInfo - WSA_WAIT_EVENT_0;
		
		for(i=startIdx;i<NumOfClientSocket;i++)
		{
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			if(netEvents.lNetworkEvents&FD_ACCEPT) // 연결 요청 시
			{
				if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					puts("Accept Error");
					break;
				}
				