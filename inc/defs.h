#ifndef _DEFS_H
#define _DEFS_H

// define function return value macro
#undef R_ERROR
#define R_ERROR -1
#undef R_OK
#define R_OK    0

// typedef some data type for global use
typedef signed char	        INT8;
typedef unsigned char       UINT8;

typedef signed short        INT16;
typedef unsigned short      UINT16;

typedef signed int          INT32;
typedef unsigned int        UINT32;

typedef signed long long    INT64;
typedef unsigned long long  UINT64;

//typedef unsigned char     u8;
//typedef unsigned short    u16;
//typedef unsigned int      u32;
//typedef unsigned long     size_t;

#define  MAX(a,b)	(((a)>(b))?(a):(b))
#define  MIN(a,b)	(((a)<(b))?(a):(b))

#define _perror() printf("%s : args error.\n", __func__)

#endif
