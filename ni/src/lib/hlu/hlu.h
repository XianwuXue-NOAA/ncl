/*
 *      $Id: hlu.h,v 1.36 1997-05-12 22:42:17 boote Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  1992			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		hlu.h
 *
 *	Author:		Jeff W. Boote
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Mon Aug 31 09:09:12 MDT 1992
 *
 *	Description:	This file contains all the public information neccessary
 *			to write a program using the hlu library.
 */
#ifndef _NHLU_h
#define _NHLU_h

#include <stdio.h>
#include <string.h>

#include <ncarg/hlu/defs.h>

#ifndef	NhlNeedProto
#ifdef	__STDC__
#define Const const
#define NhlNeedProto 1
#else
#define Const
#define NhlNeedProto 0
#endif
#endif	/* NhlNeedProto */

#if	NhlNeedProto
#define NhlNeedVarArgProto	True 
#include <stdlib.h>
#else
#define NhlNeedVarArgProto	False
#endif

#include <ncarg/c.h>
/*
 * Macro:	_NHLCALLF
 *
 * Description:	This macro is used whenever a Fortran fuction is being called
 *		from C.  It is used to deal with different calling conventions.
 *		It is also used when defining a function in C that will be
 *		called from Fortran.
 */
#ifndef	_NHLCALLF
#define	_NHLCALLF(reg,caps)	NGCALLF(reg,caps)
#endif	/* _NHLCALLF	*/

#define True 1
#define False 0
#define BIGNUMBER 99e99
#define LITTLENUMBER -99e99

#define NhlTImmediate	"Immediate"
#define NhlTProcedure	"Procedure"

/* public pointer types supported */
#define NhlTPointer	"Pointer"
#define NhlTString	"String"

#define NhlTScalar	"Scalar"

/*
 * Character is different from Byte when it comes to conversions to and from
 * String.
 */
/* public int types supported */
#define NhlTCharacter	"Character"
#define NhlTByte	"Byte"
#define NhlTShort	"Short"
#define NhlTInteger	"Integer"
#define NhlTLong	"Long"

/* public real types supported */
#define NhlTFloat	"Float"
#define NhlTDouble	"Double"

#define NhlTEnum	"Enum"

/* public enumerations supported */
#define NhlTBoolean	"Boolean"
#define	NhlTFont	"Font"

/* public Array types */
#define	NhlTGenArray		"GenArray"

/*
 * This type is a one element GenArray - used for "variable" type scalar's.
 */
#define	NhlTVariable		"Variable"

#define NhlTStringGenArray	"StringGenArray"

#define NhlTByteGenArray	"ByteGenArray"
#define NhlTCharacterGenArray	"CharacterGenArray"
#define NhlTShortGenArray	"ShortGenArray"
#define NhlTIntegerGenArray	"IntegerGenArray"
#define NhlTLongGenArray	"LongGenArray"

#define NhlTFloatGenArray	"FloatGenArray"
#define NhlTDoubleGenArray	"DoubleGenArray"

#define NhlTEnumGenArray	"EnumGenArray"

#define NhlTBooleanGenArray	"BooleanGenArray"

/* These types are needed by ncl - it is a semi-public type */
 /* They are only supported in converters as a "from" type */
#define	NhlTQuark		"Quark"
#define	NhlTQuarkGenArray	"QuarkGenArray"

typedef	char	*NhlString;
typedef	void	*NhlPointer;
typedef	int	NhlBoolean;
typedef	int	NhlFont;
typedef char	NhlByte;

/* This declaration will hopefully move to hluP.h */
typedef	struct NhlGenArrayRec_ *NhlGenArray;

typedef enum _NhlErrType{
	NhlFATAL	= -4,
	NhlWARNING	= -3,
	NhlINFO		= -2,
	NhlNOERROR	= -1
} NhlErrorTypes;


