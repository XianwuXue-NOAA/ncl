.\"
.\"	$Id: gqmntn.m,v 1.3 1993-05-03 17:27:13 haley Exp $
.\"
.TH GQMNTN 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
GQMNTN (Inquire maximum normalization transformation number) - retrieves 
the maximum number of supported normalization transformations.
.SH SYNOPSIS
CALL GQMNTN (ERRIND, MAXTRN)
.SH C-BINDING SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_max_norm_tran_num(Gint *err_ind, Gint *max_norm_tran_num);
.SH DESCRIPTION
.IP ERRIND 12
(Integer, Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in ERRIND, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP MAXTRN 12
(Integer, Output) - 
Gives the maximum number of normalization transformations 
available in the GKS package. In NCAR GKS-0A, MAXTRN = 2.
.SH ACCESS
To use GKS routines, load the NCAR GKS-0A library 
ncarg_gks.
.SH SEE ALSO
Online: 
gselnt, gqcntn, gqnt, getset, ginq_max_norm_tran_num
.sp
Hardcopy: 
"User's Guide for NCAR GKS-0A Graphics"
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
