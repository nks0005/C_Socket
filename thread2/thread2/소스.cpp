/*
@ �ϳ��� Ŀ�� ������Ʈ�� ���ؼ� sinaled ���� ( �����尡 ���� ) ���� Ȯ���� ���� ȣ���ϴ� �Լ�
WaitForSingleObject

DWORD WaitForSingleObject(HANDLE hHandle, DWOD dwMilliseconds);
	hHandle			: ���� Ȯ���� ����� �Ǵ� Ŀ�� ������Ʈ�� �ڵ��� ����
	dwMilliseconds	: 1/1000�� ������ Ÿ�Ӿƿ��� ����, ���ڷ� INFINITE ���� ��, Ŀ�� ������Ʈ�� signaled ���°� �Ǳ� ������ ��ȯ���� �ʴ´�.
>> signaled ���·� ���� ��ȯ ��, WAIT_OBJECT_0 ��ȯ, Ÿ�Ӿƿ����� ���� ��ȯ �� WAIT_TIMEOUT ��ȯ.

@ �ټ� Ŀ�� 
WaitForMultipleObjests

DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
	nCount			: �˻��� Ŀ�� ������Ʈ�� �� ����.
	lpHandles		: �ڵ������� ��� �ִ� �迭�� �ּ� �� ����
	bWaitAll		: True ���� ��, ��� �˻����� signaled ���°� �Ǿ�� ��ȯ,
					  False ���� ��, �˻� ����� �ϳ��� signaled ���°� �Ǹ� ��ȯ
	dwMilliseconds	: 1/1000�� ������ Ÿ�Ӿƿ� ����, ���ڷ� INFINITE ���� ��, Ŀ�� ������Ʈ�� signaled ���°� �Ǳ� ������ ��ȯ���� �ʴ´�.
>> ���� �� �̺�Ʈ ����, ���н�, WAIT_FALIED ��ȯ
*/

#include <stdio.h>
#include <Windows.h>
#include <process.h> /* _beginthreadex, _endthreadex */
unsigned WINAPI ThreadFunc(void *arg);

int main(int argc, char *argv[])
{
	HANDLE hThread;
	DWORD wr;
	unsigned threadID;
	int param = 5;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
	if (hThread == 0)
	{
		puts("_beginthreadex() error");
		return -1;
	}

	if ((wr = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED)
	{
		puts("thread wait error");
		return -1;
	}
	printf("%d, %d\n", hThread, threadID);
	printf("wait result: %s \n", (wr == WAIT_OBJECT_0) ? "signaled" : "time-out");
	puts("end of main");
	return 0;
}

unsigned WINAPI ThreadFunc(void *arg)
{
	int i;
	int cnt = *((int*)arg);
	for (i = 0; i < cnt; i++)
	{
		Sleep(1000); puts("running thread");
	}
	return 0;
}