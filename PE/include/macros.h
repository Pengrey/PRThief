#ifndef MACROS_H
#define MACROS_H

#define PRINTW( STR, ... )                                                              \
do {                                                                                    \
    LPWSTR buf = (LPWSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 2048 );         \
    if ( buf != NULL ) {                                                                \
        int len = wsprintfW( buf, STR, ##__VA_ARGS__ );                                 \
        WriteConsoleW( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );       \
        HeapFree( GetProcessHeap(), 0, buf );                                           \
    }                                                                                   \
} while (0)

#if defined( DEBUG )
#define DBG_PRINTF( STR, ... )                                                          \
do {                                                                                    \
    LPWSTR buf = (LPWSTR)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, 2048 );         \
    if ( buf != NULL ) {                                                                \
        int len = wsprintfW( buf, STR, ##__VA_ARGS__ );                                 \
        WriteConsoleW( GetStdHandle( STD_OUTPUT_HANDLE ), buf, len, NULL, NULL );       \
        HeapFree( GetProcessHeap(), 0, buf );                                           \
    }                                                                                   \
} while (0)
#else
#define DBG_PRINTF( STR, ... ) { ; }
#endif

#endif //MACROS_H
