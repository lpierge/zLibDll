/*$
	gzwhdr.h
	GZW interface, defines the gzw header and all related stuff.
	This code is placed into public domain as freeware.
	Luca Piergentili, 31/08/96
	lpiergentili@yahoo.com

	Note:
	- ultima modifica al codice originale: gestione files > 4GB

	- se possibile, mantenere il codice Windows-free

	- qualsiasi modifica alla struttura per l'header implica la ricompilazione obbligata della DLL zLib

	- la zLib usa le funzioni della famiglia fopen() per l'I/O su file, quindi nel resto del codice usare
	  la stessa interfaccia e non mescolarla con funzioni di basso livello come quelle dell'API di Win32

	- NON e' obbligatorio passare a x64 per gestire files > 4 GB:
	  le CRT-64 bit (_fseeki64, _ftelli64, _lseeki64, _open, ...) esistono anche in configurazione x86 (32 bit)
	  l'unico vincolo e' che il tipo usato per offset/dimensioni sia 64 bit (QWORD, __int64, uint64_t) e che si 
	  spezzino le operazioni di lettura/scrittura in blocchi ≤ 4 GB (perche' size_t su x86 e' 32 bit)
	  quindi si puo':
	  lasciare il progetto su Win32 x86
	  usare le funzioni "i64"
	  e quando si devono trasferire piu' di 4 GB in una botta, frammentare il loop (es. 1 GB per volta)
	  passare a x64 serve solo se si vuole:
	  size_t nativamente 64 bit (nessun limite sulla dimensione del singolo fread/fwrite)
	  avere una build che funzioni anche su file da 10, 20, 100 GB senza loop aggiuntivi
	  sfruttare registri e istruzioni 64 bit (ma performance comunque marginali per I/O)
*/
#ifndef _GZWHDR_H
#define _GZWHDR_H 1

#ifdef __cplusplus
  extern "C" {
#endif

#include "pragma.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "typedef.h"

#define GZ_HEADER_LEN	10							/* header size */
#define GZW_SUFFIX		".gzw"						/* default extension for gzw files */
#define GZW_SIGNATURE	"GZW4.0"					/* signature, fino a 3.0 era per files < 4GB */
#define GZW_SIGN_LEN	(sizeof(GZW_SIGNATURE)-1)	/* signature lenght */
#define GZW_VERSION		GZW_SIGNATURE				/* version */
#define GZW_PSW_MAX		15							/* max lenght for psw */
#define GZW_SALT_MAX	GZW_PSW_MAX					/* max lenght for salt */

#define GZW_EXTENSION	GZW_SUFFIX
#define GZW_TYPENAME	"gzwfile"
#define GZW_TYPEDESC	"GZW compressed data"
#define GZW_CONTENTTYPE	"application/x-gzw"

/* flag bitmask (RFC 1952) per formato .gz */
#define GZ_FLG_FTEXT    0x01  /* bit 0 */
#define GZ_FLG_FHCRC    0x02  /* bit 1 */
#define GZ_FLG_FEXTRA   0x04  /* bit 2 */
#define GZ_FLG_FNAME    0x08  /* bit 3 */
#define GZ_FLG_FCOMMENT 0x10  /* bit 4 */

/*
	definizione del tipo per la funzione callback tra classe e zLib
	pContext: puntatore a dati (il 'this' della classe C++)
	nPercent: percentuale di progresso (0-100).
	typedef void (*LPGZPROGRESSCALLBACK)(void* pContext, int nPercent);
*/
typedef void (__cdecl *LPGZPROGRESSCALLBACK)(void* pContext, int nPercent,int nAction);

/*
	GZHEADER
	header gz
*/
typedef struct _gz_header {
    BOOL     valid;          /* true se signature corretta */
    uint8_t  cm;             /* metodo compressione (deve essere 8) */
    uint8_t  flg;            /* flag byte */
    uint32_t mtime;          /* Unix time (little-endian) */
    uint8_t  xfl;            /* extra flags */
    uint8_t  os;             /* OS code */
    BOOL     has_ftext;      /* bit 0: file e' testo ASCII */
    BOOL     has_fextra;     /* true se c'e' extra field */
    BOOL     has_fname;      /* true se c'e' file name */
    BOOL     has_fcomment;   /* true se c'e' commento */
    BOOL     has_fhcrc;      /* true se c'e' CRC16 dell'header */
} GZHEADER,*LPGZHEADER;

/*
	GZWHEADER
	header gzw - defines the fixed portion of the gzw header
*/
typedef struct gzwhdr_t {
	char			szSignature[GZW_SIGN_LEN];	/* signature */
	char			szPsw[GZW_PSW_MAX];			/* psw */
	unsigned int	nPswLen;					/* psw lenght */
	char			szSalt[GZW_SALT_MAX];		/* salt */
	QWORD			qwFileSize;					/* size of the original (uncompressed) file */
	QWORD			qwFileCompressedSize;		/* size of the gzw (compressed) file */
	unsigned int	wFileDate;					/* date (MS-DOS format) */
	unsigned int	wFileTime;					/* time (MS-DOS format) */
	unsigned int	nFileNameLen;				/* filename lenght */
} GZWHEADER,*LPGZWHEADER;

/*
	GZWHDR
	header gzw - defines the full header, including the fixed portion
	the size of variable portion depends on the lenght of the filename
*/
typedef struct hdr_t {
	GZWHEADER		stGzwHdr;					/* header GZW (fixed portion) */
	char*			pFileName;					/* pointer to the filename */
} GZWHDR,*LPGZWHDR;

/*
	GZWHEADERDATA
	definisce la struttura da passare alle funzioni che scorrono il
	contenuto del file .gzw leggendo gli headers dei files compressi
*/
typedef struct _gzw_header_data {
	FILE*	pStream;							// stream del file .gzw
	GZWHDR*	pstHeader;							// puntatore alla struttura per l'header GZW
	QWORD	qwFileSize;							// dimensione del file .gzw
	QWORD	qwFilePointer;						// offset corrente nel file .gzw
	QWORD	qwTotData;							// dimensione totale dei dati compressi
	unsigned int wTotHeaders;					// contatore per headers letti
	char	szFileName[_MAX_PATH+1];			// buffer per il nome del file compresso
	char	szTmpName[_MAX_PATH+1];				// buffer per il nome del file da decomprimere
	unsigned int nRet;							// codice d'errore
} GZWHEADERDATA,*LPGZWHEADERDATA;

#ifdef __cplusplus
  }
#endif

#endif /* _GZWHDR_H */
