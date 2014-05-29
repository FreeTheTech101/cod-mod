// ==========================================================
// IW4M project
// 
// Component: clientdll
// Sub-component: steam_api
// Purpose: Modern Warfare 2 patches: out-of-band messages
//
// Initial author: NTAuthority
// Started: 2011-11-25 (copied from PatchMW2Status.cpp)
// ==========================================================

#include "StdInc.h"

static enum
{
	GSC_GETNEGO,
	GSC_GETINFO,
	GSC_GETNEGORESPONSE,
	GSR_GETSERVERSRESPONSE
} lastGsrCommand;

enum connectState_s
{
	idle,
	checking,
	searching,
	pinging,
	negotiating,
	connecting,
	listening
};

extern connectState_s connectState;
extern serverAddress_t partner;

static netadr_t me;

void CL_ServersResponsePacket( msg_t *msg );
void Info_SetValueForKey( char *s, const char *key, const char *value );
extern int isSearching;

void sendInfoResponse(netadr_t from, msg_t* msg)
{
	char infostring[1024];

	infostring[0] = 0;

	// Challenge code needs to be optimized (using the explode wrapper bugged though)
	int garbage = 12;
	int cLength = msg->cursize - garbage;

	char* challenge = (char*)malloc(cLength + 1);
	memset(challenge, 0, cLength + 1);
	strncpy(challenge, msg->data + garbage, cLength);

	if(challenge[0])
		Info_SetValueForKey(infostring, "challenge", challenge);
	Info_SetValueForKey(infostring, "gamename", "IW4SP");
	Info_SetValueForKey(infostring, "protocol", va("%i", 29));
	Info_SetValueForKey(infostring, "gametype", "coop");
	Info_SetValueForKey(infostring, "sv_maxclients", "1");
	Info_SetValueForKey(infostring, "clients", va("%i", !isSearching));
	Info_SetValueForKey(infostring, "shortversion", va("%s.%d", VERSION, version));

	Com_Printf( 0, "Sending infoResponse packet to %i.%i.%i.%i:%i\n", from.ip[0], from.ip[1], from.ip[2], from.ip[3], htons(from.port) );
	NET_OutOfBandPrint(NS_SERVER, from, "infoResponse\n%s", infostring);
}

extern int negociatedHost;

void getNego(netadr_t from, msg_t* msg)
{
	char* status;
	if(!isSearching)
	{
		status = "NET"; // NET: Not eligible for transmission
	}
	else
	{
		status = "OK"; // OK: Oll Korrect ;)
		negociatedHost = true;
		partner = natosa(from);
		connectState = connecting;
	}

	NET_OutOfBandPrint(NS_SERVER, from, "negoResponse\n%s", status);
}

extern int stateConnectStart;
extern int addrNum;

void getNegoResponse(netadr_t from, msg_t* msg)
{
	if(!strcmp(msg->data + 17, "OK"))
	{
		stateConnectStart = Com_Milliseconds();
		negociatedHost = false;
		connectState = connecting;
	}
	else
	{
		Com_Printf(0, va("Bad negotiation response: '%s'\n", msg->data + 17));
		addrNum++;
		connectState = pinging;
	}
}

void CL_DeployOOB(netadr_t* from, msg_t* msg)
{
	if(!isSearching)
		return;

	if (lastGsrCommand == GSC_GETINFO)
	{
		return sendInfoResponse(*from, msg);
	}

	if (lastGsrCommand == GSC_GETNEGO)
	{
		return getNego(*from, msg);
	}

	if (lastGsrCommand == GSC_GETNEGORESPONSE)
	{
		return getNegoResponse(*from, msg);
	}

	if (lastGsrCommand == GSR_GETSERVERSRESPONSE)
	{
		return CL_ServersResponsePacket(msg);
	}
}

CallHook gsrCmpHook;
DWORD gsrCmpHookLoc = 0x65E448;

int GsrCmpHookFunc(const char* a1, const char* a2)
{
	if (!_strnicmp(a1, "getServersResponse", 18))
	{
		lastGsrCommand = GSR_GETSERVERSRESPONSE;
		return 0;
	}

	if (!_strnicmp(a1, "getinfo", 7))
	{
		lastGsrCommand = GSC_GETINFO;
		return 0;
	}

	if (!_strnicmp(a1, "getnego", 7))
	{
		lastGsrCommand = GSC_GETNEGO;
		return 0;
	}

	if (!_strnicmp(a1, "negoResponse", 12))
	{
		lastGsrCommand = GSC_GETNEGORESPONSE;
		return 0;
	}

	return 1;
}

void __declspec(naked) GsrCmpHookStub()
{
	__asm jmp GsrCmpHookFunc
}

void __declspec(naked) CL_DeployOOBStub()
{

	__asm
	{
		// Message
		mov eax, [esp + 210h + 20h]
		push eax

		// Address
		mov eax, esp
		add eax, 21Ch // 214h + 8h
		push eax

		call CL_DeployOOB
		add esp, 8h

		mov eax, 1

		pop edi
		pop esi
		pop ebp
		pop ebx
		add esp, 200h
		retn
	}
}

void PatchMW2_OOB()
{
	if(version == 159)
	{
		// client-side OOB handler
		*(int*)0x65E454 = ((DWORD)CL_DeployOOBStub) - 0x65E452 - 6;
	}
	else
	{
		// client-side OOB handler
		*(int*)0x65BCE4 = ((DWORD)CL_DeployOOBStub) - 0x65BCE2 - 6;

		gsrCmpHookLoc = 0x65BCD8;
	}

	gsrCmpHook.initialize(gsrCmpHookLoc, GsrCmpHookStub);
	gsrCmpHook.installHook();
}