C      ALGORITHM 659, COLLECTED ALGORITHMS FROM ACM.
C      THIS WORK PUBLISHED IN TRANSACTIONS ON MATHEMATICAL SOFTWARE,
C      VOL. 14, NO. 1, P.88.
      SUBROUTINE INFAUR(FLAG,DIMEN,ATMOST)
C
C       THIS SUBROUTINE FIRST CHECKS WHETHER
C       THE USER-SUPPLIED DIMENSION "DIMEN" OF THE
C       QUASIRANDOM VECTORS IS ACCEPTABLE
C       (STRICTLY BETWEEN 1 AND 41) : IF SO,
C       FLAG(1)=.TRUE.
C
C       THEN IT CALCULATES AN UPPER SUMMATION
C       LIMIT "HISUM" BASED ON "DIMEN" AND THE
C       USER-SUPPLIED NUMBER "ATMOST" OF QUASIRANDOM
C       VECTORS REQUIRED. FLAG(2)=.TRUE. IF
C       ATMOST IS OK.
C
C       IF FLAG(1) AND FLAG(2) ARE TRUE,
C       "INFAUR" NEXT PRODUCES THE OTHER
C       OUTPUTS LISTED BELOW PASSED TO
C       SUBROUTINE GOFAUR VIA LABELLED
C       COMMON "FAURE". THESE OUTPUTS ARE
C       IRRELEVANT TO THE USER.
C
C       FIRST CALL INFAUR. IF FLAG(1) AND
C       FLAG(2) ARE TRUE, EACH (SUBSEQUENT)
C       CALL TO GOFAUR GENERATES A NEW
C       QUASIRANDOM VECTOR.
C
C       INPUTS : DIMEN, ATMOST
C
C       OUTPUTS
C          TO USERS CALLING PROGRAM:
C             FLAG
C             QSS   : SAME AS QS - SEE BELOW
C
C          TO GOFAUR:
C             S      :DIMENSION
C             QS     :SMALLEST PRIME >=S
C             COEF   :TABLE OF BINOMIAL
C                     COEFFICIENTS NEEDED
C                     BY GOFAUR.
C             NEXTN  :THE NUMBER OF THE
C                     NEXT QUASIRANDOM
C                     VECTOR,INITIALIZED
C                     TO TESTN-1 HERE.
C             TESTN  :INITIALIZED TO QS**4
C             HISUM  :AFTER BEING USED TO
C                     PRODUCE COEF, INITIALIZED
C                     TO 3 FOR GOFAUR.
C             RQS    :1.0/QS.
C
      LOGICAL FLAG(2)
C
      INTEGER S,ATMOST,QS,COEF(0:19,0:19),NEXTN,
     +        TESTN,HISUM,I,J,PRIMES(40),DIMEN
C
      REAL RQS
C
      COMMON /FAURE/ S,QS,COEF,NEXTN,TESTN,
     +             HISUM,RQS
      SAVE /FAURE/
C
      DATA (PRIMES(I),I=1,40)/1,2,3,5,5,7,7,11,11,11,11,
     +                        13,13,17,17,17,17,19,19,
     +                        23,23,23,23,29,29,29,29,
     +                        29,29,31,31,37,37,37,37,
     +                        37,37,41,41,41/
C
C       CHECK S
C
      S=DIMEN
      FLAG(1) = S.GT.1 .AND. S.LT.41
      IF (.NOT.FLAG(1)) RETURN
C
      QS=PRIMES(S)
      TESTN=QS**4
C
C         COMPUTE LOG(ATMOST+TESTN) IN BASE QS
C         USING A RATIO OF NATURAL LOGS TO GET
C         AN UPPER BOUND ON (THE NUMBER OF
C         DIGITS IN THE BASE QS REPRESENTATION
C         OF ATMOST+TESTN) MINUS ONE.
C
      HISUM=NINT(LOG(REAL(ATMOST+TESTN))/LOG(REAL(QS)))
      FLAG(2)=HISUM.LT.20
      IF(.NOT. FLAG(2)) RETURN
C
C        NOW FIND BINOMIAL COEFFICIENTS MOD QS
C        IN A LOWER-TRIANGULAR MATRIX "COEF"
C        USING RECURSION BINOM(I,J)=BINOM(I-1,J)
C        +BINOM(I-1,J-1) AND A=B+C IMPLIES MOD(A,D)=
C        MOD(MOD(B,D)+MOD(C,D),D)
C
      COEF(0,0)=1
      DO 50 J=1,HISUM
        COEF(J,0)=1
        COEF(J,J)=1
   50 CONTINUE
      DO 200 J=1,HISUM
        DO 100 I=J+1,HISUM
          COEF(I,J)=MOD(COEF(I-1,J)+COEF(I-1,J-1),QS)
  100   CONTINUE
  200 CONTINUE
C
C        CALCULATING THESE COEFFICIENTS
C        MOD QS AVOIDS POSSIBLE OVERFLOW
C        PROBLEMS WITH RAW BINOMIAL COEFFICIENTS
C
C        NOW COMPLETE INITIALIZATION
C        AS DESCRIBED IN SECTION 2.
C        NEXTN HAS 4 DIGITS IN BASE
C        QS, SO HISUM EQUALS 3.
C
      NEXTN=0
      HISUM=3
      RQS=1.0/REAL(QS)
C
      RETURN
      END
      SUBROUTINE GOFAUR(QUASI)
