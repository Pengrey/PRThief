#include <windows.h>
#include "beacon.h"

DECLSPEC_IMPORT BOOL ADVAPI32$GetUserNameA(LPSTR lpBuffer, LPDWORD pcbBuffer);
DECLSPEC_IMPORT DWORD KERNEL32$GetLastError();

void go(char* args, int argc) {
    CHAR  User[MAX_PATH + 1] = { 0 };
    DWORD Size               = MAX_PATH;

    if (!ADVAPI32$GetUserNameA(User, &Size)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] GetUserNameA Failed with Error: %ld", KERNEL32$GetLastError());
        return;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "Username: %s\n", User);
}
