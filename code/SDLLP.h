// --------------------------------------+
// System Dynamic Link Library Proxy
// by momo5502
// --------------------------------------+

#include <windows.h>
#include <map>

// Macro to declare an export
// --------------------------------------+
#define EXPORT(_export)                                           \
extern "C" __declspec(naked) __declspec(dllexport) void _export() \
{                                                                 \
	SDLLP::GetExport(__FUNCTION__);                               \
	__asm jmp eax                                                 \
}                                                                 \

// Static class
// --------------------------------------+
class SDLLP
{
	private:
		static std::map<const char*, FARPROC> mExports;
		static HINSTANCE                      mHandle;

		static void	Log(const char* message, ...);
		static void	LoadLibrary();
		static bool	LoadExport(const char* function);
		static bool	IsExportLoaded(const char* function);
		
	public:
		static FARPROC GetExport(const char* function);
};