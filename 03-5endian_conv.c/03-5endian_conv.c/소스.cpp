#include <stdio.h>
#include <WinSock2.h>
void ErrorHandling(char *message);

int main(int argc, int *argv[])
{
	WSADATA wsaData;
	unsigned short host_port = 0x1234;
	unsigned short net_port;
	unsigned short port;
	unsigned long host_addr = 0x12345678;
	unsigned long net_addr;
	unsigned long addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered port: %#x \n", net_port);
	printf("Host ordered address: %#x \n", host_addr);
	printf("Network ordered address: %#x \n", net_addr);

	port = htons(net_port);
	addr = htonl(net_addr);

	printf("Port : %#x \n", port);
	printf("Address : %#x \n", addr);

	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}