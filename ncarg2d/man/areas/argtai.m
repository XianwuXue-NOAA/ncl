�7                     
b  O      
y                               � +  $    $    $   2    N    N    N    N 
  X 
  b    b &  � x  R�           6 *  `�  $                                                                        .\".\"	$Id: argtai.m,v 1.2 1993-04-20 16:54:44 haley Exp $.\".TH ARGTAI 3NCARG "March 1993" UNIX "NCAR GRAPHICS".SH NAMEARGTAI - Gets the area identifiers associated with a given point..SH SYNOPSISCALL ARGTAI (MAP, XCD, YCD, IAI, IAG, MAI, NAI, ICF).SH C-BINDING SYNOPSIS#include <ncarg/ncargC.h>.spvoid c_argtai (int *map, float xcd, float ycd, int *iai, int *iag, int mai, int *nai, int icf).SH DESCRIPTION .IP "MAP(LMAP)" 12(Integer array, Workspace) - The area-map array..IP "XCD" 12(Real, Input) - The X coordinate, in the current user coordinate system, of a point about which you want to obtain information..IP "YCD" 12(Real, Input) - The Y coordinate, in the current user coordinate system, of a point about which you want to obtain information..IP "IAI(MAI)" 12(Integer array, Input/output) - Integer array of size MAI, to which information about the specified point will be returned..IP "IAG(MAI)" 12(Integer array, Input/output) - Integer array of size MAI, to which information about the specified point will be returned..IP "MAI" 12(Integer, Input) - Dimension of each of the arrays IAI and IAG. MAI \(>= NAI..IP "NAI" 12(Integer, Output) - Number of values returned in IAI and IAG. NAI equals the number of groups of edges that you put in the area map..IP "ICF" 12(Integer, Input) - Flag set nonzero by you to indicate that the definition of the user coordinate system has been changed since the last call to ARGTAI; in this case, calls to GETSET must be executed by ARGTAI. If you set the flag to zero, Areas assumes that the information retrieved previously is still correct and skips calls to GETSET..SH C-BINDING DESCRIPTION The C-binding argument descriptions are the same as the Fortran argument descriptions..SH ACCESSTo use ARGTAI, load the NCAR Graphics libraries ncarg, ncarg_gks,ncarg_c, and ncarg_loc, preferably in that order.  To use c_argtai, load the NCAR Graphics libraries ncargC, ncarg_gksC, ncarg, ncarg_gks, ncarg_c, and ncarg_loc, preferably in that order..SH SEE ALSOOnline:ardbpa, ardrln, aredam, argeti, argtai, arinam, arpram, arscam, arseti, areas, ncarg_cbind.spHardcopy:NCAR Graphics Contouring and Mapping Tutorial"NCAR Graphics User's Guide," Version 2.00.SH COPYRIGHT(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporation for Atmospheric Research.brAll Rights Reserved. areas, areas_params, ardbpa, ardrln, aredam, argeti, arinam, arpram, arscam, arseti, ncarg_cbindReturnsa workspace array, dimensioned LMAP, of type INTEGERan input expression of type REALan input expression of type REALan input expression of type INTEGERan output expression of type INTEGERan input expression of type INTEGER5�s|:�w;�~y��%^0{,`-a�z�x.��97e�z^0{,y�wp.�d^.�_c.�[k.�Wބ	210/.� +��     
b  
c  
e  
f  
g  
l  
z  
�  
�  
�  
�  
�  
�  
�  
�  
�  
�  
�    $  D  `  g  j  m  �  �  �  �  �    O  Y  b  �  �  �        A  I  c  �                             �                                                                                                                                                                                                                                                                                                       @  ,       =  A  u  ~  �  �  �      4  8  �  �  �  �  �  �    2  i  }  �  �  �  �      @  _  �  �  �  �  �  %  /  <  P  y  �  �  �  �    !  .  B  l  �  �    G  �  �  �  �  �    I  �  �  	  	  	  	^  	y  	}  	�  	�  	�  	�  
1  
J  
N  
b  
c  
f  
g  
�  
�  �  �    �  �    A  �  �������������������������������������������������������������������������������������������                                        !�     !�  Z    �� 	  �        �   �   �       �� 	  �        �   �   �       �� 	  �        �   �   �       �� 	  �        �   �   �   The X coordinate, in the current user coordinate system, of a point about which you want to obtain information.an output array, dimensioned MAI, of type INTEGERThe array in which area identifiers for areas containing the specified point will be returned.an output array, dimensioned MAI, of type INTEGERThe array in which group identifiers for areas containing the specified point will be returned. must be greater than or equal to  This option is valuable if you are planning to make thousands of calls to ARGTAI with the same area map.    �� 	  �        �   �   �       �� 	  �        �   �   �         
y@����      �  
y��    8 ��               =   A   u   ~   �   �   �      7  ;    �  �  �  �      @  b  �  �  �  �    C  W  i  �  �       I  �  �  �  �  	  8  E  o  �  �  �  �    B  y  �  �    W  �  �  �  �  &  =  H  �  �  	  	H  	U  	]  	�  	�  	�  	�  	�  
	  
H  
a  
e  
y !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�      �      �                ! " � � � �    H H    �(�����FG(�    H H    �(    d       '                �     @                         =�/���  �R    @      H -:LaserWriter 
     ( U� (  ��          � �          
N        N      �       �   �   �   �   �  ~    �  �    ;  @  �  �  �  �  �  j  �  �  �  �         D  I  S  �  �  �  �  �  �  �  �  �      3  E  F  I  L  c  j  o  �  �        ^  �  �  �  �  �  �  �  	
  	  	  	]  	b  	p  	�  	�  	�  	�  	�  	�  	�  	�  	�  	�  	�  
	  
u  
x  
y      �  
�     �  �  ��    ��   �     
b     {  �  
�     �  �            �     }  �  �  �  $          �  �       ;  �  O  �  Y     b     �  �    �  �     ;  �  �  �  �       �            /  �  �  �  D  �  `          P  �  A  �  I     �  �  �  �  g  �  j  �  m  �  �          �  �  �  �  �  �  �          B     c     �     �  �  
c  �  |     
e  �  �  �  �     
f  �  �     �  �  
g  �  
l  �  
z  �  
�  �  
�     
�  �  
�  �  
�  �  
�  �  
�  �  
�     
�     	y     	�     	�  �  
^     
a                                                                                                                                                                                   