#include <beacon.h>

void go(char* args, int argc) {
    BeaconPrintf(CALLBACK_OUTPUT, "Hello from Beacon Object File: %s [%d bytes]", args, argc);
}
