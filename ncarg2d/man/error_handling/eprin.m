.TH EPRIN 3NCARG "March 1994" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
EPRIN - Prints the current error message.
.SH SYNOPSIS
CALL EPRIN
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_eprin()
.SH DESCRIPTION 
If SETER's internal error flag is non-zero, the FORTRAN statement "CALL EPRIN"
prints the remembered error message associated with the non-zero value.
Otherwise, EPRIN does nothing.
.sp
EPRIN has no arguments.
.SH C-BINDING DESCRIPTION 
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH EXAMPLES
Use the ncargex command to see the following relevant
examples: 
tseter,
arex02.
.SH ACCESS
To use EPRIN, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order. To use c_eprin,
load the NCAR Graphics libraries ncargC, ncarg_gksC, ncarg, 
ncarg_gks, and ncarg_c, preferably in that order.
.SH SEE ALSO
Online:
entsr, errof, error_handling, fdum, icfell, icloem, nerro, retsr, semess, seter,
ncarg_cbind
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
