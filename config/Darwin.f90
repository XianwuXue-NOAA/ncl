/*
 *      $Id: Darwin.f90,v 1.1 2002-02-22 03:28:32 haley Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  2002			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		Darwin
 *
 *	Author:		Mary Haley
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Sat Feb  9 14:23:43 MST 2002
 *
 *	Description:	This file contains all the directives needed to
 *			tell ymake how Darwin is different from a
 *			default setup.
 */
#define HdfDefines  -DDARWIN
#define StdDefines  -DSYSV -D_POSIX_SOURCE -D_XOPEN_SOURCE
#define Cstatic 
#define Cdynamic 
#define CppCommand '/usr/bin/cpp -traditional'
#define CCompiler   cc
#define CtoFLibraries   -lU77 -lf77math -lfio
#define FCompiler   f90
#define CcOptions      -DAbsoftProFortran -ansi
#define FcOptions      -s -YEXT_NAMES="LCS" -YCOM_NAMES="LCS" -YCOM_PFX="_" -YCOM_SFX="_" -B108
#define XToolLibrary    -lXt -lSM -lICE
#define BuildShared NO
#define XLibrary -lXpm -lX11 -lXext

#define ArchRecLibSearch    -L/usr/X11R6/lib -L/usr/local/lib
#define ArchRecIncSearch    -I/usr/X11R6/include -I/usr/local/include

FC = $(F77)

/*************** Redefine Macros from Rules ********************************/

/*
 * Macro:	MakeDir
 *
 * Description:	This rule creates a directory - if a parent dir doesn't exist
 *		it attempts to create it.
 */
#ifndef MakeDir
#define MakeDir(dir)    @if (test ! -d dir); then ($(MKDIRHIER) dir); fi
#endif
