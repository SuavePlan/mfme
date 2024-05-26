//
//
//	typedefs.h
//
//	(C) C.J.Wren 2000
//
//

#ifndef __typedefs_h__
#define __typedefs_h__

#include <string.h>

#ifndef Byte
typedef unsigned char	Byte;
#endif

typedef unsigned short  Word;
typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;

#define savestate(x,y) memcpy( x, &y, sizeof(y)); x+=sizeof(y)
#define readstate(x,y) memcpy( &y, x, sizeof(y)); x+=sizeof(y)

#endif // __typedefs_h__
