//============================================================================
// Name        : motion.cpp
// Author      : Rokhmat - 2018
// Version     : 1.0
// Copyright   : Bismillah
// Description : Bismillah for everything
//============================================================================
//tes
#include "eheader.h"

/* **Gerak Dasar Menuju Bola**

   Function Member:
   **GerakHadapBola->GerakCoverBall
   				   ->GerakArahBolaLurus
   				   ->GerakCoverBallSlow
*/
int GerakHadapBolakeeping(int sdtX, int sdtY){
	int gerakR = 10;
	if(sdtY>45) //Bola Jauh
	{
		gerakR = GerakCoverBall(sdtX,sdtY);
		//fprintf(stderr, "GCB ");
	}
	else if(sdtY>30 && sdtY<=45)
	{
		gerakR = GerakCoverBallSlow(sdtX,sdtY);
		//fprintf(stderr, "GCBS ");
	}
	else if(sdtY>=0)
	{
		if(sdtX > 45) 		gerakR = 19;
		else if(sdtX < -45) 	gerakR = 19;
		else if(sdtX>25)	gerakR=26;//26
		else if(sdtX<-25)	gerakR=25;//25
		else if(sdtX > 15) 	gerakR = 14;//14
		else if(sdtX < -15)	gerakR = 13;//13
		else gerakR=10;
	}
	else gerakR=10;
	
	return gerakR;
}
int GerakHadapBola(int sdtX, int sdtY, int arahrobotskr, int arahhadap){
	int gerakR = 10;
	if(sdtY>45) //Bola Jauh
	{
		gerakR = GerakCoverBall(sdtX,sdtY);
		//fprintf(stderr, "GCB ");
	}
	else if(arahrobotskr == arahhadap)
	{
		if(sdtX < 10 && sdtX > -10 && sdtY<=7){
			gerakR = 10;
		}
		else {
			gerakR = GerakArahBolaLurus(sdtX,sdtY); 
			//fprintf(stderr, "GABL ");
		}
	}else if(sdtY>30 && sdtY<=45)
	{
		gerakR = GerakCoverBallSlow(sdtX,sdtY);
		//fprintf(stderr, "GCBS ");
	}
	else if(sdtY>=0)
	{
		//fprintf(stderr, "GOTHER ");
		if(sdtX > 45) 		gerakR = 19;
		else if(sdtX < -45) gerakR = 19;
		else if(sdtX>25)	gerakR=26;//26
		else if(sdtX<-25)	gerakR=25;//25
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX > 30)){
		 	gerakR = 14;
		}
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX < -30)){
			gerakR = 13;
		}
		//else if(sdtX < 0 && sdtX > -20 && arahrobotskr<=7 && arahrobotskr>=3) gerakR = 7;
		else if(sdtX > 15) 	gerakR = 14;//14
		else if(sdtX < -15) gerakR = 13;//13
		else if(arahhadap>=1 && arahhadap<=10 && arahhadap!=arahrobotskr){
			if(arahrobotskr > arahhadap && arahrobotskr <= arahhadap+10)
					gerakR = 18; //putar kiri
			else 	gerakR = 17; //putar kanan
		}else if(arahhadap>=11 && arahhadap<=20 && arahhadap!=arahrobotskr){
			if(arahrobotskr >= arahhadap-10 && arahrobotskr < arahhadap)
					gerakR = 17;//putar kanan
			else 	gerakR = 18; //putar kiri		
		}else gerakR=10;
	}
	else gerakR=10;
	
	return gerakR;
}

int GerakCoverBall(int sdtX, int sdtY){
	int gerakR;
	if(sdtY>70){
		if(sdtX>=66) gerakR=26;
		else if(sdtX<66  && sdtX>=39) gerakR=12;
		else if(sdtX<39  && sdtX>=31) gerakR=22;
		else if(sdtX<31  && sdtX>=15) gerakR=42;
		else if(sdtX<15  && sdtX>=4)  gerakR=52;
		else if(sdtX<4 	 && sdtX>=-4) gerakR=50;
		else if(sdtX<-4  && sdtX>=-15) gerakR=51;
		else if(sdtX<-15 && sdtX>=-31) gerakR=41;
		else if(sdtX<-31 && sdtX>=-39) gerakR=21;
		else if(sdtX<-39 && sdtX>=-66) gerakR=11;
		else if(sdtX<-66) gerakR=25;
	} else if(sdtY>60 && sdtY<=70){
		if(sdtX>=55) gerakR=26;
		else if(sdtX<55  && sdtX>=43) gerakR=12;
		else if(sdtX<43  && sdtX>=31) gerakR=22;
		else if(sdtX<31  && sdtX>=20) gerakR=32;
		else if(sdtX<20  && sdtX>=5)  gerakR=42;
		else if(sdtX<5	 && sdtX>=-5) gerakR=40;
		else if(sdtX<-5  && sdtX>=-20) gerakR=41;
		else if(sdtX<-20 && sdtX>=-30) gerakR=31;
		else if(sdtX<-31 && sdtX>=-43) gerakR=21;
		else if(sdtX<-43 && sdtX>=-55) gerakR=11;
		else if(sdtX<-55) gerakR=25;
	} else if(sdtY>35 && sdtY<=60){
		if(sdtX>=66) gerakR=26;
		else if(sdtX<66  && sdtX>=41) gerakR=12;
		else if(sdtX<41  && sdtX>=33) gerakR=12;
		else if(sdtX<33  && sdtX>=24) gerakR=22;
		else if(sdtX<24  && sdtX>=5)  gerakR=32;
		else if(sdtX<5	 && sdtX>=-5) gerakR=30;
		else if(sdtX<-5  && sdtX>=-24) gerakR=31;
		else if(sdtX<-24 && sdtX>=-33) gerakR=21;
		else if(sdtX<-33 && sdtX>=-41) gerakR=11;
		else if(sdtX<-41 && sdtX>=-66) gerakR=11;
		else if(sdtX<-66) gerakR=25;
	} else if(sdtY>25 && sdtY<=35){
		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>=44) gerakR=26;
		else if(sdtX<44  && sdtX>=36) gerakR=12;
		else if(sdtX<36  && sdtX>=27) gerakR=14;
		else if(sdtX<27  && sdtX>=5)  gerakR=24;
		else if(sdtX<5	 && sdtX>=-5) gerakR=20;
		else if(sdtX<-5  && sdtX>=-27) gerakR=23;
		else if(sdtX<-27 && sdtX>=-36) gerakR=13;
		else if(sdtX<-36 && sdtX>=-44) gerakR=11;
		else if(sdtX<-44 && sdtX>=-70) gerakR=25;
		else if(sdtX<-70) gerakR=25;
	} else if(sdtY<=25){
		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>=35) gerakR=12;
		else if(sdtX<35  && sdtX>=18) gerakR=16;
		else if(sdtX<18  && sdtX>=6)  gerakR=14;
		else if(sdtX<6   && sdtX>=-6) gerakR=10;
		else if(sdtX<-6  && sdtX>=-18) gerakR=13;
		else if(sdtX<-18 && sdtX>=-35) gerakR=15;
		else if(sdtX<-35 && sdtX>=-70) gerakR=11;
		else if(sdtX<-70) gerakR=25;
	}
	return gerakR;
}

int GerakArahBolaLurus(int sdtX, int sdtY){
		int gerakR;
		if(sdtY>70){
			if(sdtX>=35) gerakR=16;
			else if(sdtX<35  && sdtX>=25)  gerakR=34;
			else if(sdtX<25  && sdtX>=4)   gerakR=40;
			else if(sdtX<4   && sdtX>=-4)  gerakR=40;
			else if(sdtX<-4  && sdtX>=-25) gerakR=40;
			else if(sdtX<-25 && sdtX>=-35) gerakR=33;
			else if(sdtX<-35) gerakR=15;
		}
		else if(sdtY>40 && sdtY<=70){
			if(sdtX>=69) gerakR=19;
			else if(sdtX<69  && sdtX>=35)  gerakR=16;
			else if(sdtX<35  && sdtX>=25)   gerakR=24;
			else if(sdtX<25  && sdtX>=5)   gerakR=34;
			else if(sdtX<5   && sdtX>=-5)  gerakR=30;
			else if(sdtX<-5  && sdtX>=-25) gerakR=33;
			else if(sdtX<-25 && sdtX>=-35) gerakR=23;
			else if(sdtX<-35 && sdtX>=-69) gerakR=15;
			else if(sdtX<-69) gerakR=19;
		}
		else if(sdtY>30 && sdtY<=40){
			if(sdtX>=69) gerakR=19;
			else if(sdtX<69  && sdtX>=35)  gerakR=16;
			else if(sdtX<35  && sdtX>=25)   gerakR=14;
			else if(sdtX<25  && sdtX>=5)   gerakR=24;
			else if(sdtX<5   && sdtX>=-5)  gerakR=20;
			else if(sdtX<-5  && sdtX>=-25) gerakR=23;
			else if(sdtX<-25 && sdtX>=-35) gerakR=13;
			else if(sdtX<-35 && sdtX>=-69) gerakR=15;
			else if(sdtX<-69) gerakR=19;
		}
		else if(sdtY<=30){
			if(sdtX>=45) gerakR=19;
			else if(sdtX<45  && sdtX>=25)  gerakR=16;
			else if(sdtX<25  && sdtX>= 6)   gerakR=14;
			else if(sdtX< 6  && sdtX>=-6)  gerakR=10;
			else if(sdtX<-6  && sdtX>=-25) gerakR=13;
			else if(sdtX<-25 && sdtX>=-45) gerakR=15;
			else if(sdtX<-45) gerakR=19;
		} else gerakR = 10;
		return gerakR;

}

int GerakCoverBallSlow(int sdtX, int sdtY){
	int gerakR;
	if(sdtY>50 ){
		if(sdtX>=66) gerakR=26;
		else if(sdtX<66  && sdtX>=39) gerakR=12;
		else if(sdtX<39  && sdtX>=31) gerakR=22;
		else if(sdtX<31  && sdtX>=22) gerakR=32;
		else if(sdtX<22  && sdtX>=4)  gerakR=32;
		else if(sdtX<4 	 && sdtX>=-4) gerakR=30;
		else if(sdtX<-4  && sdtX>=-22) gerakR=31;
		else if(sdtX<-22 && sdtX>=-31) gerakR=31;
		else if(sdtX<-31 && sdtX>=-39) gerakR=21;
		else if(sdtX<-39 && sdtX>=-66) gerakR=11;
		else if(sdtX<-66) gerakR=25;
	}
	else if(sdtY>30 && sdtY<=50){
		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>=44) gerakR=12;
		else if(sdtX<44  && sdtX>=36) gerakR=22;
		else if(sdtX<36  && sdtX>=27) gerakR=22;
		else if(sdtX<27  && sdtX>=5)  gerakR=24;
		else if(sdtX<5	 && sdtX>=-5) gerakR=20;
		else if(sdtX<-5  && sdtX>=-27) gerakR=23;
		else if(sdtX<-27 && sdtX>=-36) gerakR=21;
		else if(sdtX<-36 && sdtX>=-44) gerakR=21;
		else if(sdtX<-44 && sdtX>=-70) gerakR=11;
		else if(sdtX<-70) gerakR=25;
	}
	else if(sdtY<=30){
		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>35) gerakR=12;
		else if(sdtX<35  && sdtX>18) gerakR=16;
		else if(sdtX<18  && sdtX>7)  gerakR=14;
		else if(sdtX<=7  && sdtX>=-7) gerakR=10;
		else if(sdtX<-7  && sdtX>=-18) gerakR=13;
		else if(sdtX<-18 && sdtX>=-35) gerakR=15;
		else if(sdtX<-35 && sdtX>=-70) gerakR=11;
		else if(sdtX<-70) gerakR=25;
	}
	return gerakR;
}

/*---------------------------------------------------------------------*/
/***********************************************************************/

