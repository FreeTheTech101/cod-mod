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

void addLocStr(const char* key, const char* value);
void removeLocStr(const char* key);

typedef void (__cdecl* sendOOB_t)(int, int, int, int, int, int, const char*);
sendOOB_t OOBPrint = (sendOOB_t)0;

void OOBPrintT(int type, netadr_t netadr, const char* message)
{
	OOBPrint = (sendOOB_t)(version == 159 ? 0x500350 : 0x43D160);

	int* adr = (int*)&netadr;

	OOBPrint(type, *adr, *(adr + 1), *(adr + 2), 0xFFFFFFFF, *(adr + 4), message);
}

void NET_OutOfBandPrint(int type, netadr_t adr, const char* message, ...)
{
	va_list args;
	char buffer[65535];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	OOBPrintT(type, adr, buffer);
}

#define MAX_SERVERSPERPACKET	256

serverAddress_t addresses[MAX_SERVERSPERPACKET];
int	numservers;

enum connectState_s
{
	idle,
	searching,
	pinging,
	negotiating,
	connecting,
	listening
};

connectState_s connectState;

void CL_ServersResponsePacket( msg_t *msg ) {
	int				i, count, max, total;
	char*			buffptr;
	char*			buffend;
	
	Com_Printf(0, "Receiving parties from master\n");

	// parse through server response string
	numservers = 0;
	buffptr    = msg->data;
	buffend    = buffptr + msg->cursize;
	while (buffptr+1 < buffend) {
		// advance to initial token
		do {
			if (*buffptr++ == '\\')
				break;		
		}
		while (buffptr < buffend);

		if ( buffptr >= buffend - 6 ) {
			break;
		}

		// parse out ip
		addresses[numservers].ip[0] = *buffptr++;
		addresses[numservers].ip[1] = *buffptr++;
		addresses[numservers].ip[2] = *buffptr++;
		addresses[numservers].ip[3] = *buffptr++;

		// parse out port
		addresses[numservers].port = (*(buffptr++))<<8;
		addresses[numservers].port += (*(buffptr++)) & 0xFF;
		addresses[numservers].port = ntohs( addresses[numservers].port );

		// syntax check
		if (*buffptr != '\\') {
			break;
		}

		/*Com_DPrintf( 0, "server: %d ip: %d.%d.%d.%d:%d\n",numservers,
				addresses[numservers].ip[0],
				addresses[numservers].ip[1],
				addresses[numservers].ip[2],
				addresses[numservers].ip[3],
				ntohs(addresses[numservers].port) );*/

		numservers++;
		if (numservers >= MAX_SERVERSPERPACKET) {
			break;
		}

		// parse out EOT
		if (buffptr[1] == 'E' && buffptr[2] == 'O' && buffptr[3] == 'T') {
			break;
		}
	}
	Com_Printf(0, "Parties received from master: %d\n", numservers);

	if(!numservers)
	{
		//*(DWORD*)(version == 159 ? 0x19FF7F4 : 0x19FC27C) = 0; // Set message type
		//((void(*)(char*))(version == 159 ? 0x063A850 : 0x638060))("No players available!"); // Display message
		connectState = listening;
	}
	else
	{
		connectState = pinging;
	}
}

void startCoopListening()
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
}

char connectAddress[50];
int negociatedHost;
int stateConnectStart;
int isSearching;
int addrNum;
serverAddress_t partner;

netadr_t satona(serverAddress_t address)
{
	netadr_t* netAddrPtr = (netadr_t*)malloc(sizeof(netadr_t));
	NET_StringToAdr(va("%i.%i.%i.%i:%i", address.ip[0], address.ip[1], address.ip[2], address.ip[3], htons(address.port)), netAddrPtr);
	return *netAddrPtr;
}

serverAddress_t natosa(netadr_t address)
{
	serverAddress_t* serverAddrPtr = (serverAddress_t*)malloc(sizeof(serverAddress_t));
	memcpy(&(serverAddrPtr->ip[0]), &(address.ip[0]), sizeof(BYTE) * 4);
	serverAddrPtr->port = address.port;
	return *serverAddrPtr;
}

netadr_t getMaster()
{
	static netadr_t	adr;
	const char* masterName = COOP_MASTER;

	if ( !masterName[0] ) {
		adr.type = NA_BAD;
		return adr;
	}

	if ( adr.type != NA_IP ) 
	{
		Com_Printf( 0, "Resolving master %s\n", masterName );
		if ( !NET_StringToAdr( masterName, &adr ) ) {
			adr.type = NA_BAD;
			Com_Printf( 0, "Couldn't resolve address: %s\n", masterName );
			return adr;
		}
		if ( !strstr( ":", masterName ) ) {
			adr.port = htons(COOP_MASTER_PORT);
		}
		Com_Printf( 0, "Master %s resolved to %i.%i.%i.%i:%i\n", masterName,
			adr.ip[0], adr.ip[1], adr.ip[2], adr.ip[3],
			htons(adr.port) );
	}

	return adr;
}

