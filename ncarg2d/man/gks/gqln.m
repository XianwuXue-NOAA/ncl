.\"
.\"	$Id: gqln.m,v 1.2 1993-04-02 16:47:01 haley Exp $
.\"
.TH GQLN 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GQLN (Inquire line type) - retrieves the current setting for linetype.
.SH SYNOPSIS
CALL GQLN (ERRIND, LTYPE)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_linetype(Gint *err_ind, Gint *linetype);
.SH DESCRIPTION
.IP ERRIND 12
(Integer, Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in ERRIND, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP LTYPE 12
(Integer, Output) - Returns the current value for the line type as set by
default or by a call to GSLN.
.RS
.IP 1 
Solid 
.IP 2
Dashed
.IP 3
Dotted
.IP 4
Dot-dash
.RE
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library ncarg_gks.
.SH SEE ALSO
Online: 
gpl, gsln, gslwsc, gscr, gsplci, gqlwsc, gqplci, dashline, ginq_linetype
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
