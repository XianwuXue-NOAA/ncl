�7                     r  �      �                               V +  �    �    �   �                 
  � 
  �    � &  � x  ��  1    Q   g *  �O  �                                                                        .\".\"	$Id: arinam.m,v 1.2 1993-04-20 16:54:47 haley Exp $.\".TH ARINAM 3NCARG "March 1993" UNIX "NCAR GRAPHICS".SH NAMEARINAM - Initializes area map..SH SYNOPSISCALL ARINAM (MAP, LMAP).SH C-BINDING SYNOPSIS#include <ncarg/ncargC.h>.spvoid c_arinam (int *map, int lmap).SH DESCRIPTION.IP "MAP(LMAP)" 12(output array, of type INTEGER, dimensioned at least LMAP) - An integer array in which an area map is to be constructed. Each vertex for an edge segment in your area map requires ten words in the array MAP. Remember that the total number of vertexes includes those added at each intersection of edges, and those added when long edge segments are broken into shorter edge segments..IP "LMAP"(an input expression of type INTEGER) - Length of the MAP array..SH C-BINDING DESCRIPTION The C-binding argument descriptions are the same as the Fortran argument descriptions..SH ACCESSTo use ARINAM, load the NCAR Graphics libraries ncarg, ncarg_gks,ncarg_c, and ncarg_loc, preferably in that order.  To use c_arinam, load the NCAR Graphics libraries ncargC, ncarg_gksC, ncarg, ncarg_gks, and ncarg_loc, preferably in that order..SH SEE ALSOOnline:ardbpa, ardrln, aredam, argeti, argtai, arinam, arpram, arscam, arseti, areas, ncarg_cbind.spHardcopy:Tutorial: A Step-by-Step Guide to Contouring and Mapping; "NCAR Graphics Autograph, A Graphing Utility," Version 2.00, August 1987"; "NCAR Graphics User's Guide," Version 2.00, and"NCAR Graphics Guide to New Utilities," Version 3.00 .SH COPYRIGHT(c) Copyright 1987, 1988, 1989, 1991, 1993 University Corporationfor Atmospheric Research.brAll Rights Reserved_paramsNCAR Graphics Fundamentals. Hardcopy:NCAR Graphics Contouring and Mapping Tutorial of the continent is to the left of each edge s     r  s  t  {  |  �  �  �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  =  A  u  ~  �  �  �  �  �  �    *  =  {  �  �  �  )  Z  �  �  �  �  �    *  k  �  �  �    `  �  �  �  �  �  �    ?  }  �  �  �  A  Z  ^  r  s  t  |  �  �������������������������������������������������������                                                                                                                                                                                                                  !�              !�  6    �� 	  �        �   �   �       �� 	  �        �   �   �       �� 	  �        �   �   �         �@����      �  ���     ��               =   A   u   ~   �   �   �   �   �   �    *  =  {  �  �  �  )  Z  �  �  �  �  �    *  k  �  �  �    P  �  �  �  �  �  �  	  7  E  �  �  �  � !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�  !�      �      �                ! " � � � �    H H    �(�����FG(�    H H    �(    d       '                �     @                         =�/���  �R    @      H -:LaserWriter 
     & \� (  � �   7  7 � �   7     
N        N      0      �  �  �  �          O  P  _  �  �  �  �  �  �  �  �  �  �  7  E  �  �         ��    Ȁ   �  �  �  �       r  �    �       s  �  P     `  �  �  �  �  �  t  �  �  �  �     {  �  �  �  �     �     �     �          q                                                                                                                                                                                                                                                                                                  