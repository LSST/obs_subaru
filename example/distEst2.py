#!/usr/bin/env python

import math
import hsc.meas.match.distest as distest

def main():
	CRPIX=distest.MAKE_Vdouble();
	CRVAL=distest.MAKE_Vdouble();
	XY=distest.MAKE_Vdouble();
	XY_GLOBL=distest.MAKE_VVdouble();
	XY_RADEC=distest.MAKE_VVdouble();
#	CRPIX.append(100.0)
#	CRPIX.append( 10.0)
#	CRVAL.append(  0.0)
#	CRVAL.append(  0.0)
#	XY.append(0.0)
#	XY.append(0.0)
	CRPIX[0]=100.0
	CRPIX[1]= 10.0
	CRVAL[0]=  0.0
	CRVAL[1]=  0.0
	print CRPIX
	print CRVAL	
	for x in range(-20,21):
		for y in range(-20,21):
			if(math.hypot(1000*x,1000*y)<17501):
				XY=[1000*x,1000*y]
				XY_GLOBL.append(XY)
	for x in range(-20,21):
		for y in range(-20,21):
			if(math.hypot(0.05*x,0.05*y)<0.751):
				XY=[0.05*x,0.05*y]
				XY_RADEC.append(XY)
	
	DIST_RADEC=distest.CALC_RADEC(CRVAL,CRPIX,XY_GLOBL)
	DIST_GLOBL=distest.CALC_GLOBL(CRVAL,CRPIX,XY_RADEC)
	DIST_RADEC_SIM=distest.CALC_RADEC_SIM(CRVAL,CRPIX,XY_GLOBL)
	DIST_GLOBL_SIM=distest.CALC_GLOBL_SIM(CRVAL,CRPIX,XY_RADEC)


	print XY_GLOBL[  0], DIST_RADEC[  0], DIST_RADEC_SIM[  0]
	print XY_GLOBL[200], DIST_RADEC[200], DIST_RADEC_SIM[200]
	print XY_GLOBL[400], DIST_RADEC[400], DIST_RADEC_SIM[400]
	print XY_GLOBL[600], DIST_RADEC[600], DIST_RADEC_SIM[600]
	print XY_GLOBL[800], DIST_RADEC[800], DIST_RADEC_SIM[800]
	print XY_GLOBL[972], DIST_RADEC[972], DIST_RADEC_SIM[972]
	print XY_RADEC[  0], DIST_GLOBL[  0], DIST_GLOBL_SIM[  0]
	print XY_RADEC[200], DIST_GLOBL[200], DIST_GLOBL_SIM[200]
	print XY_RADEC[400], DIST_GLOBL[400], DIST_GLOBL_SIM[400]
	print XY_RADEC[600], DIST_GLOBL[600], DIST_GLOBL_SIM[600]
	print XY_RADEC[700], DIST_GLOBL[700], DIST_GLOBL_SIM[700]

if __name__ == '__main__':
	main()