#define NhlOffset(p_type,field) \
        ((unsigned int) (((char *) (&(((p_type*)NULL)->field))) - ((char *) NULL)))
#define NhlNumber(arr)           ((unsigned int) (sizeof(arr) / sizeof(arr[0])))

union _NhlType_ {
	NhlPointer	ptrval;
	char		charval;
	short		shrtval;
	int		intval;
	long		lngval;
	float		fltval;
	NhlString	strval;
	double		dblval;
	unsigned long	ulngval;/* no converter support */
};

typedef	union _NhlType_	NhlArgVal;

#if	DEBUG
#define	NhlINITVAR(var)		memset(&var,0,sizeof(var))
#else
#define	NhlINITVAR(var)
#endif

NhlDOCTAG(NhlSArg)
typedef struct NhlSArgRec{
	NhlString	name;
	NhlArgVal	value;
} NhlSArg, *NhlSArgList;

NhlDOCTAG(NhlGArg)
typedef struct NhlGArgRec{
	NhlString	resname;
	NhlArgVal	value;
} NhlGArg, *NhlGArgList;

typedef void (*NhlFreeFunc)(
#if     NhlNeedProto
        NhlPointer      ptr
#endif
);

typedef struct _NhlClassRec *NhlClass;

/* These are here because they needs defs from above. */
#include <ncarg/hlu/Error.h>
#include <ncarg/hlu/ResList.h>

/*
 * These functions are used to create and destroy NhlGenArray description
 * records.
 */
