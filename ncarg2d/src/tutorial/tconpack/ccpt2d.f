      PROGRAM CCPT2D
C
C Declare required data arrays and workspace arrays.
C
        PARAMETER(LMAP=100000,LWRK=1000,M=40,N=40)
        REAL ZDAT(M,N),RWRK(LWRK),XCRA(LWRK),YCRA(LWRK)
        INTEGER IWRK(LWRK),IAMA(LMAP), IAREA(2),IGRP(2)
C
C Declare the routine which will color the areas.
C
        EXTERNAL FILL
C
C Open GKS.
C
        CALL OPNGKS
C
C Turn off the clipping indicator.
C
        CALL GSCLIP (0)
C
C Force solid fill.
C
        CALL GSFAIS (1)
C
C Define color indices.
C
        CALL COLOR
C
C Retrieve an array of test data.
C
        OPEN (11,FILE='ccpila.dat',STATUS='OLD')
	DO 10, I=1,M
	  READ (11,*) (ZDAT(I,J),J=1,N)
 10	CONTINUE
C
C Tell CONPACK to use 12 contour levels, splitting the range into 13
C equal bands, one for each of the 13 colors available.
C
        CALL CPSETI ('CLS - CONTOUR LEVEL SELECTION FLAG',-12)
C
C Draw smoothed plot to the right
C
	CALL CPSETR ('VPL - VIEWPORT LEFT',.51)
	CALL CPSETR ('VPR - VIEWPORT RIGHT',1.0)
C
C Set smoothing so that lines are very smooth
C
        CALL CPSETR ('T2D - TENSION ON 2D SPLINES',.0001)
C
C Initialize the drawing of the contour plot.
C
        CALL CPRECT (ZDAT,M,M,N,RWRK,LWRK,IWRK,LWRK)
C
C Initialize the area map and put the contour lines into it.
C
        CALL ARINAM (IAMA,LMAP)
        CALL CPCLAM (ZDAT,RWRK,IWRK,IAMA)
C
C Color the map.
C
        CALL ARSCAM (IAMA,XCRA,YCRA,LWRK,IAREA,IGRP,2,FILL)
C
C Put black contour lines over the colored map.
C
        CALL GSPLCI (0)
        CALL CPCLDR (ZDAT,RWRK,IWRK)
C
C Draw unsmoothed plot to the left
C
	CALL CPSETR ('VPL - VIEWPORT LEFT',0.0)
	CALL CPSETR ('VPR - VIEWPORT RIGHT',0.49)
C 
C Tell Conpack that we want no smoothing
C
        CALL CPSETR ('T2D - TENSION ON 2D SPLINES',0.)
C
C Initialize the drawing of the contour plot.
C
        CALL CPRECT (ZDAT,M,M,N,RWRK,LWRK,IWRK,LWRK)
C
C Initialize the area map and put the contour lines into it.
C
        CALL ARINAM (IAMA,LMAP)
        CALL CPCLAM (ZDAT,RWRK,IWRK,IAMA)
C
C Color the map.
C
        CALL ARSCAM (IAMA,XCRA,YCRA,LWRK,IAREA,IGRP,2,FILL)
C
C Put black contour lines over the colored map.
C
        CALL GSPLCI (0)
        CALL CPCLDR (ZDAT,RWRK,IWRK)
C
C Draw titles in white
C
	CALL GETSET(VPL,VPR,VPB,VPT,WL,WR,WB,WT,LOG)
	CALL SET(0.,1.,0.,1.,0.,1.,0.,1.,1)
        CALL GSPLCI (0)
	CALL PLCHHQ (.3,VPT+.015,'Unsmoothed Contours',.015,0.,0.)
	CALL PLCHHQ (.75,VPT+.015,'Over Smoothed Contours',.015,0.,0.)
C
C Advance the frame.
C
        CALL FRAME
C
C Close GKS.
C
        CALL CLSGKS
C
C Done.
C
        STOP
C
      END
      SUBROUTINE FILL (XCRA,YCRA,NCRA,IAREA,IGRP,NGRPS)
C
        REAL XCRA(NCRA),YCRA(NCRA)
        INTEGER IAREA(NGRPS),IGRP(NGRPS)
C
C Get area identifiers for contour levels and vertical strips.
C
          IFILL=0
          DO 101 I=1,NGRPS
            IF (IGRP(I).EQ.3) IFILL=IAREA(I)
  101     CONTINUE
C
C
C Fill 
C
          IF (IFILL.GT.0) THEN
            CALL GSFACI (IFILL+2)
            CALL GFA (NCRA-1,XCRA,YCRA)
          END IF
C
C Done.
C
        RETURN
C
      END

      SUBROUTINE GENDAT (DATA,IDIM,M,N,MLOW,MHGH,DLOW,DHGH)
