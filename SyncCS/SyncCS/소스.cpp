#include <stdio.h>
#include <Windows.h>
#include <process.h>

#define NUM_THREAD 50
unsigned WINAPI threadInc(void *arg);
unsigned WINAPI threadDec(void *arg);

long long num = 0;
CRITICAL_SECTION cs;

int main(int argc, char *argv[])
{
	HANDLE tHandles[NUM_THREAD];
	int i;
	InitializeCriticalSection(&cs);
	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDec, NULL, 0, NULL);
	}

	WaitForMultipleObjects(50, tHandles, TRUE, INFINITE);
	DeleteCriticalSection(&cs);
	printf("result: %lld \n", num);
	return 0;
}

unsigned WINAPI threadInc(void *arg)
{
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 1000000; i++)
		num++;
	LeaveCriticalSection(&cs);
	return 0;
}

unsigned WINAPI threadDec(void *arg)
{
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 1000000; i++)
		num--;
	LeaveCriticalSection(&cs);
	return 0;
}