
/*
 *      $Id: Variable.h,v 1.4 1993-12-21 19:18:20 ethan Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  1993			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		
 *
 *	Author:		Ethan Alpert
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Mon Jun 28 10:53:34 MDT 1993
 *
 *	Description:	
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NCVariable_h
#define _NCVariable_h



typedef struct _NclVarInfo {
	char	varname[NCL_MAX_STRING];
	int level;
	int datatype;
	unsigned int offset;
}NclVarInfo; 


#endif  /*_NCVariable_h*/

#ifdef __cplusplus
}
#endif