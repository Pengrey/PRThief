#include <windows.h>
#include "beacon.h"

void go(char* args, int argc) {
    CHAR  User[MAX_PATH + 1] = { 0 };
    DWORD Size               = MAX_PATH;

    if (!GetUserNameA(User, &Size)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] GetUserNameA Failed with Error: %ld", GetLastError());
        return;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "Username: %s\n", User);
}
