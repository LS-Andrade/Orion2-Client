/**
* Orion2 - A MapleStory2 Dynamic Link Library Localhost
*
* @author Eric
*
*/
#include "Orion.h"
#include "WinSockHook.h"
#include "NMCOHook.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	/* Initial injection from process attach. */
	if (dwReason == DLL_PROCESS_ATTACH) {
		/* Update the locale to properly decode foreign StringPools. */
		setlocale(LC_ALL, STRING_LOCALE);

		bool DEBUG = false;
		if (DEBUG) {
			NotifyMessage("Injection successful - haulting process for memory alterations.", Orion::NotifyType::Information);
			return TRUE;
		}

		/* Initiate the winsock hook for socket spoofing and redirection. */
		if (!Hook_WSPStartup(true)) {
			NotifyMessage("Failed to hook WSPStartup", Orion::NotifyType::Error);
			return FALSE;
		}

		/* Initiate the NMCO hook to fix our login passport handling. */
		if (!NMCOHook_Init()) {
			NotifyMessage("Failed to hook CallNMFunc", Orion::NotifyType::Error);
			return FALSE;
		}

		/* Initiate the CreateWindowExA hook to customize the main window. */
		if (!Hook_CreateWindowExA(true)) {
			NotifyMessage("Failed to hook CreateWindowExA", Orion::NotifyType::Error);
			return FALSE;
		}

		/* Initiate the RegisterClassExA hook to bypass the client CRC checks.  */
		if (!Hook_RegisterClassExA()) {
			NotifyMessage("Failed to hook RegisterClassExA", Orion::NotifyType::Error);
			return FALSE;
		}

		/* Hook GetCurrentDirectoryA and begin to rape the client. */
		if (!Hook_GetCurrentDirectoryA(true)) {
			NotifyMessage("Failed to hook GetCurrentDirectoryA", Orion::NotifyType::Error);
			return FALSE;
		}

		DisableThreadLibraryCalls(hModule);
	}

	return TRUE;
}
