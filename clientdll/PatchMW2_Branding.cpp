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
dvar_t* cl_paused;

void DrawDemoWarning()
{
	shiftColorHue();

	if(!cl_paused)
		cl_paused = Dvar_FindVar("cl_paused");

	// Reduce opacity ingame
	if(CL_IsCgameInitialized() && !cl_paused->current.boolean)
	{
		color[3] = 0.3f;
	}

	void* font = R_RegisterFont("fonts/normalFont");	
	R_AddCmdDrawText("DEV-BUILD", 0x7FFFFFFF, font, 10, 30, 0.7f, 0.7f, 0.0f, color, 0);
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