//______________________________SERANG_____________________________________//
int tdangT=0, stepTdg=0, cobTdg=0;
int GerakTendang(int xFrame, int yFrame, int kaki, int sdtkaki){

	if(sdtkaki >= 100)sdtkaki = (sdtkaki - 100);
	else if(sdtkaki < 100 )sdtkaki = 0;
	int gerakR = 10;
		if(yFrame > 135 && (kaki == 6 || kaki == 5 || kaki == 7 || kaki == 8 )){
			 if(kaki == 8){
				if(xFrame>280){ gerakR = 16; countKick = 0;}
				else if(xFrame<=280 && xFrame>215){ gerakR = 14; countKick = 0; }
				else if(xFrame<=215 && xFrame>190){ countKick++; if(countKick>=5)gerakR = kaki; else gerakR = 10; }
				else if(xFrame<=190 && xFrame>140){ gerakR = 13; countKick = 0; }
				else if(xFrame<=140 && xFrame > 0){ gerakR = 15; countKick = 0; }
			}else if( kaki == 7){
				if(xFrame>200){ gerakR = 16; countKick = 0;}
				else if(xFrame<=200 && xFrame>110){gerakR = 14; countKick = 0;}
				else if(xFrame<=110 && xFrame>85){countKick++;if(countKick>=5)gerakR = kaki;else gerakR = 10;}
				else if(xFrame<=85 && xFrame>20){gerakR = 13; countKick = 0;}
				else if(xFrame<=20 && xFrame > 0){gerakR = 13; countKick = 0;}
			}else if(kaki == 6){
				if(xFrame>340){ gerakR = 16; countKick = 0; }
				else if(xFrame>300 && xFrame<=340){ gerakR = 14; countKick = 0; }
				else if(xFrame<=300 && xFrame>260){ countKick++; if(countKick>=5)gerakR = kaki; else gerakR = 10; }//else if(xFrame<=200 && xFrame>185){ countKick++; if(countKick>=5)gerakR = kaki; else gerakR = 10; }
				//else if(xFrame<=260 && xFrame>160){ gerakR = 13; countKick = 0; }
				else if(xFrame<=260 && xFrame>180){ gerakR = 13; countKick = 0; }
				else if(xFrame<=180 && xFrame>145){countKick++;if(countKick>=5)gerakR = 5; else gerakR = 10;}//else if(xFrame<=110 && xFrame>95){countKick++;if(countKick>=5)gerakR = 5; else gerakR = 10;}
				else if(xFrame<=145  && xFrame>40){gerakR = 13; countKick = 0;}
				else if(xFrame<=40  && xFrame > 0){gerakR = 15; countKick = 0;}
			}else if(kaki == 5 ){
			if(xFrame>340){ gerakR = 16; countKick = 0; }
				else if(xFrame>300 && xFrame<=340){ gerakR = 14; countKick = 0; }
				else if(xFrame<=300 && xFrame>260){ countKick++; if(countKick>=5)gerakR = 6; else gerakR = 10; }//else if(xFrame<=200 && xFrame>185){ countKick++; if(countKick>=5)gerakR = kaki; else gerakR = 10; }
				//else if(xFrame<=260 && xFrame>160){ gerakR = 13; countKick = 0; }
				else if(xFrame<=260 && xFrame>180){ gerakR = 13; countKick = 0; }
				else if(xFrame<=180 && xFrame>145){countKick++;if(countKick>=5)gerakR = 5; else gerakR = 10;}//else if(xFrame<=110 && xFrame>95){countKick++;if(countKick>=5)gerakR = 5; else gerakR = 10;}
				else if(xFrame<=145  && xFrame>40){gerakR = 13; countKick = 0;}
				else if(xFrame<=40  && xFrame > 0){gerakR = 15; countKick = 0;}
			} else gerakR = 10;
		} else if(yFrame < 70 ){
			if(stepTdg!=1){tdangT=0;}

			if(kaki == 8 || kaki == 9 || kaki == 155){
				if(xFrame>250) gerakR = 16;
				else if(xFrame<=250 && xFrame>210) gerakR = 24;
				else if(xFrame<=210 && xFrame>160) {gerakR = 20; tdangT=1; }
				else if(xFrame<=160 && xFrame>120) gerakR = 23;
				else if(xFrame<=120 && xFrame > 0) gerakR = 15;

			}else if(kaki == 7 || kaki == 156){
				if(xFrame>220) gerakR = 16;
				else if(xFrame<=220 && xFrame>160) gerakR = 24;
				else if(xFrame<=160 && xFrame>130) {gerakR = 20; tdangT=1; }
				else if(xFrame<=130 && xFrame>80) gerakR = 23;
				else if(xFrame<=50 && xFrame > 0) gerakR = 15;
			} else if(kaki == 5 || kaki == 6){
				if(xFrame>240) gerakR = 16;
				else if(xFrame<=240 && xFrame>190) gerakR = 24;
				else if(xFrame<=190 && xFrame>130) {gerakR = 20; tdangT=1; }
				else if(xFrame<=130 && xFrame >80) gerakR = 23;
				else if(xFrame<=80 && xFrame > 0) gerakR = 15;
			} else gerakR = 10;
			if(tdangT==1){gerakR = 20;}
			if(dataXB==0){tdangT=0;}
			stepTdg=1;
			countKick = 0;
			cobTdg=0;
		}else if(yFrame >= 70 && yFrame < 100 ){
			if(stepTdg!=2){tdangT=0;}
			if( kaki == 8 || kaki == 9 || kaki == 155){
				if(xFrame>260) gerakR = 16;
				else if(xFrame<=260 && xFrame>200) gerakR = 24;
				else if(xFrame<=200 && xFrame>170) {gerakR = 220; tdangT=2; }
				else if(xFrame<=170 && xFrame>110) gerakR = 23;
				else if(xFrame<=110 && xFrame > 0) gerakR = 15;

			}else if(kaki == 7 || kaki == 156){
				if(xFrame>230) gerakR = 16;
				else if(xFrame<=230 && xFrame>150) gerakR = 24;
				else if(xFrame<=150 && xFrame>120) {gerakR = 220; tdangT=2; }
				else if(xFrame<=120 && xFrame>60) gerakR = 23;
				else if(xFrame<=60 && xFrame > 0) gerakR = 15;
			}  else if(kaki == 5 || kaki == 6){
				if(xFrame>250) gerakR = 16;
				else if(xFrame<=250 && xFrame>180) gerakR = 24;
				else if(xFrame<=180 && xFrame>140) {gerakR = 220; tdangT=2; }
				else if(xFrame<=140 && xFrame>90) gerakR = 23;
				else if(xFrame<=90 && xFrame > 0) gerakR = 15;
			} else gerakR = 10;
			if(tdangT==2){gerakR = 220;}
			if(dataXB==0){tdangT=0;}
			stepTdg=2;
			countKick = 0;
			cobTdg=0;
		}else if(yFrame >= 100 && yFrame < 158 ){
			if(stepTdg!=3){tdangT=0;}
			if( kaki == 8 || kaki == 9 || kaki == 155){
				if(xFrame>260) gerakR = 16;
				else if(xFrame<=260 && xFrame>200) gerakR = 24;
				else if(xFrame<=200 && xFrame>170) {gerakR = 220; tdangT=3; }
				else if(xFrame<=170 && xFrame>110) gerakR = 23;
				else if(xFrame<=110 && xFrame > 0) gerakR = 15;

			}else if(kaki == 7 || kaki == 156){
				if(xFrame>210) gerakR = 16;
				else if(xFrame<=210 && xFrame>150) gerakR = 24;
				else if(xFrame<=150 && xFrame>110) {gerakR = 220; tdangT=3; }
				else if(xFrame<=110 && xFrame>50) gerakR = 23;
				else if(xFrame<=50 && xFrame > 0) gerakR = 15;
			}  else if(kaki == 5 || kaki == 6){
				if(xFrame>240) gerakR = 16;
				else if(xFrame<=240 && xFrame>170) gerakR = 24;
				else if(xFrame<=180 && xFrame>140) {gerakR = 220; tdangT=3; }
				else if(xFrame<=150 && xFrame>110) gerakR = 23;
				else if(xFrame<=110 && xFrame > 0) gerakR = 15;
			} else gerakR = 10;
			if(tdangT==3){gerakR = 220;}
			if(dataXB==0){tdangT=0;}
			stepTdg=3;
			countKick = 0;
			cobTdg=0;
		}else if(yFrame >= 158 && yFrame < 200){
			stepTdg=4;
			cobTdg++;
			if(cobTdg<=10){gerakR=10;}
			else if(kaki == 9){
				if(xFrame>210){ gerakR = 14; countKick = 0; }
				else if(xFrame<=210 && xFrame>180){ countKick++; if(countKick>=5)gerakR = kaki; else gerakR = 10; }
				else if(xFrame<=180 && xFrame>130){ gerakR = 13; countKick = 0; }
				else if(xFrame<=130 && xFrame > 0){ gerakR = 15; countKick = 0;}
			}else if(kaki == 155){
				 if(xFrame>170+((sdtkaki*3)/2)){ gerakR = 14; countKick = 0;}
				else if(xFrame<=170+((sdtkaki*3)/2) && xFrame>140+((sdtkaki*3)/2)){ countKick++; if(countKick>=5){gerakR = kaki;} else gerakR = 10; }
				else if(xFrame<=140+((sdtkaki*3)/2) && xFrame > 0){ gerakR = 13; countKick = 0; }
			}else if( kaki == 156){
				if(xFrame>170-((sdtkaki*3)/2)){ gerakR = 14; countKick = 0;}
				else if(xFrame<=170-((sdtkaki*3)/2) && xFrame>150-((sdtkaki*3)/2)){countKick++;if(countKick>=5){gerakR = kaki; }else gerakR = 10;}
				else if(xFrame<=150-((sdtkaki*3)/2) && xFrame > 0){gerakR = 13; countKick = 0;}
			}else gerakR = 10;
			if(cobTdg >= 20)cobTdg=220;
		} else if(xFrame <= 0 && yFrame <= 0) gerakR = 10;
		//fprintf(stderr,"X(%d)Y(%d)M(%d)C(%d)S(%d)\n", dataXB,dataYB,motion,countKick,sdtkaki);
		if(kaki == 155 || kaki == 156)mode = 110+sdtkaki;
		return gerakR;
}

