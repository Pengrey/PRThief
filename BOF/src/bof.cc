#include <beacon.h>
#include <macros.h>

extern "C" {
    DFR(ADVAPI32, GetUserNameA);
    #define GetUserNameA ADVAPI32$GetUserNameA

    DFR(KERNEL32, GetLastError);
    #define GetLastError KERNEL32$GetLastError
}

void go(char* args, int argc) {
    CHAR  User[MAX_PATH + 1] = { 0 };
    DWORD Size               = MAX_PATH;

    if (!GetUserNameA(User, &Size)) {
        BeaconPrintf(CALLBACK_ERROR, "[!] GetUserNameA Failed with Error: %ld", KERNEL32$GetLastError());
        return;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "Username: %s", User);
}

