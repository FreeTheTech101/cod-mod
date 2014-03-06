#include "stdinc.h"
#include <ctime>
#include <dbghelp.h>

LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	// step 1: write minidump
	static LPEXCEPTION_POINTERS exceptionData;

	exceptionData = ExceptionInfo;

	// create a temporary stack for these calls
	DWORD* tempStack = new DWORD[16000];
	static DWORD* origStack;

	__asm
	{
		mov origStack, esp
		mov esp, tempStack
		add esp, 0FA00h
		sub esp, 1000h // local stack space over here, sort of
	}

	char error[1024];
	char filename[MAX_PATH];
	__time64_t time;
	tm* ltime;

	_time64(&time);
	ltime = _localtime64(&time);

	strftime(filename, sizeof(filename) - 1, "iw4sp - %d.%m.%Y %H-%M-%S.dmp", ltime);
	_snprintf(error, sizeof(error) - 1, "A minidump has been written to %s.", filename);

	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ex;
		memset(&ex, 0, sizeof(ex));
		ex.ThreadId = GetCurrentThreadId();
		ex.ExceptionPointers = exceptionData;
		ex.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ex, NULL, NULL);		

		CloseHandle(hFile);
	}
	else
	{
		_snprintf(error, sizeof(error) - 1, "An error (0x%x) occurred during creating %s.", GetLastError(), filename);
	}

	Com_Error(0, "Fatal error (0x%08x) at 0x%08x.\n%s", ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ExceptionRecord->ExceptionAddress, error);	

	__asm
	{
		mov esp, origStack
	}

	delete[] tempStack;

	return 0;
}