.\"
.\"	$Id: gqchh.m,v 1.3 1993-05-03 17:26:31 haley Exp $
.\"
.TH GQCHH 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GQCHH (Inquire character height) - retrieves the current character height.
.SH SYNOPSIS
CALL GQCHH (ERRIND, CHH)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_char_ht(Gint *err_ind, Gdouble *char_ht);
.SH DESCRIPTION
.IP ERRIND 12
(Integer, Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in ERRIND, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP CHH 12
(Real, Output) - 
Contains the current character height as set by default or by GSCHH.
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library 
ncarg_gks.
.SH SEE ALSO
Online: 
gtx, gstxp, gstxal, gstxfp, gschh, gschsp, gschup, 
gschxp, gscr, gstxci, gqtxp, gqtxal, gqtxfp,
gqchsp, gqchup, gqchxp, plotchar, ginq_char_ht
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
