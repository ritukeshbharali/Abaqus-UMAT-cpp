c     UMAT linear elasticity model
c     Plane stress implementation is missing
c
      SUBROUTINE UMAT(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,
     1           RPL,DDSDDT,DRPLDE,DRPLDT,
     2           STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,
     2           DPRED,CMNAME,
     3           NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,
     3           DROT,PNEWDT,
     4           CELENT,DFGRD0,DFGRD1,NOEL,NPT,
     5           LAYER,KSPT,KSTEP,KINC)

       INCLUDE 'ABA_PARAM.INC' 

      DIMENSiON STRESS(NTENS),STATEV(NSTATV),
     1     DDSDDE(NTENS,NTENS),DDSDDT(NTENS),DRPLDE(NTENS),
     2     STRAN(NTENS),DSTRAN(NTENS),TIME(2),PREDEF(1),DPRED(1),
     3     PROPS(NPROPS),DFGRD0(3,3), DFGRD1(3,3)

      integer I,J
      real E, NU, LAMBDA, MU

c     MATERIAL PARAMETERS
      E=PROPS(1)
      NU=PROPS(2)

c     PRINT *, "E      =", E
c     PRINT *, "NU     =", NU

c     LAME PARAMETERS      
      LAMBDA = E*NU/((1.0d0+NU)*(1.0d0-2.0d0*NU))
      MU = E/(2.0d0*(1.0d0+NU))
 
c     PRINT *, "LAMBDA =", LAMBDA
c     PRINT *, "MU     =", MU

c     STIFFNESS MATRIX
      do I = 1, NTENS
        do J = 1, NTENS
          DDSDDE(I,J) = 0.0d0
        end do
      end do
      do I = 1, NDI
        do J = 1, NDI
          DDSDDE(I, J) = LAMBDA
        end do 
        DDSDDE(I,I) = LAMBDA + 2.0d0*MU
      end do 

C     SHEAR CONTRIBUTION
      do I = NDI+1, NTENS
        DDSDDE(I,I) = MU
      end do 

C     EVALUATE STRESS
      do I = 1, NTENS
        do J = 1, NTENS
          STRESS(I) = STRESS(I) + DDSDDE(I,J) * STRAN(J)
        end do 
      end do 
C
      RETURN
      END
