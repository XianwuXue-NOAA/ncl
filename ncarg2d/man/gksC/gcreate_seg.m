.\"
.\"	$Id: gcreate_seg.m,v 1.9 2000-07-11 23:03:24 haley Exp $
.\"
.TH GCREATE_SEG 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
gcreate_seg (Create segment) - creates a GKS segment.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void gcreate_seg(Gint seg_name);
.SH DESCRIPTION
.IP seg_name 12
(Input) - Specifies the segment identifier that will be used in
calls to subsequent segment functions applying to this segment.  There can be
a maximum of 100 segments in a given job step.  Legal values for seg_name are
integers between 0 and 99 inclusive.
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gks and
ncarg_c libraries.
.SH SEE ALSO
Online: 
.BR geval_tran_matrix(3NCARG),
.BR gaccum_tran_matrix(3NCARG),
.BR gclose_seg(3NCARG),
.BR gcopy_seg_ws(3NCARG),
.BR gdel_seg(3NCARG),
.BR ginq_name_open_seg(3NCARG),
.BR ginq_set_seg_names(3NCARG),
.BR gset_seg_tran(3NCARG),
.BR gks(3NCARG),
.BR ncarg_gks_cbind(3NCARG)
.sp
Hardcopy: 
User's Guide for NCAR GKS-0A Graphics;
NCAR Graphics Fundamentals, UNIX Version
.SH COPYRIGHT
Copyright (C) 1987-2000
.br
University Corporation for Atmospheric Research
.br

This documentation is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA.
