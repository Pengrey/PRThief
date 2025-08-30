#ifndef STD_H
#define STD_H

// String Comparison
INT     StringCompareA( LPCSTR String1, LPCSTR String2 );
INT     StringCompareW( LPWSTR String1, LPWSTR String2 );
INT     StringNCompareW( LPWSTR String1, LPWSTR String2, INT Length );
INT     StringCompareIW( LPWSTR String1, LPWSTR String2 );
INT     StringNCompareIW( LPWSTR String1, LPWSTR String2, INT Length );

// String Manipulation
PCHAR   StringCopyA( PCHAR String1, PCHAR String2 );
PWCHAR  StringCopyW( PWCHAR String1, PWCHAR String2 );
PCHAR   StringConcatA( PCHAR String, PCHAR String2 );
PWCHAR  StringConcatW( PWCHAR String, PWCHAR String2 );
PCHAR   StringTokenA( PCHAR String, CONST PCHAR Delim );

// String Searching
LPWSTR  WcsStr( PWCHAR String, PWCHAR String2 );
LPWSTR  WcsIStr( PWCHAR String, PWCHAR String2 );
PCHAR   StrStrA( LPCSTR haystack, LPCSTR needle );
PCHAR   StrChrA( LPCSTR str, CHAR character );

// String Information and Utilities
SIZE_T  StringLengthA( LPCSTR String );
SIZE_T  StringLengthW( LPCWSTR String );
WCHAR   ToLowerCaseW( WCHAR C );
BOOL    EndsWithIW( LPWSTR String, LPWSTR Ending );

// Memory Operations
INT     MemCompare( PVOID s1, PVOID s2, INT len );
PVOID   MemSet( PVOID Destination, INT Value, SIZE_T Length );
PVOID   MemCpy( PVOID Destination, CONST PVOID Source, SIZE_T Length );

// String Conversion
SIZE_T  WCharStringToCharString( PCHAR Destination, PWCHAR Source, SIZE_T MaximumAllowed );
SIZE_T  CharStringToWCharString( PWCHAR Destination, PCHAR Source, SIZE_T MaximumAllowed );

// Console Output
VOID PrintFA( LPCSTR Format, ... );
VOID PrintFW( LPCWSTR Format, ... );

#endif // COMMON_H
