.TH AGCHNL 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
AGCHNL - 
Provides a way for the user to substitute arbitrary
character strings for the numeric labels generated by
Autograph.
.SH SYNOPSIS
 SUBROUTINE AGCHNL (IAXS,VILS,CHRM,MCIM,NCIM,IPXM, 
.br
+ CHRE,MCIE,NCIE)
.SH DESCRIPTION 
.IP IAXS 12
(an input expression of type INTEGER) is the number of
the axis being drawn. The values 1, 2, 3, and 4 imply the
left, right, bottom, and top axes, respectively.
.IP VILS 12
(an input expression of type REAL) is the value to be
represented by the numeric label, in the label system for
the axis. The value of VILS must not be altered.
.IP CHRM 12
(an input variable of type CHARACTER), on entry,
contains the mantissa of the numeric label, as it will
appear if AGCHNL makes no changes. If the numeric label
includes a "times" symbol, it is represented by a blank in
CHRM. (See IPXM, below.)  CHRM may be modified.
.IP MCIM 12
(an input expression of type INTEGER) is the length of
CHRM - the maximum number of characters that it will hold.
The value of MCIM must not be altered.
.IP NCIM 12
(an input variable of type INTEGER), on entry, is the
number of meaningful characters in CHRM. If CHRM is
changed, NCIM should be changed accordingly.
.IP IPXM 12
(an input variable of type INTEGER), on entry, is zero
if there is no "times" symbol in CHRM; if it is non-zero,
it is the index of a character position in CHRM. If AGCHNL
changes the position of the "times" symbol in CHRM, removes
it, or adds it, the value of IPXM must be changed.
.IP CHRE 12
(an input variable of type CHARACTER), on entry,
contains the exponent of the numeric label, as it will
appear if AGCHNL makes no changes. CHRE may be modified.
.IP MCIE 12
(an input expression of type INTEGER) is the length of
CHRE - the maximum number of characters that it will hold.
The value of MCIE must not be altered.
.IP NCIE 12
(an input variable of type INTEGER), on entry, is the
number of meaningful characters in CHRE. If CHRE is
changed, NCIE should be changed accordingly.
.SH USAGE
This routine is not called by the user program, but by
Autograph itself, just after the character string
representing each numeric label has been generated and just
before it is written on the graph. The user may change the
character string in any desired way. Axes may thereby be
labeled using the names of the months, Roman numerals, etc.
.sp
Note:  A user version of AGCHNL should not call any other
Autograph routine.
.SH ACCESS
To use AGCHNL, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.
.SH SEE ALSO
Online:
autograph,
agback,
agbnch,
agchax,
agchcu,
agchil,
agcurv,
agdshn,
aggetc,
aggetf,
aggeti,
aggetp,
aggetr,
agpwrt,
agrstr,
agsave,
agsetc,
agsetf,
agseti,
agsetp,
agsetr,
agstup,
agutol,
anotat,
displa,
ezmxy,
ezmy,
ezxy,
ezy
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
