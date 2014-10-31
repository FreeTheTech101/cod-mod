// --------------------------------------+
// System Dynamic Link Library Proxy
// by momo5502
// --------------------------------------+

#include "SDLLP.h"

// --------------------------------------+
//	Adapt export functions and library
// --------------------------------------+

#define LIBRARY "d3d9.dll"
EXPORT(Direct3DShaderValidatorCreate9)
EXPORT(PSGPError)
EXPORT(PSGPSampleTexture)
EXPORT(D3DPERF_BeginEvent)
EXPORT(D3DPERF_EndEvent)
EXPORT(D3DPERF_GetStatus)
EXPORT(D3DPERF_QueryRepeatFrame)
EXPORT(D3DPERF_SetMarker)
EXPORT(D3DPERF_SetOptions)
EXPORT(D3DPERF_SetRegion)
EXPORT(DebugSetLevel)
EXPORT(DebugSetMute)
EXPORT(Direct3DCreate9)
EXPORT(Direct3DCreate9Ex)

// --------------------------------------+
//	DON'T TOUCH THE STUFF BELOW
// --------------------------------------+

std::map<const char*, FARPROC> SDLLP::mExports;
HINSTANCE                      SDLLP::mHandle = NULL;

// Load necessary library
// --------------------------------------+
void SDLLP::LoadLibrary()
{
	CHAR mPath[MAX_PATH];

	GetSystemDirectoryA(mPath, MAX_PATH);
	strcat_s(mPath, "\\" LIBRARY);

	mHandle = ::LoadLibraryA(mPath);

	if(!mHandle) Log("[SDLLP] Unable to load library '%s'.", LIBRARY);
}

// Check if export already loaded
// --------------------------------------+
bool SDLLP::IsExportLoaded(const char* function)
{
	return mExports.find(function) != mExports.end();
}

// Load export from dll
// --------------------------------------+
bool SDLLP::LoadExport(const char* function)
{
	Log("[SDLLP] Export '%s' requested.", function);

	if(!mHandle) LoadLibrary();
	if(IsExportLoaded(function)) return true;

	FARPROC address = GetProcAddress(mHandle, function);

	if(address)
	{
		mExports[function] = address;
		return true;
	}

	Log("[SDLLP] Unable to load export '%s' from library '%s'.", function, LIBRARY);
	return false;
}

// Return export address
// --------------------------------------+
FARPROC SDLLP::GetExport(const char* function)
{
	if(IsExportLoaded(function) || LoadExport(function))
	{
		return mExports[function];
	}

	Log("[SDLLP] Export '%s' not availabe.", function);
	return NULL;
}

// Write debug string
// --------------------------------------+
void SDLLP::Log(const char* message, ...)
{
	CHAR buffer[1024];
	va_list ap;

	va_start(ap, message);
	vsprintf(buffer, message, ap);
	va_end(ap);

	OutputDebugStringA(buffer);
}
