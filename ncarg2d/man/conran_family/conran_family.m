.TH Conran_family 3NCARG "March 1993" UNIX "NCAR GRAPHICS"
.na
.nh
.SH NAME
Conran_family - is a set of two dimensional contour mapping routines
for irregularly distributed data.
.sp
There are three Conran_family entries, CONRAN, CONRAQ, and CONRAS.
CONRAN can be invoked in two ways.  One generates smoothed contours,
the other does not.  CONRAQ is a quick version of the package which
has a more limited set of options than the other entries.  CONRAS is
the super version of the package.
.SH STATUS
The Conran_family is approaching obsolescence.  If you have available an
interpolation package which can do a random to gridded interpolation
equal to that built into the Conran_family, then we recommend that
you grid your dataset and use the Conpack utility instead.
Refer to the conpack and
conpack_params man pages as well as the NCAR Graphics
Contouring and Mapping Tutorial.
.SH SYNOPSIS
CONRAN - Provides the standard and smooth versions for contouring of
irregularly gridded data.
.sp
CONRAQ - Provides the quick form of the package.  It does not have
as many available options as the other entries.
.sp
CONRAS - This is the super version of the package.  Not only does it
smooth and label contour lines, it can also reduce line crowding.
.sp
CONOP1 - Sets values of internal parameters of type logical.
Specifically, CONOP1 is used to turn options "OFF", or "ON".
.sp
CONOP2 - Sets values of internal parameters of type integer.
.sp
CONOP3 - Sets values of internal parameters of type real.
.sp
CONOP4 - Sets values of internal parameters of type character.
.SH ACCESS 
There are three entries in the Conran_family which create contour plots:
CONRAQ (quick), CONRAS (super), and CONRAN (standard and smooth.)
.sp
To use standard CONRAN, load the NCAR Graphics libraries ncarg, ncarg_gks,
and ncarg_c, preferably in that order.  To run any of the other three
options (quick, super, or smooth), one or more additional libraries is
needed as follows:
.sp
quick -- additionally load the conraq library.
.sp
smooth -- additionally load the dashsmth library.
.sp
super -- additionally load the conras and dashsupr libraries.
.sp
These libraries can also be implicitly loaded by using the
-quick, -smooth, or -super option of the ncargf77 command.
For example, to run a code called mycode.f which has one or more calls to
a Conran_family entry, issue one of the commands:
.sp2
.IP QUICK 10
Command: "ncargf77 -quick mycode.f"
.sp
mycode.f has one or more calls to entry CONRAQ.
.IP NORMAL 10
Command: "ncargf77 mycode.f"
.sp
mycode.f has one or more calls to entry CONRAN.
.IP SMOOTH 10
Command: "ncargf77 -smooth mycode.f"
.sp
mycode.f has one or more calls to entry CONRAN.
.IP SUPER 10
Command: "ncargf77 -super mycode.f"
.sp
mycode.f has one or more calls to entry CONRAS.
.sp
Warning:  mycode.f can only call one of the three entries,
CONRAQ, CONRAN, or CONRAS.
.SH MESSAGES
Error messages generated by CONRAN routines include:
.sp
.IP CONCLS 10
CONSTANT INPUT FIELD
.br
The input data all has a constant value.  This may indicate that
the data has not been input correctly in the user program.
.IP CONDRW 10
CONTOUR STORAGE EXHAUSTED
.br
This error is trapped and
nullified by CONRAN.  It
serves to signal the user
that a contour level may not
be complete.
.IP CONOP 10
UNDEFINED OPTION
.br
The parameter name has either been misspelled in a CONOP call, or an
out of range parameter value has been specified.  This message can
be generated by CONOP1, CONOP2, CONOP3, or CONOP4.
.IP CONOP3 10
ILLEGAL USE OF CON OPTION
WITH CIL OR CHL OPTIONS
.br
Review the definitions of these three parameters using man page
conran_family_params.
.IP CONOP3 10
NUMBER OF CONTOUR LEVELS
EXCEEDS 30
.br
Reduce the requested number of contour levels specified by parameter
CON to 30 or less.
.IP CONRAN 10
INPUT PARAMETER NDP LT NCP
.br
NDP is the total number of input values for contouring.  NCP is the
number of points used for partial derivative estimation.  NCP cannot
exceed NDP.  This error message also occurs in CONRAS and CONRAQ.
.IP CONRAN 10
NCP LT 2 OR GT NCPSZ
.br
The legal value of NCP must be between NCPSZ (dimension size allowed) and 2.
This error message also occurs in CONRAS and CONRAQ.
.IP CONSTP 10
ASPECT RATIO OF X AND Y
GREATER THAN 5 TO 1.
.br
(This error may cause a poor
quality plot.  Usually this
can be fixed by multiplying
X or Y by a constant factor.
If this solution is
unacceptable then increasing
internal parameter
SSZ to a very large value
may help.  Note:  This can be
expensive.)
.IP CONTNG 10
ALL COLINEAR DATA POINTS
.br
Indicates that the input data set to be contoured all lies along a
straight line.  Thus, triangulation cannot occur.
.IP CONTNG 10
IDENTICAL INPUT DATA POINTS FOUND
.sp
The errors listed above are defined as recoverable
errors should the user wish to use them in that
fashion.  The documentation on the ERPRT77 package
explains how to recover from an error.
.SH SEE ALSO
Online:
conran, conraq, conras, conran_family_params, conop1, conop2, conop3, conop4,
conpack, conpack_params, cpcnrc, bivar,
dashline_family, ncargf77
.sp
Hardcopy:  
NCAR Graphics Contouring and Mapping Tutorial;
NCAR Graphics Fundamentals, UNIX Version;
User's Guide for NCAR GKS-0A Graphics
.SH COPYRIGHT
Copyright 1987, 1988, 1989, 1991, 1993 University Corporation
for Atmospheric Research
.br
All Rights Reserved