int tdangT1=0, stepTdg1=0;
int GerakTendangKecil(int xFrame, int yFrame, int kaki){
	int gerakR = 10;
		if(yFrame > 150 && ( kaki == 4 || kaki == 3 )){
			if(kaki == 4){
				if(xFrame>210){
					gerakR = 16; countKick = 0;
				}else if(xFrame<=210 && xFrame>180){
					gerakR = 14; countKick = 0;
				}else if(xFrame<=180 && xFrame>160){
					countKick++;
					if(countKick>=10)gerakR = kaki;
					else gerakR = 10;
				}else if(xFrame<=160 && xFrame>110){
					gerakR = 13; countKick = 0;
				}else if(xFrame<=110 && xFrame>0){
					gerakR = 15; countKick = 0;
				}
			} else if(kaki == 3 ){
				if(xFrame>210){
					gerakR = 16; countKick = 0;
				}else if(xFrame<=210 && xFrame>160){
					gerakR = 14; countKick = 0;
				}else if(xFrame<=160 && xFrame>140){
					countKick++;
					if(countKick>=10)gerakR = kaki;
					else gerakR = 10;
				}else if(xFrame<=140 && xFrame>110){
					gerakR = 13; countKick = 0;
				}else if(xFrame<=110 && xFrame > 0){
					gerakR = 15; countKick = 0;
				}

			}
		}else if(yFrame < 55 ){
			if(stepTdg1!=1){tdangT1=0;}

			if( kaki == 2 || kaki == 4){
				if(xFrame>240) gerakR = 16;
				else if(xFrame<=240 && xFrame>190) gerakR = 24;//14;
				else if(xFrame<=190 && xFrame>160) {gerakR = 20; tdangT1=1;}//{gerakR = 220; tdangT1=1;}
				else if(xFrame<=160 && xFrame>120) gerakR = 23;//13;
				else if(xFrame<=120 && xFrame > 0) gerakR = 15;
			}else if( kaki == 1 || kaki == 3 ){
				if(xFrame>200) gerakR = 16;
				else if(xFrame<=200 && xFrame>160) gerakR = 24;//14;
				else if(xFrame<=160 && xFrame>130) {gerakR = 20; tdangT1=1;}//{gerakR = 220; tdangT1=1;}
				else if(xFrame<=130 && xFrame>80) gerakR = 23;//13;
				else if(xFrame<=80 && xFrame > 0) gerakR = 15;
			}  else gerakR = 10;

			if(tdangT1==1){gerakR = 220;}
			if(dataXB==0){tdangT1=0;}
			stepTdg1=1;
			countKick = 0;
		}else if(yFrame >= 55 && yFrame < 220 ){
			if(stepTdg1!=2){tdangT1=0;}

			if( kaki == 2 || kaki == 4){
				if(xFrame>270) gerakR = 16;
				else if(xFrame<=270 && xFrame>230) gerakR = 14;
				else if(xFrame<=230 && xFrame>200) gerakR = 14;
				else if(xFrame<=200 && xFrame>160) {gerakR = 20; tdangT1=2;}//{gerakR = 220; tdangT1=2;}
				else if(xFrame<=160 && xFrame > 0) gerakR = 13;
			}else if( kaki == 1 || kaki == 3 ){
				if(xFrame>160) gerakR = 14;
				else if(xFrame<=160 && xFrame>120){gerakR = 20; tdangT1=2;}//{gerakR = 220; tdangT1=2;}
				else if(xFrame<=120 && xFrame>90) gerakR = 13;
				else if(xFrame<=90 && xFrame>50) gerakR = 13;
				else if(xFrame<=50 && xFrame > 0) gerakR = 15;
			} else gerakR = 10;

			if(tdangT1==2){gerakR = 220;}
			if(dataXB==0){tdangT1=0;}
			stepTdg1=2;
			countKick = 0;
		}else if(yFrame >= 210){//(yFrame >= 163){
			if(kaki == 2){
				if(xFrame>270){
					gerakR = 16; countKick = 0;
				}else if(xFrame<=270 && xFrame>210){
					gerakR = 14; countKick = 0;
				}else if(xFrame<=210 && xFrame>160){
					countKick++;
					if(countKick>=2)gerakR = kaki;
					else gerakR = 10;
				}//else if(xFrame<=160 && xFrame>135){
				//	gerakR = 13; countKick = 0;
				//}
				else if(xFrame<=160 && xFrame>110){
					countKick++;
					if(countKick>=2)gerakR = 1;
					else gerakR = 10;
				}else if(xFrame<=110 && xFrame > 0){
					gerakR = 13; countKick = 0;
				}
			} else if(kaki == 1){
				if(xFrame>200){
					gerakR = 14; countKick = 0;
				}else if(xFrame<=220 && xFrame>160){
					countKick++;
					if(countKick>=2)gerakR = 2;
					else gerakR = 10;
				}//else if(xFrame<=185 && xFrame>135){
					//gerakR = 14; countKick = 0;
				//}
				else if(xFrame<=160 && xFrame>110){
					countKick++;
					if(countKick>=2) gerakR = kaki;
					else gerakR = 10;
				}else if(xFrame<=110 && xFrame>40){
					gerakR = 13; countKick = 0;
				}else if(xFrame<=40 && xFrame > 0){
					gerakR = 15; countKick = 0;
				}
			} else gerakR = 10;
		}
		else if(xFrame <= 0 && yFrame <= 0) gerakR = 10;
		return gerakR;
}

int GerakTendangDribb(int xFrame, int yFrame, int kaki, int direksi){
	int gerakR = 10;
	if(yFrame < 70 && yFrame > 0){
			if(xFrame>280) gerakR = 16;
			else if(xFrame<=280 && xFrame>210) gerakR = 16;
			else if(xFrame<=210 && xFrame>180) gerakR = 24;
			else if(xFrame<=180 && xFrame>140) gerakR = 30;
			else if(xFrame<=140 && xFrame>110) gerakR = 23;
			else if(xFrame<=100 && xFrame>40) gerakR = 15;
			else if(xFrame<=40 && xFrame > 0) gerakR = 15;
			countKick = 0;
		} else if(yFrame >= 70 && yFrame <= 150){
			if(xFrame>280) gerakR = 16;
			else if(xFrame<=280 && xFrame>200) gerakR = 16;
			else if(xFrame<=200 && xFrame>180) gerakR = 24;
			else if(xFrame<=180 && xFrame>140) gerakR = 20;
			else if(xFrame<=140 && xFrame>100) gerakR = 23;
			else if(xFrame<=100 && xFrame>40) gerakR = 15;
			else if(xFrame<=40 && xFrame > 0) gerakR = 15;
			countKick = 0;
		} else if(yFrame > 150 ){
			if(xFrame>280) gerakR = 16;
			else if(xFrame<=280 && xFrame>230) gerakR = 14;
			else if(xFrame<=230 && xFrame>210) gerakR = 14;
			else if(xFrame<=210 && xFrame>110) gerakR = kaki;
			else if(xFrame<=110 && xFrame>90) gerakR = 13;
			else if(xFrame<=90 && xFrame>40) gerakR = 13;
			else if(xFrame<=40 && xFrame > 0) gerakR = 15;
		} else if(yFrame <= 0 && xFrame <= 0) gerakR = 10;

	//fprintf(stderr,"X = %d | Y = %d | M = %d | C = %d | c = %d\n", xFrame,yFrame,gerakR,arahRobot);

		return gerakR;
}

int GerakHadapBolaDef(int sdtX, int sdtY, int arahrobotskr, int arahhadap, int lihat)
{
	int gerakR = 10;
	if(sdtY<65) {
		if(sdtX>20)//arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX > 20))
		{
			gerakR = 12;
		}
		else if(sdtX<-20)//arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX < -20))
		{
			gerakR = 11;
		}
		else gerakR = 19;
		fprintf(stderr,"-------------- Jalan Di Tempat ---------------------\n");
	}
	/*else if(arahrobotskr == lihat && sdtY > 75)
	{
		if(sdtX < 10 && sdtX > -10 && sdtY<32)gerakR = 10;
		else gerakR = GerakArahBolaLurus(sdtX,sdtY); //if(sdtX>25 || sdtX<-25)
		fprintf(stderr,"-------------- ARAH LURUS ---------------------\n");
	}*/
	/*else if(sdtY>=75)
	{
		if(arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX > 20))
		{
			gerakR = 12;
		}
		else if(arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX < -20))
		{
			gerakR = 11;
		}
		//gerakR = GerakCoverBallSlow(sdtX,sdtY);
		fprintf(stderr,"-------------- Cover Ball Slow ---------------------\n");
	}*/
	else if(sdtY>=65 && sdtY<75) {
		if(arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX > 20))
		{
			gerakR = 14;
		}
		else if(arahrobotskr >= lihat-1 && arahrobotskr <= lihat+1 && (sdtX < -20))
		{
			gerakR = 13;
		}
		else if(sdtX > 40) gerakR = 26;
		else if(sdtX < -40) gerakR = 25;
		else if(sdtX > 20) 	gerakR = 14;
		//else if(sdtX > 10) 	gerakR = 212;
		else if(sdtX < -20) gerakR = 13;
		//else if(sdtX < -10)	gerakR = 211;
		else if(arahhadap>=1 && arahhadap<=10)
		{
			//if(arahrobotskr > lihat && arahrobotskr <= lihat+10) //putar kiri
			if(arahrobotskr<=11 && arahrobotskr>=3)			
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 218;
				//else
				gerakR = 14;
				fprintf(stderr,"1\n");
			}
			else if(arahrobotskr<=18 && arahrobotskr>=11) //putar kanan
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 217;
				//else
				gerakR = 13;
				fprintf(stderr,"2\n");
			}else gerakR=0;
		}
		else if(arahhadap>=11 && arahhadap<=20)
		{
			if(arahrobotskr >= lihat-10 && arahrobotskr < lihat)//putar kanan
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 217;
				//else
				gerakR = 17;
				fprintf(stderr,"3\n");
			}
			else //putar kiri
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 218;
				//else
				gerakR = 18;
				fprintf(stderr,"4\n");
			}
		}
		else gerakR=0;
		fprintf(stderr,"-------------- Cover---------------------\n");
	}
	/*else if(sdtY>42)//35)
	{
		flagserong = false;
		if(arahhadap == 1 && (arahrobotskr == 12 || arahrobotskr == 1 || arahrobotskr == 2))
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else if(arahhadap == 12 && (arahrobotskr == 11 || arahrobotskr == 12 || arahrobotskr == 1))
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1)
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else gerakR = GerakCoverBallSlow(sdtX,sdtY);
		//fprintf(stderr,"------------ Cover Ball Slow ------------------\n");
	}*/
	else gerakR = GerakCoverBallSlow(sdtX,sdtY-30);

	return gerakR;
}

int GerakTendangDribb2(int xFrame, int yFrame, int kaki, int direksi){
	int gerakR = 10;
	if(yFrame < 70 && yFrame > 0){
			if(xFrame>320) gerakR = 16;
			else if(xFrame<=320 && xFrame>280) gerakR = 24;
			else if(xFrame<=280 && xFrame>140) gerakR = 30;
			else if(xFrame<=140 && xFrame>90) gerakR = 23;
			else if(xFrame<=90 && xFrame>40) gerakR = 15;
			else if(xFrame<=40 && xFrame > 0) gerakR = 15;
			countKick = 0;
		} else if(yFrame >= 70 && yFrame <= 150){
			if(xFrame>320) gerakR = 16;
			else if(xFrame<=320 && xFrame>280) gerakR = 24;
			else if(xFrame<=280 && xFrame>140) gerakR = 30;
			else if(xFrame<=140 && xFrame>90) gerakR = 23;
			else if(xFrame<=90 && xFrame>40) gerakR = 15;
			else if(xFrame<=40 && xFrame > 0) gerakR = 15;
			countKick = 0;
		} else if(yFrame > 150 ){
			if(xFrame>320) gerakR = 16;
			else if(xFrame<=320 && xFrame>280) gerakR = 14;
			else if(xFrame<=280 && xFrame>140) gerakR = kaki;
			else if(xFrame<=140 && xFrame>90) gerakR = 13;
			else if(xFrame<=90 && xFrame > 0) gerakR = 15;
		} else if(yFrame <= 0 && xFrame <= 0) gerakR = 10;

	//fprintf(stderr,"X = %d | Y = %d | M = %d | C = %d | c = %d\n", xFrame,yFrame,gerakR,arahRobot);

		return gerakR;
}

