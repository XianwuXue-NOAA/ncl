.\"
.\"	$Id: ginq_cur_norm_tran_num.m,v 1.4 1994-08-11 17:52:52 haley Exp $
.\"
.TH GINQ_CUR_NORM_TRAN_NUM 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
ginq_cur_norm_tran_num (Inquire current normalization transformation number) - retrieves
the number of the current normalization transformation.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void ginq_cur_norm_tran_num(Gint *err_ind, Gint *norm_tran_num);
.SH DESCRIPTION
.IP err_ind 12
(Output) - If the inquired value cannot be returned correctly,
a non-zero error indicator is returned in err_ind, otherwise a zero is returned.
Consult "User's Guide for NCAR GKS-0A Graphics" for a description of the
meaning of the error indicators.
.IP norm_tran_num 12
(Output) - 
Returns the current normalization transformation number as set by default
or by a call to gsel_norm_tran.
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gksC, ncarg_gks, and
ncarg_c libraries.
.SH SEE ALSO
Online: 
.BR gsel_norm_tran(3NCARG),
.BR ginq_max_norm_tran_num(3NCARG),
.BR ginq_norm_tran(3NCARG),
.BR getset(3NCARG),
.BR gks(3NCARG),
.BR ncarg_gks_cbind(3NCARG)
.sp
Hardcopy: 
User's Guide for NCAR GKS-0A Graphics;
NCAR Graphics Fundamentals, UNIX Version
.SH COPYRIGHT
(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
