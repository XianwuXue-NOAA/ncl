.\"
.\"	$Id: gset_char_expan.m,v 1.9 2000-07-11 23:03:32 haley Exp $
.\"
.TH GSET_CHAR_EXPAN 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.SH NAME
gset_char_expan (Set character expansion factor) - sets 
the width to
height ratio of text.
.SH SYNOPSIS
#include <ncarg/gks.h>
.sp
void gset_char_expan(Gdouble char_expan);
.SH DESCRIPTION
.IP char_expan 12
(Input) - Specifies a deviation of the width to height ratio for characters 
drawn using the GTX output primitive. char_expan 
of 10.0 would specify a character 
that is 10 times as wide as normal. 
char_expan of 0.1 would request characters 
that are 1/10th as wide as normal. 
The character height remains 
unchanged. char_expan > 0.
By default char_expan = 1.0
.SH ACCESS
To use the GKS C-binding routines, load the ncarg_gks and
ncarg_c libraries.
.SH SEE ALSO
Online: 
.BR gtext(3NCARG),
.BR gset_text_path(3NCARG),
.BR gset_text_align(3NCARG),
.BR gset_text_font_prec(3NCARG),
.BR gset_char_ht(3NCARG),
.BR gset_char_space(3NCARG),
.BR gset_char_up_vec(3NCARG),
.BR gset_colr_rep(3NCARG),
.BR gset_text_colr_ind(3NCARG),
.BR ginq_text_path(3NCARG),
.BR ginq_text_align(3NCARG),
.BR ginq_text_font_prec(3NCARG),
.BR ginq_char_ht(3NCARG),
.BR ginq_char_space(3NCARG),
.BR ginq_char_up_vec(3NCARG),
.BR ginq_char_expan(3NCARG),
.BR plotchar(3NCARG),
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