int rBall(int sdtX, int sdtY){
	int gerakR;
	if(sdtY>86){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=39) gerakR=12;
			else if(sdtX<39  && sdtX>=31) gerakR=22;
			else if(sdtX<31  && sdtX>=15) gerakR=32;
			else if(sdtX<15  && sdtX>=4)  gerakR=22;
			else if(sdtX<4 	 && sdtX>=-4) gerakR=20;
			else if(sdtX<-4  && sdtX>=-15) gerakR=21;
			else if(sdtX<-15 && sdtX>=-31) gerakR=31;
			else if(sdtX<-31 && sdtX>=-39) gerakR=21;
			else if(sdtX<-39 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
	}
	else if(sdtY>70 && sdtY<=86){ //Menunggu bola
		if(sdtX>=66) gerakR=26;
		else if(sdtX<66  && sdtX>=39) gerakR=26;
		else if(sdtX<39  && sdtX>=31) gerakR=12;
		else if(sdtX<31  && sdtX>=22) gerakR=10;
		else if(sdtX<22  && sdtX>=4)  gerakR=10;
		else if(sdtX<4	 && sdtX>=-4) gerakR=10;
		else if(sdtX<-4  && sdtX>=-22) gerakR=10;
		else if(sdtX<-22 && sdtX>=-31) gerakR=11;
		else if(sdtX<-31 && sdtX>=-39) gerakR=11;
		else if(sdtX<-39 && sdtX>=-66) gerakR=25;
		else if(sdtX<-66) gerakR=25;
	}
	else if(sdtY>34 && sdtY<=70){ // Bola sedang

		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>=44) gerakR=12;
		else if(sdtX<44  && sdtX>=36) gerakR=12;
		else if(sdtX<36  && sdtX>=27) gerakR=16;
		else if(sdtX<27  && sdtX>=9)  gerakR=19;
		else if(sdtX<9	 && sdtX>=-9) gerakR=19;
		else if(sdtX<-9  && sdtX>=-27) gerakR=19;
		else if(sdtX<-27 && sdtX>=-36) gerakR=15;
		else if(sdtX<-36 && sdtX>=-44) gerakR=11;
		else if(sdtX<-44 && sdtX>=-70) gerakR=11;
		else if(sdtX<-70) gerakR=25;
	}
	else if(sdtY<=34){ //Bola dekat
		if(sdtX>=70) gerakR=26;
		else if(sdtX<70  && sdtX>35) gerakR=12;
		else if(sdtX<35  && sdtX>18) gerakR=16;
		else if(sdtX<18  && sdtX>9)  gerakR=14;
		else if(sdtX<=9  && sdtX>=-9) gerakR=19;
		else if(sdtX<-9  && sdtX>=-18) gerakR=13;
		else if(sdtX<-18 && sdtX>=-35) gerakR=15;
		else if(sdtX<-35 && sdtX>=-70) gerakR=11;
		else if(sdtX<-70) gerakR=25;
	}
	else gerakR = 10;
	return gerakR;
}

int GerakCoverLookBall(int sdtX){
	int gerakR;

	if(sdtX>=40) gerakR=26;
	else if(sdtX<40  && sdtX>=15) gerakR=12;
	else if(sdtX<15  && sdtX>=8) gerakR=22;
	else if(sdtX<8   && sdtX>=4)  gerakR=32;
	else if(sdtX<4 	 && sdtX>=-4 && sdtX!=0) gerakR=30;
	else if(sdtX<-4  && sdtX>=-8) gerakR=31;
	else if(sdtX<-8  && sdtX>=-15) gerakR=21;
	else if(sdtX<-15 && sdtX>=-40) gerakR=11;
	else if(sdtX<-40) gerakR=25;
	else gerakR = 10;

	return gerakR;
}

int GerakCoverBall_FW(int sdtX, int sdtY, int hadap){
	int gerakR;
	if(arahRobot>4 && arahRobot<=10){
		if(sdtY>70){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=39) gerakR=12;
			else if(sdtX<39  && sdtX>=31) gerakR=22;
			else if(sdtX<31  && sdtX>=15) gerakR=42;
			else if(sdtX<15  && sdtX>=4)  gerakR=52;
			else if(sdtX<4 	 && sdtX>=-4) gerakR=52;
			else if(sdtX<-4  && sdtX>=-15) gerakR=53;
			else if(sdtX<-15 && sdtX>=-31) gerakR=43;
			else if(sdtX<-31 && sdtX>=-39) gerakR=23;
			else if(sdtX<-39 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>60 && sdtY<=70){
			if(sdtX>=55) gerakR=26;
			else if(sdtX<55  && sdtX>=43) gerakR=12;
			else if(sdtX<43  && sdtX>=31) gerakR=22;
			else if(sdtX<31  && sdtX>=20) gerakR=32;
			else if(sdtX<20  && sdtX>=5)  gerakR=42;
			else if(sdtX<5	 && sdtX>=-5) gerakR=42;
			else if(sdtX<-5  && sdtX>=-20) gerakR=43;
			else if(sdtX<-20 && sdtX>=-30) gerakR=33;
			else if(sdtX<-31 && sdtX>=-43) gerakR=23;
			else if(sdtX<-43 && sdtX>=-55) gerakR=11;
			else if(sdtX<-55) gerakR=25;
		} else if(sdtY>35 && sdtY<=60){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=41) gerakR=12;
			else if(sdtX<41  && sdtX>=33) gerakR=12;
			else if(sdtX<33  && sdtX>=24) gerakR=22;
			else if(sdtX<24  && sdtX>=5)  gerakR=32;
			else if(sdtX<5	 && sdtX>=-5) gerakR=32;
			else if(sdtX<-5  && sdtX>=-24) gerakR=33;
			else if(sdtX<-24 && sdtX>=-33) gerakR=23;
			else if(sdtX<-33 && sdtX>=-41) gerakR=13;
			else if(sdtX<-41 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>25 && sdtY<=35){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=44) gerakR=26;
			else if(sdtX<44  && sdtX>=36) gerakR=12;
			else if(sdtX<36  && sdtX>=27) gerakR=14;
			else if(sdtX<27  && sdtX>=5)  gerakR=24;
			else if(sdtX<5	 && sdtX>=-5) gerakR=22;
			else if(sdtX<-5  && sdtX>=-27) gerakR=23;
			else if(sdtX<-27 && sdtX>=-36) gerakR=13;
			else if(sdtX<-36 && sdtX>=-44) gerakR=11;
			else if(sdtX<-44 && sdtX>=-70) gerakR=25;
			else if(sdtX<-70) gerakR=25;
		} else if(sdtY<=25){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=35) gerakR=12;
			else if(sdtX<35  && sdtX>=18) gerakR=16;
			else if(sdtX<18  && sdtX>=6)  gerakR=14;
			else if(sdtX<6   && sdtX>=-6) gerakR=10;
			else if(sdtX<-6  && sdtX>=-18) gerakR=13;
			else if(sdtX<-18 && sdtX>=-35) gerakR=15;
			else if(sdtX<-35 && sdtX>=-70) gerakR=11;
			else if(sdtX<-70) gerakR=25;
		}
	}else if(arahRobot<19 && arahRobot>10){
		if(sdtY>70){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=39) gerakR=12;
			else if(sdtX<39  && sdtX>=31) gerakR=24;
			else if(sdtX<31  && sdtX>=15) gerakR=44;
			else if(sdtX<15  && sdtX>=4)  gerakR=54;
			else if(sdtX<4 	 && sdtX>=-4) gerakR=51;
			else if(sdtX<-4  && sdtX>=-15) gerakR=51;
			else if(sdtX<-15 && sdtX>=-31) gerakR=41;
			else if(sdtX<-31 && sdtX>=-39) gerakR=21;
			else if(sdtX<-39 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>60 && sdtY<=70){
			if(sdtX>=55) gerakR=26;
			else if(sdtX<55  && sdtX>=43) gerakR=12;
			else if(sdtX<43  && sdtX>=31) gerakR=24;
			else if(sdtX<31  && sdtX>=20) gerakR=34;
			else if(sdtX<20  && sdtX>=5)  gerakR=44;
			else if(sdtX<5	 && sdtX>=-5) gerakR=41;
			else if(sdtX<-5  && sdtX>=-20) gerakR=41;
			else if(sdtX<-20 && sdtX>=-30) gerakR=31;
			else if(sdtX<-31 && sdtX>=-43) gerakR=21;
			else if(sdtX<-43 && sdtX>=-55) gerakR=11;
			else if(sdtX<-55) gerakR=25;
		} else if(sdtY>35 && sdtY<=60){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=41) gerakR=12;
			else if(sdtX<41  && sdtX>=33) gerakR=14;
			else if(sdtX<33  && sdtX>=24) gerakR=24;
			else if(sdtX<24  && sdtX>=5)  gerakR=34;
			else if(sdtX<5	 && sdtX>=-5) gerakR=31;
			else if(sdtX<-5  && sdtX>=-24) gerakR=31;
			else if(sdtX<-24 && sdtX>=-33) gerakR=21;
			else if(sdtX<-33 && sdtX>=-41) gerakR=11;
			else if(sdtX<-41 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>25 && sdtY<=35){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=44) gerakR=26;
			else if(sdtX<44  && sdtX>=36) gerakR=12;
			else if(sdtX<36  && sdtX>=27) gerakR=14;
			else if(sdtX<27  && sdtX>=5)  gerakR=24;
			else if(sdtX<5	 && sdtX>=-5) gerakR=23;
			else if(sdtX<-5  && sdtX>=-27) gerakR=23;
			else if(sdtX<-27 && sdtX>=-36) gerakR=13;
			else if(sdtX<-36 && sdtX>=-44) gerakR=11;
			else if(sdtX<-44 && sdtX>=-70) gerakR=25;
			else if(sdtX<-70) gerakR=25;
		} else if(sdtY<=25){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=35) gerakR=12;
			else if(sdtX<35  && sdtX>=18) gerakR=16;
			else if(sdtX<18  && sdtX>=6)  gerakR=14;
			else if(sdtX<6   && sdtX>=-6) gerakR=10;
			else if(sdtX<-6  && sdtX>=-18) gerakR=13;
			else if(sdtX<-18 && sdtX>=-35) gerakR=15;
			else if(sdtX<-35 && sdtX>=-70) gerakR=11;
			else if(sdtX<-70) gerakR=25;
		}
	}else{
		if(sdtY>70){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=39) gerakR=12;
			else if(sdtX<39  && sdtX>=31) gerakR=22;
			else if(sdtX<31  && sdtX>=15) gerakR=42;
			else if(sdtX<15  && sdtX>=4)  gerakR=52;
			else if(sdtX<4 	 && sdtX>=-4) gerakR=50;
			else if(sdtX<-4  && sdtX>=-15) gerakR=51;
			else if(sdtX<-15 && sdtX>=-31) gerakR=41;
			else if(sdtX<-31 && sdtX>=-39) gerakR=21;
			else if(sdtX<-39 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>60 && sdtY<=70){
			if(sdtX>=55) gerakR=26;
			else if(sdtX<55  && sdtX>=43) gerakR=12;
			else if(sdtX<43  && sdtX>=31) gerakR=22;
			else if(sdtX<31  && sdtX>=20) gerakR=32;
			else if(sdtX<20  && sdtX>=5)  gerakR=42;
			else if(sdtX<5	 && sdtX>=-5) gerakR=40;
			else if(sdtX<-5  && sdtX>=-20) gerakR=41;
			else if(sdtX<-20 && sdtX>=-30) gerakR=31;
			else if(sdtX<-31 && sdtX>=-43) gerakR=21;
			else if(sdtX<-43 && sdtX>=-55) gerakR=11;
			else if(sdtX<-55) gerakR=25;
		} else if(sdtY>35 && sdtY<=60){
			if(sdtX>=66) gerakR=26;
			else if(sdtX<66  && sdtX>=41) gerakR=12;
			else if(sdtX<41  && sdtX>=33) gerakR=12;
			else if(sdtX<33  && sdtX>=24) gerakR=22;
			else if(sdtX<24  && sdtX>=5)  gerakR=32;
			else if(sdtX<5	 && sdtX>=-5) gerakR=30;
			else if(sdtX<-5  && sdtX>=-24) gerakR=31;
			else if(sdtX<-24 && sdtX>=-33) gerakR=21;
			else if(sdtX<-33 && sdtX>=-41) gerakR=11;
			else if(sdtX<-41 && sdtX>=-66) gerakR=11;
			else if(sdtX<-66) gerakR=25;
		} else if(sdtY>25 && sdtY<=35){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=44) gerakR=26;
			else if(sdtX<44  && sdtX>=36) gerakR=12;
			else if(sdtX<36  && sdtX>=27) gerakR=14;
			else if(sdtX<27  && sdtX>=5)  gerakR=24;
			else if(sdtX<5	 && sdtX>=-5) gerakR=20;
			else if(sdtX<-5  && sdtX>=-27) gerakR=23;
			else if(sdtX<-27 && sdtX>=-36) gerakR=13;
			else if(sdtX<-36 && sdtX>=-44) gerakR=11;
			else if(sdtX<-44 && sdtX>=-70) gerakR=25;
			else if(sdtX<-70) gerakR=25;
		} else if(sdtY<=25){
			if(sdtX>=70) gerakR=26;
			else if(sdtX<70  && sdtX>=35) gerakR=12;
			else if(sdtX<35  && sdtX>=18) gerakR=16;
			else if(sdtX<18  && sdtX>=6)  gerakR=14;
			else if(sdtX<6   && sdtX>=-6) gerakR=10;
			else if(sdtX<-6  && sdtX>=-18) gerakR=13;
			else if(sdtX<-18 && sdtX>=-35) gerakR=15;
			else if(sdtX<-35 && sdtX>=-70) gerakR=11;
			else if(sdtX<-70) gerakR=25;
		}
	}

	return gerakR;
}