void negotiate()
{
	addLocStr("PLATFORM_POPUP_CONNECTION", "Negotiating with partner");
	NET_OutOfBandPrint(NS_SERVER, satona(partner), "getNego");
	stateConnectStart = Com_Milliseconds();
	connectState = negotiating;
}

int lastheartbeat;

// TODO: Move NET stuff out of render thread
void listener()
{
	if(!isSearching)
		return;

	if(connectState == searching)
	{
		connectState = idle;
		addrNum = 0;
		
		netadr_t adr = getMaster();

		// Send heartbeat and serverrequest
		Com_Printf( 0, "Sending heartbeat and serverrequest to master.\n", COOP_MASTER );
		lastheartbeat = Com_Milliseconds();
		NET_OutOfBandPrint( NS_SERVER, adr, "heartbeat %s\n", "IW4SP" );
		NET_OutOfBandPrint( NS_SERVER, adr, "getservers %s %i empty", "IW4SP", 29 );
	}
	else if(connectState == pinging)
	{
		addLocStr("PLATFORM_POPUP_CONNECTION", "Selecting best partner");
		// Insert ping stuff here

		if(addrNum >= numservers)
		{
			connectState = listening;
			return;
		}

		partner = addresses[addrNum];
		negotiate();
	}
	else if(connectState == negotiating)
	{
		if(Com_Milliseconds() - stateConnectStart > 6000)
		{
			addrNum++;
			connectState = pinging;
		}
	}
	else if(connectState == connecting)
	{
		if(!negociatedHost)
		{
			addLocStr("PLATFORM_POPUP_CONNECTION", "Setting up the connection");
			//Sleep(2000); // Wow really? Sleep in render thread????

			if(Com_Milliseconds() - stateConnectStart > 1000)
			{
				connectState = idle;
				startCoopListening();
				isSearching = false;

				Dvar_SetCommand("connect_ip", va("%i.%i.%i.%i:%i", partner.ip[0], partner.ip[1], partner.ip[2], partner.ip[3], htons(partner.port)));
				Cbuf_AddText(0, "connect");
				removeLocStr("PLATFORM_POPUP_CONNECTION");
			}
		}
		else
		{
			connectState = idle;
			startCoopListening();
			isSearching = false;
			removeLocStr("PLATFORM_POPUP_CONNECTION");
		}
	}
	else if(connectState == listening)
	{
		addLocStr("PLATFORM_POPUP_CONNECTION", "Waiting for connection");

		if(Com_Milliseconds() - lastheartbeat > 20000)
		{
// 			netadr_t adr = getMaster();
// 			lastheartbeat = Com_Milliseconds();
// 			Com_Printf(0, "Sending heartbeat to master: %s", COOP_MASTER);
// 			NET_OutOfBandPrint( NS_SERVER, adr, "heartbeat %s\n", "IW4SP" );
			connectState = searching;
		}
	}
}

void xstopparty_f()
{
	// TODO: Tell master to remove from list
	OutputDebugString("Party unlisted.\n");
	connectState = idle;
	removeLocStr("PLATFORM_POPUP_CONNECTION");

	isSearching = 0;
}

void connectHook()
{
	//Cmd_ExecuteSingleCommand(0,0, "connect");
	addLocStr("PLATFORM_POPUP_CONNECTION", "Searching for partner");
	*(DWORD*)(version == 159 ? 0x19FF7F4 : 0x19FC27C) = 1; // Set message type
	((void(*)(char*))(version == 159 ? 0x063A850 : 0x638060))("@PLATFORM_POPUP_CONNECTION"); // Display message
	startCoopListening();

	// Tell server to add to list
	OutputDebugString("Party listed.\n");

	isSearching = 1;
	connectState = searching;
}

USHORT getReversedPort(const char* _port)
{
	USHORT port = (USHORT)atoi(_port);

	return (USHORT)(port >> 8) | (port << 8);
}

void connectWrapper()
{
	startCoopListening();
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

	static cmd_function_t xstopparty_cmd;
	Cmd_AddCommand("xstopparty", xstopparty_f, &xstopparty_cmd, 0);

	// Patch dvar name
	call(0x475156, nameHookFunc, PATCH_CALL);

	// Redirect output
	call(0x65E786, redirectDebugOutput, PATCH_CALL); // Inavlid OOB command...
	call(0x65D659, redirectDebugOutput, PATCH_CALL); // LOADED: Client 1 (remote)...
}