NhlDOCTAG(NhlCreateGenArray)
NhlSRCREF(hlu/hlu.c#NhlCreateGenArray)
extern NhlGenArray NhlCreateGenArray(
#if	NhlNeedProto
	NhlPointer	data,		/* data array		*/
	NhlString	type,		/* type of each element	*/
	unsigned int	size,		/* size of each element	*/
	int		num_dimensions,	/* number of dimensions	*/
	int		*len_dimensions	/* number of dimensions	*/
#endif
);

NhlDOCTAG(NhlFreeGenArray)
NhlSRCREF(hlu/hlu.c#NhlFreeGenArray)
extern void NhlFreeGenArray(
#if	NhlNeedProto
	NhlGenArray	gen	/* GenArray description record to free	*/
#endif
);

/*
 * These functions will allow us to impliment our own malloc if we need to
 */

extern void *NhlMalloc(
#if	NhlNeedProto
	unsigned int	size
#endif
);

extern void *NhlRealloc(
#if	NhlNeedProto
	void*,		/* pointer to copy		*/
	unsigned int	/* size of requested memory	*/
#endif
);

extern NhlErrorTypes NhlFree(
#if	NhlNeedProto
	void*		/* pointer to memory to free	*/
#endif
);

extern Const char *NhlName(
#if	NhlNeedProto
	int	pid	/* id of an object	*/
#endif
);

extern Const char *NhlClassName(
#if	NhlNeedProto
	int	pid	/* id of object whose class name is requested */
#endif
);

extern NhlBoolean NhlClassIsSubclass(
#if	NhlNeedProto
	NhlClass	cl,
	NhlClass	ref_class
#endif
);

extern NhlBoolean NhlIsClass(
#if	NhlNeedProto
	int		id,
	NhlClass	cl
#endif
);

extern NhlClass NhlClassOfObject(
#if	NhlNeedProto
	int		id
#endif
);

NhlDOCTAG(NhlSetSArg)
NhlSRCREF(hlu/hlu.c#NhlSetSArg)
/*VARARGS2*/
extern void NhlSetSArg(
#if	NhlNeedVarArgProto
	NhlSArg		*arg,		/* arg to set		*/
	NhlString	resname,	/* resource to set	*/
	...				/* value to set arg to	*/
#endif
);

NhlDOCTAG(NhlSetGArg)
NhlSRCREF(hlu/hlu.c#NhlSetGArg)
/*VARARGS2*/
extern void NhlSetGArg(
#if	NhlNeedVarArgProto
	NhlGArg		*arg,		/* arg to set		*/
	NhlString	resname,	/* resource to set	*/
	...				/* value to set arg to	*/
#endif
);

NhlDOCTAG(NhlVACreate)
NhlSRCREF(hlu/hlu.c#NhlSetGArg)
/*VARARGS4*/
extern NhlErrorTypes NhlVACreate(
#if	NhlNeedVarArgProto
	int*,			/* return plot id			*/
	Const char*,		/* name					*/
	NhlClass,		/* requested class			*/
	int,			/* parent's id				*/
	...			/* res names/values - NULL terminated	*/
#endif
);

NhlDOCTAG(NhlALCreate)
extern NhlErrorTypes NhlALCreate(
#if	NhlNeedProto
	int*,			/* return plot id	*/
	Const char*,		/* name			*/
	NhlClass,		/* requested class	*/
	int,			/* parent's id		*/
	NhlSArgList,		/* setarg list		*/
	int			/* number of Sarg's	*/
#endif
);

extern NhlErrorTypes NhlCreate(
#if	NhlNeedProto
	int*,			/* return plot id	*/
	Const char*,		/* name			*/
	NhlClass,		/* requested class	*/
	int,			/* parent's id		*/
	int			/* RL list id		*/
#endif
);

NhlDOCTAG(NhlDestroy)
extern NhlErrorTypes NhlDestroy(
#if	NhlNeedProto
	int		/* plot id	*/
#endif
);

NhlDOCTAG(NhlDraw)
extern NhlErrorTypes NhlDraw(
#if	NhlNeedProto
	int	/* id */
#endif
);

NhlDOCTAG(NhlOpen)
extern void NhlOpen(
#if	NhlNeedProto
	void
#endif
);

extern void NhlInitialize(
#if	NhlNeedProto
	void
#endif
);

NhlDOCTAG(NhlClose)
extern void NhlClose(
#if	NhlNeedProto
	void 
#endif 
); 

NhlDOCTAG(NhlVASetValues)
/*VARARGS1*/
extern NhlErrorTypes NhlVASetValues(
#if	NhlNeedVarArgProto 
	int,		/* id		*/
	...		/* resource names and values - NULL terminated	*/
#endif 
);

NhlDOCTAG(NhlALSetValues)
extern NhlErrorTypes NhlALSetValues(
#if	NhlNeedProto
	int		pid,	/* id		*/
	NhlSArgList	args,	/* args to set	*/
	int		nargs	/* num args	*/
#endif
);

extern NhlErrorTypes NhlSetValues(
#if	NhlNeedProto
	int	pid,	/* id of object	*/
	int	rlid	/* RL list id	*/
#endif
);

NhlDOCTAG(NhlVAGetValues)
/*VARARGS1*/
extern NhlErrorTypes NhlVAGetValues(
#if	NhlNeedVarArgProto
	int,		/* id		*/
	...		/* resource names and values - NULL terminated	*/
#endif
);

extern NhlErrorTypes NhlALGetValues(
#if	NhlNeedProto
	int		pid,	/* id			*/
	NhlGArgList	args,	/* args to retrieve	*/
	int		nargs	/* num args		*/
#endif
);

extern NhlErrorTypes NhlGetValues(
#if	NhlNeedProto
	int	pid,	/* id of object	*/
	int	rlid	/* RL list id	*/
#endif
);

/*
 * Currently only access to "reparent" method.
 */
extern NhlErrorTypes NhlChangeWorkstation(
#if	NhlNeedProto
	int	/*plotid*/,		/* plotid to move to new workstation	*/
	int	/*workid*/		/* workid of workstation		*/
#endif
);

extern int NhlGetParentWorkstation(
#if	NhlNeedProto
	int	pid
#endif
);

extern int NhlGetParentId(
#if	NhlNeedProto
	int	pid
#endif
);
#endif /* _NHLU_h */