int GerakNggiring(int sdtX, int sdtY)
{
	int gerakR;
	if(sdtY>60){
		if(sdtX>=35) gerakR=16;
		else if(sdtX<35  && sdtX>=25)  gerakR=34;
		else if(sdtX<25  && sdtX>=4)   gerakR=40;
		else if(sdtX<4   && sdtX>=-4)  gerakR=40;
		else if(sdtX<-4  && sdtX>=-25) gerakR=40;
		else if(sdtX<-25 && sdtX>=-35) gerakR=33;
		else if(sdtX<-35) gerakR=15;
	}
	else if(sdtY>34 && sdtY<=60){
		if(sdtX>=69) gerakR=19;
		else if(sdtX<69  && sdtX>=35)  gerakR=16;
		else if(sdtX<35  && sdtX>=25)   gerakR=24;
		else if(sdtX<25  && sdtX>=5)   gerakR=34;
		else if(sdtX<5   && sdtX>=-5)  gerakR=30;
		else if(sdtX<-5  && sdtX>=-25) gerakR=33;
		else if(sdtX<-25 && sdtX>=-35) gerakR=23;
		else if(sdtX<-35 && sdtX>=-69) gerakR=15;
		else if(sdtX<-69) gerakR=19;
	}
	else if(sdtY<=34){
		if(sdtX>=45) gerakR=19;
		else if(sdtX<45  && sdtX>=25)  gerakR=16;
		else if(sdtX<25  && sdtX>= 6)   gerakR=34;
		else if(sdtX< 6  && sdtX>=-6)  gerakR=30;
		else if(sdtX<-6  && sdtX>=-25) gerakR=33;
		else if(sdtX<-25 && sdtX>-45) gerakR=15;
		else if(sdtX<=-45) gerakR=19;
	} else gerakR = 10;
	return gerakR;
}

int GerakJalanKompas(int arahRobots, int arahR, int FoRev, int SudutGWX){
	int invarahR, GerakR;
	//FoRev = untuk Flag
	if(arahR <= 6){
		invarahR = arahR + 6;
	} else if(arahR > 6){
		invarahR = arahR - 6;
	}

	if(arahRobots == arahR ){
		if(FoRev == 1){GerakR = 20;}
		else if(FoRev == 2){GerakR = 19;}
		else if(FoRev == 3){
			if(SudutGWX >= 10 && SudutGWX <= 15){
				GerakR = 0;
			} else if(SudutGWX < 10){
				GerakR = 13;
			} else if(SudutGWX > 15){
				GerakR = 14;
			}
		} else if(FoRev == 4){
			GerakR = 20;
		} else if(FoRev == 5){
			GerakR = 10;
		} else if(FoRev == 6){
			GerakR = 0;
		} else if(FoRev == 7){
			GerakR = 0;
		}
	} else if(arahRobots != arahR && arahR <= 6){
		if(arahRobots < arahR || arahRobots >= invarahR){
			if(FoRev == 1){GerakR = 22;}
			else if(FoRev == 7){GerakR = 0;}
			else {GerakR = 212;}
		} else if(arahRobots > arahR && arahRobots < invarahR){
			if(FoRev == 1){GerakR = 21;}
			else if(FoRev == 7){GerakR = 0;}
			else {GerakR = 211;}
		}
	} else if(arahRobots != arahR && arahR > 6){
		if(arahRobots < arahR && arahRobots >= invarahR){
			if(FoRev == 1){GerakR = 22;}
			else if(FoRev == 7){GerakR = 0;}
			else {GerakR = 212;}
		} else if(arahRobots > arahR || arahRobots < invarahR){
			if(FoRev == 1){GerakR = 21;}
			else if(FoRev == 7){GerakR = 0;}
			else {GerakR = 211;}
		}
	} else GerakR = 10;
	return GerakR;
}

int GerakCoverBlokBall(int xGrk, int yGrk){
	int gerakR;
	if(yGrk>2150){
		if(xGrk<=1297) gerakR=26;
		else if(xGrk>1297 && xGrk<=1600) gerakR=12;
		else if(xGrk>1600 && xGrk<=1700) gerakR=32;
		else if(xGrk>1700 && xGrk<=1800) gerakR=42;
		else if(xGrk>1800 && xGrk<=2000) gerakR=42;
		else if(xGrk>2000 && xGrk<=2100) gerakR=40;
		else if(xGrk>2100 && xGrk<=2300) gerakR=41;
		else if(xGrk>2300 && xGrk<=2400) gerakR=41;
		else if(xGrk>2400 && xGrk<=2500) gerakR=31;
		else if(xGrk>2500 && xGrk<=2797) gerakR=11;
		else if(xGrk>2797) gerakR=25;
	}
	else if(yGrk>2080 && yGrk<=2150){
		if(xGrk<=1300) gerakR=26;
		else if(xGrk>1300 && xGrk<=1700) gerakR=22;
		else if(xGrk>1700 && xGrk<=2000) gerakR=32;
		else if(xGrk>2000 && xGrk<=2100) gerakR=30;
		else if(xGrk>2100 && xGrk<=2400) gerakR=31;
		else if(xGrk>2400 && xGrk<=2800) gerakR=21;
		else if(xGrk>2800) gerakR=25;
	}
	else if(yGrk>1990 && yGrk<=2080){
		if(xGrk<=1400) gerakR=12;
		else if(xGrk>1400 && xGrk<1800) gerakR=22;
		else if(xGrk>=1800 && xGrk<=2300) gerakR=20;
		else if(xGrk>2300 && xGrk<=2700) gerakR=21;
		else if(xGrk>2700) gerakR=11;
	}
	else if(yGrk>1900 && yGrk<=1990){
		if(xGrk<=1600) gerakR=12;
		else if(xGrk>1600 && xGrk<1900) gerakR=22;
		else if(xGrk>=1900 && xGrk<=2200) gerakR=10;
		else if(xGrk>2200 && xGrk<=2500) gerakR=21;
		else if(xGrk>2500) gerakR=11;
	}else if(yGrk<=1900){
		if(xGrk<=1600) gerakR=12;
		else if(xGrk>1600 && xGrk<1900) gerakR=204;
		else if(xGrk>=1900 && xGrk<=2200) gerakR=19;
		else if(xGrk>2200 && xGrk<=2500) gerakR=203;
		else if(xGrk>2500) gerakR=11;
	}
	return gerakR;
}

int GerakParabola(int xFrame, int yFrame){
	int gerakR;
	if(yFrame > 140){
		if(xFrame>280) gerakR = 212;
		else if(xFrame<=280 && xFrame>210) gerakR = 212;
		else if(xFrame<=210 && xFrame>180) gerakR = 212;
		else if(xFrame<=180 && xFrame>140) gerakR = 23;
		else if(xFrame<=140 && xFrame>110) gerakR = 211;
		else if(xFrame<=100 && xFrame>40) gerakR = 211;
		else if(xFrame<=40 && xFrame > 0) gerakR = 211;
	} else if(yFrame <= 140 && yFrame >= 100){
		if(xFrame>280) gerakR = 212;
		else if(xFrame<=280 && xFrame>210) gerakR = 212;
		else if(xFrame<=210 && xFrame>180) gerakR = 212;
		else if(xFrame<=180 && xFrame>140) gerakR = 20;
		else if(xFrame<=140 && xFrame>110) gerakR = 211;
		else if(xFrame<=100 && xFrame>40) gerakR = 211;
		else if(xFrame<=40 && xFrame > 0) gerakR = 211;
	} else if(yFrame < 100 && yFrame > 0){
		if(xFrame>280) gerakR = 212;
		else if(xFrame<=280 && xFrame>210) gerakR = 212;
		else if(xFrame<=210 && xFrame>180) gerakR = 212;
		else if(xFrame<=180 && xFrame>140) gerakR = 24;
		else if(xFrame<=140 && xFrame>110) gerakR = 211;
		else if(xFrame<=100 && xFrame>40) gerakR = 211;
		else if(xFrame<=40 && xFrame > 0) gerakR = 211;
	}
	return gerakR;
}

int GerakSync(int arahRobots, int arahR){
	int SudutR, invarahR;

	if(arahR <= 6){
		invarahR = arahR + 6;
	} else if(arahR > 6){
		invarahR = arahR - 6;
	}
	if(arahRobots == arahR ){
		SudutR = 0;
	} else if(arahRobots != arahR && arahR <= 6){
		if(arahRobots < arahR || arahRobots >= invarahR){
			if(arahRobots < arahR){
				SudutR = arahRobots - arahR;
			} else if(arahRobots >= invarahR){
				SudutR = arahRobots - (invarahR + 6);
			}
		} else if(arahRobots > arahR && arahRobots < invarahR){
			SudutR = arahRobots - arahR;
		}
	} else if(arahRobots != arahR && arahR > 6){
		if(arahRobots < arahR && arahRobots >= invarahR){
			SudutR = arahRobots - arahR;
		} else if(arahRobots > arahR || arahRobots < invarahR){
			if(arahRobots > arahR){
				SudutR = arahRobots - arahR;
			} else if(arahRobots < invarahR){
				SudutR = (arahRobots+6) - invarahR;
			}
		}
	}

	SudutR *= -30;

	return SudutR;
}

