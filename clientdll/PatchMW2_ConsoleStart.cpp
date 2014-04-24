// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Print stuff on console start
//
// Initial author: momo5502
// Started: 2014-03-10
// ==========================================================

#include "stdinc.h"

void printHelloWorld(int type)
{
	Com_Printf(type, "----------------------------------\n");
	Com_Printf(type, CONSOLESTRING "\n");
	Com_Printf(type, "----------------------------------\n");
	Com_Printf(type, "Creted by momo5502 & Dasfonia\n");
	Com_Printf(type, "----------------------------------\n");
	Com_Printf(type, "Credit and big thanks to:\n");
	Com_Printf(type, "- NTAuthority (http://refint.net)\n");
	Com_Printf(type, "- kiwidog (kiwidog.me | allenthinks.com)\n");
	Com_Printf(type, "- zxz0O0\n");
	Com_Printf(type, "- Icedream\n");
	Com_Printf(type, "- The_Uli\n");
	Com_Printf(type, "- Dekart811\n");
	Com_Printf(type, "- archony (http://triobit.net, http://archon.rekt.in)\n");
	Com_Printf(type, "----------------------------------\n");
}

void __declspec(naked) printVersionHook()
{
	printHelloWorld(16);
	__asm retn
}

void PatchMW2_ConsoleStart()
{
	call(printVersionHookLoc, printVersionHook, PATCH_CALL);
}
