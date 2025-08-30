typedef struct _CAP_PKG_INPUT {
    ULONG ulMessageType;
    GUID ProviderGuid;
    ULONG ulInputSize;
    BYTE abInput[1];
} CAP_PKG_INPUT, *PCAP_PKG_INPUT;
