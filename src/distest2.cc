//--------------------------------------------------
//distest2.cc
//
//Last Update 2013/03/15
//--------------------------------------------------
#include<iostream>
#include<vector>
#include<cmath>
#include "lsst/obs/hsc/distest2.h"

using namespace std;

namespace {

std::vector< double > InvPROJECTION(std::vector< double >CRVAL,std::vector< double >POS);
std::vector< double >    PROJECTION(std::vector< double >CRVAL,std::vector< double >POS);
std::vector< double > CALC_IMPIX(std::vector< double >POS);
std::vector< double > CALC_IMWLD(std::vector< double >POS);
std::vector< double > CALC_IMPIX_SIM(std::vector< double >POS);
std::vector< double > CALC_IMWLD_SIM(std::vector< double >POS);

} // anonymous namespace

namespace lsst {
namespace obs {
namespace hsc {

std::vector< double > MAKE_Vdouble(){
	std::vector< double > A(2);
	return A;
}
std::vector< std::vector< double > > MAKE_VVdouble(){
	std::vector< std::vector< double > > A;
	return A;
}

std::vector< std::vector< double >  > CALC_RADEC(std::vector< double > CRVAL,std::vector< double > CRPIX,std::vector< std::vector< double >  > POSITION){
	std::vector< std::vector< double >  > DIST_RADEC;
	std::vector< double > RADEC(2);
	std::vector< double > POS(2);
	std::vector< double > POS2(2);
	int i;
	for(i=0;i<POSITION.size();i++){
		POS[0]=POSITION[i][0]-CRPIX[0];
		POS[1]=POSITION[i][1]-CRPIX[1];
		POS2=CALC_IMWLD(POS);
//		RADEC=InvPROJECTION(CRVAL,POS2);
		RADEC[0]=POS2[0];
		RADEC[1]=POS2[1];
		DIST_RADEC.push_back(RADEC);
	}

	return DIST_RADEC;
}
std::vector< std::vector< double >  > CALC_GLOBL(std::vector< double > CRVAL,std::vector< double > CRPIX,std::vector< std::vector< double >  > POSITION){
	std::vector< std::vector< double >  > DIST_GLOBL;
	std::vector< double > GLOBL(2);
	std::vector< double > POS(2);
	std::vector< double > POS2(2);
	int i;
	for(i=0;i<POSITION.size();i++){
//		POS=PROJECTION(CRVAL,POSITION[i]);
		POS[0]=POSITION[i][0];
		POS[1]=POSITION[i][1];
		POS2=CALC_IMPIX(POS);
		GLOBL[0]=POS2[0]+CRPIX[0];
		GLOBL[1]=POS2[1]+CRPIX[1];
		DIST_GLOBL.push_back(GLOBL);
	}

	return DIST_GLOBL;
}
std::vector< std::vector< double >  > CALC_RADEC_SIM(std::vector< double > CRVAL,std::vector< double > CRPIX,std::vector< std::vector< double >  > POSITION){
	std::vector< std::vector< double >  > DIST_RADEC;
	std::vector< double > RADEC(2);
	std::vector< double > POS(2);
	std::vector< double > POS2(2);
	int i;
	for(i=0;i<POSITION.size();i++){
		POS[0]=POSITION[i][0]-CRPIX[0];
		POS[1]=POSITION[i][1]-CRPIX[1];
		POS2=CALC_IMWLD_SIM(POS);
//		RADEC=InvPROJECTION(CRVAL,POS2);
		RADEC[0]=POS2[0];
		RADEC[1]=POS2[1];
		DIST_RADEC.push_back(RADEC);
	}

	return DIST_RADEC;
}
std::vector< std::vector< double >  > CALC_GLOBL_SIM(std::vector< double > CRVAL,std::vector< double > CRPIX,std::vector< std::vector< double >  > POSITION){
	std::vector< std::vector< double >  > DIST_GLOBL;
	std::vector< double > GLOBL(2);
	std::vector< double > POS(2);
	std::vector< double > POS2(2);
	int i;
	for(i=0;i<POSITION.size();i++){
//		POS=PROJECTION(CRVAL,POSITION[i]);
		POS[0]=POSITION[i][0];
		POS[1]=POSITION[i][1];
		POS2=CALC_IMPIX_SIM(POS);
		GLOBL[0]=POS2[0]+CRPIX[0];
		GLOBL[1]=POS2[1]+CRPIX[1];
		DIST_GLOBL.push_back(GLOBL);
	}

	return DIST_GLOBL;
}
//------

#define PI (4*atan(1.0))
#define LP 180
std::vector< double > InvPROJECTION(std::vector< double >PPOINT,std::vector< double >Pdeg){
	std::vector< double >Cdeg(2);
    double NRAD[2];//native psi,theta (RAD)
	
    NRAD[0]=atan2(Pdeg[0],-Pdeg[1]);	
    NRAD[1]=atan(180/PI/sqrt(Pdeg[0]*Pdeg[0]+Pdeg[1]*Pdeg[1]));
    if(NRAD[0]<0)
    NRAD[0]+=2*PI;                                     
    if(NRAD[0]>2*PI)
    NRAD[0]-=2*PI;

    Cdeg[1]=180/PI*asin(sin(NRAD[1])*sin(PPOINT[1]/180*PI)+cos(NRAD[1])*cos(NRAD[0]-LP/180*PI)*cos(PPOINT[1]/180*PI));//-90deg - 90deg
    Cdeg[0]=180/PI*atan2(-cos(NRAD[1])*sin(NRAD[0]-LP/180*PI),sin(NRAD[1])*cos(PPOINT[1]/180*PI)-cos(NRAD[1])*cos(NRAD[0]-LP/180*PI)*sin(PPOINT[1]/180*PI))+PPOINT[0];

	return Cdeg;
}
std::vector< double > PROJECTION(std::vector< double >PPOINT,std::vector< double >Cdeg){
	std::vector< double >Pdeg(2);
    double NRAD[2];
	
    NRAD[1]=asin(sin(Cdeg[1]/180*PI)*sin(PPOINT[1]/180*PI)+cos(Cdeg[1]/180*PI)*cos(PPOINT[1]/180*PI)*cos(Cdeg[0]/180*PI-PPOINT[0]/180*PI));
    NRAD[0]=atan2(-cos(Cdeg[1]/180*PI)*sin(Cdeg[0]/180*PI-PPOINT[0]/180*PI),sin(Cdeg[1]/180*PI)*cos(PPOINT[1]/180*PI)-cos(Cdeg[1]/180*PI)*sin(PPOINT[1]/180*PI)*cos(Cdeg[0]/180*PI-PPOINT[0]/180*PI))+LP/180*PI;

    Pdeg[0]= 180/PI/tan(NRAD[1])*sin(NRAD[0]);//+PPOINT[0];
    Pdeg[1]=-180/PI/tan(NRAD[1])*cos(NRAD[0]);//+PPOINT[1];

	return Pdeg;
}
#undef LP
#undef PI
std::vector< double > CALC_IMPIX(std::vector< double >POS){
	std::vector< double > POS2(2);
/*	double COEF[2][10][10]={0};
COEF[0][ 0 ][ 0 ]= 0.00357457263523;
COEF[0][ 0 ][ 1 ]= -21330.9528185;
COEF[0][ 0 ][ 2 ]= 3.08718040412;
COEF[0][ 0 ][ 3 ]= -1013.77500367;
COEF[0][ 0 ][ 4 ]= -10.8343950476;
COEF[0][ 0 ][ 5 ]= -277.243768186;
COEF[0][ 0 ][ 6 ]= 21.0964905942;
COEF[0][ 0 ][ 7 ]= 9.88489720833;
COEF[0][ 0 ][ 8 ]= -5.72612584238;
COEF[0][ 0 ][ 9 ]= -250.922869767;
COEF[0][ 1 ][ 0 ]= 25.7532984084;
COEF[0][ 1 ][ 1 ]= -2.16988019757;
COEF[0][ 1 ][ 2 ]= 2.02295245558;
COEF[0][ 1 ][ 3 ]= 3.75735556815;
COEF[0][ 1 ][ 4 ]= -44.2382793466;
COEF[0][ 1 ][ 5 ]= 9.02247110084;
COEF[0][ 1 ][ 6 ]= 214.679970545;
COEF[0][ 1 ][ 7 ]= -6.75635797434;
COEF[0][ 1 ][ 8 ]= -239.864551239;
COEF[0][ 2 ][ 0 ]= 0.13257860942;
COEF[0][ 2 ][ 1 ]= -1069.00385308;
COEF[0][ 2 ][ 2 ]= 15.8525209771;
COEF[0][ 2 ][ 3 ]= -141.267330235;
COEF[0][ 2 ][ 4 ]= -6.00985679047;
COEF[0][ 2 ][ 5 ]= -1077.64407346;
COEF[0][ 2 ][ 6 ]= -19.394034333;
COEF[0][ 2 ][ 7 ]= -30.1809714841;
COEF[0][ 3 ][ 0 ]= 4.52289632003;
COEF[0][ 3 ][ 1 ]= 10.3038742469;
COEF[0][ 3 ][ 2 ]= -7.13546447828;
COEF[0][ 3 ][ 3 ]= -24.3263266108;
COEF[0][ 3 ][ 4 ]= -4.5525589894;
COEF[0][ 3 ][ 5 ]= -13.1607850362;
COEF[0][ 3 ][ 6 ]= -119.278439255;
COEF[0][ 4 ][ 0 ]= 4.71050726766;
COEF[0][ 4 ][ 1 ]= 82.1869681766;
COEF[0][ 4 ][ 2 ]= -111.923145271;
COEF[0][ 4 ][ 3 ]= -1748.53068979;
COEF[0][ 4 ][ 4 ]= 145.688876538;
COEF[0][ 4 ][ 5 ]= 779.482006784;
COEF[0][ 5 ][ 0 ]= -35.5883523795;
COEF[0][ 5 ][ 1 ]= -18.3776735173;
COEF[0][ 5 ][ 2 ]= 56.6704610735;
COEF[0][ 5 ][ 3 ]= 75.8898286658;
COEF[0][ 5 ][ 4 ]= 75.0114877288;
COEF[0][ 6 ][ 0 ]= -11.9280635128;
COEF[0][ 6 ][ 1 ]= -778.28864704;
COEF[0][ 6 ][ 2 ]= 114.732852;
COEF[0][ 6 ][ 3 ]= 893.535935697;
COEF[0][ 7 ][ 0 ]= 86.5069838486;
COEF[0][ 7 ][ 1 ]= 3.83060601673;
COEF[0][ 7 ][ 2 ]= -72.4873960965;
COEF[0][ 8 ][ 0 ]= 9.18899290553;
COEF[0][ 8 ][ 1 ]= 316.242800729;
COEF[0][ 9 ][ 0 ]= -64.084580894;
COEF[1][ 0 ][ 0 ]= -0.00235831460096;
COEF[1][ 0 ][ 1 ]= -26.4052259416;
COEF[1][ 0 ][ 2 ]= 0.73371695177;
COEF[1][ 0 ][ 3 ]= 14.6794968215;
COEF[1][ 0 ][ 4 ]= 4.14042441126;
COEF[1][ 0 ][ 5 ]= -78.1419088649;
COEF[1][ 0 ][ 6 ]= -5.86293689607;
COEF[1][ 0 ][ 7 ]= 131.97269948;
COEF[1][ 0 ][ 8 ]= 5.75971239845;
COEF[1][ 0 ][ 9 ]= -79.4870542457;
COEF[1][ 1 ][ 0 ]= -21329.6549608;
COEF[1][ 1 ][ 1 ]= -2.62154199772;
COEF[1][ 1 ][ 2 ]= -1051.73872845;
COEF[1][ 1 ][ 3 ]= 48.7841128949;
COEF[1][ 1 ][ 4 ]= -88.2890088963;
COEF[1][ 1 ][ 5 ]= -154.983086355;
COEF[1][ 1 ][ 6 ]= -415.653610738;
COEF[1][ 1 ][ 7 ]= 146.593551721;
COEF[1][ 1 ][ 8 ]= 100.33610647;
COEF[1][ 2 ][ 0 ]= -0.593182608153;
COEF[1][ 2 ][ 1 ]= -26.4596683867;
COEF[1][ 2 ][ 2 ]= -11.3732220688;
COEF[1][ 2 ][ 3 ]= 82.6717682811;
COEF[1][ 2 ][ 4 ]= 89.6191164596;
COEF[1][ 2 ][ 5 ]= -181.472568836;
COEF[1][ 2 ][ 6 ]= -119.82142646;
COEF[1][ 2 ][ 7 ]= 239.057374696;
COEF[1][ 3 ][ 0 ]= -1045.74410389;
COEF[1][ 3 ][ 1 ]= 18.2702974641;
COEF[1][ 3 ][ 2 ]= -193.461918882;
COEF[1][ 3 ][ 3 ]= -94.4649487377;
COEF[1][ 3 ][ 4 ]= -1001.3883232;
COEF[1][ 3 ][ 5 ]= 130.851765068;
COEF[1][ 3 ][ 6 ]= -13.8994011879;
COEF[1][ 4 ][ 0 ]= 9.41047196199;
COEF[1][ 4 ][ 1 ]= 174.945021985;
COEF[1][ 4 ][ 2 ]= 51.6651053708;
COEF[1][ 4 ][ 3 ]= -379.748903334;
COEF[1][ 4 ][ 4 ]= -90.0410413519;
COEF[1][ 4 ][ 5 ]= 130.581101026;
COEF[1][ 5 ][ 0 ]= -124.785370861;
COEF[1][ 5 ][ 1 ]= -22.5094209611;
COEF[1][ 5 ][ 2 ]= -1077.34056248;
COEF[1][ 5 ][ 3 ]= 72.9324401417;
COEF[1][ 5 ][ 4 ]= 68.2834257398;
COEF[1][ 6 ][ 0 ]= -15.2682159077;
COEF[1][ 6 ][ 1 ]= -421.870052276;
COEF[1][ 6 ][ 2 ]= -57.8334960011;
COEF[1][ 6 ][ 3 ]= 543.726109798;
COEF[1][ 7 ][ 0 ]= -309.039110797;
COEF[1][ 7 ][ 1 ]= 9.20219749691;
COEF[1][ 7 ][ 2 ]= 89.9577581542;
COEF[1][ 8 ][ 0 ]= 13.6126929861;
COEF[1][ 8 ][ 1 ]= 324.417923389;
COEF[1][ 9 ][ 0 ]= -7.26100172315;
	int i,j;
	POS2[0]=POS2[1]=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS2[0]+=COEF[0][i][j]*pow(POS[0],i)*pow(POS[1],j);
		POS2[1]+=COEF[1][i][j]*pow(POS[0],i)*pow(POS[1],j);
	}
*/
	double COEF[4][10*10]={0};
COEF[0][ 0 ]= -0.00047203560468;
COEF[0][ 1 ]= -1.5427988883e-05;
COEF[0][ 2 ]= -5.95865625284e-10;
COEF[0][ 3 ]= 2.22594651446e-13;
COEF[0][ 4 ]= -1.51583805989e-17;
COEF[0][ 5 ]= -6.08317204253e-21;
COEF[0][ 6 ]= 8.06561200947e-26;
COEF[0][ 7 ]= 3.10689047143e-29;
COEF[0][ 8 ]= -1.2634302141e-34;
COEF[0][ 9 ]= -4.78814991762e-38;
COEF[0][ 10 ]= -0.000210433037672+1.0;
COEF[0][ 11 ]= 4.66142798084e-09;
COEF[0][ 12 ]= -1.0957097072e-10;
COEF[0][ 13 ]= -4.6906678848e-17;
COEF[0][ 14 ]= 4.22216001121e-20;
COEF[0][ 15 ]= 1.87541306804e-25;
COEF[0][ 16 ]= -3.11027701152e-28;
COEF[0][ 17 ]= -1.25424492312e-34;
COEF[0][ 18 ]= 3.70673801604e-37;
COEF[0][ 19 ]= -6.85371734365e-09;
COEF[0][ 20 ]= -1.26203995922e-14;
COEF[0][ 21 ]= -5.4517702042e-17;
COEF[0][ 22 ]= -9.94551136649e-22;
COEF[0][ 23 ]= 9.09298367647e-25;
COEF[0][ 24 ]= 2.45558081417e-29;
COEF[0][ 25 ]= -1.92962666077e-33;
COEF[0][ 26 ]= -6.7994922883e-38;
COEF[0][ 27 ]= -1.04551853816e-10;
COEF[0][ 28 ]= -1.95257983584e-17;
COEF[0][ 29 ]= 2.66068980901e-20;
COEF[0][ 30 ]= 2.05500547863e-25;
COEF[0][ 31 ]= -7.00557780822e-28;
COEF[0][ 32 ]= -1.27106133492e-33;
COEF[0][ 33 ]= 1.17427552052e-36;
COEF[0][ 34 ]= 5.48308544761e-17;
COEF[0][ 35 ]= -6.6977079558e-21;
COEF[0][ 36 ]= -3.88079500249e-26;
COEF[0][ 37 ]= -1.12430203404e-29;
COEF[0][ 38 ]= -2.3228732246e-33;
COEF[0][ 39 ]= 5.83996936659e-38;
COEF[0][ 40 ]= -2.55808760342e-20;
COEF[0][ 41 ]= -5.51546375606e-26;
COEF[0][ 42 ]= -4.43037636885e-28;
COEF[0][ 43 ]= 3.26328244851e-34;
COEF[0][ 44 ]= 1.30831113179e-36;
COEF[0][ 45 ]= -2.3965595459e-25;
COEF[0][ 46 ]= 7.38850410175e-29;
COEF[0][ 47 ]= 6.31709084288e-34;
COEF[0][ 48 ]= -5.69997824021e-38;
COEF[0][ 49 ]= -3.49962832225e-30;
COEF[0][ 50 ]= 9.21035992064e-35;
COEF[0][ 51 ]= 4.48010296471e-37;
COEF[0][ 52 ]= 2.36522390769e-34;
COEF[0][ 53 ]= -1.7686068989e-37;
COEF[0][ 54 ]= -8.6880691822e-38;
COEF[1][ 0 ]= -2.27525408678e-05;
COEF[1][ 1 ]= -0.000149438556393+1.0;
COEF[1][ 2 ]= 1.47288649136e-09;
COEF[1][ 3 ]= -1.07681558891e-10;
COEF[1][ 4 ]= -4.52745194926e-17;
COEF[1][ 5 ]= 5.33446374932e-21;
COEF[1][ 6 ]= 1.59765278412e-25;
COEF[1][ 7 ]= -1.35281754124e-28;
COEF[1][ 8 ]= -2.58952055468e-34;
COEF[1][ 9 ]= 1.18384181522e-37;
COEF[1][ 10 ]= -1.54279888831e-05;
COEF[1][ 11 ]= 5.10149451107e-09;
COEF[1][ 12 ]= -2.20369366154e-12;
COEF[1][ 13 ]= -8.12440053288e-17;
COEF[1][ 14 ]= 3.16674570469e-20;
COEF[1][ 15 ]= 2.36720490323e-25;
COEF[1][ 16 ]= -1.54887554063e-28;
COEF[1][ 17 ]= -2.18878587707e-34;
COEF[1][ 18 ]= 2.42019175449e-37;
COEF[1][ 19 ]= -1.36641013138e-09;
COEF[1][ 20 ]= -1.08210753878e-10;
COEF[1][ 21 ]= 3.24065404366e-17;
COEF[1][ 22 ]= 2.21741676333e-20;
COEF[1][ 23 ]= -3.30404486918e-25;
COEF[1][ 24 ]= -4.7223051146e-28;
COEF[1][ 25 ]= 8.48620744583e-34;
COEF[1][ 26 ]= 5.84549240581e-37;
COEF[1][ 27 ]= 1.65013522193e-12;
COEF[1][ 28 ]= -2.04698537311e-16;
COEF[1][ 29 ]= 1.36596617211e-20;
COEF[1][ 30 ]= 8.77160647683e-25;
COEF[1][ 31 ]= -1.36731060152e-28;
COEF[1][ 32 ]= -1.43968368509e-33;
COEF[1][ 33 ]= 4.00898492827e-37;
COEF[1][ 34 ]= -2.27951193984e-17;
COEF[1][ 35 ]= 1.16796604208e-20;
COEF[1][ 36 ]= -6.53927583976e-25;
COEF[1][ 37 ]= -4.41168731276e-28;
COEF[1][ 38 ]= 1.38404520921e-33;
COEF[1][ 39 ]= 8.26267449077e-37;
COEF[1][ 40 ]= -1.75167734408e-20;
COEF[1][ 41 ]= 1.35671719277e-24;
COEF[1][ 42 ]= -5.56167978363e-29;
COEF[1][ 43 ]= -2.43608580718e-33;
COEF[1][ 44 ]= 9.3744233119e-38;
COEF[1][ 45 ]= 8.31436843296e-26;
COEF[1][ 46 ]= -1.73569476217e-28;
COEF[1][ 47 ]= 1.90770699097e-33;
COEF[1][ 48 ]= 4.98143401516e-37;
COEF[1][ 49 ]= 6.57627509385e-29;
COEF[1][ 50 ]= -2.64064071957e-33;
COEF[1][ 51 ]= 1.56461570921e-37;
COEF[1][ 52 ]= -1.50783715462e-34;
COEF[1][ 53 ]= 1.98549941035e-37;
COEF[1][ 54 ]= -8.74305862185e-38;
COEF[2][ 0 ]= 0.00365271948353;
COEF[2][ 1 ]= 1.70911115723e-05;
COEF[2][ 2 ]= 1.5204217229e-10;
COEF[2][ 3 ]= -3.08715201043e-13;
COEF[2][ 4 ]= 2.39597939294e-17;
COEF[2][ 5 ]= 7.81157081952e-21;
COEF[2][ 6 ]= -1.29621716896e-25;
COEF[2][ 7 ]= -4.16263764639e-29;
COEF[2][ 8 ]= 2.15552971078e-34;
COEF[2][ 9 ]= 6.82597059998e-38;
COEF[2][ 10 ]= 0.000209404097794+1.0;
COEF[2][ 11 ]= -4.75019821106e-09;
COEF[2][ 12 ]= 1.10190607837e-10;
COEF[2][ 13 ]= 4.93339497018e-17;
COEF[2][ 14 ]= -1.85924839513e-20;
COEF[2][ 15 ]= -1.95896472784e-25;
COEF[2][ 16 ]= 3.88256510948e-28;
COEF[2][ 17 ]= 9.69693004122e-35;
COEF[2][ 18 ]= -3.47706701826e-37;
COEF[2][ 19 ]= 6.71381011215e-09;
COEF[2][ 20 ]= -9.50158088079e-14;
COEF[2][ 21 ]= 7.93173879809e-17;
COEF[2][ 22 ]= 1.60828614399e-21;
COEF[2][ 23 ]= -1.21754098187e-24;
COEF[2][ 24 ]= -2.62579677501e-29;
COEF[2][ 25 ]= 2.74663158936e-33;
COEF[2][ 26 ]= 7.59945998098e-38;
COEF[2][ 27 ]= 1.04498582488e-10;
COEF[2][ 28 ]= 1.74134310921e-17;
COEF[2][ 29 ]= 3.24251148641e-20;
COEF[2][ 30 ]= -2.26265099203e-25;
COEF[2][ 31 ]= 8.70112786558e-28;
COEF[2][ 32 ]= 1.68537897358e-33;
COEF[2][ 33 ]= -9.78020507263e-37;
COEF[2][ 34 ]= -5.13651337215e-17;
COEF[2][ 35 ]= 1.04041291095e-20;
COEF[2][ 36 ]= -7.65133837619e-26;
COEF[2][ 37 ]= 3.53907656972e-30;
COEF[2][ 38 ]= 3.47366147648e-33;
COEF[2][ 39 ]= -8.85077550431e-38;
COEF[2][ 40 ]= 6.30556215478e-20;
COEF[2][ 41 ]= 9.43999734447e-26;
COEF[2][ 42 ]= 5.33882127553e-28;
COEF[2][ 43 ]= -3.92799329224e-34;
COEF[2][ 44 ]= -8.4676694827e-37;
COEF[2][ 45 ]= 2.185067016e-25;
COEF[2][ 46 ]= -1.09047001782e-28;
COEF[2][ 47 ]= -4.39266689902e-34;
COEF[2][ 48 ]= 1.31099098133e-37;
COEF[2][ 49 ]= -5.78460936624e-30;
COEF[2][ 50 ]= -1.38369955347e-34;
COEF[2][ 51 ]= 3.92691778634e-38;
COEF[2][ 52 ]= -1.23086882478e-34;
COEF[2][ 53 ]= 2.67441530618e-37;
COEF[2][ 54 ]= 2.76078356876e-37;
COEF[3][ 0 ]= -0.00243520601215;
COEF[3][ 1 ]= 0.000147893495017+1.0;
COEF[3][ 2 ]= -1.37763224595e-09;
COEF[3][ 3 ]= 1.07833848918e-10;
COEF[3][ 4 ]= 4.62596775002e-17;
COEF[3][ 5 ]= 2.80575848534e-20;
COEF[3][ 6 ]= -1.65367128388e-25;
COEF[3][ 7 ]= 1.54820423917e-28;
COEF[3][ 8 ]= 3.22741961727e-34;
COEF[3][ 9 ]= 7.08058901535e-39;
COEF[3][ 10 ]= 1.29459993973e-05;
COEF[3][ 11 ]= -5.75758775598e-09;
COEF[3][ 12 ]= 2.59566096648e-12;
COEF[3][ 13 ]= 8.89501051555e-17;
COEF[3][ 14 ]= -3.98215173203e-20;
COEF[3][ 15 ]= -2.42815205576e-25;
COEF[3][ 16 ]= 2.11105473817e-28;
COEF[3][ 17 ]= 2.17987210027e-34;
COEF[3][ 18 ]= -3.57655925495e-37;
COEF[3][ 19 ]= 1.71046572758e-09;
COEF[3][ 20 ]= 1.08416040618e-10;
COEF[3][ 21 ]= -5.39914444014e-17;
COEF[3][ 22 ]= 4.3772487775e-20;
COEF[3][ 23 ]= 5.39490871566e-25;
COEF[3][ 24 ]= 5.37804304106e-28;
COEF[3][ 25 ]= -1.33522335425e-33;
COEF[3][ 26 ]= -1.00922213245e-37;
COEF[3][ 27 ]= -1.61692503254e-12;
COEF[3][ 28 ]= 2.37001625897e-16;
COEF[3][ 29 ]= -1.87396962399e-20;
COEF[3][ 30 ]= -1.01303057487e-24;
COEF[3][ 31 ]= 1.88379875457e-28;
COEF[3][ 32 ]= 1.74279811617e-33;
COEF[3][ 33 ]= -5.95775054508e-37;
COEF[3][ 34 ]= 1.73901668516e-17;
COEF[3][ 35 ]= 2.03868624457e-20;
COEF[3][ 36 ]= 9.5148136402e-25;
COEF[3][ 37 ]= 4.98415367039e-28;
COEF[3][ 38 ]= -2.07693213639e-33;
COEF[3][ 39 ]= -7.27517738702e-38;
COEF[3][ 40 ]= 1.71531399232e-20;
COEF[3][ 41 ]= -1.65907556613e-24;
COEF[3][ 42 ]= 9.02531724098e-29;
COEF[3][ 43 ]= 3.06533133333e-33;
COEF[3][ 44 ]= -1.43492096446e-37;
COEF[3][ 45 ]= -4.32749987233e-26;
COEF[3][ 46 ]= 2.04328665654e-28;
COEF[3][ 47 ]= -2.81367089917e-33;
COEF[3][ 48 ]= 1.74213007753e-38;
COEF[3][ 49 ]= -6.279347973e-29;
COEF[3][ 50 ]= 3.45559728633e-33;
COEF[3][ 51 ]= -2.637596961e-37;
COEF[3][ 52 ]= 9.34962455855e-35;
COEF[3][ 53 ]= -1.03493809035e-37;
COEF[3][ 54 ]= 8.09745928121e-38;
	int i,j,ij;
	POS2[0]=POS2[1]=0;
	ij=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS2[0]+=COEF[2][ij]*pow(POS[0],i)*pow(POS[1],j);
		POS2[1]+=COEF[3][ij]*pow(POS[0],i)*pow(POS[1],j);
		ij++;
	}
	return POS2;
}
std::vector< double > CALC_IMWLD(std::vector< double >POS){
	std::vector< double > POS2(2);
/*	double COEF[2][10][10]={0};
COEF[0][ 0 ][ 0 ]= 1.19334688956e-09;
COEF[0][ 0 ][ 1 ]= -4.68829398468e-05;
COEF[0][ 0 ][ 2 ]= -6.60313272549e-14;
COEF[0][ 0 ][ 3 ]= 5.04837178619e-15;
COEF[0][ 0 ][ 4 ]= 2.0924188506e-21;
COEF[0][ 0 ][ 5 ]= -2.42752070917e-25;
COEF[0][ 0 ][ 6 ]= -7.36911043931e-30;
COEF[0][ 0 ][ 7 ]= 6.32123123211e-33;
COEF[0][ 0 ][ 8 ]= 1.19653678515e-38;
COEF[0][ 0 ][ 9 ]= -5.53185124363e-42;
COEF[0][ 1 ][ 0 ]= 5.81493687851e-08;
COEF[0][ 1 ][ 1 ]= -2.39378895833e-13;
COEF[0][ 1 ][ 2 ]= 9.72065709264e-17;
COEF[0][ 1 ][ 3 ]= 3.77394370895e-21;
COEF[0][ 1 ][ 4 ]= -1.47992134815e-24;
COEF[0][ 1 ][ 5 ]= -1.08901075482e-29;
COEF[0][ 1 ][ 6 ]= 7.22028563965e-33;
COEF[0][ 1 ][ 7 ]= 9.97446995627e-39;
COEF[0][ 1 ][ 8 ]= -1.12753398435e-41;
COEF[0][ 2 ][ 0 ]= 6.01387174447e-14;
COEF[0][ 2 ][ 1 ]= 5.07395056275e-15;
COEF[0][ 2 ][ 2 ]= -1.54808489365e-21;
COEF[0][ 2 ][ 3 ]= -1.03040020697e-24;
COEF[0][ 2 ][ 4 ]= 1.58812643918e-29;
COEF[0][ 2 ][ 5 ]= 2.20739503218e-32;
COEF[0][ 2 ][ 6 ]= -4.04578909184e-38;
COEF[0][ 2 ][ 7 ]= -2.72765287959e-41;
COEF[0][ 3 ][ 0 ]= -8.45308766225e-17;
COEF[0][ 3 ][ 1 ]= 9.56522394371e-21;
COEF[0][ 3 ][ 2 ]= -6.4299486774e-25;
COEF[0][ 3 ][ 3 ]= -4.07622664685e-29;
COEF[0][ 3 ][ 4 ]= 6.38550843751e-33;
COEF[0][ 3 ][ 5 ]= 6.62016952576e-38;
COEF[0][ 3 ][ 6 ]= -1.87438739899e-41;
COEF[0][ 4 ][ 0 ]= 1.1615097931e-21;
COEF[0][ 4 ][ 1 ]= -5.54616117963e-25;
COEF[0][ 4 ][ 2 ]= 3.05197628603e-29;
COEF[0][ 4 ][ 3 ]= 2.0680565281e-32;
COEF[0][ 4 ][ 4 ]= -6.58510732381e-38;
COEF[0][ 4 ][ 5 ]= -3.86578772558e-41;
COEF[0][ 5 ][ 0 ]= 8.40328570976e-25;
COEF[0][ 5 ][ 1 ]= -6.32950520485e-29;
COEF[0][ 5 ][ 2 ]= 2.58620424591e-33;
COEF[0][ 5 ][ 3 ]= 1.13682821642e-37;
COEF[0][ 5 ][ 4 ]= -4.31632075117e-42;
COEF[0][ 6 ][ 0 ]= -4.53664935288e-30;
COEF[0][ 6 ][ 1 ]= 8.20646252996e-33;
COEF[0][ 6 ][ 2 ]= -8.84405249438e-38;
COEF[0][ 6 ][ 3 ]= -2.34493944928e-41;
COEF[0][ 7 ][ 0 ]= -3.20185476421e-33;
COEF[0][ 7 ][ 1 ]= 1.23061232275e-37;
COEF[0][ 7 ][ 2 ]= -7.30124984562e-42;
COEF[0][ 8 ][ 0 ]= 8.37169822172e-39;
COEF[0][ 8 ][ 1 ]= -9.46744833237e-42;
COEF[0][ 9 ][ 0 ]= 4.31195364554e-42;
COEF[1][ 0 ][ 0 ]= 2.20651543069e-08;
COEF[1][ 0 ][ 1 ]= -5.66828078642e-08;
COEF[1][ 0 ][ 2 ]= 3.38290586096e-14;
COEF[1][ 0 ][ 3 ]= -5.16829279996e-18;
COEF[1][ 0 ][ 4 ]= 6.6345089916e-22;
COEF[1][ 0 ][ 5 ]= 2.93692667955e-25;
COEF[1][ 0 ][ 6 ]= -3.66979792657e-30;
COEF[1][ 0 ][ 7 ]= -1.48448596078e-33;
COEF[1][ 0 ][ 8 ]= 5.87682211253e-39;
COEF[1][ 0 ][ 9 ]= 2.28853616928e-42;
COEF[1][ 1 ][ 0 ]= -4.68801083133e-05;
COEF[1][ 1 ][ 1 ]= -2.18740673662e-13;
COEF[1][ 1 ][ 2 ]= 5.13879722084e-15;
COEF[1][ 1 ][ 3 ]= 2.21798456335e-21;
COEF[1][ 1 ][ 4 ]= -1.98548076196e-24;
COEF[1][ 1 ][ 5 ]= -8.81835376516e-30;
COEF[1][ 1 ][ 6 ]= 1.45933216677e-32;
COEF[1][ 1 ][ 7 ]= 5.72916682931e-39;
COEF[1][ 1 ][ 8 ]= -1.7379010676e-41;
COEF[1][ 2 ][ 0 ]= 3.24708569904e-13;
COEF[1][ 2 ][ 1 ]= 6.91515749463e-18;
COEF[1][ 2 ][ 2 ]= 2.44446694743e-21;
COEF[1][ 2 ][ 3 ]= 5.20826533446e-26;
COEF[1][ 2 ][ 4 ]= -4.15579190929e-29;
COEF[1][ 2 ][ 5 ]= -1.17309148294e-33;
COEF[1][ 2 ][ 6 ]= 8.83201922078e-38;
COEF[1][ 2 ][ 7 ]= 3.24603797537e-42;
COEF[1][ 3 ][ 0 ]= 4.90303818109e-15;
COEF[1][ 3 ][ 1 ]= 9.33451772393e-22;
COEF[1][ 3 ][ 2 ]= -1.26263828201e-24;
COEF[1][ 3 ][ 3 ]= -1.05909952537e-29;
COEF[1][ 3 ][ 4 ]= 3.29111047862e-32;
COEF[1][ 3 ][ 5 ]= 6.23697908767e-38;
COEF[1][ 3 ][ 6 ]= -5.51536976087e-41;
COEF[1][ 4 ][ 0 ]= -2.60973870705e-21;
COEF[1][ 4 ][ 1 ]= 3.09374339316e-25;
COEF[1][ 4 ][ 2 ]= 2.36766867219e-30;
COEF[1][ 4 ][ 3 ]= 4.85344009997e-34;
COEF[1][ 4 ][ 4 ]= 1.05969320362e-37;
COEF[1][ 4 ][ 5 ]= -2.54519142101e-42;
COEF[1][ 5 ][ 0 ]= 1.19182573538e-24;
COEF[1][ 5 ][ 1 ]= 2.64902670318e-30;
COEF[1][ 5 ][ 2 ]= 2.08666772249e-32;
COEF[1][ 5 ][ 3 ]= -1.30377962897e-38;
COEF[1][ 5 ][ 4 ]= -6.15475156737e-41;
COEF[1][ 6 ][ 0 ]= 1.1439366118e-29;
COEF[1][ 6 ][ 1 ]= -3.42275871639e-33;
COEF[1][ 6 ][ 2 ]= -3.0202189982e-38;
COEF[1][ 6 ][ 3 ]= 2.75546886553e-42;
COEF[1][ 7 ][ 0 ]= 1.97052522852e-34;
COEF[1][ 7 ][ 1 ]= -4.85308743016e-39;
COEF[1][ 7 ][ 2 ]= -2.11653537549e-41;
COEF[1][ 8 ][ 0 ]= -1.14523183474e-38;
COEF[1][ 8 ][ 1 ]= 8.21507352457e-42;
COEF[1][ 9 ][ 0 ]= 4.02751121909e-42;
	int i,j;
	POS2[0]=POS2[1]=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS2[0]+=COEF[0][i][j]*pow(POS[0],i)*pow(POS[1],j);
		POS2[1]+=COEF[1][i][j]*pow(POS[0],i)*pow(POS[1],j);
	}
*/
	double COEF[4][10*10]={0};
COEF[0][ 0 ]= -0.00047203560468;
COEF[0][ 1 ]= -1.5427988883e-05;
COEF[0][ 2 ]= -5.95865625284e-10;
COEF[0][ 3 ]= 2.22594651446e-13;
COEF[0][ 4 ]= -1.51583805989e-17;
COEF[0][ 5 ]= -6.08317204253e-21;
COEF[0][ 6 ]= 8.06561200947e-26;
COEF[0][ 7 ]= 3.10689047143e-29;
COEF[0][ 8 ]= -1.2634302141e-34;
COEF[0][ 9 ]= -4.78814991762e-38;
COEF[0][ 10 ]= -0.000210433037672+1.0;
COEF[0][ 11 ]= 4.66142798084e-09;
COEF[0][ 12 ]= -1.0957097072e-10;
COEF[0][ 13 ]= -4.6906678848e-17;
COEF[0][ 14 ]= 4.22216001121e-20;
COEF[0][ 15 ]= 1.87541306804e-25;
COEF[0][ 16 ]= -3.11027701152e-28;
COEF[0][ 17 ]= -1.25424492312e-34;
COEF[0][ 18 ]= 3.70673801604e-37;
COEF[0][ 19 ]= -6.85371734365e-09;
COEF[0][ 20 ]= -1.26203995922e-14;
COEF[0][ 21 ]= -5.4517702042e-17;
COEF[0][ 22 ]= -9.94551136649e-22;
COEF[0][ 23 ]= 9.09298367647e-25;
COEF[0][ 24 ]= 2.45558081417e-29;
COEF[0][ 25 ]= -1.92962666077e-33;
COEF[0][ 26 ]= -6.7994922883e-38;
COEF[0][ 27 ]= -1.04551853816e-10;
COEF[0][ 28 ]= -1.95257983584e-17;
COEF[0][ 29 ]= 2.66068980901e-20;
COEF[0][ 30 ]= 2.05500547863e-25;
COEF[0][ 31 ]= -7.00557780822e-28;
COEF[0][ 32 ]= -1.27106133492e-33;
COEF[0][ 33 ]= 1.17427552052e-36;
COEF[0][ 34 ]= 5.48308544761e-17;
COEF[0][ 35 ]= -6.6977079558e-21;
COEF[0][ 36 ]= -3.88079500249e-26;
COEF[0][ 37 ]= -1.12430203404e-29;
COEF[0][ 38 ]= -2.3228732246e-33;
COEF[0][ 39 ]= 5.83996936659e-38;
COEF[0][ 40 ]= -2.55808760342e-20;
COEF[0][ 41 ]= -5.51546375606e-26;
COEF[0][ 42 ]= -4.43037636885e-28;
COEF[0][ 43 ]= 3.26328244851e-34;
COEF[0][ 44 ]= 1.30831113179e-36;
COEF[0][ 45 ]= -2.3965595459e-25;
COEF[0][ 46 ]= 7.38850410175e-29;
COEF[0][ 47 ]= 6.31709084288e-34;
COEF[0][ 48 ]= -5.69997824021e-38;
COEF[0][ 49 ]= -3.49962832225e-30;
COEF[0][ 50 ]= 9.21035992064e-35;
COEF[0][ 51 ]= 4.48010296471e-37;
COEF[0][ 52 ]= 2.36522390769e-34;
COEF[0][ 53 ]= -1.7686068989e-37;
COEF[0][ 54 ]= -8.6880691822e-38;
COEF[1][ 0 ]= -2.27525408678e-05;
COEF[1][ 1 ]= -0.000149438556393+1.0;
COEF[1][ 2 ]= 1.47288649136e-09;
COEF[1][ 3 ]= -1.07681558891e-10;
COEF[1][ 4 ]= -4.52745194926e-17;
COEF[1][ 5 ]= 5.33446374932e-21;
COEF[1][ 6 ]= 1.59765278412e-25;
COEF[1][ 7 ]= -1.35281754124e-28;
COEF[1][ 8 ]= -2.58952055468e-34;
COEF[1][ 9 ]= 1.18384181522e-37;
COEF[1][ 10 ]= -1.54279888831e-05;
COEF[1][ 11 ]= 5.10149451107e-09;
COEF[1][ 12 ]= -2.20369366154e-12;
COEF[1][ 13 ]= -8.12440053288e-17;
COEF[1][ 14 ]= 3.16674570469e-20;
COEF[1][ 15 ]= 2.36720490323e-25;
COEF[1][ 16 ]= -1.54887554063e-28;
COEF[1][ 17 ]= -2.18878587707e-34;
COEF[1][ 18 ]= 2.42019175449e-37;
COEF[1][ 19 ]= -1.36641013138e-09;
COEF[1][ 20 ]= -1.08210753878e-10;
COEF[1][ 21 ]= 3.24065404366e-17;
COEF[1][ 22 ]= 2.21741676333e-20;
COEF[1][ 23 ]= -3.30404486918e-25;
COEF[1][ 24 ]= -4.7223051146e-28;
COEF[1][ 25 ]= 8.48620744583e-34;
COEF[1][ 26 ]= 5.84549240581e-37;
COEF[1][ 27 ]= 1.65013522193e-12;
COEF[1][ 28 ]= -2.04698537311e-16;
COEF[1][ 29 ]= 1.36596617211e-20;
COEF[1][ 30 ]= 8.77160647683e-25;
COEF[1][ 31 ]= -1.36731060152e-28;
COEF[1][ 32 ]= -1.43968368509e-33;
COEF[1][ 33 ]= 4.00898492827e-37;
COEF[1][ 34 ]= -2.27951193984e-17;
COEF[1][ 35 ]= 1.16796604208e-20;
COEF[1][ 36 ]= -6.53927583976e-25;
COEF[1][ 37 ]= -4.41168731276e-28;
COEF[1][ 38 ]= 1.38404520921e-33;
COEF[1][ 39 ]= 8.26267449077e-37;
COEF[1][ 40 ]= -1.75167734408e-20;
COEF[1][ 41 ]= 1.35671719277e-24;
COEF[1][ 42 ]= -5.56167978363e-29;
COEF[1][ 43 ]= -2.43608580718e-33;
COEF[1][ 44 ]= 9.3744233119e-38;
COEF[1][ 45 ]= 8.31436843296e-26;
COEF[1][ 46 ]= -1.73569476217e-28;
COEF[1][ 47 ]= 1.90770699097e-33;
COEF[1][ 48 ]= 4.98143401516e-37;
COEF[1][ 49 ]= 6.57627509385e-29;
COEF[1][ 50 ]= -2.64064071957e-33;
COEF[1][ 51 ]= 1.56461570921e-37;
COEF[1][ 52 ]= -1.50783715462e-34;
COEF[1][ 53 ]= 1.98549941035e-37;
COEF[1][ 54 ]= -8.74305862185e-38;
COEF[2][ 0 ]= 0.00365271948353;
COEF[2][ 1 ]= 1.70911115723e-05;
COEF[2][ 2 ]= 1.5204217229e-10;
COEF[2][ 3 ]= -3.08715201043e-13;
COEF[2][ 4 ]= 2.39597939294e-17;
COEF[2][ 5 ]= 7.81157081952e-21;
COEF[2][ 6 ]= -1.29621716896e-25;
COEF[2][ 7 ]= -4.16263764639e-29;
COEF[2][ 8 ]= 2.15552971078e-34;
COEF[2][ 9 ]= 6.82597059998e-38;
COEF[2][ 10 ]= 0.000209404097794+1.0;
COEF[2][ 11 ]= -4.75019821106e-09;
COEF[2][ 12 ]= 1.10190607837e-10;
COEF[2][ 13 ]= 4.93339497018e-17;
COEF[2][ 14 ]= -1.85924839513e-20;
COEF[2][ 15 ]= -1.95896472784e-25;
COEF[2][ 16 ]= 3.88256510948e-28;
COEF[2][ 17 ]= 9.69693004122e-35;
COEF[2][ 18 ]= -3.47706701826e-37;
COEF[2][ 19 ]= 6.71381011215e-09;
COEF[2][ 20 ]= -9.50158088079e-14;
COEF[2][ 21 ]= 7.93173879809e-17;
COEF[2][ 22 ]= 1.60828614399e-21;
COEF[2][ 23 ]= -1.21754098187e-24;
COEF[2][ 24 ]= -2.62579677501e-29;
COEF[2][ 25 ]= 2.74663158936e-33;
COEF[2][ 26 ]= 7.59945998098e-38;
COEF[2][ 27 ]= 1.04498582488e-10;
COEF[2][ 28 ]= 1.74134310921e-17;
COEF[2][ 29 ]= 3.24251148641e-20;
COEF[2][ 30 ]= -2.26265099203e-25;
COEF[2][ 31 ]= 8.70112786558e-28;
COEF[2][ 32 ]= 1.68537897358e-33;
COEF[2][ 33 ]= -9.78020507263e-37;
COEF[2][ 34 ]= -5.13651337215e-17;
COEF[2][ 35 ]= 1.04041291095e-20;
COEF[2][ 36 ]= -7.65133837619e-26;
COEF[2][ 37 ]= 3.53907656972e-30;
COEF[2][ 38 ]= 3.47366147648e-33;
COEF[2][ 39 ]= -8.85077550431e-38;
COEF[2][ 40 ]= 6.30556215478e-20;
COEF[2][ 41 ]= 9.43999734447e-26;
COEF[2][ 42 ]= 5.33882127553e-28;
COEF[2][ 43 ]= -3.92799329224e-34;
COEF[2][ 44 ]= -8.4676694827e-37;
COEF[2][ 45 ]= 2.185067016e-25;
COEF[2][ 46 ]= -1.09047001782e-28;
COEF[2][ 47 ]= -4.39266689902e-34;
COEF[2][ 48 ]= 1.31099098133e-37;
COEF[2][ 49 ]= -5.78460936624e-30;
COEF[2][ 50 ]= -1.38369955347e-34;
COEF[2][ 51 ]= 3.92691778634e-38;
COEF[2][ 52 ]= -1.23086882478e-34;
COEF[2][ 53 ]= 2.67441530618e-37;
COEF[2][ 54 ]= 2.76078356876e-37;
COEF[3][ 0 ]= -0.00243520601215;
COEF[3][ 1 ]= 0.000147893495017+1.0;
COEF[3][ 2 ]= -1.37763224595e-09;
COEF[3][ 3 ]= 1.07833848918e-10;
COEF[3][ 4 ]= 4.62596775002e-17;
COEF[3][ 5 ]= 2.80575848534e-20;
COEF[3][ 6 ]= -1.65367128388e-25;
COEF[3][ 7 ]= 1.54820423917e-28;
COEF[3][ 8 ]= 3.22741961727e-34;
COEF[3][ 9 ]= 7.08058901535e-39;
COEF[3][ 10 ]= 1.29459993973e-05;
COEF[3][ 11 ]= -5.75758775598e-09;
COEF[3][ 12 ]= 2.59566096648e-12;
COEF[3][ 13 ]= 8.89501051555e-17;
COEF[3][ 14 ]= -3.98215173203e-20;
COEF[3][ 15 ]= -2.42815205576e-25;
COEF[3][ 16 ]= 2.11105473817e-28;
COEF[3][ 17 ]= 2.17987210027e-34;
COEF[3][ 18 ]= -3.57655925495e-37;
COEF[3][ 19 ]= 1.71046572758e-09;
COEF[3][ 20 ]= 1.08416040618e-10;
COEF[3][ 21 ]= -5.39914444014e-17;
COEF[3][ 22 ]= 4.3772487775e-20;
COEF[3][ 23 ]= 5.39490871566e-25;
COEF[3][ 24 ]= 5.37804304106e-28;
COEF[3][ 25 ]= -1.33522335425e-33;
COEF[3][ 26 ]= -1.00922213245e-37;
COEF[3][ 27 ]= -1.61692503254e-12;
COEF[3][ 28 ]= 2.37001625897e-16;
COEF[3][ 29 ]= -1.87396962399e-20;
COEF[3][ 30 ]= -1.01303057487e-24;
COEF[3][ 31 ]= 1.88379875457e-28;
COEF[3][ 32 ]= 1.74279811617e-33;
COEF[3][ 33 ]= -5.95775054508e-37;
COEF[3][ 34 ]= 1.73901668516e-17;
COEF[3][ 35 ]= 2.03868624457e-20;
COEF[3][ 36 ]= 9.5148136402e-25;
COEF[3][ 37 ]= 4.98415367039e-28;
COEF[3][ 38 ]= -2.07693213639e-33;
COEF[3][ 39 ]= -7.27517738702e-38;
COEF[3][ 40 ]= 1.71531399232e-20;
COEF[3][ 41 ]= -1.65907556613e-24;
COEF[3][ 42 ]= 9.02531724098e-29;
COEF[3][ 43 ]= 3.06533133333e-33;
COEF[3][ 44 ]= -1.43492096446e-37;
COEF[3][ 45 ]= -4.32749987233e-26;
COEF[3][ 46 ]= 2.04328665654e-28;
COEF[3][ 47 ]= -2.81367089917e-33;
COEF[3][ 48 ]= 1.74213007753e-38;
COEF[3][ 49 ]= -6.279347973e-29;
COEF[3][ 50 ]= 3.45559728633e-33;
COEF[3][ 51 ]= -2.637596961e-37;
COEF[3][ 52 ]= 9.34962455855e-35;
COEF[3][ 53 ]= -1.03493809035e-37;
COEF[3][ 54 ]= 8.09745928121e-38;
	int i,j,ij;
	POS2[0]=POS2[1]=0;
	ij=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS2[0]+=COEF[0][ij]*pow(POS[0],i)*pow(POS[1],j);
		POS2[1]+=COEF[1][ij]*pow(POS[0],i)*pow(POS[1],j);
		ij++;
	}
	return POS2;
}
std::vector< double > CALC_IMPIX_SIM(std::vector< double >POS){
	std::vector< double > POS1(2);
	std::vector< double > POS2(2);
	double Coef[2][10*10]={0},CD[2][2];

	CD[0][0]= 1.0;
	CD[0][1]= 0.0;
	CD[1][0]= 0.0;
	CD[1][1]= 1.0;
	POS1[0]=( CD[1][1]*POS[0]-CD[0][1]*POS[1])/(CD[0][0]*CD[1][1]-CD[0][1]*CD[1][0]);
	POS1[1]=(-CD[1][0]*POS[0]+CD[0][0]*POS[1])/(CD[0][0]*CD[1][1]-CD[0][1]*CD[1][0]);

    Coef[0][0] = 4.542690e-06;
    Coef[0][1] = 1.296800e-10;
    Coef[0][2] = 2.211780e-13;
    Coef[0][3] = -6.022910e-18;
    Coef[0][4] = -4.738940e-22;
    Coef[0][5] = 5.167470e-26;
    Coef[0][6] = -1.416920e-30;
    Coef[0][7] = -1.796020e-34;
    Coef[0][8] = 2.495480e-39;
    Coef[0][9] = 3.404300e-43;
    Coef[0][10] = 1.000000e+00;
    Coef[0][11] = -1.992900e-10;
    Coef[0][12] = 1.054550e-10;
    Coef[0][13] = -7.053790e-19;
    Coef[0][14] = 4.113600e-20;
    Coef[0][15] = -1.998890e-27;
    Coef[0][16] = 1.254120e-28;
    Coef[0][17] = 6.411820e-37;
    Coef[0][18] = 3.408310e-38;
    Coef[0][19] = -9.447260e-13;
    Coef[0][20] = 3.963980e-18;
    Coef[0][21] = -2.987850e-20;
    Coef[0][22] = 4.327510e-25;
    Coef[0][23] = 7.795610e-29;
    Coef[0][24] = -2.726600e-33;
    Coef[0][25] = 1.879840e-38;
    Coef[0][26] = 1.131500e-43;
    Coef[0][27] = 1.053850e-10;
    Coef[0][28] = -8.569080e-19;
    Coef[0][29] = 8.227350e-20;
    Coef[0][30] = -1.948830e-27;
    Coef[0][31] = 3.775440e-28;
    Coef[0][32] = -2.887790e-36;
    Coef[0][33] = 1.316990e-37;
    Coef[0][34] = 2.612040e-20;
    Coef[0][35] = -6.318480e-25;
    Coef[0][36] = 4.778220e-28;
    Coef[0][37] = -3.111430e-33;
    Coef[0][38] = -9.045120e-37;
    Coef[0][39] = 3.206950e-41;
    Coef[0][40] = 4.230190e-20;
    Coef[0][41] = -9.930950e-28;
    Coef[0][42] = 3.762150e-28;
    Coef[0][43] = -5.858310e-36;
    Coef[0][44] = 1.981810e-37;
    Coef[0][45] = -2.009800e-28;
    Coef[0][46] = 8.325950e-33;
    Coef[0][47] = -1.609580e-36;
    Coef[0][48] = -1.482370e-41;
    Coef[0][49] = 1.186840e-28;
    Coef[0][50] = -8.108250e-37;
    Coef[0][51] = 1.362140e-37;
    Coef[0][52] = 4.180720e-37;
    Coef[0][53] = -2.454560e-41;
    Coef[0][54] = 4.666950e-38;
    Coef[1][0] = 8.899000e-05;
    Coef[1][1] = 1.000000e+00;
    Coef[1][2] = 2.027010e-10;
    Coef[1][3] = 1.053860e-10;
    Coef[1][4] = -4.067880e-19;
    Coef[1][5] = 4.228600e-20;
    Coef[1][6] = -3.754940e-27;
    Coef[1][7] = 1.187960e-28;
    Coef[1][8] = 4.797460e-36;
    Coef[1][9] = 4.642720e-38;
    Coef[1][10] = 2.334020e-10;
    Coef[1][11] = -2.574770e-14;
    Coef[1][12] = -9.744220e-18;
    Coef[1][13] = -1.508580e-23;
    Coef[1][14] = 4.971630e-26;
    Coef[1][15] = 8.632380e-31;
    Coef[1][16] = -7.487640e-35;
    Coef[1][17] = 3.833490e-41;
    Coef[1][18] = 2.232670e-44;
    Coef[1][19] = 4.171750e-10;
    Coef[1][20] = 1.054540e-10;
    Coef[1][21] = -6.046790e-19;
    Coef[1][22] = 8.227860e-20;
    Coef[1][23] = -3.126960e-27;
    Coef[1][24] = 3.762750e-28;
    Coef[1][25] = 1.171560e-38;
    Coef[1][26] = 1.359200e-37;
    Coef[1][27] = -1.179380e-17;
    Coef[1][28] = 1.184970e-21;
    Coef[1][29] = 4.390170e-25;
    Coef[1][30] = -3.313780e-30;
    Coef[1][31] = -1.682110e-33;
    Coef[1][32] = -1.898110e-38;
    Coef[1][33] = 1.530080e-42;
    Coef[1][34] = 6.011380e-20;
    Coef[1][35] = 4.115600e-20;
    Coef[1][36] = -2.309640e-27;
    Coef[1][37] = 3.774010e-28;
    Coef[1][38] = 1.524900e-36;
    Coef[1][39] = 1.981170e-37;
    Coef[1][40] = 1.522440e-25;
    Coef[1][41] = -1.039640e-29;
    Coef[1][42] = -4.454080e-33;
    Coef[1][43] = 4.759690e-38;
    Coef[1][44] = 9.262920e-42;
    Coef[1][45] = -1.276740e-28;
    Coef[1][46] = 1.253000e-28;
    Coef[1][47] = 3.366590e-36;
    Coef[1][48] = 1.322750e-37;
    Coef[1][49] = -6.981670e-34;
    Coef[1][50] = 1.544400e-38;
    Coef[1][51] = 1.205360e-41;
    Coef[1][52] = 6.493160e-37;
    Coef[1][53] = 3.425900e-38;
    Coef[1][54] = 1.033080e-42;
	int i,j,ij;
	ij=0;
	POS2[0]=POS2[1]=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS2[0]+=Coef[0][ij]*pow(POS1[0],i)*pow(POS1[1],j);
		POS2[1]+=Coef[1][ij]*pow(POS1[0],i)*pow(POS1[1],j);
		ij++;
	}
	return POS2;
}
std::vector< double > CALC_IMWLD_SIM(std::vector< double >POS){
	std::vector< double > POS1(2);
	std::vector< double > POS2(2);
	double Coef[2][10*10]={0},CD[2][2];

	CD[0][0]= 1.0;
	CD[0][1]= 0.0;
	CD[1][0]= 0.0;
	CD[1][1]= 1.0;

    Coef[0][0] = -4.380370e-06;
    Coef[0][1] = -1.995040e-10;
    Coef[0][2] = -1.817160e-13;
    Coef[0][3] = 5.617380e-18;
    Coef[0][4] = 2.964630e-22;
    Coef[0][5] = -4.136840e-26;
    Coef[0][6] = 1.251490e-30;
    Coef[0][7] = 1.350210e-34;
    Coef[0][8] = -1.833610e-39;
    Coef[0][9] = -2.459310e-43;
    Coef[0][10] = 1.000000e+00;
    Coef[0][11] = 2.007060e-10;
    Coef[0][12] = -1.055900e-10;
    Coef[0][13] = 6.600740e-19;
    Coef[0][14] = -6.189660e-21;
    Coef[0][15] = 1.620700e-27;
    Coef[0][16] = -1.122810e-28;
    Coef[0][17] = -2.491050e-36;
    Coef[0][18] = 1.018330e-37;
    Coef[0][19] = 8.616620e-13;
    Coef[0][20] = 1.040290e-17;
    Coef[0][21] = 2.342430e-20;
    Coef[0][22] = -4.253810e-25;
    Coef[0][23] = -5.310430e-29;
    Coef[0][24] = 2.046700e-33;
    Coef[0][25] = -2.211180e-38;
    Coef[0][26] = 1.731020e-43;
    Coef[0][27] = -1.055360e-10;
    Coef[0][28] = 7.971890e-19;
    Coef[0][29] = -1.237090e-20;
    Coef[0][30] = 1.561240e-27;
    Coef[0][31] = -3.377410e-28;
    Coef[0][32] = -3.722440e-36;
    Coef[0][33] = 4.102940e-37;
    Coef[0][34] = -2.256700e-20;
    Coef[0][35] = 1.831590e-25;
    Coef[0][36] = -3.534470e-28;
    Coef[0][37] = 3.929320e-33;
    Coef[0][38] = 6.098000e-37;
    Coef[0][39] = -2.359170e-41;
    Coef[0][40] = -7.040310e-21;
    Coef[0][41] = 7.398160e-28;
    Coef[0][42] = -3.369280e-28;
    Coef[0][43] = -1.452360e-36;
    Coef[0][44] = 6.150350e-37;
    Coef[0][45] = 1.645070e-28;
    Coef[0][46] = -4.046500e-33;
    Coef[0][47] = 1.108910e-36;
    Coef[0][48] = 4.139220e-42;
    Coef[0][49] = -1.076870e-28;
    Coef[0][50] = -1.198030e-36;
    Coef[0][51] = 4.076940e-37;
    Coef[0][52] = -3.242150e-37;
    Coef[0][53] = 1.283090e-41;
    Coef[0][54] = 9.379990e-38;
    Coef[1][0] = -8.286180e-05;
    Coef[1][1] = 1.000000e+00;
    Coef[1][2] = -2.043080e-10;
    Coef[1][3] = -1.055360e-10;
    Coef[1][4] = 5.734950e-19;
    Coef[1][5] = -7.031790e-21;
    Coef[1][6] = 2.754290e-27;
    Coef[1][7] = -1.077510e-28;
    Coef[1][8] = -4.929120e-36;
    Coef[1][9] = 9.393580e-38;
    Coef[1][10] = -2.012200e-10;
    Coef[1][11] = 5.215440e-14;
    Coef[1][12] = 8.202230e-18;
    Coef[1][13] = -1.367250e-22;
    Coef[1][14] = -3.571670e-26;
    Coef[1][15] = -6.255230e-31;
    Coef[1][16] = 4.801170e-35;
    Coef[1][17] = -8.679450e-41;
    Coef[1][18] = -1.184220e-44;
    Coef[1][19] = -4.173330e-10;
    Coef[1][20] = -1.055890e-10;
    Coef[1][21] = 8.467120e-19;
    Coef[1][22] = -1.238130e-20;
    Coef[1][23] = 2.317620e-27;
    Coef[1][24] = -3.369240e-28;
    Coef[1][25] = -4.791800e-36;
    Coef[1][26] = 4.077870e-37;
    Coef[1][27] = 9.621110e-18;
    Coef[1][28] = -2.268360e-21;
    Coef[1][29] = -3.570260e-25;
    Coef[1][30] = 7.894370e-30;
    Coef[1][31] = 1.145440e-33;
    Coef[1][32] = 1.340330e-38;
    Coef[1][33] = -9.379170e-43;
    Coef[1][34] = 7.940900e-20;
    Coef[1][35] = -6.207820e-21;
    Coef[1][36] = 1.734180e-27;
    Coef[1][37] = -3.375970e-28;
    Coef[1][38] = -5.680250e-36;
    Coef[1][39] = 6.149470e-37;
    Coef[1][40] = -1.170020e-25;
    Coef[1][41] = 2.081400e-29;
    Coef[1][42] = 3.516930e-33;
    Coef[1][43] = -6.731210e-38;
    Coef[1][44] = -5.956520e-42;
    Coef[1][45] = 7.247450e-29;
    Coef[1][46] = -1.121890e-28;
    Coef[1][47] = -3.616860e-36;
    Coef[1][48] = 4.098260e-37;
    Coef[1][49] = 5.022440e-34;
    Coef[1][50] = -4.451430e-38;
    Coef[1][51] = -9.308030e-42;
    Coef[1][52] = -2.769340e-37;
    Coef[1][53] = 1.017020e-37;
    Coef[1][54] = -6.874890e-43;
	int i,j,ij;
	ij=0;
	POS1[0]=POS1[1]=0;
	for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	if( i + j<10){
		POS1[0]+=Coef[0][ij]*pow(POS[0],i)*pow(POS[1],j);
		POS1[1]+=Coef[1][ij]*pow(POS[0],i)*pow(POS[1],j);
		ij++;
	}
	POS2[0]=CD[0][0]*POS1[0]+CD[0][1]*POS1[1];
	POS2[1]=CD[1][0]*POS1[0]+CD[1][1]*POS1[1];
	return POS2;
}