int tdangT2=0, stepTdg2=0;
int GerakOfset(int xFrame, int yFrame, int arahRobots, int arahR){
	int gerakR, invarahR;

	if(arahR <= 6){
		invarahR = arahR + 6;
	} else if(arahR > 6){
		invarahR = arahR - 6;
	}
	if(yFrame < 70 && yFrame > 0){
		if(stepTdg2!=1){tdangT2=0;}

		if(xFrame>280) gerakR = 16;
		else if(xFrame<=280 && xFrame>210) gerakR = 14;
		else if(xFrame<=210 && xFrame>180) gerakR = 34;
		else if(xFrame<=180 && xFrame>140) gerakR = 30;
		else if(xFrame<=140 && xFrame>110) gerakR = 33;
		else if(xFrame<=100 && xFrame>40) gerakR = 13;
		else if(xFrame<=40 && xFrame > 0) gerakR = 15;


		countKick = 0;
	} else if(yFrame >= 70 && yFrame <= 110){
		if(xFrame>280) gerakR = 16;
		else if(xFrame<=280 && xFrame>210) gerakR = 14;
		else if(xFrame<=210 && xFrame>180) gerakR = 14;
		else if(xFrame<=180 && xFrame>140) gerakR = 20;
		else if(xFrame<=140 && xFrame>110) gerakR = 13;
		else if(xFrame<=100 && xFrame>40) gerakR = 13;
		else if(xFrame<=40 && xFrame > 0) gerakR = 16;
		countKick = 0;
	} else if(yFrame > 110 && yFrame < 165 && countKick == 0){
		if(xFrame>280) {gerakR = 16; countKick = 0;}
		else if(xFrame<=280 && xFrame>210) {gerakR = 14; countKick = 0;}
		else if(xFrame<=210 && xFrame>180) {gerakR = 14; countKick = 0;}
		else if(xFrame<=180 && xFrame>140) {
			if(arahRobots == arahR){
				gerakR = 10;
			} else if(arahRobots != arahR && arahR <= 6){
				if(arahRobots < arahR || arahRobots >= invarahR){
					if(arahRobots < arahR){
						countKick = arahRobots - arahR;
					} else if(arahRobots >= invarahR){
						countKick = arahRobots - (invarahR + 6);
					}
				} else if(arahRobots > arahR && arahRobots < invarahR){
					countKick = arahRobots - arahR;
				}
			} else if(arahRobots != arahR && arahR > 6){
				if(arahRobots < arahR && arahRobots >= invarahR){
					countKick = arahRobots - arahR;
				} else if(arahRobots > arahR || arahRobots < invarahR){
					if(arahRobots > arahR){
						countKick = arahRobots - arahR;
					} else if(arahRobots < invarahR){
						countKick = (arahRobots+6) - invarahR;
					}
				}
			}

			hadapGawang = countKick*25;
			if(hadapGawang <= 0)hadapGawang *= -1 ;

		}
		else if(xFrame<=140 && xFrame>110) { gerakR = 13; countKick = 0;}
		else if(xFrame<=100 && xFrame>40) { gerakR = 13; countKick = 0;}
		else if(xFrame<=40 && xFrame > 0) { gerakR = 15; countKick = 0;}
	} else if(yFrame >= 165){
		gerakR = 19;
	}  else if(hadapGawang > 0){
		if(countKick < 0){
			hadapGawang--;
			gerakR = 217;
		} else if(countKick > 0){
			hadapGawang--;
			gerakR = 218;
		}
		if(hadapGawang<=0){
			gerakR = 10;
			hadapGawang=0;
			countKick = 0;
		}
	} else gerakR = 10;
	return gerakR;
}

int GerakOverShoot(int xFrame, int yFrame, int sdtGeser, int sdthdp ){
	int gerakR, sdtR;

	if(sdtGeser >= 0){
		if(sdthdp >= sdtGeser){
			sdtR = sdthdp + sdtGeser;
			if(sdtR > 180){
				sdtR = sdtR - 360;
			}
		} else if(sdthdp < sdtGeser){
			sdtR = sdthdp - sdtGeser;
			if(sdtR > 180){
				sdtR = sdtR - 360;
			}
		}
	} else if(sdtGeser < 0){
		if(sdthdp >= sdtGeser){
			sdtR = sdthdp - sdtGeser;
			if(sdtR > 180){
				sdtR = sdtR - 360;
			}
		} else if(sdthdp < sdtGeser){
			sdtR = sdthdp + sdtGeser;
			if(sdtR > 180){
				sdtR = sdtR - 360;
			}
		}
	}

	if(yFrame < 70 && yFrame > 0){
		if(xFrame>280) gerakR = 16;
		else if(xFrame<=280 && xFrame>210) gerakR = 16;
		else if(xFrame<=210 && xFrame>180) gerakR = 14;
		else if(xFrame<=180 && xFrame>140) gerakR = 30;
		else if(xFrame<=140 && xFrame>110) gerakR = 13;
		else if(xFrame<=100 && xFrame>40) gerakR = 15;
		else if(xFrame<=40 && xFrame > 0) gerakR = 15;
	} else if(yFrame >= 70 && yFrame <= 150){
		if(xFrame>280) gerakR = 16;
		else if(xFrame<=280 && xFrame>200) gerakR = 16;
		else if(xFrame<=200 && xFrame>180) gerakR = 14;
		else if(xFrame<=180 && xFrame>140) gerakR = 220;
		else if(xFrame<=140 && xFrame>100) gerakR = 13;
		else if(xFrame<=100 && xFrame>40) gerakR = 15;
		else if(xFrame<=40 && xFrame > 0) gerakR = 15;
	} else if(yFrame > 150 ){
		if(xFrame>280) gerakR = 16;
		else if(xFrame<=280 && xFrame>230) gerakR = 14;
		else if(xFrame<=230 && xFrame>190) gerakR = 14;
		else if(xFrame<=190 && xFrame>130){
			if(sdtR <= 10 && sdtR >= -10){
				gerakR = 10;
			} else if(sdtR > 10){
				gerakR = 218;
			} else if(sdtR < -10){
				gerakR = 217;
			}
		}
		else if(xFrame<=130 && xFrame>90) gerakR = 13;
		else if(xFrame<=90 && xFrame>40) gerakR = 13;
		else if(xFrame<=40 && xFrame > 0) gerakR = 15;
	} else if(yFrame <= 0 && xFrame <= 0) gerakR = 10;
	//fprintf(stderr," (%d)(%d)(%d)(%d)\n",gerakR,sdtR,sdtGeser,sdthdp);

	return gerakR;
}

int GerakOfsetSdt(int xFrame, int yFrame, int sdtGeser, int sdthdp){
	int gerakR, sdtR;

	if(sdtGeser >= 0){
		sdtR = sdthdp - sdtGeser;
		if(sdtR < -180){
			sdtR = sdtR + 360;
		}
	} else if(sdtGeser < 0){
		sdtR = sdthdp + sdtGeser;
		if(sdtR > 180){
			sdtR = sdtR - 360;
		}
	}

	if(yFrame < 70 && yFrame > 0){
		if(xFrame>280) gerakR = 14;
		else if(xFrame<=280 && xFrame>210) gerakR = 14;
		else if(xFrame<=210 && xFrame>180) gerakR = 14;
		else if(xFrame<=180 && xFrame>140) gerakR = 20;
		else if(xFrame<=140 && xFrame>110) gerakR = 13;
		else if(xFrame<=100 && xFrame>40) gerakR = 13;
		else if(xFrame<=40 && xFrame > 0) gerakR = 13;
		 countKick = 0;
	} else if(yFrame >= 70 && yFrame <= 150){
		if(xFrame>280) gerakR = 14;
		else if(xFrame<=280 && xFrame>200) gerakR = 14;
		else if(xFrame<=200 && xFrame>180) gerakR = 14;
		else if(xFrame<=180 && xFrame>140) gerakR = 220;
		else if(xFrame<=140 && xFrame>100) gerakR = 13;
		else if(xFrame<=100 && xFrame>40) gerakR = 13;
		else if(xFrame<=40 && xFrame > 0) gerakR = 13;
		 countKick = 0;
	} else if(yFrame > 150 && yFrame <= 170 && countKick == 0){
		if(xFrame>280) { gerakR = 14; countKick = 0; }
		else if(xFrame<=280 && xFrame>230){ gerakR = 14; }
		else if(xFrame<=230 && xFrame>190){ gerakR = 14; }
		else if(xFrame<=190 && xFrame>130){
			if(sdtR <= 15 && sdtR >= -15){
				gerakR = 10;
				countKick = 0;
			} else if(sdtR > 15){
				countKick = sdtR;
			} else if(sdtR < -15){
				countKick = sdtR;
			}
			hadapGawang = countKick;
			if(hadapGawang <= 0)hadapGawang *= -1 ;
		}
		else if(xFrame<=130 && xFrame>90) { gerakR = 13; }
		else if(xFrame<=90 && xFrame>40) { gerakR = 13; }
		else if(xFrame<=40 && xFrame > 0) { gerakR = 13; }
	} else if(yFrame <= 0 && xFrame <= 0) {
		gerakR = 10; countKick = 0;
	} else if(yFrame > 180){
		gerakR = 19;
	} else if(hadapGawang > 0){
		if(countKick > 0){
			hadapGawang--;
			gerakR = 217;
		} else if(countKick < 0){
			hadapGawang--;
			gerakR = 218;
		}
		if(hadapGawang<=0){
			gerakR = 10;
			hadapGawang=0;
			countKick = 0;
		}
	} else gerakR = 10;

	fprintf(stderr," (%d)(%d)(%d)(%d)(%d)(%d)\n",gerakR,countKick,hadapGawang,sdtR,sdtGeser,sdthdp);
	return gerakR;
}

int GerakPutarBola(int xFrame, int sdtY, int geser){
		int gerakR;

		if(sdtY > 50 ){
				if(xFrame>260+geser) gerakR = 16;
				else if(xFrame<=260+geser && xFrame>180+geser) gerakR = 34;
				else if(xFrame<=180+geser && xFrame>140+geser) gerakR = 30;
				else if(xFrame<=140+geser && xFrame> 60+geser) gerakR = 33;
				else if(xFrame<= 60+geser && xFrame > 0) gerakR = 15;
				else gerakR = 10;
		}else if(sdtY <= 50 && sdtY > 27 ){
				if(xFrame>260+geser) gerakR = 14;
				else if(xFrame<=260+geser && xFrame>190+geser) gerakR = 24;
				else if(xFrame<=190+geser && xFrame>130+geser) gerakR = 20;
				else if(xFrame<=130+geser && xFrame> 60+geser) gerakR = 23;
				else if(xFrame<= 60+geser && xFrame > 0) gerakR = 13;
				else gerakR = 10;
		}else if(sdtY <= 27){
				if(xFrame>260+geser) gerakR = 16;
				else if(xFrame<=260+geser && xFrame>200+geser) gerakR = 14;
				else if(xFrame<=200+geser && xFrame>120+geser) gerakR = 10;
				else if(xFrame<=120+geser && xFrame> 60+geser)  gerakR = 13;
				else if(xFrame<= 60+geser && xFrame > 0) gerakR = 15;
				else gerakR = 10;
		}else gerakR = 10;
		return gerakR;
}

int GerakLurusBola(int sdtX, int sdtY)
{
	int gerakR = 10;
	if(sdtX > 20) gerakR = 12;
	else if(sdtX < -20) gerakR = 11;
	else gerakR = 10;
	return gerakR;
}

int GerakLurusArahCMPS(int arahrobotskr, int arahhadap)
{
	int gerakR = 10;
	if(arahrobotskr == arahhadap) gerakR = 30;
	else if(arahrobotskr == rotasiarah(arahhadap+1)) gerakR = 31;
	else if(arahrobotskr == rotasiarah(arahhadap+2)) gerakR = 31;
	else if(arahrobotskr == rotasiarah(arahhadap+3)) gerakR = 11;
	else if(arahrobotskr == rotasiarah(arahhadap+4)) gerakR = 25;
	else if(arahrobotskr == rotasiarah(arahhadap+5)) gerakR = 25;
	else if(arahrobotskr == rotasiarah(arahhadap-1)) gerakR = 32;
	else if(arahrobotskr == rotasiarah(arahhadap-2)) gerakR = 32;
	else if(arahrobotskr == rotasiarah(arahhadap-3)) gerakR = 12;
	else if(arahrobotskr == rotasiarah(arahhadap-4)) gerakR = 26;
	else if(arahrobotskr == rotasiarah(arahhadap-5)) gerakR = 26;
	else if(arahrobotskr == rotasiarah(arahhadap-6)) gerakR = 26;
	return gerakR;
}

