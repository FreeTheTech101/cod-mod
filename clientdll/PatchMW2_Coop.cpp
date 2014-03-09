// ==========================================================
// MW2 coop
// 
// Component: IW4SP
// Sub-component: clientdll
// Purpose: Coop stuff
//
// Initial author: momo5502
// Started: 2014-03-04
// ==========================================================

#include "stdinc.h"

void connectHook()
{
	//Cmd_ExecuteSingleCommand(0,0, "openmenu connect_coop");
	Cmd_ExecuteSingleCommand(0,0, "connect");
}

USHORT getReversedPort(const char* _port)
{
	USHORT port = (USHORT)atoi(_port);

	return (USHORT)(port >> 8) | (port << 8);
}

void connectWrapper()
{
	// Preparation stuff
	Dvar_SetCommand("arcademode", "0");
	Dvar_SetCommand("arcademode_full", "0");
	Dvar_SetCommand("start", "0");
	Dvar_SetCommand("solo_play", "0");
	Dvar_SetCommand("ui_last_opened_group", "0");
	Dvar_SetCommand("ui_skip_level_select", "1");
	Dvar_SetCommand("so_ophud_player0", "0");
	Dvar_SetCommand("so_ophud_player1", "0");
	Cmd_ExecuteSingleCommand(0, 0, "stopListening; set arcademode_lives 2; set arcademode_combined_score 0; set splitscreen 0; set mblur_enable 0");
	Cmd_ExecuteSingleCommand(0, 0, "listen");
	//Cmd_ExecuteSingleCommand(0, 0, "set coop_mapName so_killspree_trainer");

	const char* ip = "";

	if(Cmd_Argc() == 3)
	{
		// Original connect
		Com_Printf(0, "Connecting to %s:%d\n", Cmd_Argv(1), getReversedPort(Cmd_Argv(2)));
		Dvar_SetCommand("connect_ip", va("%s:%d",Cmd_Argv(1), getReversedPort(Cmd_Argv(2))));
		Cmd_ExecuteSingleCommand(0,0, va("connect_coop %s %s", Cmd_Argv(1), Cmd_Argv(2)));
		return;
	}
	else if(Cmd_Argc() == 2)
	{
		ip = Cmd_Argv(1);
	}
	else
	{
		dvar_t* connect_ip = Dvar_FindVar("connect_ip");
		ip = connect_ip->current.string;
	}

	auto parts = explode(ip, ":");

	dvar_t* net_port = Dvar_FindVar("net_port");
	const char* port = va("%d", net_port->current.integer);

	if(parts.size() == 2)
	{
		ip = parts[0].c_str();
		port = parts[1].c_str();

		Dvar_SetCommand("connect_ip", va("%s:%s", ip, port));
	}
	else
	{
		Dvar_SetCommand("connect_ip", va("%s", ip));
	}

	Com_Printf(0, "Connecting to %s:%s\n", ip, port);
	Cmd_ExecuteSingleCommand(0,0, va("connect_coop %s %d", ip, getReversedPort(port)));
}

void redirectDebugOutput(int channel, const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	char message[400];
	int res = _vsnprintf( message, 400, fmt, ap );
	va_end(ap);

	OutputDebugString(message);
}

dvar_t* nameHookFunc(const char* name, const char* defaultVal, int flags, const char* description)
{
	// Doesn't belong here ;)
	Dvar_RegisterString("connect_ip", va("127.0.0.1:%d", *(DWORD*)0x494E27), DVAR_FLAG_SAVED, "Temporary dvar used to connect to coop.");

	return Dvar_RegisterString(name, defaultVal, DVAR_FLAG_SAVED | DVAR_FLAG_SERVER | DVAR_FLAG_USERINFO, description);
}

void PatchMW2_Coop()
{
	// Skip iw-connect
	*(BYTE*)0x44E824 = 0xEB;
	*(BYTE*)0x44E808 = 0xEB;

	// Default port (net_port)
	*(DWORD*)0x494E27 = 28960;

	// Ignore protocol mismatch
	*(BYTE*)0x65D0C2 = 0xEB;
	*(BYTE*)0x65D0ED = 0xEB;

	// Ignore XUID match
	*(BYTE*)0x65D1AF = 0xEB;

	// Skip dvar output
	*(BYTE*)0x4CD2B7 = 0xEB;

	// Ignore server dvar change for clients
	*(BYTE*)0x63580B = 0xEB;

	// Ignore 'MAX_PACKET_USERCMDS'
	*(BYTE*)0x4B1436 = 0xEB;

	// Disable 'replay' output
	nop(0x65F14A, 5);

	// Disable heartbeat output
	nop(0x57EF87, 5);

	// Force Winsocket initialization
	nop(0x42B649, 2);

	// Ignore full lobby
	//*(BYTE*)0x65D157 = 0xEB;

	// Change 'connect' to 'connect_coop'
	*(DWORD*)0x475417 = (DWORD)"connect_coop";

	static cmd_function_t connectWrapper_cmd;
	Cmd_AddCommand("connect", connectWrapper, &connectWrapper_cmd, 0);

	// Patch dvar name
	call(0x475156, nameHookFunc, PATCH_CALL);

	// Redirect output
	call(0x65E786, redirectDebugOutput, PATCH_CALL); // Inavlid OOB command...
	call(0x65D659, redirectDebugOutput, PATCH_CALL); // LOADED: Client 1 (remote)...
}