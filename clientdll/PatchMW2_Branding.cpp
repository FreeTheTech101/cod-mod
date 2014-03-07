// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Branding
//
// Initial author: momo5502
// Started: 2014-03-04
// ==========================================================

#include "stdinc.h"

CallHook drawDevStuffHook;

void DrawDemoWarning()
{
	float alpha = 1.0f;

	// Reduce opacity ingame
	if(CL_IsCgameInitialized())
	{
		alpha = 0.3f;
	}

	void* font = R_RegisterFont("fonts/normalFont");
	float color[4] = { 1.0f, 1.0f, 1.0f, alpha };
	R_AddCmdDrawText("IW4SP: DEV-BUILD", 0x7FFFFFFF, font, 10, 30, 0.7f, 0.7f, 0.0f, color, 0);
}

void __declspec(naked) DrawDevStuffHookStub()
{
	__asm
	{
		call DrawDemoWarning
		jmp drawDevStuffHook.pOriginal
	}
}

void PatchMW2_Branding()
{
	drawDevStuffHook.initialize(drawDevStuffHookLoc, DrawDevStuffHookStub);
	drawDevStuffHook.installHook();
}