int GerakLurusArah(int sdtX, int sdtY, int arahrobotskr, int arahhadap)
{
	int gerakR = 10;
	if (sdtY>80)
	{
		gerakR = GerakCoverBall(sdtX,sdtY);
	}
	else
	{
		if(arahhadap>=1 && arahhadap<=6)
		{
			if(arahrobotskr > arahhadap && arahrobotskr <= arahhadap+6) //putar kiri
			{
				if(arahrobotskr - arahhadap < 3 && arahrobotskr - arahhadap > -3) gerakR = 21;
				else gerakR = 11;
			}
			else //putar kanan
			{
				if(arahrobotskr - arahhadap < 3 && arahrobotskr - arahhadap > -3) gerakR = 22;
				else gerakR = 12;
			}
		}
		else if(arahhadap>=7 && arahhadap<=12)
		{
			if(arahrobotskr >= arahhadap-6 && arahrobotskr < arahhadap)//putar kanan
			{
				if(arahrobotskr - arahhadap < 3 && arahrobotskr - arahhadap > -3) gerakR = 22;
				else gerakR = 12;
			}
			else //putar kiri
			{
				if(arahrobotskr - arahhadap < 3 && arahrobotskr - arahhadap > -3) gerakR = 21;
				gerakR = 11;
			}
		}
		else if(sdtX>30) gerakR = 24;
		else if(sdtX<-30) gerakR = 23;
		//else gerakR = 30;
		else if(sdtX>30) gerakR = 30;
		else if(sdtX<-30) gerakR = 30;
		else if(sdtX>0) gerakR =23;
		else if(sdtX<0) gerakR =24;
		else gerakR = 20;
	}
	return gerakR;
}

// Naufal
/*
int GerakHadapBola(int sdtX, int sdtY, int arahrobotskr, int arahhadap)
{
	int gerakR = 10;
	if(sdtY>60)
	{
		gerakR = GerakCoverBall(sdtX,sdtY);
	}
	else if(arahrobotskr == arahhadap)
	{
		if(sdtX < 10 && sdtX > -10 && sdtY<32)gerakR = 10;
		else gerakR = GerakArahBolaLurus(sdtX,sdtY); //if(sdtX>25 || sdtX<-25)
		//else if(sdtX>10) gerakR = 14;
		//else if(sdtX<-10) gerakR = 13;
	}
	else if(sdtY>35)
	{
		flagserong = false;
		if(arahhadap == 1 && (arahrobotskr == 12 || arahrobotskr == 1 || arahrobotskr == 2))
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else if(arahhadap == 12 && (arahrobotskr == 11 || arahrobotskr == 12 || arahrobotskr == 1))
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1)
		{
			gerakR = GerakCoverBallSlow(sdtX,sdtY);
		}
		else gerakR = GerakCoverBallSlow(sdtX,sdtY);
	}
	else if(sdtY>0)
	{
//		counterserong++;
//		if(counterserong>100 || sdtX<15 && sdtX >-15)
//		{
//			flagserong = false;
//			counterserong=0;
//		}
//		if(flagserong == true && (sdtX>15 || sdtX <-15))
//		{
//			gerakR=20;
//		}

		if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX > 20))
		{
		 	gerakR = 14;
		}
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX < -20))
		{
			gerakR = 13;
		}
		else if(sdtX > 38) gerakR = 26;
		else if(sdtX < -38) gerakR = 25;
		else if(sdtX > 15) 	gerakR = 14;
		//else if(sdtX > 10) 	gerakR = 212;
		else if(sdtX < -15) gerakR = 13;
		//else if(sdtX < -10)	gerakR = 211;
		else if(arahhadap>=1 && arahhadap<=6)
		{
			if(arahrobotskr > arahhadap && arahrobotskr <= arahhadap+6) //putar kiri
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 218;
				//else
				gerakR = 18;
			}
			else //putar kanan
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 217;
				//else
				gerakR = 17;
			}
		}
		else if(arahhadap>=7 && arahhadap<=12)
		{
			if(arahrobotskr >= arahhadap-6 && arahrobotskr < arahhadap)//putar kanan
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 217;
				//else
				gerakR = 17;
			}
			else //putar kiri
			{
				//if(arahrobotskr - arahhadap < 2 && arahrobotskr - arahhadap > -2) gerakR = 218;
				//else
				gerakR = 18;
			}
		}
		else gerakR=20;
	}
	else gerakR = 10;

	return gerakR=20;
}
*/

int GerakHadapBolaBeta(int sdtX, int sdtY, int arahrobotskr, int arahhadap)
{
	int gerakR = 10;
	if(sdtY>45) //Bola Jauh
	{
		gerakR = GerakCoverBall(sdtX,sdtY);
		//gerakR=GerakCoverBall_FW(sdtX,sdtY,1);
		fprintf(stderr, "GCB ");
	}
	else if(arahrobotskr == arahhadap)
	{
		if(sdtX < 10 && sdtX > -10 && sdtY<=7){
			gerakR = 10;
		}
		else {
			gerakR = GerakArahBolaLurus(sdtX,sdtY); 
			fprintf(stderr, "GABL ");
		}
	}else if(sdtY>30 && sdtY<=45)
	{
		gerakR = GerakCoverBallSlow(sdtX,sdtY);
		fprintf(stderr, "GCBS ");
	}
	else if(sdtY>0)
	{
		fprintf(stderr, "GOTHER ");
		if(sdtX > 45) gerakR = 19;
		else if(sdtX < -45) gerakR = 19;
		else if(sdtX>25)gerakR=26;//26
		else if(sdtX<-25)gerakR=25;//25
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX > 30))
		{
		 	gerakR = 14;
		}
		else if(arahrobotskr >= arahhadap-1 && arahrobotskr <= arahhadap+1 && (sdtX < -30))
		{
			gerakR = 13;
		}
		else if(sdtX > 15) 	gerakR = 14;//14
		else if(sdtX < -15) gerakR = 13;//13
		else if(arahhadap>=1 && arahhadap<=10 && arahhadap!=arahrobotskr)
		{
			if(arahrobotskr > arahhadap && arahrobotskr <= arahhadap+10) //putar kiri
			{
				gerakR = 18;
			}
			else //putar kanan
			{
				gerakR = 17;
			}
		}
		else if(arahhadap>=11 && arahhadap<=20 && arahhadap!=arahrobotskr)
		{
			if(arahrobotskr >= arahhadap-10 && arahrobotskr < arahhadap)//putar kanan
			{
				gerakR = 17;
			}
			else //putar kiri
			{
				gerakR = 18;
			}
		}
		else gerakR=10;
	}
	else gerakR=10;
	
	return gerakR;
}

int GerakTendangdekatfast(int sdtX, int sdtY)
{
	int gerakR;
	if(sdtY>35) gerakR = GerakCoverBall(sdtX,sdtY+10);
	else
	{
		if(sdtX > 40) gerakR = 26;
		else if(sdtX < -40) gerakR = 25;
		else if(sdtX > 20) 	gerakR = 14;
		else if(sdtX < -20) gerakR = 13;
		else if(sdtY>15) gerakR = 20;
		else
		{
			if(sdtX > 0) gerakR = 2;
			else gerakR = 1;
		}
	}
	return gerakR;
}

int GerakTendangjauhfast(int sdtX, int sdtY)
{
	static int cnttndg;
	int gerakR = 10;
	if(sdtY>50) gerakR = GerakCoverBallSlow(sdtX,sdtY);
	else if (arahRobot==arahLihat && sdtY>50) gerakR = GerakArahBolaLurus(sdtX,sdtY);
	else
	{
		if(sdtY>35) //23
		{
			if(sdtX > 40) gerakR = 26;
			else if(sdtX <-40) gerakR = 25;
			else if(sdtX > 15) 	gerakR = 14;
			else if(sdtX <-15) gerakR = 13;
			else gerakR = 220;
		}
		else
		{
		if(sdtX > 30) gerakR = 19;
		else if(sdtX <-30) gerakR = 19;
		else if(sdtX > 18) 	gerakR = 14;
		else if(sdtX <-18) gerakR = 13;
		else if(sdtX > 6) gerakR = 6;
		else if(sdtX <-6) gerakR = 5;
		else if(sdtX > 0) gerakR = 14;
		else if(sdtX < 0) gerakR = 13;
		else gerakR = 6;
		}
	}
	if(gerakR==5 || gerakR==6) cnttndg++;
	else cnttndg=0;

	if(cnttndg>200){
		cnttndg=201;
	}
	else if(cnttndg<=200 && (gerakR==5 || gerakR==6)){
		gerakR=10;
	}
	else if(cnttndg<0){
		cnttndg=0;
	}
	return gerakR;
}

int GerakEksekusiDribble(int dataX, int dataY)
{
	int gerakR;
	if(dataX == 0 && dataY == 0) gerakR = 10;
	else if(dataY<120)
	{
		if(dataX<53) 		 gerakR = 23;
		else if(dataX < 106) gerakR = 23;
		else if(dataX < 159) gerakR = 23;
		else if(dataX < 212) gerakR = 23;
		else if(dataX < 265) gerakR = 30;
		else if(dataX < 318) gerakR = 24;
		else if(dataX < 371) gerakR = 24;
		else if(dataX < 424) gerakR = 24;
	}
	else if(dataY<240)
	{
		if(dataX<53) 		 gerakR = 19;
		else if(dataX < 106) gerakR = 13;
		else if(dataX < 159) gerakR = 13;
		else if(dataX < 212) gerakR = 20;
		else if(dataX < 265) gerakR = 20;
		else if(dataX < 318) gerakR = 14;
		else if(dataX < 371) gerakR = 14;
		else if(dataX < 424) gerakR = 19;
	}
	return gerakR;
}

int GerakAvoidFast2(int dataX, int dataY, int kakix)
{	
	int gerakR,kka,kki,kt;
	if(kakix==1){kka=7;kki=14;kt=14;} //bola ke kiri
	else{kka=13;kki=8;kt=13;}		  //bola ke kanan

	if(dataX == 0 && dataY == 0) gerakR = 10;
	else if(dataY<250)
	{
		if(dataX<64) gerakR = 23;
		else if(dataX < 128) gerakR = 33;
		else if(dataX < 192) gerakR = 33;
		else if(dataX < 256) gerakR = 33;
		else if(dataX < 320) gerakR = 30;
		else if(dataX < 384) gerakR = 30;
		else if(dataX < 448) gerakR = 34;
		else if(dataX < 512) gerakR = 34;
		else if(dataX < 576) gerakR = 34;
		else if(dataX < 640) gerakR = 34;
	}
	else if(dataY<280)
	{
		if(dataX<64) gerakR = 23;
		else if(dataX < 128) gerakR = 13;
		else if(dataX < 192) gerakR = 20;
		else if(dataX < 256) gerakR = 20;
		else if(dataX < 320) gerakR = 20;
		else if(dataX < 384) gerakR = 20;
		else if(dataX < 448) gerakR = 20;
		else if(dataX < 512) gerakR = 20;
		else if(dataX < 576) gerakR = 14;
		else if(dataX < 640) gerakR = 24;
	}
	else if(dataY<380)
	{
		if(dataX<64) gerakR = 23;
		else if(dataX < 128) gerakR = 13;
		else if(dataX < 192) gerakR = 220;
		else if(dataX < 256) gerakR = 220;
		else if(dataX < 320) gerakR = 220;
		else if(dataX < 384) gerakR = 220;
		else if(dataX < 448) gerakR = 220;
		else if(dataX < 512) gerakR = 220;
		else if(dataX < 576) gerakR = 14;
		else if(dataX < 640) gerakR = 24;
	}
	else if(dataY<460)
	{
		if(dataX<64) gerakR = 15;
		else if(dataX < 210) gerakR = 15;
		else if(dataX < 230) gerakR = 13;
		else if(dataX < 300) gerakR = kka;
		else if(dataX < 365) gerakR = kt;
		else if(dataX < 430) gerakR = kki;
		else if(dataX < 450) gerakR = 14;
		else if(dataX < 640) gerakR = 16;
	}
	else if(dataY<480)
	{
		if(dataX<64) gerakR = 19;
		else if(dataX < 128) gerakR = 19;
		else if(dataX < 192) gerakR = 19;
		else if(dataX < 256) gerakR = 19;
		else if(dataX < 290) gerakR = 19;
		else if(dataX < 315) gerakR = kt;
		else if(dataX < 384) gerakR = 19;
		else if(dataX < 448) gerakR = 19;
		else if(dataX < 512) gerakR = 19;
		else if(dataX < 576) gerakR = 19;
		else if(dataX < 640) gerakR = 19;
	}
	return gerakR;
}

