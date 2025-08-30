#include <common.h>

INT StringCompareA( LPCSTR String1, LPCSTR String2 )
{
    for (; *String1 == *String2; String1++, String2++)
    {
        if (*String1 == '\0')
            return 0;
    }

    return ((*(LPCSTR)String1 < *(LPCSTR)String2) ? -1 : +1);

}

INT StringCompareW( LPWSTR String1, LPWSTR String2 )
{
    for (; *String1 == *String2; String1++, String2++)
    {
        if (*String1 == '\0')
            return 0;
    }

    return ((*(LPWSTR)String1 < *(LPWSTR)String2) ? -1 : +1);

}

INT StringNCompareW( LPWSTR String1, LPWSTR String2, INT Length )
{
    for (; *String1 == *String2; String1++, String2++, Length--)
    {
        if (*String1 == '\0')
            return 0;

        if ( Length == 1 )
            return 0;
    }

    return ((*(LPWSTR)String1 < *(LPWSTR)String2) ? -1 : +1);

}

WCHAR ToLowerCaseW( WCHAR C )
{
    return C > 0x40 && C < 0x5b ? C | 0x60 : C;
}

INT StringCompareIW( LPWSTR String1, LPWSTR String2 )
{
    for (; ToLowerCaseW( *String1 ) == ToLowerCaseW( *String2 ); String1++, String2++)
    {
        if (*String1 == '\0')
            return 0;
    }

    return ((*(LPWSTR)String1 < *(LPWSTR)String2) ? -1 : +1);

}

INT StringNCompareIW( LPWSTR String1, LPWSTR String2, INT Length )
{
    for (; ToLowerCaseW( *String1 ) == ToLowerCaseW( *String2 ); String1++, String2++, Length--)
    {
        if (*String1 == '\0')
            return 0;

        if ( Length == 1 )
            return 0;
    }

    return ((*(LPWSTR)String1 < *(LPWSTR)String2) ? -1 : +1);

}

SIZE_T StringLengthW(LPCWSTR String)
{
    LPCWSTR String2;

    for (String2 = String; *String2; ++String2);

    return (String2 - String);
}

BOOL EndsWithIW( LPWSTR String, LPWSTR Ending )
{
    DWORD Length1 = 0;
    DWORD Length2 = 0;

    if ( ! String || ! Ending )
        return FALSE;

    Length1 = StringLengthW( String );
    Length2 = StringLengthW( Ending );

    if ( Length1 < Length2 )
        return FALSE;

    String = &String[ Length1 - Length2 ];

    return StringCompareIW( String, Ending ) == 0;
}

PCHAR StringCopyA(PCHAR String1, PCHAR String2)
{
    PCHAR p = String1;

    while ((*p++ = *String2++) != 0);

    return String1;
}

PWCHAR StringCopyW(PWCHAR String1, PWCHAR String2)
{
    PWCHAR p = String1;

    while ((*p++ = *String2++) != 0);

    return String1;
}

SIZE_T StringLengthA( LPCSTR String ) {
    LPCSTR p = String;
    while ( *p ) {
        p++;
    }
    return ( p - String );
}

PCHAR StringConcatA(PCHAR String, PCHAR String2)
{
    StringCopyA( &String[ StringLengthA( String ) ], String2 );

    return String;
}

PWCHAR StringConcatW(PWCHAR String, PWCHAR String2)
{
    StringCopyW( &String[ StringLengthW( String ) ], String2 );

    return String;
}

LPWSTR WcsStr( PWCHAR String, PWCHAR String2 )
{
    if ( ! String || ! String2 )
        return NULL;

    UINT32 Size1 = StringLengthW( String );
    UINT32 Size2 = StringLengthW( String2 );

    if ( Size2 > Size1 )
        return NULL;

    for ( UINT32 i = 0; i < Size1 - Size2 + 1; i++ )
    {
        if ( StringNCompareW( String + i, String2, Size2 ) == 0 )
            return String + i;
    }

    return NULL;
}

LPWSTR WcsIStr( PWCHAR String, PWCHAR String2 )
{
    if ( ! String || ! String2 )
        return NULL;

    UINT32 Size1 = StringLengthW( String );
    UINT32 Size2 = StringLengthW( String2 );

    if ( Size2 > Size1 )
        return NULL;

    for ( UINT32 i = 0; i < Size1 - Size2 + 1; i++ )
    {
        if ( StringNCompareIW( String + i, String2, Size2 ) == 0 )
            return String + i;
    }

    return NULL;
}

