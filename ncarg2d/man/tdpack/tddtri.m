.TH TDDTRI 3NCARG "July 1997" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
TDDTRI - Draw the triangles defined by a triangle list.
.SH SYNOPSIS
CALL TDDTRI (RTRI, MTRI, NTRI, ITWK)
.SH C-BINDING SYNOPSIS
#include <ncarg/ncargC.h>
.sp
void c_tddtri(float *rtri, int mtri, int *ntri, int *itwk)
.SH DESCRIPTION
This routine draws the triangles defined by a triangle list.  The triangles
are drawn in the order specified by the contents of the permutation array ITWK.
.sp
The exact manner in which each triangle is drawn depends on the
rendering style selected for it.  Generally, it is first filled with a
user-selected color; if a wire-frame image is desired and no lines are
to be considered hidden, this step may be skipped.  The fill color can be a
specific one chosen by the user (sometimes, it is useful to pick the
background color) or one selected from a range of user-specified colors
according to the angle between a line from the light source to the
center of the triangle and the normal to the plane of the triangle
(which results in a shading of the surface).  After the triangle is filled,
line segments representing its boundary and/or its intersection with
planes of constant U, V, and/or W may be drawn in a user-specified
contrasting color.
.sp
The arguments of TDDTRI are as follows:
.IP "RTRI" 8
(an input array, of type REAL, dimensioned 10 x MTRI) -
a list of triangles, probably created by means of calls to TDSTRI, TDITRI,
and/or TDMTRI, and sorted, probably by means of a call to TDOTRI.
.IP " " 8
If you put triangles in the triangle list directly, keep in mind the
following: For each value of i from 1 to NTRI, RTRI(1,i), RTRI(2,i),
and RTRI(3,i) are the U, V, and W coordinates of the first vertex of the
triangle, RTRI(4,i), RTRI(5,i), and RTRI(6,i) are the U, V, and W
coordinates of the second vertex of the triangle, RTRI(7,i), RTRI(8,i),
and RTRI(9,i) are the U, V, and W coordinates of the third vertex of
the triangle, and RTRI(10,i)=REAL(irst), where irst is the index of the
desired rendering style array for the triangle. The order of the vertices
is meaningful: they must be given in counter-clockwise order as
viewed from the "top". It is better if all of the triangles in a triangle list
are about the same size.
.IP "MTRI" 8
(an input expression of type INTEGER) - the second dimension of RTRI
and thus the maximum number of triangles the triangle list will hold.
.IP "NTRI" 8
(an input expression of type INTEGER) - specifies the number of triangles
currently in the list.
.IP "ITWK" 8
(an input array, of type INTEGER, dimensioned at least NTRI) -
contains a permutation of the integers from 1 to NTRI (probably
generated by calling the routine TDOTRI).
.SH C-BINDING DESCRIPTION 
The C-binding argument descriptions are the same as the FORTRAN 
argument descriptions.
.SH ACCESS
To use TDDTRI or c_tddtri, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order. 
.SH SEE ALSO
Online:
tdclrs, tdctri, tdgeti, tdgetr, tdgrds, tdgrid, tdgtrs, tdinit, tditri, tdlbla,
tdlbls, tdline, tdlnpa, tdmtri, tdotri, tdpack, tdpack_params, tdpara,
tdplch, tdprpa, tdprpi, tdprpt, tdseti, tdsetr, tdsort, tdstri, tdstrs
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

