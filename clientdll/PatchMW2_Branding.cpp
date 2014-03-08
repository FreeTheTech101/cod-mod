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
#include "ColorShift.h"

CallHook drawDevStuffHook;

void DrawDemoWarning()
{
	shiftColorHue();

	// Reduce opacity ingame
	if(CL_IsCgameInitialized())
	{
		color[3] = 0.3f;
	}

	void* font = R_RegisterFont("fonts/normalFont");
	
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