	PROGRAM CCPCFF

        PARAMETER (M=40,N=40,LRWK=3500,LIWK=4000)
	REAL Z(M,N), RWRK(LRWK)
	INTEGER IWRK(LIWK)

	CALL GETDAT (Z, M, N)
C Open GKS
	CALL OPNGKS
	CALL GSCLIP (0)
C Initialize Conpack
	CALL CPRECT(Z, M, M, N, RWRK, LRWK, IWRK, LIWK)
	CALL CPGETI('CFF - CONSTANT FIELD FOUND FLAG',ICFF)
	IF (ICFF .NE. 0) GOTO 101
C Draw Perimeter
	CALL CPBACK(Z, RWRK, IWRK)
C Draw Contours
	CALL CPLBDR(Z,RWRK,IWRK)
	CALL CPCLDR(Z,RWRK,IWRK)

C Close frame and close GKS
	CALL FRAME
	CALL CLSGKS
	STOP

 101	WRITE (6,*) 'The field is constant.'
 	WRITE (6,*) 'This program does not create a valid CGM file.'
 
	STOP
	END

	SUBROUTINE GETDAT (Z, M, N)

	REAL Z(M,N)
	INTEGER I,J,M,N

	DO 10, I=1,M
	  DO 20, J=1,N
	    Z(I,J)=13.0
  20	  CONTINUE
  10	CONTINUE

	RETURN
	END