int GerakEksekusiwide(int dataX, int dataY, int modeeks)
{
	int gerakR;
	int kakikiri, kakikanan;

	if(dataX == 0 && dataY == 0) gerakR = 10;

	else if(dataY<120)
	{
		if(dataX<53) gerakR = 23;
		else if(dataX < 106) gerakR = 13;
		else if(dataX < 159) gerakR = 13;
		else if(dataX < 212) gerakR = 13;
		else if(dataX < 265) gerakR = 20;
		else if(dataX < 318) gerakR = 20;
		else if(dataX < 371) gerakR = 14;
		else if(dataX < 424) gerakR = 14;
	}
	else if(dataY<240)
	{
		if(dataX<53) gerakR = 15;
		else if(dataX < 106) gerakR = 13;
		else if(dataX < 159) gerakR = 5;
		else if(dataX < 212) gerakR = 13;
		else if(dataX < 265) gerakR = 13;
		else if(dataX < 318) gerakR = 6;
		else if(dataX < 371) gerakR = 14;
		else if(dataX < 424) gerakR = 16;
	}
	
	return gerakR;
}

int Gerakluruswide(int dataX, int dataY)
{
	int gerakR;
	int kakikiri, kakikanan;
	if(dataX == 0 && dataY == 0) gerakR = 10;
	else if(dataY<104)
	{
		if(dataX<64) gerakR = 33;
		else if(dataX < 128) gerakR = 33;
		else if(dataX < 192) gerakR = 33;
		else if(dataX < 256) gerakR = 33;
		else if(dataX < 320) gerakR = 30;
		else if(dataX < 384) gerakR = 30;
		else if(dataX < 448) gerakR = 34;
		else if(dataX < 512) gerakR = 34;
		else if(dataX < 576) gerakR = 34;
		else if(dataX < 640) gerakR = 34;
	}
	else if(dataY<168)
	{
		if(dataX<64) gerakR = 23;
		else if(dataX < 128) gerakR = 23;
		else if(dataX < 192) gerakR = 23;
		else if(dataX < 256) gerakR = 23;
		else if(dataX < 320) gerakR = 30;
		else if(dataX < 384) gerakR = 30;
		else if(dataX < 448) gerakR = 24;
		else if(dataX < 512) gerakR = 24;
		else if(dataX < 576) gerakR = 24;
		else if(dataX < 640) gerakR = 24;
	}
	else if(dataY<232)
	{
		if(dataX<64) gerakR = 15;
		else if(dataX < 128) gerakR = 15;
		else if(dataX < 192) gerakR = 13;
		else if(dataX < 256) gerakR = 13;
		else if(dataX < 320) gerakR = 10;
		else if(dataX < 384) gerakR = 10;
		else if(dataX < 448) gerakR = 14;
		else if(dataX < 512) gerakR = 14;
		else if(dataX < 576) gerakR = 16;
		else if(dataX < 640) gerakR = 16;
	}
	else if(dataY<290)
	{
		if(dataX<64) gerakR = 15;
		else if(dataX < 128) gerakR = 15;
		else if(dataX < 192) gerakR = 13;
		else if(dataX < 256) gerakR = 13;
		else if(dataX < 288) gerakR = 10;
		else if(dataX < 320) gerakR = 10;
		else if(dataX < 352) gerakR = 10;
		else if(dataX < 384) gerakR = 10;
		else if(dataX < 448) gerakR = 14;
		else if(dataX < 512) gerakR = 14;
		else if(dataX < 576) gerakR = 16;
		else if(dataX < 640) gerakR = 16;
	}
	else if(dataY<360)
	{
		if(dataX<64) gerakR = 19;
		else if(dataX < 128) gerakR = 19;
		else if(dataX < 192) gerakR = 19;
		else if(dataX < 256) gerakR = 19;
		else if(dataX < 320) gerakR = 10;
		else if(dataX < 384) gerakR = 10;
		else if(dataX < 448) gerakR = 19;
		else if(dataX < 512) gerakR = 19;
		else if(dataX < 576) gerakR = 19;
		else if(dataX < 640) gerakR = 19;
	}
	return gerakR;
}

int GerakPutarArah(int arahrobotskr, int arahhadap)
{
	int gerakR;
	if(arahrobotskr == arahhadap) gerakR = 0;
	else if(arahhadap>=1 && arahhadap<=6)
	{
		if(arahrobotskr > arahhadap && arahrobotskr <= arahhadap+6) //putar kiri
		{
			gerakR = 11;
		}
		else //putar kanan
		{
			gerakR = 12;
		}
	}
	else if(arahhadap>=7 && arahhadap<=12)
	{
		if(arahrobotskr >= arahhadap-6 && arahrobotskr < arahhadap)//putar kanan
		{
			gerakR = 12;
		}
		else //putar kiri
		{
			gerakR = 11;
		}
	}
	else gerakR = 0;
	return gerakR;
}

int Gerakrefkepala(int sdtX,int sdtY, int sdtXmin,int sdtXmax,int sdtYmin,int sdtYmax)
{
	int gerakR;
	if(sdtY > sdtYmax) gerakR = 20;
	else if(sdtY < sdtYmin) gerakR = 19;
	else if(sdtX > (sdtXmax+15) ) gerakR = 16;
	else if(sdtX > sdtXmax) gerakR = 14;
	else if(sdtX < (sdtXmin-15) ) gerakR = 15;
	else if(sdtX < sdtXmin) gerakR = 13;
	else gerakR = 0;


	return gerakR;
}

int Geraktendangsdt(int sdtX,int sdtY,int sdttendang)
{
	int gerakR = 10;
	int kakitendang;

	if(sdttendang<0)
	{
		sdttendang *= -1;
		kakitendang = 156;
	}
	else kakitendang = 155;

	sdttendang +=100;

	if(sdtY>30) gerakR = GerakCoverBall(sdtX,sdtY);
	else if(sdtY>20)
	{
		if		(sdtX<-10) 	gerakR = 23;
		else if	(sdtX>10) 	gerakR = 24;
		else 				gerakR = 20;
	}
	else
	{
		if		(sdtX <-40) gerakR = 15;
		else if	(sdtX <-12) gerakR = 13;
		else if	(sdtX > 40) gerakR = 16;
		else if (sdtX >-12) gerakR = 14;
		else gerakR = kakitendang;
	}

	//motionAct(xBall,yBall,gerakR,sdttendang);
	return gerakR;
}

int GerakUncoverBallDimas(int sdtX, int sdtY){
	int gerakR;
	if(sdtY>86){
			if(sdtX>=70) gerakR=10;
			else if(sdtX<70  && sdtX>=66)  gerakR=10;//gerakR=26;
			else if(sdtX<66  && sdtX>=39)  gerakR=20;//gerakR=10;//gerakR=12;
			else if(sdtX<39  && sdtX>=31)  gerakR=20;//gerakR=23;//gerakR=22;
			else if(sdtX<31  && sdtX>=15)  gerakR=20;//gerakR=23;//gerakR=32;
			else if(sdtX<15  && sdtX>=4 )  gerakR=23;//gerakR=22;
			else if(sdtX<4 	 && sdtX>=0 )  gerakR=23;//gerakR=20;
			else if(sdtX<0   && sdtX>=-4 ) gerakR=24;
			else if(sdtX<-4  && sdtX>=-15) gerakR=24;//gerakR=21;
			else if(sdtX<-15 && sdtX>=-31) gerakR=20;//gerakR=24;//gerakR=31;
			else if(sdtX<-31 && sdtX>=-39) gerakR=20;//gerakR=24;//gerakR=21;
			else if(sdtX<-39 && sdtX>=-66) gerakR=20;//gerakR=11;
			else if(sdtX<-66 && sdtX>=-70) gerakR=10;//gerakR=25;
			else if(sdtX<-70) gerakR =10;
	}
	else if(sdtY>70 && sdtY<=86){ //Menunggu bola
		if(sdtX>=70) gerakR=10;
		else if(sdtX<70  && sdtX>=66)  gerakR=10;//gerakR=26;
		else if(sdtX<66  && sdtX>=39)  gerakR=20;//gerakR=23;//gerakR=26;
		else if(sdtX<39  && sdtX>=31)  gerakR=20;//gerakR=20;//gerakR=12;
		else if(sdtX<31  && sdtX>=22)  gerakR=20;//gerakR=23;//gerakR=10;
		else if(sdtX<22  && sdtX>=4 )  gerakR=23;//gerakR=10;
		else if(sdtX<4	 && sdtX>=0 )  gerakR=23;//gerakR=24;//gerakR=10;
		else if(sdtX<0   && sdtX>=-4 ) gerakR=24;
		else if(sdtX<-4  && sdtX>=-22) gerakR=24;//gerakR=10;
		else if(sdtX<-22 && sdtX>=-31) gerakR=20;//gerakR=20;//gerakR=11;
		else if(sdtX<-31 && sdtX>=-39) gerakR=20;//gerakR=20;//gerakR=11;
		else if(sdtX<-39 && sdtX>=-66) gerakR=20;//gerakR=24;//gerakR=25;
		else if(sdtX<-66 && sdtX>=-70) gerakR=10;//gerakR=25;
		else if(sdtX<-70) gerakR =10;
	}
	else if(sdtY>34 && sdtY<=70){ // Bola sedang
		if(sdtX>=70) gerakR=10;
		else if(sdtX<70  && sdtX>=60) gerakR=23;//gerakR=26;
		//if(sdtX>=70) gerakR=23;//gerakR=26;
		else if(sdtX<60  && sdtX>=44) gerakR=20;//gerakR=20;//gerakR=12;
		else if(sdtX<44  && sdtX>=36) gerakR=20;//gerakR=20;//gerakR=12;
		else if(sdtX<36  && sdtX>=27) gerakR=20;//gerakR=23;//gerakR=16;
		else if(sdtX<27  && sdtX>=9)  gerakR=23;//gerakR=19;
		else if(sdtX<9	 && sdtX>=0) gerakR=23;//gerakR=24;//gerakR=19;
		else if(sdtX<0   && sdtX>=-9) gerakR=24;
		else if(sdtX<-9  && sdtX>=-27) gerakR=24;//gerakR=19;
		else if(sdtX<-27 && sdtX>=-36) gerakR=20;//gerakR=24;//gerakR=15;
		else if(sdtX<-36 && sdtX>=-44) gerakR=20;//gerakR=20;//gerakR=11;
		else if(sdtX<-44 && sdtX>=-60) gerakR=20;//gerakR=20;//gerakR=11;
		else if(sdtX<-60 && sdtX>=-70) gerakR=24;//gerakR=25;
		else if(sdtX<-70) gerakR=10;
	}
	else if(sdtY<=34){ //Bola dekat
		if(sdtX>=70) gerakR=10;
		else if(sdtX<70  && sdtX>=60) gerakR=10;//gerakR=26;
		//if(sdtX>=70) gerakR=23;//gerakR=26;
		else if(sdtX<60  && sdtX>=35) gerakR=10;//gerakR=20;//gerakR=12;
		else if(sdtX<35  && sdtX>=18) gerakR=10;//gerakR=16;
		else if(sdtX<18  && sdtX>=9)  gerakR=10;//gerakR=14;
		else if(sdtX<9  && sdtX>=0) gerakR=13;//gerakR=24;//gerakR=19;
		else if(sdtX<0   && sdtX>=-9) gerakR=14;
		else if(sdtX<-9  && sdtX>=-18) gerakR=10;//gerakR=13;
		else if(sdtX<-18 && sdtX>=-35) gerakR=10;//gerakR=15;
		else if(sdtX<-35 && sdtX>=-60) gerakR=10;//gerakR=20;//gerakR=11;
		else if(sdtX<-60 && sdtX>=-70) gerakR=10;//gerakR=25;
		else if(sdtX<-70) gerakR=10;
	}
	else gerakR = 10;
	return gerakR;
}
