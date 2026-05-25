/*
	zlibapi.h
	Interfaccia zLib (dll/libreria/codice).
	Nei progetti (zLibdll/lib) definire le macro _ZLIB_DLL/ZLIB_DLL o _ZLIB_LIB/ZLIB_LIB a seconda se
	ci si trova nel progetto per la dll o per la libreria.
	Nel chiamante definire la macro _ZLIB_DLL per l'utilizzo della dll, _ZLIB_LIB per l'utilizzo della
	libreria o nessuna delle due per il link diretto col codice.
	La libreria/dll vengono compilate usando la dll multithreads (CRT).
	Luca Piergentili, 25/12/99
	lpiergentili@yahoo.com
*/
#ifndef _ZLIBAPI_H
#define _ZLIBAPI_H 1

#include "pragma.h"

/*
	_ZLIB_DLL, creazione della dll
*/
#if defined(_ZLIB_DLL)
  #ifdef ZLIB_DLL
    /* dentro il codice della dll, esporta le funzioni */
    #define EXTERN
    #define INTERNAL static
    #define ZLIBAPI  __declspec(dllexport)
  #else
    /* dentro il codice del chiamante, importa le funzioni della dll */
    #define EXTERN extern
    #define INTERNAL
    #define ZLIBAPI  __declspec(dllimport)
    #ifdef _DEBUG
      #pragma comment(lib,"zLibdll.d.lib")
    #else
      #pragma comment(lib,"zLibdll.lib")
    #endif
  #endif
/*
	_ZLIB_LIB, creazione della libreria
*/
#elif defined(_ZLIB_LIB)
  #ifdef ZLIB_LIB
    /* dentro il codice della libreria */
    #define EXTERN
    #define INTERNAL static
    #define ZLIBAPI
  #else
    /* dentro il codice del chiamante, vede le funzioni della libraria come extern */
    #define EXTERN extern
    #define INTERNAL
    #define ZLIBAPI
    #ifdef _DEBUG
      #pragma comment(lib,"zLiblib.d.lib")
    #else
      #pragma comment(lib,"zLiblib.lib")
    #endif
  #endif
/*
	link diretto del codice
*/
#else
    #undef EXTERN
    #define EXTERN
    #define INTERNAL static
    #define ZLIBAPI
#endif

#endif /* _ZLIBAPI_H */
