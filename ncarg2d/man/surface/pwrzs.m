.TH PWRZS 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
PWRZS - A character-plotting routine for plotting characters in
three-space when using SRFACE.
.SH SYNOPSIS
CALL PWRZS (X,Y,Z,ID,N,ISIZE,LINE,ITOP,ICNT)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_pwrzs (float x, float y, float z, char *id, 
.br
int n, int isize, int line, int itop, int icnt)
.SH DESCRIPTION
.IP "X,Y,Z" 12
Positioning coordinates for the characters to be drawn.
These are floating point numbers in the same
three-space as used in SRFACE.
.IP ID 12
Character string to be drawn. ID is of type CHARACTER.
.IP N 12
The number of characters in ID.
.IP ISIZE 12
Size of the character:
.RS
.IP \(bu 2
If between 0 and 3, ISIZE is 1., 1.5, 2., or
3. times a standard width equal to 1/128th
of the screen width.
.IP \(bu 2
If greater than 3, ISIZE is the character
width in plotter address units.
.RE
.IP LINE 12
The direction in which the characters are to be
written.
.in +5
.sp
1 = +X -1 = -X
.br
2 = +Y -2 = -Y
.br
3 = +Z -3 = -Z
.in -5
.IP ITOP 12
The direction from the center of the first character to
the top of the first character (the potential values
for ITOP are the same as those for LINE as given
above). Note that LINE cannot equal ITOP even in
absolute value.
.IP ICNT 12
Centering option.
.RS
.IP "-1 (X,Y,Z)" 12
is the center of the left edge of
the first character.
.IP "0 (X,Y,Z)" 12 
is the center of the entire string.
.IP "1 (X,Y,Z)" 12 
is the center of the right edge of
the last character.
.RE
.IP ""
Note that the hidden character problem is solved correctly for
characters near, but not inside, the three-space object.
.SH C-BINDING DESCRIPTION
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH USAGE
Call PWRZS after calling SRFACE and before calling FRAME.
.SH EXAMPLES
Use the ncargex command to see the following relevant
examples: 
fsrpwrzs,
tpwrzs.
.SH ACCESS
To use PWRZS or c_pwrzs, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  
.SH SEE ALSO
Online:
surface,
surface_params,
ezsrfc,
setr,
srface.
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
