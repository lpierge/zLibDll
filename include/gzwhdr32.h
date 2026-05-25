/*
	gzwhdr.h
	GZW interface, defines the gzw header and all related stuff.
	This code is placed into public domain as freeware.
	Luca Piergentili, 31/08/96
	lpiergentili@yahoo.com
*/
#ifndef _GZWHDR_H
#define _GZWHDR_H 1

#ifdef __cplusplus
  extern "C" {
#endif

#include "pragma.h"
#include <stdlib.h>

#define GZ_HEADER_LEN	10							/* header size */
#define GZW_SUFFIX		".gzw"						/* default extension for gzw files */
#define GZW_SIGNATURE	"GZW3.0"					/* signature */
#define GZW_SIGN_LEN	(sizeof(GZW_SIGNATURE)-1)	/* signature lenght */
#define GZW_VERSION		GZW_SIGNATURE				/* version */
#define GZW_PSW_MAX		15							/* max lenght for psw */

#define GZW_EXTENSION	GZW_SUFFIX
#define GZW_TYPENAME	"gzwfile"
#define GZW_TYPEDESC	"GZW compressed data"
#define GZW_CONTENTTYPE	"application/x-gzw"

/*
	GZWHEADER
	header gzw - defines the fixed portion of the gzw header
*/
typedef struct gzwhdr_t {
	char			szSignature[GZW_SIGN_LEN];		/* signature */
	unsigned int	nPswLen;						/* psw lenght */
	char			szPsw[GZW_PSW_MAX+1];			/* psw */
	unsigned long	dwFileSize;						/* size of the original (uncompressed) file */
	unsigned long	dwFileCompressedSize;			/* size of the gzw (compressed) file */
	unsigned int	wFileDate;						/* date (MS-DOS format) */
	unsigned int	wFileTime;						/* time (MS-DOS format) */
	unsigned int	nFileNameLen;					/* filename lenght */
} GZWHEADER,*LPGZWHEADER;

/*
	GZWHDR
	header gzw - defines the full header, including the fixed portion
	the size of variable portion depends on the lenght of the filename
*/
typedef struct hdr_t {
	GZWHEADER		stGzwHdr;						/* header GZW (fixed portion) */
	char*			pFileName;						/* pointer to the filename */
} GZWHDR,*LPGZWHDR;


#ifdef __cplusplus
  }
#endif

#endif /* _GZWHDR_H */