INT MemCompare( PVOID s1, PVOID s2, INT len)
{
    PUCHAR p = (PUCHAR) s1;
    PUCHAR q = (PUCHAR) s2;
    INT charCompareStatus = 0;

    if ( s1 == s2 ) {
        return charCompareStatus;
    }

    while (len > 0)
    {
        if (*p != *q)
        {
            charCompareStatus = (*p >*q)?1:-1;
            break;
        }
        len--;
        p++;
        q++;
    }
    return charCompareStatus;
}

__attribute__((__no_builtin__))
PVOID MemSet( PVOID Destination, INT Value, SIZE_T Length ) {
    PBYTE p = ( PBYTE )Destination;
    while ( Length-- ) {
        *p++ = ( BYTE )Value;
    }
    return Destination;
}

__attribute__((__no_builtin__))
PVOID MemCpy( PVOID Destination, CONST PVOID Source, SIZE_T Length ) {
    char* pDest = (char*)Destination;
    const char* pSrc = (const char*)Source;

    // Check for trivial case
    if (!Destination || !Source) {
        return NULL;
    }

    for (size_t i = 0; i < Length; i++) {
        pDest[i] = pSrc[i];
    }

    return Destination;
}

SIZE_T WCharStringToCharString(PCHAR Destination, PWCHAR Source, SIZE_T MaximumAllowed)
{
    INT Length = MaximumAllowed;

    while (--Length >= 0)
    {
        if (!(*Destination++ = *Source++))
            return MaximumAllowed - Length - 1;
    }

    return MaximumAllowed - Length;
}

SIZE_T CharStringToWCharString( PWCHAR Destination, PCHAR Source, SIZE_T MaximumAllowed )
{
    INT Length = (INT)MaximumAllowed;

    while (--Length >= 0)
    {
        if ( ! ( *Destination++ = *Source++ ) )
            return MaximumAllowed - Length - 1;
    }

    return MaximumAllowed - Length;
}

PCHAR StringTokenA(PCHAR String, CONST PCHAR Delim)
{
    PCHAR SpanP, Token;
    INT C, SC;

    if ( String == NULL )
        return NULL;

    CONTINUE:

    C = *String++;

    for (SpanP = (PCHAR)Delim; (SC = *SpanP++) != ERROR_SUCCESS;)
    {
        if (C == SC)
            goto CONTINUE;
    }

    if (C == ERROR_SUCCESS)
        return NULL;

    Token = String - 1;

    for (;;)
    {
        C = *String++;
        SpanP = (PCHAR)Delim;

        do {
            if ((SC = *SpanP++) == C)
            {
                if (C == ERROR_SUCCESS)
                    String = NULL;
                else
                    String[-1] = '\0';

                return Token;
            }
        } while (SC != ERROR_SUCCESS);
    }

    return NULL;

}

PCHAR StrChrA( LPCSTR str, CHAR character ) {
    while ( *str != '\0' ) {
        if ( *str == character ) {
            return (PCHAR)str;
        }
        str++;
    }
    if ( *str == character ) {
        return (PCHAR)str;
    }
    return NULL;
}

PCHAR StrStrA( LPCSTR haystack, LPCSTR needle ) {
    if ( !*needle ) {
        return (PCHAR)haystack;
    }

    LPCSTR p1 = haystack;
    while ( *p1 ) {
        LPCSTR p1Begin = p1;
        LPCSTR p2 = needle;
        while ( *p1 && *p2 && *p1 == *p2 ) {
            p1++;
            p2++;
        }
        if ( !*p2 ) {
            return (PCHAR)p1Begin;
        }
        p1 = p1Begin + 1;
    }
    return NULL;
}

VOID PrintFA( LPCSTR Format, ... ) {
    LPSTR buf = (LPSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 2048 );
    if ( buf != NULL ) {
        va_list args;
        va_start( args, Format );
        int len = wvsprintfA( buf, Format, args );
        va_end( args );
        WriteConsoleA( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );
        HeapFree( GetProcessHeap(), 0, buf );
    }
}

VOID PrintFW( LPCWSTR Format, ... ) {
    LPWSTR buf = (LPWSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 2048 * sizeof(WCHAR) );
    if ( buf != NULL ) {
        va_list args;
        va_start( args, Format );
        int len = wvsprintfW( buf, Format, args );
        va_end( args );
        WriteConsoleW( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );
        HeapFree( GetProcessHeap(), 0, buf );
    }
}
