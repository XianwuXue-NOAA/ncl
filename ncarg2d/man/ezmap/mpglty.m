.TH MPGLTY 3NCARG "April 1998" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
MPGLTY - Retrieves the type of the line currently being drawn by MPLNDM.
.SH SYNOPSIS
CALL MPGLTY (ILTY)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_mpglty (int *ilty)
.SH DESCRIPTION 
.IP ILTY 12
(an output variable of type INTEGER) is the type of the line: 1 => a line
separating land from water, 2 => a line separating one "continent" from another
(as, for example, Africa from Eurasia, North America from Central America, or
Central America from South America), 3 => a line separating one country from
another, 4 => a line separating one state from another, and 5 => a line
separating one county from another.  If the current line is used on more than
one level (as, for example, a portion of the California coastline, which is
used on all five levels), then the value of ILTY will be the smallest of the
numerical values that apply.
.SH C-BINDING DESCRIPTION
The C-binding argument description is the same as the FORTRAN 
argument description.
.SH USAGE
MPGLTY may be called from within a line-processing routine whose name is used
in a call to MPLNDM to retrieve, in ILTY, the type of the line being processed.
This information may be used to determine how the line is to be drawn.
.SH EXAMPLES
Use the ncargex command to see the following relevant example: mpex11.
.SH ACCESS
To use MPGLTY or c_mpglty, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  
.SH SEE ALSO
Online:
ezmap,
ezmap_params,
mapaci,
mapbla,
mapblm,
mapdrw,
mapeod,
mapfst,
mapgci,
mapgrd,
mapgrm,
mapgtc,
mapgti,
mapgtl,
mapgtr,
mapint,
mapiq,
mapiqa,
mapiqd,
mapiqm,
mapit,
mapita,
mapitd,
mapitm,
maplbl,
maplmb,
maplot,
mappos,
maproj,
maprs,
maprst,
mapsav,
mapset,
mapstc,
mapsti,
mapstl,
mapstr,
maptra,
maptri,
maptrn,
mapusr,
mapvec,
mpchln,
mpfnme,
mpgetc,
mpgeti,
mpgetl,
mpgetr,
mpiaty,
mpifnb,
mpilnb,
mpiola,
mpiosa,
mpipai,
mpipan,
mpipar,
mpisci,
mplnam,
mplndm,
mplndr,
mplnri,
mpname,
mprset,
mpsetc,
mpseti,
mpsetl,
mpsetr,
supmap,
supcon,
ncarg_cbind
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
