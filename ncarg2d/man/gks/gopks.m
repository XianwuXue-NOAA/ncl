.\"
.\"	$Id: gopks.m,v 1.2 1993-04-02 16:45:16 haley Exp $
.\"
.TH GOPKS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GOPKS (Open GKS) - opens the GKS package
.SH SYNOPSIS
CALL GOPKS (ERRFIL, BUFA)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void gopen_gks(const char *err_file, size_t mem_unit);
.SH DESCRIPTION
.IP ERRFIL 12
(Input, Integer) - The Fortran unit number to which 
error messages are to be written. 
Typically this should be unit 6.
.IP BUFA 12
(Input, Integer) - The dimension of an internal buffer. 
Currently in NCAR GKS-0A, BUFA is ignored.
.SH USAGE
To get output to any workstation,
GOPKS, GOPWK and GACWK must be called in that order.
For jobs producing only CGM output, the three calls to GOPKS, GOPWK and GACWK 
can be replaced with a call to the SPPS function OPNGKS.
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library 
ncarg_gks.
.SH SEE ALSO
Online: 
gopwk, gacwk, gdawk, gclwk, gclks, guwk, opngks, clsgks, gopen_gks
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