C
C       THIS SUBROUTINE GENERATES A NEW
C       QUASIRANDOM VECTOR WITH EACH CALL
C
C       IT IMPLEMENTS A METHOD OF H.FAURE,
C       "ACTA ARITHMETICA XLI(1982),337-351".
C       (SEE ESPECIALLY PAGE 342).
C
C       THE USER MUST CALL "INFAUR" BEFORE
C       CALLING "GOFAUR".
C       AFTER CALLING "INFAUR", TEST FLAG(1)
C       AND FLAG(2); IF EITHER IS FALSE, DO
C       NOT CALL GOFAUR. READ THE COMMENTS AT
C       THE BEGINNING OF INFAUR AND THEN
C       THOSE BELOW.
C
C       ALL INPUTS COME FROM "INFAUR" VIA
C       LABELLED COMMON "FAURE"; FOR THEIR
C       DEFINITIONS, SEE "INFAUR".
C
C       INPUTS:
C         S,QS,COEF,NEXTN,TESTN,HISUM,RQS
C
C       OUTPUTS:
C         TO USER'S CALLING PROGRAM:
C         QUASI - A NEW QUASIRANDOM VECTOR
C
      INTEGER S,QS,COEF(0:19,0:19),NEXTN,TESTN,
     +        HISUM,I,J,K,YTEMP(0:19),ZTEMP,
     +        KTEMP,LTEMP,MTEMP
C
      REAL QUASI(40),RQS,R
C
      COMMON /FAURE/ S,QS,COEF,NEXTN,TESTN,
     +             HISUM,RQS
      SAVE /FAURE/
C
C       FIND QUASI(1) USING FAURE (SECTION 3.3)
C
C       NEXTN HAS A REPRESENTATION IN BASE
C       QS OF THE FORM: SUM OVER J FROM ZERO
C       TO HISUM OF YTEMP(J)*(QS**J)
C
C       WE NOW COMPUTE THE YTEMP(J)'S.
C
      KTEMP=TESTN
      LTEMP=NEXTN
      DO 100 I=HISUM,0,-1
          KTEMP=KTEMP/QS
          MTEMP=MOD(LTEMP,KTEMP)
          YTEMP(I)=(LTEMP-MTEMP)/KTEMP
          LTEMP=MTEMP
  100   CONTINUE
C
C       QUASI(K) HAS THE FORM SUM OVER J
C       FROM ZERO TO HISUM OF
C       YTEMP(J)*(QS**(-(J+1)))
C
C       READY TO COMPUTE QUASI(1)
C       USING NESTED MULTIPLICATION
C
      R=YTEMP(HISUM)
      DO 200 I=HISUM-1,0,-1
          R=YTEMP(I)+RQS*R
  200   CONTINUE
      QUASI(1)=R*RQS
C
C       FIND THE OTHER S-1 COMPONENTS
C       OF QUASI USING "FAURE" (SECTIONS
C       3.2 AND 3.3)
C
      DO 500 K=2,S
          QUASI(K)=0.0
          R=RQS
          DO 400 J=0,HISUM
              ZTEMP=0
              DO 300 I=J,HISUM
                  ZTEMP=ZTEMP+COEF(I,J)*YTEMP(I)
C
C       NO APPARENT ALTERNATIVE
C       ONE-DIMENSIONAL COEFFICIENT ARRAY
C       EXCEPT VIA SUBSCRIPT ADDRESS
C       COMPUTATIONS AND EQUIVALENCING
C
  300           CONTINUE
C
C       NEW YTEMP(J) IS THE SUM
C       OVER I FROM J TO HISUM
C       OF (OLD YTEMP(I)*BINOM(I,J))
C       MOD QS
C
              YTEMP(J)=MOD(ZTEMP,QS)
              QUASI(K)=QUASI(K)+YTEMP(J)*R
              R=R*RQS
  400       CONTINUE
  500   CONTINUE
C
C       UPDATE NEXTN AND, IF NEEDED, TESTN AND
C       HISUM
C
      NEXTN=NEXTN+1
      IF(NEXTN.EQ.TESTN) THEN
        TESTN=TESTN*QS
        HISUM=HISUM+1
C
C       SINCE FLAG(2) IS TRUE,
C       HISUM STAYS UNDER 20
C
      ENDIF
C
      RETURN
      END
      PROGRAM TESTF
C
C       THIS PROGRAM TESTS ACCURACY OF
C       NUMERICAL INTEGRATION USING "GOFAUR"
C       AND INTEGRAND (2) OF DAVIS AND
C       RABINOWITZ, PAGE 406
C
C       IT USES A NONSTANDARD TIMING
C       ROUTINE "SECOND"
C
C       PARAMETER STATEMENT SPECIFIES INPUT
C       AND OUTPUT UNITS
C
      LOGICAL FLAG(2)
      INTEGER DIMEN,ATMOST,I,INPUT,OUTPUT
      REAL QUASI(40),F,SUM
      REAL T1,T2
      PARAMETER(INPUT=5,OUTPUT=6)
C
      READ(INPUT,*) DIMEN,ATMOST
C
      CALL INFAUR(FLAG,DIMEN,ATMOST)
      IF(.NOT. FLAG(1)) THEN
          WRITE(OUTPUT,*) 'DIMENSION = ',DIMEN
          WRITE(OUTPUT,*) 'DIMEN IS NOT OK'
          STOP
        ENDIF
      IF(.NOT. FLAG(2)) THEN
          WRITE(OUTPUT,*) 'ATMOST = ',ATMOST
          WRITE(OUTPUT,*) 'ATMOST IS NOT OK'
          STOP
        ENDIF
C
      DO 100 I=1,ATMOST
          CALL GOFAUR(QUASI)
          WRITE(OUTPUT,*) (QUASI(J),J=1,DIMEN)
 100    CONTINUE
C
      STOP
      END
