.TH TICKS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
TICKS - 
Allows for program control of tick mark length and
direction. This routine has been superseded by TICK4, which
should be used instead.
.SH SYNOPSIS
CALL TICKS(LMJR,LMNR)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_ticks (int lmjr, int lmnr)
.SH DESCRIPTION 
.IP "LMJR and LMNR" 12
See the description in the TICK4 man page.
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH USAGE
The statement
.RS 3 
.sp
CALL TICKS (LMJR,LMNR)
.sp
.RE
is equivalent to 
.RS 3
.sp
CALL TICK4 (LMJR,LMNR,LMJR,LMNR)
.RE
.SH ACCESS
To use TICKS, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  To use c_ticks, load the 
NCAR Graphics libraries ncargC, ncarg_gksC, ncarg, ncarg_gks,
and ncarg_c, preferably in that order.
.SH SEE ALSO
Online:
gridall,
gridall_params,
gacolr,
gagetc,
gageti,
gagetr,
gasetc,
gaseti,
gasetr,
grid,
gridal,
gridl,
halfax,
labmod,
perim,
periml,
tick4,
ncarg_cbind.
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
