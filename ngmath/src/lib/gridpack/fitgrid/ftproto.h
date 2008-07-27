/*
 * $Id: ftproto.h,v 1.12 2008-07-27 04:02:36 haley Exp $
 */
/************************************************************************
*                                                                       *
*                Copyright (C)  2000                                    *
*        University Corporation for Atmospheric Research                *
*                All Rights Reserved                                    *
*                                                                       *
*    The use of this Software is governed by a License Agreement.       *
*                                                                       *
************************************************************************/

#include <ncarg/ngmath.h>
 
/*
 * Prototypes for internal routines.
 */
extern void *copy_dtof(void *dval, int size);
extern void *copy_ftod(void *fval, int size);
extern void convert_to_double(FTdata *z);
extern void convert_to_float(FTdata *z);
