/*
@ 하나의 커널 오브젝트에 대해서 sinaled 상태 ( 쓰레드가 종료 ) 인지 확인을 위해 호출하는 함수
WaitForSingleObject

DWORD WaitForSingleObject(HANDLE hHandle, DWOD dwMilliseconds);
	hHandle			: 상태 확인의 대상이 되는 커널 오브젝트의 핸들을 전달
	dwMilliseconds	: 1/1000초 단위로 타임아웃을 지정, 인자로 INFINITE 전달 시, 커널 오브젝트가 signaled 상태가 되기 전에는 반환되지 않는다.
>> signaled 상태로 인한 반환 시, WAIT_OBJECT_0 반환, 타임아웃으로 인한 반환 시 WAIT_TIMEOUT 반환.

@ 다수 커널 
WaitForMultipleObjests

DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
	nCount			: 검사할 커널 오브젝트의 수 전달.
	lpHandles		: 핸들정보를 담고 있는 배열의 주소 값 전달
	bWaitAll		: True 전달 시, 모든 검사대상이 signaled 상태가 되어야 반환,
					  False 전달 시, 검사 대상중 하나라도 signaled 상태가 되면 반환
	dwMilliseconds	: 1/1000초 단위로 타임아웃 지정, 인자로 INFINITE 전달 시, 커넡 오브젝트가 signaled 상태가 되기 전에는 반환하지 않는다.
>> 성공 시 이벤트 정보, 실패시, WAIT_FALIED 반환
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