typedef struct {
    LPCWSTR name;
    LPCWSTR data;
    ULONG flags;
    LPCWSTR p3pHeader;
} ProofOfPossessionCookieInfo;

typedef struct IProofOfPossessionCookieInfoManager IProofOfPossessionCookieInfoManager;

typedef struct IProofOfPossessionCookieInfoManagerVtbl {
    BEGIN_INTERFACE

    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        IProofOfPossessionCookieInfoManager *This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        IProofOfPossessionCookieInfoManager *This);

    ULONG (STDMETHODCALLTYPE *Release)(
        IProofOfPossessionCookieInfoManager *This);

    HRESULT (STDMETHODCALLTYPE *GetCookieInfoForUri)(
        IProofOfPossessionCookieInfoManager *This,
        LPCWSTR uri,
        ULONG *cookieCount,
        ProofOfPossessionCookieInfo **cookies);

    END_INTERFACE
} IProofOfPossessionCookieInfoManagerVtbl;

struct IProofOfPossessionCookieInfoManager {
    CONST_VTBL IProofOfPossessionCookieInfoManagerVtbl *lpVtbl;
};