C
C This is a routine to generate test data for two-dimensional graphics
C routines.  Given an array "DATA", dimensioned "IDIM x 1", it fills
C the sub-array ((DATA(I,J),I=1,M),J=1,N) with a two-dimensional field
C of data having approximately "MLOW" lows and "MHGH" highs, a minimum
C value of exactly "DLOW" and a maximum value of exactly "DHGH".
C
C "MLOW" and "MHGH" are each forced to be greater than or equal to 1
C and less than or equal to 25.
C
C The function used is a sum of exponentials.
C
        DIMENSION DATA(IDIM,1),CCNT(3,50)
C
        FOVM=9./FLOAT(M)
        FOVN=9./FLOAT(N)
C
        NLOW=MAX0(1,MIN0(25,MLOW))
        NHGH=MAX0(1,MIN0(25,MHGH))
        NCNT=NLOW+NHGH
C
        DO 101 K=1,NCNT
          CCNT(1,K)=1.+(FLOAT(M)-1.)*FRAN()
          CCNT(2,K)=1.+(FLOAT(N)-1.)*FRAN()
          IF (K.LE.NLOW) THEN
            CCNT(3,K)=-1.
          ELSE
            CCNT(3,K)=+1.
          END IF
  101   CONTINUE
C
        DMIN=+1.E36
        DMAX=-1.E36
        DO 104 J=1,N
          DO 103 I=1,M
            DATA(I,J)=.5*(DLOW+DHGH)
            DO 102 K=1,NCNT
              TEMP=-((FOVM*(FLOAT(I)-CCNT(1,K)))**2+
     +               (FOVN*(FLOAT(J)-CCNT(2,K)))**2)
              IF (TEMP.GE.-20.) DATA(I,J)=DATA(I,J)+
     +            .5*(DHGH-DLOW)*CCNT(3,K)*EXP(TEMP)
  102       CONTINUE
            DMIN=AMIN1(DMIN,DATA(I,J))
            DMAX=AMAX1(DMAX,DATA(I,J))
  103     CONTINUE
  104   CONTINUE
C
        DO 106 J=1,N
          DO 105 I=1,M
            DATA(I,J)=(DATA(I,J)-DMIN)/(DMAX-DMIN)*(DHGH-DLOW)+DLOW
  105     CONTINUE
  106   CONTINUE
C
        RETURN
C
      END
      FUNCTION FRAN ()
        DIMENSION RSEQ (100)
        SAVE ISEQ
        DATA RSEQ / .749,.973,.666,.804,.081,.483,.919,.903,.951,.960 ,
     +              .039,.269,.270,.756,.222,.478,.621,.063,.550,.798 ,
     +              .027,.569,.149,.697,.451,.738,.508,.041,.266,.249 ,
     +              .019,.191,.266,.625,.492,.940,.508,.406,.972,.311 ,
     +              .757,.378,.299,.536,.619,.844,.342,.295,.447,.499 ,
     +              .688,.193,.225,.520,.954,.749,.997,.693,.217,.273 ,
     +              .961,.948,.902,.104,.495,.257,.524,.100,.492,.347 ,
     +              .981,.019,.225,.806,.678,.710,.235,.600,.994,.758 ,
     +              .682,.373,.009,.469,.203,.730,.588,.603,.213,.495 ,
     +              .884,.032,.185,.127,.010,.180,.689,.354,.372,.429 /
        DATA ISEQ / 0 /
        ISEQ=MOD(ISEQ,100)+1
        FRAN=RSEQ(ISEQ)
        RETURN
      END
      SUBROUTINE COLOR
C
C     BACKGROUND COLOR
C     BLACK
      CALL GSCR(1,0,0.,0.,0.)
C
C     FORGROUND COLORS
C White
      CALL GSCR(1, 1, 1.0, 1.0, 1.0)
C Aqua
      CALL GSCR(1, 2, 0.0, 0.9, 1.0)
C Red1
      CALL GSCR(1, 3, 0.9, 0.25, 0.0)
C OrangeRed1
      CALL GSCR(1, 4, 1.0, 0.0, 0.2)
C Orange1
      CALL GSCR(1, 5, 1.0, 0.65, 0.0)
C Yellow1
      CALL GSCR(1, 6, 1.0, 1.0, 0.0)
C GreenYellow1
      CALL GSCR(1, 7, 0.7, 1.0, 0.2)
C Chartreuse1
      CALL GSCR(1, 8, 0.5, 1.0, 0.0)
C Celeste1
      CALL GSCR(1, 9, 0.2, 1.0, 0.5)
C Green1
      CALL GSCR(1, 10, 0.2, 0.8, 0.2)
C DeepSkyBlue1
      CALL GSCR(1, 11, 0.0, 0.75, 1.0)
C RoyalBlue1
      CALL GSCR(1, 12, 0.25, 0.45, 0.95)
C SlateBlue1
      CALL GSCR(1, 13, 0.4, 0.35, 0.8)
C DarkViolet1
      CALL GSCR(1, 14, 0.6, 0.0, 0.8)
C Orchid1
      CALL GSCR(1, 15, 0.85, 0.45, 0.8)

      RETURN
      END
