.TH VVGETI 3NCARG "April 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
VVGETI - 
Gets the current value of a
Vectors parameter of type INTEGER.
.SH SYNOPSIS
CALL VVGETI (CNM,IVL) 
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_vvgeti(char *cnm, int *ivl)
.SH DESCRIPTION 
.IP CNM 12
(CHARACTER, input) is the name of a parameter whose
integer value is to be retrieved. Only the first three
characters of CNM are examined. The three characters may
either be entirely upper or entirely lower case; mixed case
is not recognized. It is recommended that the rest of the
character string be used to improve the readability of the
code. For example, instead of just \'PLR\', use \'PLR - Polar
Input Mode\'.
.IP IVL 12
(INTEGER, output) is a variable in which the value of
the parameter specified by CNM is to be returned.
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN
argument descriptions.
.SH USAGE
This routine allows you to retrieve the current value of Vectors
parameters of type INTEGER.  For a complete list of parameters
available in this utility, see the vectors_params man page.
.SH ACCESS
To use VVGETI or c_vvgeti, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  
.SH MESSAGES
See the vectors man page for a description of all Vectors error
messages and/or informational messages.
.SH SEE ALSO
Online:
vectors,
vectors_params,
vvectr,
vvgetc,
vvgetr,
vvinit,
vvrset,
vvsetc,
vvseti,
vvsetr,
vvudmv,
vvumxy,
ncarg_cbind.
.sp
Hardcopy:
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
