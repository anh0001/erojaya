//============================================================================
// Name        : tactics.cpp / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include "eheader.h"
#include <math.h>
#define R2D M_PI/180
#define D2R 180/M_PI

double cosd(double x)
{
	return cos(R2D*x);
}

double sind(double x)
{
	return sin(R2D*x);
}

// Striker 1st ##################################
void Initialize(){
	resetOdometry=true;
	if(dtaXPOS==0 && dtaYPOS==0){
		if (dtaFall==2){
			//flagGetPosition=0; //reset flag pada taktik PositionGenerator
			step=1; //Lostball
		}else
		if(dtaFall>=40 && dtaFall<=49){
			step=-1;
			mainx=KANAN;
		}else 
		if(dtaFall>=50 && dtaFall<=59){
			step=-1;
			mainx=KIRI;
		}
		else if(dtaFall==6)step=-2;
	}

	initialPosition=10;
	// if(flagStrategi==false){
	// 	batasGiringY=2000;
	// 	arahNggiring=1;
	// }
	arahTendang=1;
	countLock=0;
	flagPEX=0;
	countertimeout=CountLost=countLihat=countn=CountAda=counterNyaduk=0; //reset all counter
	flagNyaduk2 = flagNyaduk1 = false; //reset flag nyaduk
	stepG=stepK=majulost=majuloss=0; //reset all variable to zero
	flagsudahdekat=false;
	flagrotasilostball=false;
	flaginposition=false;
	kakiTendang=0;
	catchflagsama=0;
	sprintf(debug_print,"[Initialize] :: Press button to run me [%d]",arahRobot);
	DebugAI(debug_print);
	motionAct(sdtblax,sdtdefy,0,0);
}

void PositionGenerator(){
	ethreadsearching=0;
	//arahTendang=1;
	if(dtaYPOS==0 && dtaXPOS==0){
		if(stepG==0){
			int max_counterpos1=300; //changeable value
			yBall=sdtdefy;
			xBall=sdtblax;
			counterpos1++;
			// DebugAI("[PosGen] :: [0] I'm waiting my neck to reach goal position");
			if (counterpos1>max_counterpos1){ //counter untuk memastikan yBall dan xBall telah tercapai
				relativePosition0=0;
				posDir=1;
				flagLocalize=1;
				counterpos1=0;
				flagcounterpos2=false;
				counterpos2=0;
				stepG=1;
				DebugAI("[PosGen] :: [0] I'm in position");
			}
		}else if (stepG==1){
			int max_counterpos1=100,max_counterpos2=200;//changeable value
			counterpos1++;
			if(counterpos1<=max_counterpos1 && flagcounterpos2==false)relativePosition0+=getPosition;
			if (counterpos1>max_counterpos1 && flagcounterpos2==false){
				relativePosition0=relativePosition0/max_counterpos1;
				if(relativePosition0>=3){
					batasGiringY=1000;//changeable value
					arahTendang=1;
					stepBuffer=5; //Nggiring dulu --> tendang
					counterNyaduk=1;
					flagNyaduk1=false;
					flagcounterpos2=1;
					initialPosition=10;
					sprintf(debug_print,"[PosGen] :: [1] Am I still far ? I wanna dribble it Y => %1.2f",relativePosition0);
					DebugAI(debug_print);

					xBall=sdtblax;
					yBall=sdtexcute;
				}else if(relativePosition0>2){
					batasGiringY=1000;//changeable value
					arahTendang=1;
					stepBuffer=5; //Nggiring dulu --> tendang
					counterNyaduk=2;
					flagNyaduk1=false;
					flagcounterpos2=1;
					initialPosition=10;
					sprintf(debug_print,"[PosGen] :: [1] Am I still far ? I wanna dribble it again Y => %1.2f",relativePosition0);
					DebugAI(debug_print);

					xBall=sdtblax;			
					yBall=sdtexcute;		
				// }else if(relativePosition0>1.6){
				// 	batasGiringY=(int)(1000*(relativePosition0-1.3));//changeable value
				// 	stepBuffer=4; //Nggiring dikit lagi
				// 	flagcounterpos2=1;
				// 	initialPosition=8;
				// 	sprintf(debug_print,"[PosGen] :: [1] I am tired, how far I must dribble it again ? %d",batasGiringY);
				// 	DebugAI(debug_print);	
					
				// 	xBall=sdtblax;
				// 	yBall=sdtexcute;			
				}else{
					counterpos1=0;
					DebugAI("[PosGen] :: [1] I wanna kick it, where must I kick ?");
					stepG++;
				}
			}

			//memastikan sudut kepala sudah goal position
			if(flagcounterpos2==true && counterpos2<max_counterpos2){ //belum
				counterpos1=0;
				counterpos2++;
			}else if(flagcounterpos2==true && counterpos2>=max_counterpos2){ //sudah
				counterpos1=0;
				counterpos2=0;
				flagcounterpos2=0;
				step=stepBuffer;
				stepG=0;
				flagLocalize=false;
			}

		}else if(stepG==2){
			int max_counterpos1=200;//changeable value
			relativePosition1=relativePosition2=0;
			yBall=sdtdefy;
			xBall=generatesdtX(90);
			posDir=2;
			flagLocalize=1;
			counterpos1++;
			if (counterpos1>max_counterpos1)
			{
				stepG=3;
				counterpos1=0;
				DebugAI("[PosGen] :: [2] What is it on right ?");
			}
		}else if (stepG==3){
			int max_counterpos1=10;//changeable value
			relativePosition1+=getPosition;
			counterpos1++;
			if (counterpos1>max_counterpos1)
			{
				stepG=4;
				counterpos1=0;
				relativePosition1=relativePosition1/10;
				sprintf(debug_print,"[PosGen] :: [3] I'm in X => %1.2f from right",relativePosition1);
				DebugAI(debug_print);
			}
		}else if(stepG==4){
			int max_counterpos1=300;//changeable value
			yBall=sdtdefy;
			xBall=generatesdtX(-90);
			posDir=3;
			flagLocalize=1;
			counterpos1++;
			if (counterpos1>max_counterpos1)
			{
				stepG=5;
				counterpos1=0;
				DebugAI("[PosGen] :: [4] What is it on left ?");
			}
		}else if (stepG==5){
			int max_counterpos1=10;
			relativePosition2+=getPosition;
			counterpos1++;
			if (counterpos1>max_counterpos1)
			{
				stepG=6;
				counterpos1=0;
				relativePosition2=relativePosition2/10;
				sprintf(debug_print,"[PosGen] :: [5] I'm in X => %1.2f from left",relativePosition2);
				DebugAI(debug_print);
			}
		}else{
			yBall=sdtexcute;
			xBall=generatesdtX(0);
			if (relativePosition1>3.5)
			{
				vPos=(relativePosition1*0.6+relativePosition2*0.4);
			}else if(relativePosition2<3.5){
				vPos=(relativePosition1*0.4+relativePosition2*0.6);
			}else{
				vPos=(relativePosition1+relativePosition2)/2;
			}
			sprintf(debug_print,"[PosGen] :: [6] I think I'm in X => %1.2f",vPos);
			DebugAI(debug_print);

			stepG=0; //Reset all variable
			step=5;  //Luruskan gawang
			flagLocalize=false;

			//Generate vPos to arahTendang
			if(vPos<1.5) 					{arahTendang=4;initialPosition=1;}//step=8;kakiTendang=2;}
			else if(vPos>=1.5 && vPos<1.8)	{arahTendang=4;initialPosition=1;}//step=8;kakiTendang=2;}
			else if(vPos>=1.8 && vPos<2.3)	{arahTendang=4;initialPosition=2;}
			else if(vPos>=2.3 && vPos<2.4)	{arahTendang=3;initialPosition=2;}
			else if(vPos>=2.4 && vPos<2.8)	{arahTendang=2;initialPosition=2;}
			else if(vPos>=2.8 && vPos<3.2)	{arahTendang=1;initialPosition=3;}
			else if(vPos>=3.2 && vPos<3.6)	{arahTendang=20;initialPosition=3;}
			else if(vPos>=3.6 && vPos<3.9)	{arahTendang=19;initialPosition=3;}
			else if(vPos>=3.9 && vPos<4.1)	{arahTendang=18;initialPosition=4;}
			else if(vPos>=4.1 && vPos<4.4)	{arahTendang=18;initialPosition=4;}
			else if(vPos>=4.4)				{arahTendang=18;initialPosition=5;}//step=8;kakiTendang=1;}

			sprintf(debug_print,"[PosGen] :: I must kick the ball to %d",arahTendang);
			DebugAI(debug_print);
		}
	}else{
		resetOdometry=true;
		DebugAI("[PosGen] :: My system not yet reset odometry");
	}
	motion=0;
	motionAct(xBall, yBall,0, 10);
}

void lostball(){
	//nBall = ftBall3(dataXB, dataYB, 0);
	aktifkansearching();
	if(nBall == 1){
		countn++;
		motion=10;
		if(countn>150){	
			majulost=0;
			langkah=0;
			CountAda++;
			flagrotasilostball=false;
			if((arahLihat == last_arahLihat || arahLihat == rotasiarah(arahLihat+1) || arahLihat == rotasiarah(arahLihat-1))
				&& sdtbolay>=lastsdtbolay-10 && sdtbolay<=lastsdtbolay+10){
				DebugAI("[Lostball] :: Bola ideal");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;				
					step++;
				}
			}else if(arahLihat >= 6 && arahLihat <= 15){
				DebugAI("[Lostball] :: I look the ball beside me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step++;
				}
			}else if(((1 <= arahLihat <= 5) || (16 <= arahLihat <= 20)) && sdtbolay <= 90){
				DebugAI("[Lostball] :: Look! The ball in front of me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step++;
				}
			}else{
				DebugAI("[Lostball] :: I look the ball, but I don't know where it is");
				if(CountAda>50){
					countn=CountAda=0;
					hit_arahRobot=false;	
					step++;
				}
			}
			motion = GerakLurusBola(sdtbolax,sdtbolay);
		}

		if(countn>10 && sdtbolay<30){
			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahRobot);
		}
	}else{
		countn--;
		if(!flaginposition) step=-1;
		if(countn<=0)countn=0;
		if(flagsudahdekat==false){
			if(arahRobot<20 && arahRobot>11)motion=12;
			else if(arahRobot>2 && arahRobot<=11)motion=11;
			else{
				if(majulost<1000){
					motion=20;
				}else if(majulost>=1000){
					majulost=1000;
					majuloss++;
				}

				if(majulost>=1000 && majuloss<1200){
					motion=10;
				}else if(majulost>=1000 && majuloss>=1200){
					majulost=0;
					majuloss=0;
				}
				majulost++;
			}
		}else{
			switch(stepK){
				case 0:
					init_arahRobot=arahRobot;
					hit_arahRobot=false;
					last_arahRobot=arahRobot;
					stepK=1;
					count10=0;
					lostball_arahx=arahX;
				break;
				case 1:
					count10++;
					if(arahRobot==rotasiarah(init_arahRobot+10))hit_arahRobot=true;
					if(arahRobot==init_arahRobot && hit_arahRobot){
						stepK=0;
						if(flagrotasilostball){
							flagsudahdekat=false;
							flagrotasilostball=false;
						}else{
							flagrotasilostball=true;
						}

					}else{
						if(lostball_arahx){
							if(flagrotasilostball)motion=22;
							else motion=12;
						}else{
							if(flagrotasilostball)motion=21;
							else motion=11;
						}
					}
					if(flagrotasilostball){
						if(count10<0){
							motion=10;
						}else count10=5;
					}else{
						if(count10<0){
							motion=10;
						}else count10=30;
					}

					if(last_arahRobot!=arahRobot){
						count10=0;
					}
					last_arahRobot=arahRobot;
				break;
			}
		}	
	}

	//fprintf(stderr, "xBall=%d | yBall=%d | motion=%d\n",xBall,yBall,motion);
	motionAct(xBall, yBall,motion, 7);
}

void lostball_bertahan()
{	
	aktifkansearching();
	if(nBall == 1){
		countn++;
		motion=10;
		if(countn>150){	
			majulost=0;
			langkah=0;
			CountAda++;
			flagrotasilostball=false;
			if((arahLihat == last_arahLihat || arahLihat == rotasiarah(arahLihat+1) || arahLihat == rotasiarah(arahLihat-1))
				&& sdtbolay>=lastsdtbolay-10 && sdtbolay<=lastsdtbolay+10){
				DebugAI("[Lostball] :: Bola ideal");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;				
					step++;
				}
			}else if(arahLihat >= 6 && arahLihat <= 15){
				DebugAI("[Lostball] :: I look the ball beside me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step++;
				}
			}else if(((1 <= arahLihat <= 5) || (16 <= arahLihat <= 20)) && sdtbolay <= 90){
				DebugAI("[Lostball] :: Look! The ball in front of me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step++;
				}
			}else{
				DebugAI("[Lostball] :: I look the ball, but I don't know where it is");
				if(CountAda>50){
					countn=CountAda=0;
					hit_arahRobot=false;	
					step++;
				}
			}
			motion = GerakLurusBola(sdtbolax,sdtbolay);
		}
	}else{
		countn--;
		if(!flaginposition) step=-1;
		if(countn<=0)countn=0;
		//sebelah kanannya
		if(InvSudut(bufferTendang)-InvSudut(bufferTendangMax)<=0){
			if(arahRobot>=1 && arahRobot<11){
				motion=11;
			}else{
				if(arahRobot<16)motion=12;
				else if(arahRobot>16)motion=11;
				else{
					if(InvSudut(bufferTendang)-InvSudut(bufferTendangMax)<-54){
						motion=20;
					}else
					if(InvSudut(bufferTendang)-InvSudut(bufferTendangMax)>-36){
						motion=19;
					}else motion=0;
				}
			}
		}else{//sebelah kirinya
			if(arahRobot>=1 && arahRobot<11){
				if(arahRobot<6)motion=12;
				else if(arahRobot>6)motion=11;
				else{
					if(InvSudut(bufferTendang)-InvSudut(bufferTendangMax)>54){
						motion=20;
					}else
					if(InvSudut(bufferTendang)-InvSudut(bufferTendangMax)<36){
						motion=19;
					}else motion=0;
				}
			}else{
				motion=12;
			}
		}

		// if(){
		// 	if(arahRobot<20 && arahRobot>11)motion=12;
		// 	else if(arahRobot>2 && arahRobot<=11)motion=11;
		// 	else{
		// 		if(majulost<1000){
		// 			motion=20;
		// 		}else if(majulost>=1000){
		// 			majulost=1000;
		// 			majuloss++;
		// 		}

		// 		if(majulost>=1000 && majuloss<1200){
		// 			motion=10;
		// 		}else if(majulost>=1000 && majuloss>=1200){
		// 			majulost=0;
		// 			majuloss=0;
		// 		}
		// 		majulost++;
		// 	}
		// }else{
		// 	switch(stepK){
		// 		case 0:
		// 			init_arahRobot=arahRobot;
		// 			hit_arahRobot=false;
		// 			last_arahRobot=arahRobot;
		// 			stepK=1;
		// 			count10=0;
		// 			lostball_arahx=arahX;
		// 		break;
		// 		case 1:
		// 			count10++;
		// 			if(arahRobot==rotasiarah(init_arahRobot+10))hit_arahRobot=true;
		// 			if(arahRobot==init_arahRobot && hit_arahRobot){
		// 				stepK=0;
		// 				if(flagrotasilostball){
		// 					flagsudahdekat=false;
		// 					flagrotasilostball=false;
		// 				}else{
		// 					flagrotasilostball=true;
		// 				}

		// 			}else{
		// 				if(lostball_arahx){
		// 					if(flagrotasilostball)motion=22;
		// 					else motion=12;
		// 				}else{
		// 					if(flagrotasilostball)motion=21;
		// 					else motion=11;
		// 				}
		// 			}
		// 			if(flagrotasilostball){
		// 				if(count10<0){
		// 					motion=10;
		// 				}else count10=5;
		// 			}else{
		// 				if(count10<0){
		// 					motion=10;
		// 				}else count10=30;
		// 			}

		// 			if(last_arahRobot!=arahRobot){
		// 				count10=0;
		// 			}
		// 			last_arahRobot=arahRobot;
		// 		break;
		// 	}
		// }	
	}

	if(catchflagsama==1){
		step=-3;
	}

	//fprintf(stderr, "xBall=%d | yBall=%d | motion=%d\n",xBall,yBall,motion);
	motionAct(xBall, yBall,motion, 10);
}

void TaktikBertahan(){
	int arahhadap = 1;
	aktifkansearching();
	if(nBall == 1){
		lastXBall = xBall;
		lastYBall = yBall;

		motion = GerakHadapBola(sdtbolax,sdtbolay-30,arahRobot,arahhadap);
		if(sdtbolax>12)	motion=26;
		else if(sdtbolax<-12)	motion=25;
		if(motion==17 || motion==18) motion=10;

		if(sdtbolay <= 70 && (arahLihat == rotasiarah(arahhadap)   || 
															 arahLihat == rotasiarah(arahhadap-1) ||
															 arahLihat == rotasiarah(arahhadap+1) ))
		{
			countertimeout++;
			sprintf(debug_print,"[TaktikBertahan] :: I'm already to defend the ball [%d]",countertimeout);
			DebugAI(debug_print);
			if(countertimeout>500) { 
				countertimeout=500;
				motion = 10;
				flagsudahdekat=true;
				flagrotasilostball=true;
			}

		}else{
			countertimeout--;	
		} 
		if (countertimeout<=0)countertimeout=0;
		CountLost--;
		if(CountLost<=0)CountLost=0;
	}else{
		countertimeout--;
		if (countertimeout<=0)countertimeout=0;
		CountLost++;
		if(CountLost>200){
			DebugAI("[TaktikBertahan] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = countertimeout = 0;
		}
	}

	if(catchflagsama==1){
		step=-3;
	}
	motionAct(xBall,yBall,motion,0);
	//fprintf(stderr,"Countlhtgw = %d\n",countlihatgw);
}

void TaktikkeBola3(int arahhadap){
	aktifkansearching();
	kaki = 0;
	if(nBall == 1){
		lastXBall = xBall;
		lastYBall = yBall;

		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);

		if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot == rotasiarah(arahhadap)   || 
															 arahRobot == rotasiarah(arahhadap-1) ||
															 arahRobot == rotasiarah(arahhadap+1) ))
		{
			countertimeout++;
			sprintf(debug_print,"[TaktikkeBola] :: I'm already to kick the ball [%d]",countertimeout);
			DebugAI(debug_print);
			if(countertimeout>50) { 
				if(initialPosition>5){
					step=4;//Nggiring 
				}else{
					//perubahan besar pada odometry X / Y
					if(dtaYPOS<-800){
						step=4; //ke nggiring
						arahNggiring=1;//hadap 1 utk siap siap position generator
						initialPosition=7;
						batasGiringY=dtaYPOS-300; //langsung reset odometry, karena giring sudah terpenuhi
					}else if(dtaXPOS>400 || dtaXPOS<-400){
						arahTendang=rotasiarah((-1*((int)(dtaXPOS/400)))+arahTendang);
						step=5;
						resetOdometry=true;
					}else{
						step=5;//Luruskan ke arah tendang
						resetOdometry=true; 
					}
				}
				countertimeout = countLihatblg = 0; 
				motion = 10;
				flagsudahdekat=true;
			}
		}
		else countertimeout--;

		/*if (sdtbolay >= 50 && ((arahRobot <= 13 && arahRobot>=11 && arahLihat>=11) || ( arahRobot >= 7 && arahRobot<11 && arahLihat<=11) || (arahRobot<=12 && arahRobot>=10))){
			counterTerobos++ ;
			motion = GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 1");
			DebugAI(debug_print);
			if (counterTerobos > 100){
				flagCuri = true ;
				if(arahLihat<11)arahTerobos=1;
				else arahTerobos=2;
					
			} 
		}

		if(flagCuri==true){
			if(arahTerobos==1)motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,16);
			else motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,5);
			if(sdtbolay>40)motion=GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && ((arahRobot <= 16 && arahRobot >= 14) || (arahRobot <= 6 && arahRobot >= 4))){
				flagCuri2=true;
			}
		}

		if (flagCuri2== true){
			countRebut++ ;
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 2");
			DebugAI(debug_print);
			motion = 20 ;
			if (countRebut > 300){
				counterTerobos = countRebut = 0 ;
				flagCuri = flagCuri2 = false;
			}
		}*/

		if (countertimeout<=0)countertimeout=0;
		CountLost--;
		if(CountLost<=0)CountLost=0;
	}else{
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikkeBola] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikkeBola] :: I step back to find the ball");
			motion=19;//ganti
		}else
		if(CountLost>200){
			DebugAI("[TaktikkeBola] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}
	motionAct(xBall,yBall,motion,0);
	//fprintf(stderr,"Countlhtgw = %d\n",countlihatgw);
}

void TaktikkeBola4(int arahhadap){
	aktifkansearching();
	kaki = 0;
	if(nBall == 1){
		lastXBall = xBall;
		lastYBall = yBall;

		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
		if(motion==10)count10kebola++;
		if(count10kebola>120){
			motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,arahhadap);
		}

		if(sdtbolay <= 40 && sdtbolax<15 && sdtbolax>-15 && (arahRobot == rotasiarah(arahhadap)   || 
															 arahRobot == rotasiarah(arahhadap-1) ||
															 arahRobot == rotasiarah(arahhadap+1) ))
		{
			countertimeout++;
			sprintf(debug_print,"[TaktikkeBola] :: I'm already to kick the ball [%d]",countertimeout);
			DebugAI(debug_print);
			if(countertimeout>50){
				countertimeout=0;
				//counterNyaduk++;
				//step=9; //with game controller
				//if(dtaYPOS>1500){
				//	counterNyaduk=3;
				//}//else if(initialPosition>5 && dtaYPOS<1500)counterNyaduk=2;
				sprintf(debug_print,"[TaktikkeBola] :: Tendang dulu bro 1 [%d]",counterNyaduk);
				DebugAI(debug_print);
				//if (counterNyaduk>2){flagNyaduk1 = true;counterNyaduk=3;}

				//if(flagNyaduk1 == true) { 				
				if(true){
					//arahTendang=bufferTendang;
					//if(initialPosition>5){
					if(false){
						batasGiringY=0;
						arahNggiring=1;
						step=4;//Nggiring 
					}else{
						//perubahan besar pada odometry X / Y
						//if(dtaYPOS<-800){
						if(false){
							step=4; //ke nggiring
							arahNggiring=arahTendang=1;//hadap 1 utk siap siap position generator
							initialPosition=7;
							batasGiringY=dtaYPOS-300; //langsung reset odometry, karena giring sudah terpenuhi
							flagNyaduk1 = flagNyaduk2 = false;
							counterNyaduk=0;
						}else if(dtaXPOS>400 || dtaXPOS<-400){
							//arahTendang=rotasiarah((-1*((int)(dtaXPOS/400)))+arahTendang);
							step=5;
							//resetOdometry=true;
						}else{
							step=5;//Luruskan ke arah tendang
							//resetOdometry=true; 
						}
					}
					countertimeout = countLihatblg = count10kebola = 0; 
					motion = 10;
					flagsudahdekat=true;
				}

			}
		}
		else countertimeout--;

/*		if (sdtbolay >= 50 && (arahRobot<=12 && arahRobot>=10)){
			counterTerobos++ ;
			motion = GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 1");
			DebugAI(debug_print);
			if (counterTerobos > 100){
				flagCuri = true ;
				if(arahLihat<11)arahTerobos=1;
				else arahTerobos=2;
					
			} 
		}

		if(flagCuri==true){
			if(arahTerobos==1)motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,16);
			else motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,5);
			if(sdtbolay>40)motion=GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			if(sdtbolay <= 30 && sdtbolax<30 && sdtbolax>-30 && ((arahRobot <= 16 && arahRobot >= 14) || (arahRobot <= 6 && arahRobot >= 4))){
				if(sdtbolax > 15) 	motion = 14;
				else if(sdtbolax < -15) motion = 13;
			}
			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && ((arahRobot <= 16 && arahRobot >= 14) || (arahRobot <= 6 && arahRobot >= 4))){
				flagCuri2=true;
			}
		}

		if (flagCuri2== true){
			countRebut++ ;
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 2");
			DebugAI(debug_print);
			motion = 20 ;
			if (countRebut > 300){
				counterTerobos = countRebut = 0 ;
				flagCuri = flagCuri2 = false;
			}
		}
*/
		if (countertimeout<=0)countertimeout=0;
		CountLost--;
		if(CountLost<=0)CountLost=0;
	}else{
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikkeBola] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikkeBola] :: I step back to find the ball");
			motion=19;//ganti
		}else
		if(CountLost>200){
			DebugAI("[TaktikkeBola] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}
	motionAct(xBall,yBall,motion,0);
	// motionAct(xBall,yBall,0,0);
	//fprintf(stderr,"Countlhtgw = %d\n",countlihatgw);
}

/*============================Taktik Penalty===========================================*/
void TaktikEksekusi4_pinalty(){
	int batascounttendang = 100;
	
	if(flagKickOff)motion=GerakEksekusiwide(dataXB,dataYB,1);
	else motion=GerakEksekusiwide(dataXB,dataYB,0);

	if(dataXB !=0 && dataYB!=0){
		lastdataXB=dataXB;
		if(motion == 5 || motion == 6 || motion == 7 || motion == 8)  { 
			DebugAI("[TaktikEksekusi] :: Bismillah, I kick the ball");
			Kick = 1; 
			kaki = motion; 
			//arahTendang=bufferTendang;
			//resetOdometry=true;
			//if(arahRobot!=madepgawang){countLuruskanGawang++;motion=10;}
			//else 
			countgerak++;
	 	}else countgerak--;
		if(countgerak>batascounttendang){ 
			motion = kaki; 
			if(motion==0) motion =20; 
			countgerak = batascounttendang+1; 
			//tendangTrue=1;
		}else if(countgerak<0) countgerak=0;
		//if(countLuruskanGawang>50){step=4;countLuruskanGawang=0;motion=10;}
		CountLost = 0;
	}else{
		if(Kick==1){
			countLihat++;
			if(countLihat>200) {step++; countLihat =  countgerak = 0;}//else{motion=10;}
			stepT = 1;
			DebugAI("[TaktikEksekusi] :: The ball is kicked");
		}else{
			Kick=0;
			DebugAI("[TaktikEksekusi] :: I'm not kicked the ball, why the ball is missing?");
			countLihat=0;
			CountLost++;
			if(CountLost > 300){
				if(lastdataXB<212)motion=5;
				else motion=6;
				//step++;
				Kick=1;
			}
		}
	}
	if(countgerak>batascounttendang){
		aktifkansearching();
		if(nBall==1)countLock++;
		else countLock--;

		if(countLock<=0){
			countLock=0;
			step=1;
		}else if(countLock>100){
			countLock=0;
			step=2;
		}
		motionAct(xBall,yBall,motion,0);	
	}else motionAct(sdtblax,sdtexcute,motion,0);	
}

void TaktikkeBola_pinalty(int arahhadap){
	aktifkansearching();
	kaki = 0;
	if(nBall == 1){
		lastXBall = xBall;
		lastYBall = yBall;

		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
		if(motion==10)count10kebola++;
		if(count10kebola>120){
			motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,arahhadap);
		}

		if(sdtbolay <= 40 && sdtbolax<15 && sdtbolax>-15 && (arahRobot == rotasiarah(arahhadap)   || 
															 arahRobot == rotasiarah(arahhadap-1) ||
															 arahRobot == rotasiarah(arahhadap+1) ))
		{
			countertimeout++;
			sprintf(debug_print,"[TaktikkeBola] :: I'm already to kick the ball [%d]",countertimeout);
			DebugAI(debug_print);
			if(countertimeout>50){
				countertimeout=0;
				//counterNyaduk++;
				//step=9; //with game controller
				//if(dtaYPOS>1500){
				//	counterNyaduk=3;
				//}//else if(initialPosition>5 && dtaYPOS<1500)counterNyaduk=2;
				sprintf(debug_print,"[TaktikkeBola] :: Tendang dulu bro 1 [%d]",counterNyaduk);
				DebugAI(debug_print);
				//if (counterNyaduk>2){flagNyaduk1 = true;counterNyaduk=3;}

				//if(flagNyaduk1 == true) { 				
				if(true){
					//arahTendang=bufferTendang;
					//if(initialPosition>5){
					if(false){
						batasGiringY=0;
						arahNggiring=1;
						//step=4;//Nggiring 
					}else{
						//perubahan besar pada odometry X / Y
						//if(dtaYPOS<-800){
						if(false){
							//step=4; //ke nggiring
							arahNggiring=arahTendang=1;//hadap 1 utk siap siap position generator
							initialPosition=7;
							batasGiringY=dtaYPOS-300; //langsung reset odometry, karena giring sudah terpenuhi
							flagNyaduk1 = flagNyaduk2 = false;
							counterNyaduk=0;
						}else if(dtaXPOS>400 || dtaXPOS<-400){
							//arahTendang=rotasiarah((-1*((int)(dtaXPOS/400)))+arahTendang);
							step=3;
							//resetOdometry=true;
						}else{
							step=3;//Luruskan ke arah tendang
							//resetOdometry=true; 
						}
					}
					countertimeout = countLihatblg = count10kebola = 0; 
					motion = 10;
					flagsudahdekat=true;
				}

			}
		}
		else countertimeout--;

/*		if (sdtbolay >= 50 && (arahRobot<=12 && arahRobot>=10)){
			counterTerobos++ ;
			motion = GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 1");
			DebugAI(debug_print);
			if (counterTerobos > 100){
				flagCuri = true ;
				if(arahLihat<11)arahTerobos=1;
				else arahTerobos=2;
					
			} 
		}

		if(flagCuri==true){
			if(arahTerobos==1)motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,16);
			else motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,5);
			if(sdtbolay>40)motion=GerakTerobosBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
			if(sdtbolay <= 30 && sdtbolax<30 && sdtbolax>-30 && ((arahRobot <= 16 && arahRobot >= 14) || (arahRobot <= 6 && arahRobot >= 4))){
				if(sdtbolax > 15) 	motion = 14;
				else if(sdtbolax < -15) motion = 13;
			}
			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && ((arahRobot <= 16 && arahRobot >= 14) || (arahRobot <= 6 && arahRobot >= 4))){
				flagCuri2=true;
			}
		}

		if (flagCuri2== true){
			countRebut++ ;
			sprintf(debug_print,"[TaktikkeBola] :: Maju Terobos step 2");
			DebugAI(debug_print);
			motion = 20 ;
			if (countRebut > 300){
				counterTerobos = countRebut = 0 ;
				flagCuri = flagCuri2 = false;
			}
		}
*/
		if (countertimeout<=0)countertimeout=0;
		CountLost--;
		if(CountLost<=0)CountLost=0;
	}else{
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikkeBola] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikkeBola] :: I step back to find the ball");
			motion=19;//ganti
		}else
		if(CountLost>200){
			DebugAI("[TaktikkeBola] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}
	motionAct(xBall,yBall,motion,0);
	// motionAct(xBall,yBall,0,0);
	//fprintf(stderr,"Countlhtgw = %d\n",countlihatgw);
}

void TaktikLuruskanGW_pinalty(){
	aktifkansearching();
	if(nBall == 1){
		//lastXBall = xBall;
		//lastYBall = yBall;
		motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,bufferTendang);

		if(arahRobot==bufferTendang && sdtbolax>-15 && sdtbolax<15){
			DebugAI("[TaktikLuruskan] :: I wanna kick it now");
			countertimeout++;
			if(countertimeout>50){
				motion = 10;
				//arahTendang=1; //reset variable
				step=4; //TaktikEksekusi
				countertimeout=0;
			}
		}else countertimeout--;
		if (countertimeout<=0)countertimeout=0;

		// if(dtaYPOS<-800){
		// 	step=4; //ke nggiring
		// 	arahNggiring=1;//hadap 1 utk siap siap position generator
		// 	batasGiringY=dtaYPOS-300; //langsung reset odometry, karena giring sudah terpenuhi
		// }
		CountLost=0;
	}else{
		
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikLuruskan] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikLuruskan] :: I step back to find the ball");
			motion=19;
		}else
		if(CountLost>200){
			DebugAI("[TaktikLuruskan] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}

	motionAct(xBall,yBall,motion,0);
	//fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
}

/*====================================================================================================*/

void TaktikNggiring3(){
	aktifkansearching();
	if (nBall == 1 ){
		motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,arahNggiring);
		if(arahRobot == arahNggiring && arahLihat==1){
			if(yBall<sdtexcute)yBall=sdtexcute;
			xBall=sdtblax;
			motion = GerakEksekusiDribble(dataXB,dataYB);
			sprintf(debug_print,"[TaktikNggiring] :: Giring.. Giring.. Y=>%d",dtaYPOS);
			DebugAI(debug_print);
			if(resetOdometry==false){
				if(true){
				//if(dtaYPOS>batasGiringY){
					sprintf(debug_print,"[TaktikNggiring] :: I'm tired, I wanna check my position [%d][%d]",dtaYPOS,batasGiringY);
					DebugAI(debug_print);
					if(arahNggiring!=1){
						batasGiringY=1000;
						arahNggiring=1;
					}else{
						batasGiringY=0;
						step=3; //Check position
					}
					resetOdometry=true;
					motion=10;
				}
			}
		}else{
			DebugAI("[TaktikNggiring] :: Where must I dribble d'ball");
		}
		if(sdtbolay>50 || sdtbolax>70 || sdtbolax<-70){
			step=2;
		}
	}else {
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikNggiring] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikNggiring] :: I step back to find the ball");
			motion=19;
		}else
		if(CountLost>200){
			DebugAI("[TaktikNggiring] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}
	motionAct(xBall, yBall,motion, 10);
}

void TaktikLuruskanGW_wide(){
	aktifkansearching();
	if(nBall == 1){
		//lastXBall = xBall;
		//lastYBall = yBall;
		motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,bufferTendang);

		if(arahRobot==bufferTendang && sdtbolax>-15 && sdtbolax<15){
			DebugAI("[TaktikLuruskan] :: I wanna kick it now");
			countertimeout++;
			if(countertimeout>50){
				motion = 10;
				//arahTendang=1; //reset variable
				step=6; //TaktikEksekusi
				countertimeout=0;
			}
		}else countertimeout--;
		if (countertimeout<=0)countertimeout=0;

		// if(dtaYPOS<-800){
		// 	step=4; //ke nggiring
		// 	arahNggiring=1;//hadap 1 utk siap siap position generator
		// 	batasGiringY=dtaYPOS-300; //langsung reset odometry, karena giring sudah terpenuhi
		// }
		CountLost=0;
	}else{
		
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikLuruskan] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikLuruskan] :: I step back to find the ball");
			motion=19;
		}else
		if(CountLost>200){
			DebugAI("[TaktikLuruskan] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}

	motionAct(xBall,yBall,motion,0);
	//fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
}

void TaktikLuruskanGW_wide_rokh(){
	aktifkansearching();
	if(nBall == 1){
		//lastXBall = xBall;
		//lastYBall = yBall;
		motion = GerakHadapBolaSensitive(sdtbolax,sdtbolay,arahRobot,arahTendang);

		if(arahRobot==arahTendang && sdtbolax>-15 && sdtbolax<15){
			DebugAI("[TaktikLuruskan] :: I wanna kick it now");
			countertimeout++;
			if(countertimeout>50){
				motion = 10;
				//arahTendang=1; //reset variable
				arahTendang=arahNggiring=1;
				step=6; //TaktikEksekusi
				countertimeout=0;
			}
		}else countertimeout--;
		if (countertimeout<=0)countertimeout=0;
		CountLost=0;
	}else{
		
		CountLost++;
		if(CountLost < 100 && CountLost > 50) {
			DebugAI("[TaktikLuruskan] :: The ball disappear, are you an assassin?");
			motion=10;
		}else
		if(CountLost < 200 && CountLost > 100) {
			DebugAI("[TaktikLuruskan] :: I step back to find the ball");
			motion=19;
		}else
		if(CountLost>200){
			DebugAI("[TaktikLuruskan] :: I'm already lost the ball");
			step=1; //kembali ke lostball
			CountLost = 0;
		}
	}

	motionAct(xBall,yBall,motion,0);
	//fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
}

void TaktikEksekusi4(){
	int batascounttendang = 100;
	
	if(flagKickOff)motion=GerakEksekusiwide(dataXB,dataYB,1);
	else motion=GerakEksekusiwide(dataXB,dataYB,0);

	if(dataXB !=0 && dataYB!=0){
		lastdataXB=dataXB;
		if(motion == 5 || motion == 6 || motion == 7 || motion == 8)  { 
			DebugAI("[TaktikEksekusi] :: Bismillah, I kick the ball");
			Kick = 1; 
			kaki = motion; 
			//arahTendang=bufferTendang;
			//resetOdometry=true;
			//if(arahRobot!=madepgawang){countLuruskanGawang++;motion=10;}
			//else 
			countgerak++;
	 	}else countgerak--;
		if(countgerak>batascounttendang){ 
			motion = kaki; 
			if(motion==0) motion =20; 
			countgerak = batascounttendang+1; 
			//tendangTrue=1;
		}else if(countgerak<0) countgerak=0;
		//if(countLuruskanGawang>50){step=4;countLuruskanGawang=0;motion=10;}
		CountLost = 0;
	}else{
		if(Kick==1){
			countLihat++;
			if(countLihat>200) {step++; countLihat =  countgerak = 0;}//else{motion=10;}
			stepT = 1;
			DebugAI("[TaktikEksekusi] :: The ball is kicked");
		}else{
			Kick=0;
			DebugAI("[TaktikEksekusi] :: I'm not kicked the ball, why the ball is missing?");
			countLihat=0;
			CountLost++;
			if(CountLost > 300){
				if(lastdataXB<212)motion=5;
				else motion=6;
				//step++;
				Kick=1;
			}
		}
	}
	if(countgerak>batascounttendang){
		aktifkansearching();
		if(nBall==1)countLock++;
		else countLock--;

		if(countLock<=0){
			countLock=0;
			step=1;
		}else if(countLock>100){
			countLock=0;
			step=2;
		}
		motionAct(xBall,yBall,motion,0);	
	}else motionAct(sdtblax,sdtexcute,motion,0);	
}

void TaktikEksekusi5(){
	int batascounttendang = 100;
	aktifkansearching();
	motion=GerakTendangjauhfast(sdtbolax,sdtbolay+3);

	if(dataXB !=0 && dataYB!=0){
		lastdataXB=dataXB;
		if(motion == 5 || motion == 6 || motion == 7 || motion == 8)  { 
			DebugAI("[TaktikEksekusi] :: Bismillah, I kick the ball");
			Kick = 1; 
			kaki = motion; 
			arahTendang=bufferTendang;
			resetOdometry=true;
			//if(arahRobot!=madepgawang){countLuruskanGawang++;motion=10;}
			//else 
			countgerak++;
	 	}else countgerak--;
		if(countgerak>batascounttendang){ 
			motion = kaki; 
			if(motion==0) motion =20; 
			countgerak = batascounttendang+1; 
			//tendangTrue=1;
		}else if(countgerak<0) countgerak=0;
		//if(countLuruskanGawang>50){step=4;countLuruskanGawang=0;motion=10;}
		CountLost = 0;
	}else{
		if(Kick==1){
			countLihat++;
			if(countLihat>200) {step++; countLihat =  countgerak = 0;}//else{motion=10;}
			stepT = 1;
			DebugAI("[TaktikEksekusi] :: The ball is kicked");
		}else{
			Kick=0;
			DebugAI("[TaktikEksekusi] :: I'm not kicked the ball, why the ball is missing?");
			countLihat=0;
			CountLost++;
			if(CountLost > 300){
				if(lastdataXB<212)motion=5;
				else motion=6;
				//step++;
				Kick=1;
			}
		}
	}
	if(countgerak>batascounttendang){
		aktifkansearching();
		if(nBall==1)countLock++;
		else countLock--;

		if(countLock<=0){
			countLock=0;
			step=1;
		}else if(countLock>100){
			countLock=0;
			step=2;
		}
		//motionAct(xBall,yBall,motion,0);	
	}//else motionAct(sdtblax,sdtexcute,motion,0);	
	motionAct(xBall,yBall,motion,0);	
}

void TaktikPEksekusi(){
	flagPEX=true;
	aktifkansearching();
	flagKickOff=false;
	if(dtaYPOS==0 && dtaXPOS==0){
		if(countLihat<0){
			xBall=sdtblax;
			yBall=sdtdefy;
			DebugAI("[TaktikPEksekusi] :: Where is the ball ?");
		}else if(countLihat>190 && countLihat<=200){
			if(nBall){
				sprintf(debug_print,"[TaktikPEksekusi] :: I look the ball [%d]",countLihat);
				DebugAI(debug_print);
				flagPEX=false;
				countLihat=0;
				step=2; //ke bola
			}
		}else if(countLihat>200 && countLihat<=300){
			if (kaki==7){
				xBall=sdtblax+500;
				yBall=sdtdefy-100;
			}else if (kaki==8){
				xBall=sdtblax-500;
				yBall=sdtdefy-100;
			}else{
				xBall=sdtblax;
				yBall=sdtdefy;
			}
		}else if(countLihat>300){
			sprintf(debug_print,"[TaktikPEksekusi] :: I miss the ball [%d]",countLihat);
			DebugAI(debug_print);
			step=1; //Lostball
			flagPEX=false;
			countLihat=0;
		}
		countLihat++;
	}else{
		kakiTendang=0; //Reset kakiTendang untuk Tendang Samping
		resetOdometry=true;
	}
	motion=10;
	motionAct(xBall,yBall,motion,0);	
}
void TaktikEksekusiSamping(){
	int batascounttendang = 100;
	//int madepgawang=0;//CariArahGawang(suduteksekusi,0);
	//enablevision = 0b00001;
	if (kakiTendang==1)	motion=GerakEksekusiSampingKananwide(dataXB,dataYB,0);
	else if (kakiTendang==2) motion=GerakEksekusiSampingKiriwide(dataXB,dataYB,0);
	else step=6; //Selain itu ada kesalahan fungsi, maka tendang lurus

	if(dataXB !=0 && dataYB!=0){
		lastdataXB=dataXB;
		if(motion == 7 || motion == 8)  { 
			DebugAI("[TaktikEksekusiS] :: Bismillah, I kick the ball");
			Kick = 1; 
			kaki = motion; 
			resetOdometry=true;
			//if(arahRobot!=madepgawang){countLuruskanGawang++;motion=10;}
			//else 
			countgerak++;
	 	}else countgerak--;
		if(countgerak>batascounttendang){ 
			motion = kaki; 
			if(motion==0) motion =20; 
			countgerak = batascounttendang+1; 
			//tendangTrue=1;
		}else if(countgerak<0) countgerak=0;
		//if(countLuruskanGawang>50){step=4;countLuruskanGawang=0;motion=10;}
		CountLost = 0;
	}else{
		if(Kick==1){
			countLihat++;
			if(countLihat>200) {step++; countLihat =  countgerak = 0;}else{motion=10;}
			stepT = 1;
			DebugAI("[TaktikEksekusiS] :: The ball is kicked");
		}else{
			Kick=0;
			DebugAI("[TaktikEksekusiS] :: I'm not kicked the ball, why the ball is missing?");
			countLihat=0;
			CountLost++;
			if(CountLost > 300){
				if(lastdataXB<212)motion=5;
				else motion=6;
				step++;
				//Kick=1;
			}
		}
	}
	if(countgerak>batascounttendang){
		aktifkansearching();
		if(nBall==1)countLock++;
		else countLock--;

		if(countLock<=0){
			countLock=0;
			step=1;
		}else if(countLock>100){
			countLock=0;
			step=2;
		}
		motionAct(xBall,yBall,motion,0);	
	}else motionAct(sdtblax,sdtexcute,motion,0);	
}

void lostball_positioning(){
	int MIN_MAIN_KIRI=-4000,MAX_MAIN_KIRI=500;
	int MIN_MAIN_KANAN=-1200,MAX_MAIN_KANAN=3000;
	aktifkansearching();
	if(nBall == 1){
		countn++;
		motion=10;
		if(countn>200){	
			majulost=0;
			langkah=0;
			CountAda++;
			if((arahLihat == last_arahLihat || arahLihat == rotasiarah(arahLihat+1) || arahLihat == rotasiarah(arahLihat-1))
				&& sdtbolay>=lastsdtbolay-10 && sdtbolay<=lastsdtbolay+10){
				DebugAI("[LostballPos] :: Bola ideal");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;				
					step=2;
					flaginposition=true;
				}
			}else if(arahLihat >= 6 && arahLihat <= 15){
				DebugAI("[LostballPos] :: I look the ball beside me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step=2;
					flaginposition=true;
				}
			}else if(((1 <= arahLihat <= 5) || (16 <= arahLihat <= 20)) && sdtbolay <= 90){
				DebugAI("[LostballPos] :: Look! The ball in front of me");
				if(CountAda>50){
					countn=CountAda=0;	
					hit_arahRobot=false;
					step=2;
					flaginposition=true;
				}
			}else{
				DebugAI("[Lostball] :: I look the ball, but I don't know where it is");
				if(CountAda>50){
					countn=CountAda=0;
					hit_arahRobot=false;	
					step=2;
					flaginposition=true;
				}
			}
			motion = GerakLurusBola(sdtbolax,sdtbolay);
		}	
	}else{

		if(dtaFall==51  || dtaFall == 41)serongLost=2;
		else 
		if(dtaFall==52 || dtaFall==42)serongLost=1;
		else 
		if( dtaFall==55 || dtaFall == 45)serongLost=-2;
		else
		if(dtaFall==54 || dtaFall == 44)serongLost=-1;

		countn--;
		if(countn<=0)countn=0;
		if(mainx==KIRI){
			if(arahRobot<=20 && arahRobot>=11){ // Lepaskan robot dari kanan
				//16
				arahTendang=17;
				initialPosition=10;
				if(arahRobot>16-serongLost)motion=21;
				else
				if(arahRobot<16-serongLost)motion=22;
				else {
					if(majulost<1000){
						motion=20;
					}else if(majulost>=1000){
						majulost=1000;
						majuloss++;
					}

					if(majulost>=1000 && majuloss<0){
						motion=10;
					}else if(majulost>=1000 && majuloss>=0){
						majulost=0;
						majuloss=0;
					}
					majulost++;
				}
				if(dtaXPOS<MIN_MAIN_KIRI){
					initialPosition=9;
					step=1;
					flaginposition=true;
					majulost=majuloss=0;
				}
				sprintf(debug_print,"[LostballPos] :: Going to left [%d][%d]",dtaXPOS,MIN_MAIN_KIRI);
				DebugAI(debug_print);
			}else 
			if(arahRobot>=1 && arahRobot<11){ //Lepaskan robot dari kiri
				//6
				arahTendang=5;
				initialPosition=9;
				if(arahRobot>6-serongLost)motion=21;
				else
				if(arahRobot<6-serongLost)motion=22;
				else {
					if(majulost<1000){
						motion=20;
					}else if(majulost>=1000){
						majulost=1000;
						majuloss++;
					}

					if(majulost>=1000 && majuloss<0){
						motion=10;
					}else if(majulost>=1000 && majuloss>=0){
						majulost=0;
						majuloss=0;
					}
					majulost++;
				}
				if(dtaXPOS>MAX_MAIN_KIRI){
					initialPosition=9;
					step=1;
					flaginposition=true;
					majulost=majuloss=0;
				}
			}
			sprintf(debug_print,"[LostballPos] :: Going to left [%d][%d]",dtaXPOS,MAX_MAIN_KIRI);
			DebugAI(debug_print);
		}else
		if(mainx==KANAN){
			if(arahRobot<=20 && arahRobot>=11){ // Lepaskan robot dari kanan
				//16
				arahTendang=17;
				initialPosition=10;
				if(arahRobot>16-serongLost)motion=21;
				else
				if(arahRobot<16-serongLost)motion=22;
				else {
					if(majulost<1000){
						motion=20;
					}else if(majulost>=1000){
						majulost=1000;
						majuloss++;
					}

					if(majulost>=1000 && majuloss<0){
						motion=10;
					}else if(majulost>=1000 && majuloss>=0){
						majulost=0;
						majuloss=0;
					}
					majulost++;
				}
				if(dtaXPOS<MIN_MAIN_KANAN){
					initialPosition=10;
					step=1;
					flaginposition=true;
					majulost=majuloss=0;
				}
				sprintf(debug_print,"[LostballPos] :: Going to right [%d][%d]",dtaXPOS,MIN_MAIN_KANAN);
				DebugAI(debug_print);
			}else 
			if(arahRobot>=1 && arahRobot<11){ //Lepaskan robot dari kiri
				//6
				arahTendang=5;
				initialPosition=9;
				if(arahRobot>6-serongLost)motion=21;
				else
				if(arahRobot<6-serongLost)motion=22;
				else {
					if(majulost<1000){
						motion=20;
					}else if(majulost>=1000){
						majulost=1000;
						majuloss++;
					}

					if(majulost>=1000 && majuloss<0){
						motion=10;
					}else if(majulost>=1000 && majuloss>=0){
						majulost=0;
						majuloss=0;
					}
					majulost++;
				}
				if(dtaXPOS>MAX_MAIN_KANAN){
					initialPosition=10;
					step=1;
					flaginposition=true;
					majulost=majuloss=0;
				}
				sprintf(debug_print,"[LostballPos] :: Going to right [%d][%d]",dtaXPOS,MAX_MAIN_KANAN);
				DebugAI(debug_print);
			}
		}else{
			motion=10;
			DebugAI("[LostballPos] :: What must I do?");
		}
	}

	//fprintf(stderr, "xBall=%d | yBall=%d | motion=%d\n",xBall,yBall,motion);
	motionAct(xBall, yBall,motion, 10);
}


//Keeper ######################################
void lostball_keeping(){
	aktifkansearching();
	if(nBall==1){
		if(CountLook>200 && abs(lastErrX)<=120 && abs(lastErrY)<=70)step++;
		else if (CountLook>100)
		{
			motion=10;
		}
	}else{
		if (CountLook<=100){
			motion=0;
		}else{
			motion=10;
		}
	}
	motionAct(xBall, yBall,0, 10);
}

void Bertahan(){
	aktifkansearching();
	if(nBall == 1){ //Cover Striker
		motionAct(xBall, yBall, 0, 10);
		flagmundur=0;
		arahRobot=1;
		//fprintf(stderr,"COVER - ");
	}
	else if(nBall==0){ //Mundur sampai ke POS 2
		//xBall = sdtblax;		
		//nBall = 0;
		//yBall = sdtdefy;
		//motion = 0;
		//flagmundur=1;
		//fprintf(stderr,"MUNDUR - ");
		//counterdef++;
		//if(counterdef>1000){counterdef=0;step=0;}
	}
	//motionAct(xBall, yBall, motion, 0);
	//fprintf(stderr," POSY = %d cunter=%d\n",posrobotY,counterdef);
}

void TaktikkeBola_keeping(){

}

void TaktikEksekusi_keeping(){

}

void TaktikBacktoPos_keeping(){
	
}



















/*========================= Taktik KickOff Start ====================================*/
// void TaktikNggiringKickOff(){
// 	aktifkansearching();
// 	if (nBall == 1 ){ //&& (kaki == 20 || kaki <= 30 || kaki == 27 || kaki == 28)
// 		//kakie = 0;
// 		if(arahRobot == arahNggiring && arahRobot==arahLihat){
// 			//fprintf(stderr,"nggiring\n"); 
// 			//xBall=sdtblax;yBall=sdtexcute;
// 			//motion = GerakEksekusiDribble(dataXB,dataYB);
// 			motion = GerakTendangdekatfast(sdtbolax,sdtbolay);
// 		}else{
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahNggiring);
// 		}
// 		if(countDribb > 0 && motion >= 20)countDribb--;
// 		else if(countDribb == 0)countDribb=0;
// 		if(countDribb<=0) {
// 			if(countDribbChange>0){//CountDribbChange untuk nggiring parabola melewati obstacle
// 				if(arahNggiring>1 && arahNggiring<10)arahNggiring--;
// 				else if(arahNggiring<20 && arahNggiring>10)arahNggiring++;
// 				else if(arahNggiring==20)arahNggiring=1;
// 				else{
// 					countDribbChange=0;
// 				}
// 			}else{
// 				step++;
// 				countDribb=0;
// 			}
// 		}
// 	}
// 	else {
// 		//yBall = sdtexcute;
// 		//xBall = sdtblax;
// 		motion = 10;
// 		//CountLost++;
// 		/*if(CountLost > 200) {
// 			step = 1;
// 			CountLost = 0;
// 		}*/
// 	}
// 		fprintf(stderr,"(%d)(%d)(%d)(%d)(%d)\n",countDribb,arahNggiring,sdtbolax,sdtbolay,motion);
// 		motionAct(xBall, yBall,motion, 10);
// }

// void TaktikLuruskanKickOff(int sudutputar)
// {
// 	aktifkansearching();
// 	int madepgawang=CariArahGawang(sudutputar,0);
// 	fprintf(stderr, "mG=%d | ",madepgawang);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		//motion = Gerakluruswide(dataXB,dataYB);
// 		if(dataXB !=0 && dataYB!=0)
// 		{
// 				if(stepP==1){
// 					if(arahRobot<=20 && arahRobot>=12 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=17;
// 					else if(arahRobot<=7 && arahRobot>=1 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=18;
// 					else if(arahRobot<=20 && arahRobot>=12 && (madepgawang>=14 && madepgawang<=20 && madepgawang<arahRobot))
// 						motion=18;
// 					else if(arahRobot<=7  && arahRobot>=1  && (madepgawang>=1  && madepgawang<=7  && madepgawang>arahRobot))
// 						motion=17;
// 					else motion=10;
// 					if(lastposrobot!=arahRobot || motion==10){motion=10;CountDelay++;}
// 					if(arahRobot==madepgawang){motion=10;CountDelay++;}
// 					if(CountDelay>=10){CountDelay=0;lastposrobot=arahRobot;stepP=2;}
// 				}
// 				if(stepP==2){
// 					if(arahRobot!=arahLihat || arahRobot!=madepgawang){
// 						fprintf(stderr,"GCBE ");
// 						if(sdtbolax > 40) motion = 16;
// 						else if(sdtbolax < -40) motion = 15;
// 						else if(sdtbolax > 7) 	motion = 14;
// 						else if(sdtbolax < -7) motion = 13;
// 						else motion = 10;
// 						if ((sdtbolax>-5 && sdtbolax<5) || motion==10){CountDelay++;motion=10;}
// 						if(CountDelay>=5){stepP=1;CountDelay=0;}
// 					}
// 					if(arahRobot==madepgawang && sdtbolax>-5 && sdtbolax<5){
// 						step++;
// 						motion = 10;
// 						suduteksekusi = countLihatblg = 0;
// 						CountDelay=0;
// 					}
// 				}
// 		}
// 		CountLost=0;
// 	}
// 	else {
// 			ethreadsearching=0;
// 			xBall=sdtblax;
// 			yBall=sdtexcute;
// 			if(arahRobot<=20 && arahRobot>=12 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																				 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 				motion=17;
// 			else if(arahRobot<=7 && arahRobot>=1 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																				 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 				motion=18;
// 			else if(arahRobot<=20 && arahRobot>=12 && (madepgawang>=14 && madepgawang<=20 && madepgawang<arahRobot))
// 				motion=18;
// 			else if(arahRobot<=7  && arahRobot>=1  && (madepgawang>=1  && madepgawang<=7  && madepgawang>arahRobot))
// 				motion=17;
// 			else motion=10;
// 			if(madepgawang==arahRobot)step++;
// 			CountLost++;
// 			if(CountLost > 1000) {
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
// }
/*========================= Taktik KickOff End =====================================*/


/*========================= Taktik Serang Start ====================================*/

//Kebola dengan melihat gawang
// void TaktikkeBola4(int arahhadap)
// {
// 	int sdtXTiang;
// 	aktifkansearching();
// 	enablevision |= 0b100000;
// 	kaki = 0;
// 	//fprintf(stderr, "nB=%d | ",nBall);
// 	//if(suduteksekusi!=0)arahhadap=CariArahGawang(suduteksekusi,0);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		//------Goal Prediction Function Start
// 		if(sdtbolay>43 && posTiang ==2 && (arahLihat<=6 || arahLihat>=16) && countlihatgw<=200 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg222\n");
// 			countlihatgw++; posisiGWX = dataXG; posisiGWY = dataYG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=2){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=20 && selY>=-20)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat<=4){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=0;
// 				else if(selY<=0 && selY>=-160)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat>=16){
// 				selY=dataYB-dataYG;//dekat plus
// 				selX=dataXB-dataXG;
// 				if(selY<=30 && selY>=-30 && selX<=30 && selX>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selX>30 && selY>30)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY<-30)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY>30)suduteksekusi=-1*(int)(atan((float)abs(selY)/(float)abs(selX))*D2R) + (int)(sdtbolay/5);
// 				else if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang+18;			
// 				else suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 			}/*else if(arahLihat>=16){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=0 && selY>=-160)suduteksekusi=0;
// 				else if(sdtXTiang>0)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(20-arahLihat)*18+sdtXTiang;
// 			}*/else if(arahLihat<=6){
// 				selY=dataYB-dataYG;//dekat plus
// 				if(selY<0 && selY>=-20)suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 				else if(selY<=20 && selY>=0)suduteksekusi=(arahLihat-1)*18+sdtXTiang;
// 				else if(selY>20){suduteksekusi=(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"bariiii");}
// 				else if(selY<-20){suduteksekusi=-1*(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"baruuu");}
// 				//suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else{
// 				suduteksekusi=0;
// 			}
// 		}else
// 		if(sdtbolay>60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg111\n");
// 			countlihatgw++; posisiGWX = dataXG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=6){
// 				if(sdtXTiang<0)suduteksekusi=-28;
// 				if(sdtbolay>75 && sdtXTiang<0)suduteksekusi=-64;
// 			}
// 			else if(arahLihat>=16){
// 				if(sdtXTiang>0)suduteksekusi=28;
// 				if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=64;		
// 			}else{
// 				suduteksekusi=0;countlihatgw=0;stepK=3;
// 			}
// 		}else if(sdtbolay<60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg333\n");
// 			suduteksekusi=0;countlihatgw=0;//stepK=3;
// 		}
// 		//------Goal Prediction Function End

// 		//if(sdtbolay>43 && posTiang ==0) countlihatgw--;

// 		if(countlihatgw<0) {countlihatgw = 0;}
// 		if(countlihatgw>200) countlihatgw = 200;
// 		if(countlihatgw>80) sudahlihatGW = 2;
// 		else sudahlihatGW = 0;

// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 		if(sdtbolay <= 	35 && sdtbolax<15 && sdtbolax>-15 && sudahlihatGW == 2)
// 		{
// 			countertimeout++;
// 			fprintf(stderr,"====wes 1\n");
// 			if(countertimeout>60) { 
// 				step=4; countlihatgw=countertimeout = countLihatblg = 0;
// 			}
// 		}
// 		else if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot <= 3 || arahRobot>=19)) //arahRobot==arahhadap
// 		{
// 			countertimeout++;
// 			fprintf(stderr,"====wes 2\n");
// 			if(countertimeout>60) { step=2; countertimeout = countLihatblg = 0; motion = 10;}
// 		}
// 		else countertimeout--;

// 		if(arahLihat >= 5 && arahLihat <= 17) countLihatblg++;
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		else if(countLihatblg>50) countLihatblg=50;

// 		if(countLihatblg>=40) flagsudahdekat=0;
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			/*if(CountLost < 100){
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 			}else if(CountLost>200){
// 				xBall=sdtblax;
// 				yBall = sdtblay;
// 			}*/
// 			if(CountLost > 400) {
// 				step=0;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"sEx = %d | cG=%d \n",suduteksekusi,countlihatgw);
// }

//Kebola dengan memindah bola sedikit -- masih riset, Var: stepK
// void TaktikkeBola5(int arahhadap)
// {
// 	//int sdtXTiang;
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	//enablevision |= 0b100000;
// 	kaki = 0;
// 	//fprintf(stderr, "nB=%d | ",nBall);
// 	//if(suduteksekusi!=0)arahhadap=CariArahGawang(suduteksekusi,0);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;
// 		if(stepK==1){
// 			if(sdtbolax<20 && sdtbolax>-20)motion = GerakHadapBola(sdtbolax,sdtbolay+15,arahRobot,arahhadap);
// 			else motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);

// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=4 || arahRobot>=18)){
// 				CountDelay++;
// 				if(CountDelay>100){stepK++;CountDelay=0;}
// 			}
// 		}else if(stepK==2){
// 			if(sdtbolay <= 28 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=2 || arahRobot>=20)){
// 				countertimeout++;
// 				if(sdtbolax<0)motion=1;
// 				else if(sdtbolay>0)motion=2;
// 				else motion=30;
// 				if(countertimeout>200) { stepK++; countertimeout = countLihatblg = 0; motion = 10;}
// 			}else{
// 				stepK--;
// 			}
// 		}else if(stepK==3){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=3 || arahRobot>=19)){
// 				countertimeout++;
// 				if(countertimeout>50) { step=2; countertimeout = countLihatblg = 0; motion = 10;stepK=1;}
// 			}
// 		}
// 		else countertimeout--;

// 		if(arahLihat >= 5 && arahLihat <= 17) countLihatblg++;
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		else if(countLihatblg>50) countLihatblg=50;

// 		if(countLihatblg>=40) flagsudahdekat=0;
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			if(CountLost < 100){
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 			}/*else if(CountLost>200){
// 				xBall=sdtblax;
// 				yBall = sdtblay;
// 			}*/
// 			if(CountLost > 200) {
// 				step=0;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"sK= %d\n",stepK);
// }

//TaktikkeBla4 + TaktikkeBla5
// void TaktikkeBola6(int arahhadap){
// 	int sdtXTiang;
// 	aktifkansearching();
// 	enablevision |= 0b100000;
// 	kaki = 0;
// 	//fprintf(stderr, "nB=%d | ",nBall);
// 	//if(suduteksekusi!=0)arahhadap=CariArahGawang(suduteksekusi,0);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		if(sdtbolay>43 && posTiang ==2 && (arahLihat<=6 || arahLihat>=16) && countlihatgw<=200 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg222\n");
// 			countlihatgw++; posisiGWX = dataXG; posisiGWY = dataYG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=2){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=20 && selY>=-20)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat<=4){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=0;
// 				else if(selY<=0 && selY>=-160)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat>=16){
// 				selY=dataYB-dataYG;//dekat plus
// 				selX=dataXB-dataXG;
// 				if(selY<=30 && selY>=-30 && selX<=30 && selX>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selX>30 && selY>30)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY<-30)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY>30)suduteksekusi=-1*(int)(atan((float)abs(selY)/(float)abs(selX))*D2R) + (int)(sdtbolay/5);
// 				else if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang+18;			
// 				else suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 			}/*else if(arahLihat>=16){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=0 && selY>=-160)suduteksekusi=0;
// 				else if(sdtXTiang>0)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(20-arahLihat)*18+sdtXTiang;
// 			}*/else if(arahLihat<=6){
// 				selY=dataYB-dataYG;//dekat plus
// 				if(selY<0 && selY>=-20)suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 				else if(selY<=20 && selY>=0)suduteksekusi=(arahLihat-1)*18+sdtXTiang;
// 				else if(selY>20){suduteksekusi=(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"bariiii");}
// 				else if(selY<-20){suduteksekusi=-1*(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"baruuu");}
// 				//suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else{
// 				suduteksekusi=0;
// 			}
// 		}else
// 		if(sdtbolay>60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg111\n");
// 			countlihatgw++; posisiGWX = dataXG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=6){
// 				if(sdtXTiang<0)suduteksekusi=-28;
// 				if(sdtbolay>75 && sdtXTiang<0)suduteksekusi=-64;
// 			}
// 			else if(arahLihat>=16){
// 				if(sdtXTiang>0)suduteksekusi=28;
// 				if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=64;		
// 			}else{
// 				suduteksekusi=0;countlihatgw=0;stepK=3;
// 			}
// 		}else if(sdtbolay<60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg333\n");
// 			suduteksekusi=0;countlihatgw=0;stepK=3;
// 		}
// 		//if(sdtbolay>43 && posTiang ==0) countlihatgw--;

// 		if(countlihatgw<0) {countlihatgw = 0;}
// 		if(countlihatgw>200) countlihatgw = 200;
// 		if(countlihatgw>80) sudahlihatGW = 2;
// 		else sudahlihatGW = 0;
		
// 		if(arahLihat>=9 && arahLihat<=13){sudahlihatGW=0;suduteksekusi=0;}		

// 		if(stepK==1){
// 			if(sudahlihatGW==2){
// 				arahhadap=CariArahGawang(suduteksekusi,0);
// 				motion = GerakHadapBola(sdtbolax,sdtbolay+15,arahRobot,arahhadap);
// 				if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahhadap==arahRobot || arahRobot==rotasiarah(arahhadap-1) 
// 																						  || arahRobot==rotasiarah(arahhadap+1))){
// 					CountDelay++;
// 					if(CountDelay>100){stepK++;CountDelay=0;}
// 					fprintf(stderr,"iki.. %d",CountDelay);
// 				}
// 				fprintf(stderr,"ikaa..");
// 			}else{
// 				motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 				if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=3 || arahRobot>=19)){
// 					CountDelay++;
// 					if(CountDelay>50){stepK=3;CountDelay=0;}
// 				}
// 			}
// 		}else if(stepK==2){
// 			if(sdtbolay <= 28 && sdtbolax<15 && sdtbolax>-15){
// 				countertimeout++;
// 				if(countertimeout<100)motion=10;
// 				else if(sdtbolax<0)motion=1;
// 				else if(sdtbolax>0)motion=2;
// 				else motion=30;
// 				if(countertimeout>200) { stepK++; countertimeout = countLihatblg = 0; motion = 10;}
// 			}else{
// 				stepK--;
// 			}
// 		}else if(stepK==3){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			arahhadap=CariArahGawang(suduteksekusi,0);
// 			if(sdtbolay <= 	35 && sdtbolax<15 && sdtbolax>-15 && sudahlihatGW == 2 && (arahhadap==arahRobot || arahRobot==rotasiarah(arahhadap-1) 
// 																						  || arahRobot==rotasiarah(arahhadap+1))){
// 				countertimeout++;
// 				fprintf(stderr,"====wes 1\n");
// 				if(countertimeout>60) { 
// 					step=4; countlihatgw=countertimeout = countLihatblg = 0; stepK=1;
// 				}
// 			}
// 			else if(sdtbolay <= 35 && sdtbolax<15 && sdtbolax>-15 && (arahRobot <= 3 || arahRobot>=19)) //arahRobot==arahhadap
// 			{
// 				countertimeout++;
// 				fprintf(stderr,"====wes 2\n");
// 				if(countertimeout>60) { step=2; countertimeout = countLihatblg = 0; motion = 10; stepK=1;}
// 			}else{
// 				CountDelay++;
// 				if(CountDelay>50){CountDelay=0;stepK=1;}
// 			}
// 		}else countertimeout--;
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			if(CountLost > 200) {
// 				step=0;
// 				stepK=1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 				countlihatgw=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"cG=%d | sK=%d | ct=%d \n",countlihatgw,stepK,countertimeout);
// }

// void TaktikkeBola7(int arahhadap)
// {
// 	//int sdtXTiang;
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	//enablevision |= 0b100000;
// 	kaki = 0;
// 	//fprintf(stderr, "nB=%d | ",nBall);
// 	//if(suduteksekusi!=0)arahhadap=CariArahGawang(suduteksekusi,0);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;
		
// 		if(sdtbolay<=50 && sdtbolay>35 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=4 || arahRobot>=18)){
// 			fprintf(stderr,"masuukk");
// 			unsigned char posisiobs;
// 			posisiobs = checkobstacle(0);
// 			if((posisiobs & 0b010) > 0 && flagaktifkanobs) {
// 				fprintf(stderr,"Ada Obs di Tengah !\n");
// 				counterobs2++;
// 				if(counterobs2 >100) {
// 					counterobs2 = 0;
// 					if(stepK!=5)stepK=4;			
// 				}
// 			}
// 		}		

// 		if(stepK==1){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay+15,arahRobot,arahhadap);
// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=4 || arahRobot>=18)){
// 				CountDelay++;
// 				if(CountDelay>100){stepK++;CountDelay=0;}
// 			}
// 		}else if(stepK==2){
// 			if(sdtbolay <= 28 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=2 || arahRobot>=20)){
// 				countertimeout++;
// 				if(sdtbolax<0)motion=1;
// 				else if(sdtbolay>0)motion=2;
// 				else motion=30;
// 				if(countertimeout>200) { stepK++; countertimeout = countLihatblg = 0; motion = 10;}
// 			}else{
// 				stepK--;
// 			}
// 		}else if(stepK==3){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=3 || arahRobot>=19)){
// 				countertimeout++;
// 				if(countertimeout>50) { step=2; countertimeout = countLihatblg = 0; motion = 10;stepK=1;}
// 			}
// 		}else if(stepK==4){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=4 || arahRobot>=18)){
// 				countertimeout++;
// 				if(countertimeout>50) { countertimeout = countLihatblg = 0; motion = 10;stepK=5;}
// 			}
// 		}else if(stepK==5){
// 			xBall=sdtblax;
// 			yBall=sdtexcute;
// 			if(arahRobot<=5)motion = GerakAvoidFast2(dataXB,dataYB,1);
// 			else motion = GerakAvoidFast2(dataXB,dataYB,0);
// 			if(motion==7 || motion==8)countertimeout++;
// 			if(countertimeout>=30){stepK=1; countertimeout=0;}
// 		}
// 		else countertimeout--;

// 		if(arahLihat >= 5 && arahLihat <= 17) countLihatblg++;
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		else if(countLihatblg>50) countLihatblg=50;

// 		if(countLihatblg>=40) flagsudahdekat=0;
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			if(CountLost < 100){
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				stepK=1;
// 			}/*else if(CountLost>200){
// 				xBall=sdtblax;
// 				yBall = sdtblay;
// 			}*/
// 			if(CountLost > 200) {
// 				step=0;
// 				stepK=1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"sK= %d\n",stepK);
// }

//Taktikkebola6(LihatGawang + Giring) + Taktikkebola7(Avoid Obs)
// void TaktikkeBola8(int arahhadap){
// 	int sdtXTiang;
// 	aktifkansearching();
// 	enablevision |= 0b100000;
// 	kaki = 0;
// 	//fprintf(stderr, "nB=%d | ",nBall);
// 	//if(suduteksekusi!=0)arahhadap=CariArahGawang(suduteksekusi,0);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		if(sdtbolay<=50 && sdtbolay>25 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=4 || arahRobot>=18)){
// 			fprintf(stderr,"masuukk");
// 			unsigned char posisiobs;
// 			posisiobs = checkobstacle(0);
// 			if((posisiobs & 0b010) > 0 && flagaktifkanobs) {
// 				fprintf(stderr,"Ada Obs di Tengah !\n");
// 				counterobs2++;
// 				if(counterobs2 >100) {
// 					counterobs2 = 0;
// 					if(stepK!=5)stepK=4;			
// 				}
// 			}
// 		}

// 		if(sdtbolay>43 && posTiang ==2 && (arahLihat<=6 || arahLihat>=16) && countlihatgw<=200 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg222\n");
// 			countlihatgw++; posisiGWX = dataXG; posisiGWY = dataYG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=2){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=20 && selY>=-20)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat<=4){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=0;
// 				else if(selY<=0 && selY>=-160)suduteksekusi=InvSudut(arahLihat);
// 				else if(sdtXTiang>0)suduteksekusi=(arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else if(arahLihat>=16){
// 				selY=dataYB-dataYG;//dekat plus
// 				selX=dataXB-dataXG;
// 				if(selY<=30 && selY>=-30 && selX<=30 && selX>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selX>30 && selY>30)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY<-30)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else if(selX<-30 && selY>30)suduteksekusi=-1*(int)(atan((float)abs(selY)/(float)abs(selX))*D2R) + (int)(sdtbolay/5);
// 				else if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang+18;			
// 				else suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;
// 			}/*else if(arahLihat>=16){
// 				selY=dataXB-dataXG;//dekat plus
// 				if(selY<=30 && selY>=-30)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=160 && selY>=0)suduteksekusi=InvSudut(arahLihat);
// 				else if(selY<=0 && selY>=-160)suduteksekusi=0;
// 				else if(sdtXTiang>0)suduteksekusi=(21-arahLihat)*18+sdtXTiang;
// 				else suduteksekusi=-1*(20-arahLihat)*18+sdtXTiang;
// 			}*/else if(arahLihat<=6){
// 				selY=dataYB-dataYG;//dekat plus
// 				if(selY<0 && selY>=-20)suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 				else if(selY<=20 && selY>=0)suduteksekusi=(arahLihat-1)*18+sdtXTiang;
// 				else if(selY>20){suduteksekusi=(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"bariiii");}
// 				else if(selY<-20){suduteksekusi=-1*(int)(atan((float)abs(dataYB-dataYG)/(float)abs(dataXB-dataXG))*D2R);fprintf(stderr,"baruuu");}
// 				//suduteksekusi=-1*(arahLihat-1)*18+sdtXTiang;
// 			}
// 			else{
// 				suduteksekusi=0;
// 			}
// 		}else
// 		if(sdtbolay>60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg111\n");
// 			countlihatgw++; posisiGWX = dataXG;
// 			sdtXTiang=(posisiGWX-frametengah)/5;
// 			if(arahLihat<=6){
// 				if(sdtXTiang<0)suduteksekusi=-28;
// 				if(sdtbolay>75 && sdtXTiang<0)suduteksekusi=-64;
// 			}
// 			else if(arahLihat>=16){
// 				if(sdtXTiang>0)suduteksekusi=28;
// 				if(sdtbolay>75 && sdtXTiang>0)suduteksekusi=64;		
// 			}else{
// 				suduteksekusi=0;countlihatgw=0;stepK=3;
// 			}
// 		}else if(sdtbolay<60 && posTiang ==1 && ((arahLihat>=3 && arahLihat<=6) || (arahLihat<=19 && arahLihat>=16)) 
// 			&& countlihatgw<100 && dataXB!=0 && dataYB!=0) { 
// 			fprintf(stderr,"====Lihat Gawangg333\n");
// 			suduteksekusi=0;countlihatgw=0;stepK=3;
// 		}
// 		//if(sdtbolay>43 && posTiang ==0) countlihatgw--;

// 		if(countlihatgw<0) {countlihatgw = 0;}
// 		if(countlihatgw>200) countlihatgw = 200;
// 		if(countlihatgw>80) sudahlihatGW = 2;
// 		else sudahlihatGW = 0;
		
// 		if(arahLihat>=9 && arahLihat<=13){sudahlihatGW=0;suduteksekusi=0;}		

// 		if(stepK==1){
// 			if(sudahlihatGW==2){
// 				arahhadap=CariArahGawang(suduteksekusi,0);
// 				motion = GerakHadapBola(sdtbolax,sdtbolay+15,arahRobot,arahhadap);
// 				if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahhadap==arahRobot || arahRobot==rotasiarah(arahhadap-1) 
// 																						  || arahRobot==rotasiarah(arahhadap+1))){
// 					CountDelay++;
// 					if(CountDelay>100){stepK++;CountDelay=0;}
// 					fprintf(stderr,"iki.. %d",CountDelay);
// 				}
// 				fprintf(stderr,"ikaa..");
// 			}else{
// 				motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 				if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=3 || arahRobot>=19)){
// 					CountDelay++;
// 					if(CountDelay>50){stepK=3;CountDelay=0;}
// 				}
// 			}
// 		}else if(stepK==2){
// 			arahhadap=CariArahGawang(suduteksekusi,0);
// 			if(sdtbolay <= 28 && sdtbolax<15 && sdtbolax>-15 && (arahhadap==arahRobot || arahRobot==rotasiarah(arahhadap-1) 
// 																					  || arahRobot==rotasiarah(arahhadap+1))){
// 				countertimeout++;
// 				if(sdtbolax<0)motion=1;
// 				else if(sdtbolax>0)motion=2;
// 				else motion=30;
// 				if(countertimeout>200) { stepK++; countertimeout = countLihatblg = 0; motion = 10;}
// 			}else{
// 				stepK--;
// 			}
// 		}else if(stepK==3){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			arahhadap=CariArahGawang(suduteksekusi,0);
// 			if(sdtbolay <= 	35 && sdtbolax<15 && sdtbolax>-15 && sudahlihatGW == 2 && (arahhadap==arahRobot || arahRobot==rotasiarah(arahhadap-1) 
// 																						  || arahRobot==rotasiarah(arahhadap+1))){
// 				countertimeout++;
// 				fprintf(stderr,"====wes 1\n");
// 				if(countertimeout>60) { 
// 					step=4; countlihatgw=countertimeout = countLihatblg = 0; stepK=1;
// 				}
// 			}
// 			else if(sdtbolay <= 35 && sdtbolax<15 && sdtbolax>-15 && (arahRobot <= 3 || arahRobot>=19)) //arahRobot==arahhadap
// 			{
// 				countertimeout++;
// 				fprintf(stderr,"====wes 2\n");
// 				if(countertimeout>60) { step=2; countertimeout = countLihatblg = 0; motion = 10; stepK=1;}
// 			}else{
// 				CountDelay++;
// 				if(CountDelay>50){CountDelay=0;stepK=1;}
// 			}
// 		}else if(stepK==4){
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			if(sdtbolay <= 30 && sdtbolax<15 && sdtbolax>-15 && (arahRobot<=3 || arahRobot>=19)){
// 				countertimeout++;
// 				if(countertimeout>50) { countertimeout = countLihatblg = 0; motion = 10;stepK=5;}
// 			}
// 		}else if(stepK==5){
// 			xBall=sdtblax;
// 			yBall=sdtexcute;
// 			if(arahRobot<=5)motion = GerakAvoidFast2(dataXB,dataYB,1);
// 			else motion = GerakAvoidFast2(dataXB,dataYB,0);
// 			if(motion==7 || motion==8)countertimeout++;
// 			if(countertimeout>=30){stepK=1; countertimeout=0;}
// 		}else countertimeout--;
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			if(CountLost > 200) {
// 				step=0;
// 				stepK=1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 				countlihatgw=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"cG=%d | sK=%d | ct=%d \n",countlihatgw,stepK,countertimeout);
// }

// void Taktikcekjarak3()
// {
// 	enablevision = 0b1110000;
// 	ethreadsearching = 0;
// 	int gensdt;
// 	if(arahRobot==1)gensdt=sdtblax;
// 	else if(arahRobot<11)gensdt=generatesdtX(-1*(arahRobot-1)*18);
// 	else gensdt=generatesdtX((21-arahRobot)*18);
// 	yBall = sdtdefy;
// 	xBall = gensdt;
// 	motion = 0;
// 	if(CountUlang > 0) 
// 	{
// 		step = 4;
// 		suduteksekusi = 0;
// 	}
// 	else
// 	{
// 		//if(countertimeout>250) { step++; stepG=1; countertimeout = 0; }
// 		if(posTiang == 2 && countertimeout > 150)
// 		{

// 			step=6;
// 			countertimeout = 0;
// 			suduteksekusi=0;
// 			fprintf(stderr,"Satu \n");
// 		}
// 		else if(posrobotY==4 && countertimeout>200)
// 		{
// 			step = 4;
// 			suduteksekusi = 0;
// 			countDribb=5000; //edit sesuai kebutuhan
// 			countertimeout = 0;
// 			xBall = lastXBall;
// 			yBall = lastYBall;
// 			flagsudahdekat = 0;
// 			CountUlang = 1;
// 			fprintf(stderr,"Dua \n");
// 		}
// 		else if(posrobotY>=5 && countertimeout>200)
// 		{
// 			step = 4;
// 			suduteksekusi = 0;
// 			countDribb=5000; //edit sesuai kebutuhan
// 			countertimeout = 0;
// 			xBall = lastXBall;
// 			yBall = lastYBall;
// 			flagsudahdekat = 0;
// 			CountUlang = 2;
// 			fprintf(stderr,"Tiga \n");
// 		}
// 		else if(posrobotY<4 && countertimeout>200)
// 		{
// 			flagsudahdekat = 1;
// 			step=3;
// 			countertimeout = 0;
// 			stepG = 1;
// 			fprintf(stderr,"Empat \n");
// 		}

// 		countertimeout++;
// 	}
// 	//step=2;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"posY : %d | CT : %d | %d",posrobotY,countertimeout,gensdt);
// }

// void TaktikcekGW_wide()
// {
// 	static short suduttolehx,arahtoleh;
// 	static int countertimeouttmbh = 0;
// 	static short hasilgw[3];
// 	static unsigned char simpanposlihat[3];
// 	static unsigned char simpanobs[3];
// 	static int sudutY;
// 	static int sudutX = 73; // sudutY = 75;
// 	int batascountertimeout = 80, bataslihat = 35, delaytoleh = 35;
// 	int batasjarakjauh;
// 	int posisiobs;

// 	batasjarakjauh=5;
// 	//if(stepG==1) enablevision = 0b1100000;
// 	//else enablevision = 0b100000;
// 	sudutY = carisudutbolay(sdtdefy);
// 	enablevision = 0b0110100;//0b1100000;
// 	if(player == 3 || player == 4) {batascountertimeout = 180; bataslihat = 70; delaytoleh = 70;} //sudutY = 83; }
// 	if(player == 2 || player == 1) {batascountertimeout = 180; bataslihat = 70; delaytoleh = 50;} //sudutY = 79; }
// 	ethreadsearching = 0;

// 	if(datakompc!=0 && stepG<4 && stepG>1) {
// 		stepG = 4;
// 		countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 	}

// 	if(stepG == 1)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);
// 		xBall = sdtblax;

// 		//posisiobs = checkobstacle(180);
// 		posisiobs = checkobstacle(0);

// 		if((posisiobs & 0b010) > 0 && flagaktifkanobs) {
// 			fprintf(stderr,"Ada Obs di tengah !\n");
// 			counterobseks++;
// 			if(counterobseks >100) {
// 				counterobseks = 100;
// 				suduteksekusi = countLihatblg = 0;
// 				step=4;
// 				arhRbt2obs=arahRobot;
// 			}
// 			//else modeeksekusi = modetendang;
// 		}
// 		else counterobseks--;
// 		if(counterobseks<0) counterobseks = 0;

// 		if(posrobotY>batasjarakjauh && flagsudahdekat==0) //kalo masih jauh
// 		{
// 			if(posTiang == 2) { counterpos2++; countertimeouttmbh+=2; }
// 			if (counterpos2 > bataslihat) { simpanposisiGW = 2; hasilgw[1] = 2; sudutTiang[1] = (dataXG-frametengah)/5;} //X2X

// 			if(countertimeout>batascountertimeout)
// 			{
// 				//araheksekusi =1;
// 				//suduteksekusi = 0;
// 				fprintf(stderr,"ikih");
// 				piliheksekusi = 2;
// 				stepG=4;
// 				countertimeout = 0;
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				countDribb = 1500;
// 			}
// 		}
// 		else
// 		{
// 			if(posTiang == 1) { counterpos1++; countertimeouttmbh+=2; }
// 			else if(posTiang == 2) { counterpos2++; countertimeouttmbh+=2; }
// 			if(dataOBS[0]!=0) {counterobs++; fprintf(stderr,"cobs (%d) ",counterobs);}

// 			if (counterpos2 > bataslihat) { simpanposisiGW = 2; hasilgw[1] = 2; sudutTiang[1] = (dataXG-frametengah)/5;} //X2X
// 			else if(counterpos1 > bataslihat) { simpanposisiGW = 1; hasilgw[1] = 1; sudutTiang[1] = (dataXG-frametengah)/5; if(sudutxTiang>0) arahtoleh=kiri; 				else arahtoleh=kanan;} //X1X
// 			else if(countertimeout>batascountertimeout) { simpanposisiGW = 0; hasilgw[1] = 0; sudutTiang[1] = 0;}  //X0X

// 			if(countertimeout>batascountertimeout+countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat) {
// 				if(posrobotY == 1) sudutX = 55;
// 				else if(posrobotY == 2) sudutX = 50;
// 				else sudutX = 40;
// 				simpanposlihat[1]= jrkGwang;
// 				if(arahtoleh==kiri) suduttolehx = -sudutX; else suduttolehx = sudutX;
// 				flagsudahdekat = 1;
// 				if(counterobs > countertimeout/2) simpanobs[1] = 1; else simpanobs[1] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;

// 			}
// 			piliheksekusi = 1;
// 		}
// 		countertimeout++;
// 		simpanposrobotY = posrobotY;
// 		fprintf(stderr,"RobotY (%d) ",posrobotY);
// 	}
// 	else if(stepG == 2)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);

// 		if(posTiang == 1 && countertimeout>delaytoleh) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>delaytoleh) { counterpos2++; countertimeouttmbh+=2; }
// 		if(dataOBS[0]!=0 && countertimeout>delaytoleh) {counterobs++; fprintf(stderr,"cobs (%d) ",counterobs);}

// 		countertimeout++;

// 		if(simpanposisiGW < 2)
// 		{
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > bataslihat) { simpanposisiGW2 = hasilgw[arahtoleh] = 2; sudutTiang[arahtoleh] = suduttolehx + (dataXG-frametengah)/5;}
// 			else if(counterpos1 > bataslihat) { simpanposisiGW2 = hasilgw[arahtoleh] = 1; sudutTiang[arahtoleh] = suduttolehx + ((dataXG-frametengah)/5);}
// 			else if(countertimeout>batascountertimeout) { simpanposisiGW2 = hasilgw[arahtoleh] =  sudutTiang[arahtoleh] = 0; }


// 			if(countertimeout>batascountertimeout + countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat)
// 			{
// 				if(counterobs > ((countertimeout-delaytoleh)*3/4)) simpanobs[arahtoleh] = 1; else simpanobs[arahtoleh] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 				simpanposlihat[arahtoleh]= jrkGwang;
// 				if(simpanposisiGW > 0 && simpanposisiGW2 >0) simpanposisiGW2 = 2;
// 				if(arahtoleh==kiri) arahtoleh = kanan; else arahtoleh = kiri;
// 				if(arahtoleh==kiri) suduttolehx = -sudutX; else suduttolehx = sudutX;
// 			}
// 		}
// 		else if(simpanposisiGW >= 2)
// 		{
// 			simpanposisiGW2 = 2;
// 			countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 			stepG++;
// 		}
// 	}
// 	else if(stepG == 3)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);

// 		if(posTiang == 1 && countertimeout>delaytoleh) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>delaytoleh) { counterpos2++; countertimeouttmbh+=2; }
// 		if(dataOBS[0]!=0 && countertimeout>delaytoleh) {counterobs++;}//fprintf(stderr,"cobs (%d) ",counterobs);}

// 		countertimeout++;

// 		if(simpanposisiGW2 < 2)
// 		{
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > bataslihat) { hasilgw[arahtoleh] = 2; sudutTiang[arahtoleh] = suduttolehx + (dataXG-frametengah)/5;}
// 			else if(counterpos1 > bataslihat) { hasilgw[arahtoleh] = 1; sudutTiang[arahtoleh] = suduttolehx + ((dataXG-frametengah)/5);}
// 			else if(countertimeout>batascountertimeout) { hasilgw[arahtoleh] = sudutTiang[arahtoleh] = 0; }

// 			if(countertimeout>batascountertimeout + countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat)
// 			{
// 				if(counterobs > ((countertimeout-delaytoleh)*3/4)) simpanobs[arahtoleh] = 1; else simpanobs[arahtoleh] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 				simpanposlihat[arahtoleh]= jrkGwang;
// 			}
// 		}
// 		else if(simpanposisiGW2 >= 2)
// 		{
// 			countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 			stepG++;
// 		}
// 	}
// 	else if(stepG == 4)
// 	{
// 		motion = 10;
// 		suduteksekusi = 0;

// 		for(int i = 0; i<3; i++)
// 		{
// 			if(hasilgw[i] == 1) {
// 				suduteksekusi = sudutTiang[i]*3/2; fprintf(stderr,"Lihat1gwng \n");

// 				if(i == 1) {
// 					suduteksekusi = sudutTiang[i];
// 					if(simpanobs[0] != 0) suduteksekusi-= 20;
// 					if(simpanobs[2] != 0) suduteksekusi+= 20;
// 					fprintf(stderr,"Lihat1gwngdtengah \n");
// 					fprintf(stderr,"obskiri : %d | obskanan : %d | \n",simpanobs[0],simpanobs[2]);

// 					/*if(simpanposlihat[0] > simpanposlihat[2]) suduteksekusi = sudutTiang[i]-20;
// 					else if(simpanposlihat[0] < simpanposlihat[2]) suduteksekusi = sudutTiang[i]+20;
// 					fprintf(stderr,"Lihat1gwngdtengah \n");
// 					fprintf(stderr,"jarakkiri : %d | jarakkanan : %d | \n",simpanposlihat[0],simpanposlihat[2]);
// 					*/
// 				}
// 			}
// 		}


// 		for(int i = 0; i<2; i++)
// 		{
// 			for(int j = i+1; j<3; j++)
// 			{
// 				if(hasilgw[i] == 1 && hasilgw[j] == 1) {suduteksekusi = (sudutTiang[i]  + sudutTiang[j]) / 2; fprintf(stderr,"Lihat1dan1 \n"); break;}
// 			}
// 		}

// 		for(int i = 0; i<3 ; i++)
// 		{
// 			if(hasilgw[i] == 2) {
// 				suduteksekusi = sudutTiang[i]; if(posrobotY<2) {if(suduteksekusi>0) suduteksekusi+=5; else suduteksekusi-=5;}   
// 				fprintf(stderr,"lihat2langsung \n"); break;
// 			} // Lihat 2 Tiang langsung
// 		}
// 		if(hasilgw[0]>=1 && hasilgw[2]>=1 && hasilgw[1]==0){suduteksekusi=0;step=6;}

// 		if(suduteksekusi > 90){ suduteksekusi+=5; disampinggw = 1;}
// 		else if(suduteksekusi < -90){ suduteksekusi-=5; disampinggw = 2;}
// 		else disampinggw = 0;

// 		if(suduteksekusi > 90) suduteksekusi = 84;
// 		else if(suduteksekusi <-90) suduteksekusi =-84;

// 		switch(datakompc)
// 		{
// 			case 1: suduteksekusi = 80; break;
// 			case 2: suduteksekusi = 60; break;
// 			case 3: suduteksekusi = 40; break;
// 			case 4: suduteksekusi = 20; break;
// 			case 5: suduteksekusi = 0; break;
// 			case 6: suduteksekusi = -20; break;
// 			case 7: suduteksekusi = -40; break;
// 			case 8: suduteksekusi = -60; break;
// 			case 9: suduteksekusi = -80; break;
// 		}
		
// 		//Tambahan		
// 		if(arahRobot<=5 && suduteksekusi<0){suduteksekusi+=(arahRobot-1)*18;}
// 		else if(arahRobot>=16 && suduteksekusi>0){suduteksekusi-=(20-arahRobot)*18;}

// 		fprintf(stderr,"======================================================");
// 		for(int i = 0; i<3 ; i++) {fprintf(stderr,"%d(%d)(%d) ",i,hasilgw[i],sudutTiang[i]); hasilgw[i] = sudutTiang[i] = simpanposlihat[i] = 0;}
// 		fprintf(stderr,"SE(%d)PC(%d)\n",suduteksekusi,datakompc);
// 		fprintf(stderr,"======================================================");

// 		stepG++;
// 	}
// 	else if(stepG == 5)
// 	{
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 		motion = 10;
// 		countertimeout++;
// 		simpansuduteksekusi = suduteksekusi;
// 		heading_skr = countLihatblg = 0;
// 		if(player == 2 || player == 3) {if(countertimeout>233) {countertimeout = stepG = 1; step++; }}
// 		else if(countertimeout>33) {countertimeout = stepG = 1; step++; }
// 	}
// 	fprintf(stderr,"StepG(%d)Pos(%d)dataXG(%d)eks(%d)pc(%d)\n",stepG,posTiang,dataXG,suduteksekusi,datakompc);
// 	araheksekusi = cariarahRobot(suduteksekusi, myGcol);
// 	motionAct(xBall,yBall,0,0);
// }


//Jika bola hilang autoregenerate motion 17 & 18 dengan membandingkan arahRobot
// void TaktikLuruskanGW_wide4(int sudutputar)
// {
// 	aktifkansearching();
// 	int madepgawang=CariArahGawang(sudutputar,0);
// 	if(sudutputar==0)madepgawang=1;
// 	if(counterobseks>=100 && posrobotY<=2 && sudutputar==0){//ada obstacle, gawang gak kelihatan
// 		if(arhRbt2obs<=20 || arhRbt2obs>11)madepgawang=3;
// 		else if(arhRbt2obs<=11 || arhRbt2obs>=1)madepgawang=19;
// 		else madepgawang=3;
// 	}else if(counterobseks>=100 && posrobotY<=2 && sudutputar!=0){ //ada obstacle lurus gawang, posisi robot dekat gawang
// 		counterobseks=0; 
// 	}else if(counterobseks>=100 && posrobotY>2)madepgawang=3; //ada obs tapi jauh dari gawang

// 	if(madepgawang>4 && madepgawang<=10)madepgawang=4;
// 	else if(madepgawang<18 && madepgawang>10)madepgawang=18;
// 	fprintf(stderr, "mG=%d | ",madepgawang);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		//motion = Gerakluruswide(dataXB,dataYB);
// 		if(dataXB !=0 && dataYB!=0)
// 		{
// 				if(stepP==1){
// 					if(arahRobot<=20 && arahRobot>=12 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=17;
// 					else if(arahRobot<=7 && arahRobot>=1 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=18;
// 					else if(arahRobot<=20 && arahRobot>=12 && (madepgawang>=14 && madepgawang<=20 && madepgawang<arahRobot))
// 						motion=18;
// 					else if(arahRobot<=7  && arahRobot>=1  && (madepgawang>=1  && madepgawang<=7  && madepgawang>arahRobot))
// 						motion=17;
// 					if(lastposrobot!=arahRobot || motion==10){CountDelay++;}
// 					if(arahRobot==madepgawang){motion=10;CountDelay++;}
// 					if(CountDelay>=10){CountDelay=0;lastposrobot=arahRobot;stepP=2;}
// 				}
// 				if(stepP==2){
// 					if(arahRobot!=arahLihat || arahRobot!=madepgawang){
// 						fprintf(stderr,"GCBE ");
// 						if(sdtbolax > 40) motion = 14;
// 						else if(sdtbolax < -40) motion = 13;
// 						else if(sdtbolax > 7) 	motion = 14;
// 						else if(sdtbolax < -7) motion = 13;
// 						else motion = 10;
// 						if ((sdtbolax>-5 && sdtbolax<5) || motion==10){CountDelay++;motion=10;}
// 						if(CountDelay>=5){stepP=1;CountDelay=0;}
// 					}
// 					motion=10;CountDelay++;
// 					if(arahRobot!=madepgawang && CountDelay>20){stepP=1; CountDelay=0;}
// 					if(arahRobot==madepgawang){
// 						step=6;
// 						motion = 10;
// 						CountDelay=0;
// 					}
// 				}
// 		}
// 		CountLost=0;
// 		if(sdtbolay>43 || sdtbolax > 40 || sdtbolax < -40) {
// 			countLihat++;
// 			if(countLihat>200) {step = 1; countLihat=0;}
// 		}else{
// 			countLihat=0;
// 		}
// 	}
// 	else {
// 			ethreadsearching=0;
// 			//xBall=sdtblax;
// 			//yBall=sdtexcute;
// 			if(arahRobot<=20 && arahRobot>=12 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																				 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 				motion=17;
// 			else if(arahRobot<=7 && arahRobot>=1 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																				 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 				motion=18;
// 			else if(arahRobot<=20 && arahRobot>=12 && (madepgawang>=14 && madepgawang<=20 && madepgawang<arahRobot))
// 				motion=18;
// 			else if(arahRobot<=7  && arahRobot>=1  && (madepgawang>=1  && madepgawang<=7  && madepgawang>arahRobot))
// 				motion=17;
// 			else motion=10;
// 			if(madepgawang==arahRobot)step++;
// 			/*CountLost++;
// 			if(CountLost > 200) {
// 				//step = 1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}*/
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
// }

// void TaktikEksekusi3()
// {
// 	int batascounttendang = 50;
// 	int madepgawang=CariArahGawang(suduteksekusi,0);
// 	//nBall = ftBall3(dataXB, dataYB, 0);
// 	fprintf(stderr, "nB = %d | ",nBall);
// 	enablevision = 0b00001;
// 	ethreadsearching=0;
// 	if(dataXB !=0 || dataYB !=0) nBall=1;
// 	else nBall=0;
// 	if(nBall==1)
// 	{
// 		//motion = GerakTendangjauhfast(sdtbolax,sdtbolay);
// 		motion=GerakEksekusiwide(dataXB,dataYB,0);
// 		if(motion == 5 || motion == 6)  { 
// 			Kick = 1; 
// 			kaki = motion; 
// 			countgerak++;
// 			if(arahRobot!=madepgawang)countLuruskanGawang++;
// 	 	}
// 		else countgerak--;
// 		if(countLuruskanGawang>20){step=4;countLuruskanGawang=0;}
// 		if(countgerak>batascounttendang) { motion = kaki; if(motion==0) motion =20; countgerak = batascounttendang+1; tendangTrue=1;}
// 		else if(countgerak<0) countgerak=0;

// 		/*if(sdtbolay>60 || sdtbolax > 40 || sdtbolax < -40) {
// 			countLihat++;
// 			if(countLihat>400) {step = 1; countLihat=0;}
// 		}else if(sdtbolay>40 || sdtbolax > 25 || sdtbolax < -25) {
// 			countLihat++;
// 			if(countLihat>200) {motion=20; countLihat=0;}
// 		}*/
// 		CountLost = 0;
// 		//if(arahRobot>6 && arahRobot<8 && sdtbolay<30) {step=1; countLihat=0;}
// 	}
// 	else
// 	{
// 		if(Kick==1)
// 		{

// 			countLihat++;
// 			if(countLihat>200) {step++; countLihat =  countgerak = 0;}else{motion=0;}
// 			stepT = 1;
// 			fprintf(stderr,"TES1!!!! ");
// 		}
// 		else
// 		{
// 			Kick=0;
// 			fprintf(stderr,"TES2!!!! ");
// 			countLihat=0;
// 			CountLost++;
// 			//if(CountLost < 100){
// 			//	xBall = lastXBall;
// 			//	yBall = lastYBall;
// 			//}
// 			if(CountLost > 400) {
// 				countertimeout = countgerak = 0;
// 				step = 0;
// 				CountLost = 0;
// 				suduteksekusi=0;
// 			}
// 			//step = 1;
// 		}
// 	}

// 	motionAct(sdtblax,sdtexcute,motion,0);
// 	fprintf(stderr,"NB(%d)Kick(%d)CG(%d)batas(%d)kaki(%d)motion(%d)\n",nBall,Kick,countgerak,batascounttendang, kaki, motion);
// }



// void TaktikTrackBall3()
// {
// 	aktifkansearching();
// 	motion = 10;
// 	if(stepT == 1)
// 	{
// 		countertimeout++;
// 		if(nBall == 1 && countertimeout > 100)
// 		{
// 			stepT++; countertimeout = 0;
// 			lastsuduteksekusi = suduteksekusi;
// 		}
// 		else if(countertimeout<400)
// 		{
// 			if(kaki == 5 || kaki == 6)
// 			{
// 				yBall = generatesdtY(70);
// 				xBall = sdtblax;
// 				motion = 10;
// 			}
// 			else
// 			{
// 				yBall = sdtexcute;
// 				xBall = sdtblax;
// 			}
// 		}
// 		else if(countertimeout > 250)
// 		{
// 			step = 1;
// 			countertimeout = 0;
// 			stepT = 1;
// 			suduteksekusi=0;
// 		}
// 	}
// 	else if (stepT == 2)
// 	{
// 		if(sudahtrackball >= batastrackball || flagsudahdekat == 0 || piliheksekusi == 2 || (arahRobot > 3 && arahRobot <11)) //batas maksimum trackball
// 		{
// 			sudahtrackball = 0;
// 			step = 1;
// 			stepT = 1;
// 			countertimeout = 0;
// 			suduteksekusi=0;
// 		}
// 		else
// 		{
// 			countertimeout++;
// 			if(nBall == 1)
// 			{
// 				if((sdtbolax < 35 && sdtbolax > -35) && (simpansuduteksekusi < 35 && simpansuduteksekusi > -35))
// 				{
// 					if(countertimeout > 30){
// 						if(sdtbolay < 40){
// 							suduteksekusi = 0;
// 						}
// 						else if(sdtbolay<65){
// 							suduteksekusi = sdtbolax * -2 / 3;
// 						}
// 						else suduteksekusi = sdtbolax *-1;

// 						stepT++; countertimeout = 0;
// 					}

// 				}
// 				else
// 				{
// 					if(countertimeout>40){
// 						step = 1;
// 						countertimeout = 0;
// 						stepT = 1;
// 						suduteksekusi=0;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(countertimeout>100){
// 					step = 1;
// 					countertimeout = 0;
// 					stepT = 1;
// 					suduteksekusi=0;
// 				}
// 			}
// 		}

// 	}
// 	else if (stepT == 3)
// 	{
// 		if(disampinggw == 1) {suduteksekusi = -90; sudahtrackball = 5;}
// 		else if(disampinggw == 2) {suduteksekusi = 90; sudahtrackball= 5; }
// 		else
// 		{
// 			sudahtrackball++;
// 		}

// 		step = 3;
// 		stepT = 1;
// 		countertimeout = simpansuduteksekusi = 0;
// 		countLihatblg=0;
// 	}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// }

// void Taktikcekjarak2()
// {
// 	enablevision = 0b1100000;
// 	ethreadsearching = 0;
// 	int gensdt,sdtXTiang;
// 	if(arahRobot==1)gensdt=sdtblax;
// 	else if(arahRobot<11)gensdt=generatesdtX(-1*(arahRobot-1)*18);
// 	else gensdt=generatesdtX((21-arahRobot)*18);
// 	yBall = sdtdefy;
// 	xBall = gensdt;
// 	motion = 0;
// 	if(countertimeout>100) { step++; stepG=1; countertimeout = 0; }
// 	if(posTiang==2)posisiGWX = dataXG;
// 	if(posTiang == 2 && countertimeout > 50)
// 	{
// 		countertimeout = 0;
// 		sdtXTiang=(posisiGWX-frametengah)/5;
// 		if(arahLihat<=6){
// 			suduteksekusi=(arahLihat-1)*18+sdtXTiang;
// 		}
// 		else if(arahLihat>=17){
// 			suduteksekusi=-1*(21-arahLihat)*18+sdtXTiang;			
// 		}else{
// 			suduteksekusi=0;
// 		}
// 		step=4;
// 	}
// 	else if(posrobotY>=4 && countertimeout>100)
// 	{
// 		step = 5;
// 		countDribb=5000; //edit sesuai kebutuhan
// 		countertimeout = 0;
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 		flagsudahdekat = 0;
// 	}
// 	else if(posrobotY<4 && countertimeout>100)
// 	{
// 		flagsudahdekat = 1;
// 		step=3;
// 		countertimeout = 0;
// 		stepG = 1;
// 	}

// 	countertimeout++;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"posY : %d | CT : %d | %d",posrobotY,countertimeout,gensdt);
// }

// void TaktikLuruskanGW_wide2(int sudutputar)
// {
// 	aktifkansearching();
// 	int madepgawang=CariArahGawang(sudutputar,0);
// 	if(sudutputar==0)madepgawang=1;
// 	if(counterobseks>=100 && posrobotY<=2 && sudutputar==0){//ada obstacle, gawang gak kelihatan
// 		if(arhRbt2obs<=20 || arhRbt2obs>11)madepgawang=3;
// 		else if(arhRbt2obs<=11 || arhRbt2obs>=1)madepgawang=19;
// 		else madepgawang=3;
// 	}else if(counterobseks>=100 && posrobotY<=2 && sudutputar!=0){ //ada obstacle lurus gawang, posisi robot dekat gawang
// 		counterobseks=0; 
// 	}else if(counterobseks>=100 && posrobotY>2)madepgawang=3; //ada obs tapi jauh dari gawang
// 	fprintf(stderr, "mG=%d | ",madepgawang);
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		//motion = Gerakluruswide(dataXB,dataYB);
// 		if(dataXB !=0 && dataYB!=0)
// 		{
// 				if(stepP==1){
// 					if(arahRobot<=20 && arahRobot>=12 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=17;
// 					else if(arahRobot<=7 && arahRobot>=1 && ((madepgawang>=14 && madepgawang<=20 && madepgawang>arahRobot) 
// 																						 || (madepgawang>=1 && madepgawang<=7 && madepgawang<arahRobot)))
// 						motion=18;
// 					else if(arahRobot<=20 && arahRobot>=12 && (madepgawang>=14 && madepgawang<=20 && madepgawang<arahRobot))
// 						motion=18;
// 					else if(arahRobot<=7  && arahRobot>=1  && (madepgawang>=1  && madepgawang<=7  && madepgawang>arahRobot))
// 						motion=17;
// 					else motion=10;
// 					if(lastposrobot!=arahRobot || motion==10){motion=10;CountDelay++;}
// 					if(arahRobot==madepgawang){motion=10;CountDelay++;}
// 					if(CountDelay>=10){CountDelay=0;lastposrobot=arahRobot;stepP=2;}
// 				}
// 				if(stepP==2){
// 					if(arahRobot!=arahLihat || arahRobot!=madepgawang){
// 						fprintf(stderr,"GCBE ");
// 						if(sdtbolax > 40) motion = 14;
// 						else if(sdtbolax < -40) motion = 13;
// 						else if(sdtbolax > 7) 	motion = 14;
// 						else if(sdtbolax < -7) motion = 13;
// 						else motion = 10;
// 						if ((sdtbolax>-5 && sdtbolax<5) || motion==10){CountDelay++;motion=10;}
// 						if(CountDelay>=5){stepP=1;CountDelay=0;}
// 					}
// 					if(arahRobot==madepgawang && sdtbolax>-5 && sdtbolax<5){
// 						step=5;
// 						motion = 10;
// 						suduteksekusi = countLihatblg = 0;
// 						CountDelay=0;
// 					}
// 				}
// 		}
// 		CountLost=0;
// 		if(sdtbolay>35 || sdtbolax > 40 || sdtbolax < -40) {
// 			countLihat++;
// 			//yBall=sdtexcute;
// 			//xBall=sdtblax;
// 			if(countLihat>300) {step = 1; countLihat=0;}
// 		}else{
// 			countLihat=0;
// 		}
// 	}
// 	else {
// 			//xBall=sdtblax;
// 			//yBall=sdtexcute;
			
// 			CountLost++;
// 			if(CountLost > 300) {
// 				step = 1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;

// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"Motion(%d) Heading(%d) eks(%d) sdtptar(%d) stepP(%d)\n",motion,heading_skr,suduteksekusi,sudutputar,stepP);
// }

// void TaktikEksekusi2()
// {
// 	int batascounttendang = 50;
// 	int madepgawang=CariArahGawang(suduteksekusi,0);
// 	aktifkansearching();
// 	if(nBall==1)
// 	{
// 		//motion = GerakTendangjauhfast(sdtbolax,sdtbolay);
// 		motion=GerakEksekusiwide(dataXB,dataYB,0);
// 		if(motion == 5 || motion == 6)  { 
// 			Kick = 1; 
// 			kaki = motion; 
// 			countgerak++;
// 			if(arahRobot!=madepgawang)countLuruskanGawang++;
// 	 	}
// 		else countgerak--;
// 		if(countLuruskanGawang>20){step=4;countLuruskanGawang=0;}
// 		if(countgerak>batascounttendang) { motion = kaki; if(motion==0) motion =20; countgerak = batascounttendang+1; tendangTrue=1;}
// 		else if(countgerak<0) countgerak=0;
// 		CountLost = 0;
// 	}
// 	else
// 	{
// 		if(Kick==1)
// 		{

// 			countLihat++;
// 			if(countLihat>200) {step++; countLihat =  countgerak = 0;}else{motion=0;}
// 			stepT = 1;
// 			fprintf(stderr,"TES1!!!! ");
// 		}
// 		else
// 		{
// 			Kick=0;
// 			fprintf(stderr,"TES2!!!! ");
// 			countLihat=0;
// 			CountLost++;
// 			//if(CountLost < 100){
// 			//	xBall = lastXBall;
// 			//	yBall = lastYBall;
// 			//}
// 			if(CountLost > 400) {
// 				countertimeout = countgerak = 0;
// 				step = 0;
// 				CountLost = 0;
// 			}
// 			//step = 1;
// 		}
// 	}

// 	motionAct(sdtblax,sdtexcute,motion,0);
// 	fprintf(stderr,"NB(%d)Kick(%d)CG(%d)batas(%d)kaki(%d)motion(%d)\n",nBall,Kick,countgerak,batascounttendang, kaki, motion);
// }

// void TaktikTrackBall2()
// {
// 	aktifkansearching();
// 	motion = 10;
// 	if(stepT == 1)
// 	{
// 		countertimeout++;
// 		if(nBall == 1 && countertimeout > 100)
// 		{
// 			stepT++; countertimeout = 0;
// 			lastsuduteksekusi = suduteksekusi;
// 		}
// 		else if(countertimeout<200)
// 		{
// 			if(kaki == 5 || kaki == 6)
// 			{
// 				yBall = generatesdtY(70);
// 				xBall = sdtblax;
// 				motion = 10;
// 			}
// 			if(kaki == 8)
// 			{
// 				yBall = generatesdtY(70);
// 				xBall = generatesdtX(50);
// 				motion = 10;
// 			}
// 			else
// 			{
// 				yBall = sdtexcute;
// 				xBall = sdtblax;
// 			}
// 		}
// 		else if(countertimeout > 250)
// 		{
// 			step = 1;
// 			countertimeout = 0;
// 			stepT = 1;
// 		}
// 	}
// 	else if (stepT == 2)
// 	{
// 		if(sudahtrackball >= batastrackball || flagsudahdekat == 0 || piliheksekusi == 2 || (arahRobot > 3 && arahRobot <11)) //batas maksimum trackball
// 		{
// 			sudahtrackball = 0;
// 			step = 1;
// 			stepT = 1;
// 			countertimeout = 0;
// 		}
// 		else if(pilihstrategi == 3)
// 		{
// 			step = 2;
// 			stepT = 1;
// 			countertimeout = 0;
// 			sudahtrackball++;
// 		}
// 		else
// 		{
// 			countertimeout++;
// 			if(nBall == 1)
// 			{
// 				if((sdtbolax < 35 && sdtbolax > -35) && (simpansuduteksekusi < 35 && simpansuduteksekusi > -35))
// 				{
// 					if(countertimeout > 30){
// 						if(sdtbolay < 40){
// 							suduteksekusi = 0;
// 						}
// 						else if(sdtbolay<65){
// 							suduteksekusi = sdtbolax * -2 / 3;
// 						}
// 						else suduteksekusi = sdtbolax *-1;

// 						stepT++; countertimeout = 0;
// 					}

// 				}
// 				else
// 				{
// 					if(countertimeout>40){
// 						step = 1;
// 						countertimeout = 0;
// 						stepT = 1;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(countertimeout>100){
// 					step = 1;
// 					countertimeout = 0;
// 					stepT = 1;
// 				}
// 			}
// 		}

// 	}
// 	else if (stepT == 3)
// 	{
// 		if(disampinggw == 1) {suduteksekusi = -90; sudahtrackball = 5;}
// 		else if(disampinggw == 2) {suduteksekusi = 90; sudahtrackball= 5; }
// 		else
// 		{
// 			sudahtrackball++;
// 		}

// 		step = 3;
// 		stepT = 1;
// 		countertimeout = simpansuduteksekusi = 0;
// 		countLihatblg=0;
// 	}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// }
/*=========================  Taktik Serang End  ====================================*/

/*========================= Taktik Defend Start ====================================*/
// void lostballdef()
// {
// 	//nBall = ftBall4(dataXB, dataYB, 0);
// 	aktifkansearching();
// 	if(nBall == 1){
// 		if((arahLihat == last_arahLihat || arahLihat == rotasiarah(arahLihat+1) || arahLihat == rotasiarah(arahLihat-1))
// 				&& sdtbolay>=lastsdtbolay-10 && sdtbolay<=lastsdtbolay+10)
// 		{
// 			if(CountAda>20)step++;
// 		}
// 		else if(arahLihat >= 6 && arahLihat <= 16)
// 		{
// 			if(CountAda>50)
// 			{
// 				//printf("bola samping belakang");
// 				step++;
// 				flagsudahdekat=0;
// 			}
// 		}
// 		else if(((1 <= arahLihat <= 5) || (17 <= arahLihat <= 20)) && sdtbolay <= 85)
// 		{
// 			if(CountAda>50)
// 			{
// 				//printf("bola depan");
// 				step++;
// 				flagsudahdekat=0;
// 			}
// 		}
// 		else
// 		{
// 			int countdefense;
// 			countdefense++;
// 			if(countdefense > 100)
// 			{
// 				step = laststep;
// 				countdefense = 0;
// 			}
// 		}
// 		CountAda++;
// 		motion = GerakLurusBola(sdtbolax,sdtbolay);
// 	}else{
// 		if(arahRobot == 10) flaglost=1;
// 		else if(arahRobot==1 && flaglost==1){
// 			step++;flaglost=0;
// 		}
// 		CountAda--;
// 		if(CountAda<0) CountAda = 0;
// 	}
// 	fprintf(stderr,"LOSTBALL\n");
// 	//fprintf(stderr, "xBall=%d | yBall=%d | motion=%d\n",xBall,yBall,motion);
// 	motionAct(xBall, yBall,motion, 10);
// }

// void Bertahan(){
// 	aktifkansearching();
// 	/*if(flagmundur==1) {
// 		enablevision = 0b10001;
// 		if(dataXB !=0 || dataYB !=0) nBall=1;
// 		else nBall=0;
// 	}
// 	else {
// 		aktifkansearching();
// 		enablevision |= 0b10000;
// 	}*/
// 	//nBall = ftBall4(dataXB,dataYB,0);
// 	//arahRobot = CariSudutFull(0,myGcol);
// 	//int flagmundur;

// 	/*if(posrobotY>=2 && flagmundur==1){ //ketika sudah dibelakang, dia dapat bola / gak
// 		if(nBall==0){ //ketika dia tidak dapat bola
// 			counterdef++;
// 			if(counterdef>500){
// 				step--;
// 				counterdef=0;
// 			}
// 			fprintf(stderr,"Count = %d ", counterdef);
// 		}
// 		else if(nBall==1){ //ketika dia dapat bola
// 			if(sdtbolay>75) motion = GerakHadapBolaDef(sdtbolax,sdtbolay,arahRobot,1, arahLihat);
// 			else motion = 0;
// 		}
// 	}
// 	else*/ 
// 	if(nBall == 1){ //Cover Striker
// 		motion = GerakHadapBolaDef(sdtbolax,sdtbolay,arahRobot,1, arahLihat);
// 		flagmundur=0;
// 		fprintf(stderr,"COVER - ");
// 	}
// 	else if(nBall==0){ //Mundur sampai ke POS 2
// 		//xBall = sdtblax;		
// 		//nBall = 0;
// 		//yBall = sdtdefy;
// 		motion = 0;
// 		flagmundur=1;
// 		fprintf(stderr,"MUNDUR - ");
// 		counterdef++;
// 		if(counterdef>1000){counterdef=0;step=0;}
// 	}
// 	motionAct(xBall, yBall, motion, 0);
// 	fprintf(stderr," POSY = %d cunter=%d\n",posrobotY,counterdef);
// }
/*=========================  Taktik Defend End  ====================================*/


// void Taktikcekjarak()
// {
// 	enablevision = 0b1100000;
// 	ethreadsearching = 0;
// 	yBall = generatesdtY(75);
// 	xBall = sdtblax;
// 	motion = 0;
// 	if(countertimeout>100) { step++; stepG=1; countertimeout = 0; }
// 	if(posTiang == 2 && countertimeout > 50)
// 	{
// 		step=3;
// 		countertimeout = 0;
// 		stepG = 1; //step gawang
// 	}
// 	else if((posrobotY>=4 && countertimeout>100) || flagsudahdekat==0)
// 	{
// 		step = 5;
// 		countDribb=5000; //edit sesuai kebutuhan
// 		countertimeout = 0;
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 		flagsudahdekat = 0;
// 	}
// 	else if(posrobotY<3 && countertimeout>100)
// 	{
// 		flagsudahdekat = 1;
// 		step=3;
// 		countertimeout = 0;
// 		stepG = 1;
// 	}

// 	countertimeout++;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"posY : %d | CT : %d ",posrobotY,countertimeout);
// }

// void TaktikNggiring()
// {
// 	aktifkansearching();
// 	if (nBall == 1 ){ //&& (kaki == 20 || kaki <= 30 || kaki == 27 || kaki == 28)
// 		//kakie = 0;
// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahNggiring);//GerakTendangjauhfast(sdtbolax,sdtbolay+35);
// 		if(arahRobot == arahNggiring && arahRobot==arahLihat){
// 			fprintf(stderr,"nggiring\n"); 
// 			xBall=sdtblax;yBall=sdtexcute;
// 			motion = GerakEksekusiDribble(dataXB,dataYB);
// 		}
// 		if(countDribb > 0 && motion >= 20)countDribb--;
// 		else if(countDribb == 0)countDribb=0;
// 		if(countDribb<=0) {
// 			if(countDribbChange>0){//CountDribbChange untuk nggiring parabola melewati obstacle
// 				if(arahNggiring>1 && arahNggiring<10)arahNggiring--;
// 				else if(arahNggiring<20 && arahNggiring>10)arahNggiring++;
// 				else if(arahNggiring==20)arahNggiring=1;
// 				else{
// 					countDribbChange=0;
// 				}
// 			}else{
// 				step=1;
// 				countDribb=0;
// 			}
// 		}
// 	}
// 	else {
// 		//yBall = sdtexcute;
// 		//xBall = sdtblax;
// 		motion = 10;
// 		CountLost++;
// 		if(CountLost > 200) {
// 			step = 1;
// 			CountLost = 0;
// 		}
// 	}
// 		fprintf(stderr,"(%d)(%d)(%d)(%d)(%d)\n",countDribb,arahNggiring,sdtbolax,sdtbolay,motion);
// 		motionAct(xBall, yBall,motion, 10);
// }

// void TaktikEksekusi()
// {
// 	int batascounttendang = 50;
// 	int madepgawang=CariArahGawang(suduteksekusi,0);
// 	if(player == 2) batascounttendang = 50;
// 	else if(player == 3) batascounttendang = 50;
// 	nBall = ftBall3(dataXB, dataYB, 0);
// 	fprintf(stderr, "nB = %d | ",nBall);
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	if(nBall==1)
// 	{
// 		//motion = GerakTendangjauhfast(sdtbolax,sdtbolay);
// 		motion=GerakEksekusiwide(dataXB,dataYB,0);
// 		if(motion == 5 || motion == 6)  { 
// 			Kick = 1; 
// 			kaki = motion; 
// 			countgerak++;
// 			if(arahRobot!=madepgawang)countLuruskanGawang++;
// 	 	}
// 		else countgerak--;
// 		if(countLuruskanGawang>20){step=4;countLuruskanGawang=0;}
// 		if(countgerak>batascounttendang) { motion = kaki; if(motion==0) motion =20; countgerak = batascounttendang+1; tendangTrue=1;}
// 		else if(countgerak<0) countgerak=0;

// 		/*if(sdtbolay>55 || sdtbolax > 40 || sdtbolax < -40) {
// 			countLihat++;
// 			if(countLihat>200) {step = 1; countLihat=0;}
// 		}else if(sdtbolay>40 || sdtbolax > 25 || sdtbolax < -25) {
// 			countLihat++;
// 			if(countLihat>200) {step = 4; countLihat=0;}
// 		}*/
// 		CountLost = 0;
// 		//if(arahRobot>6 && arahRobot<8 && sdtbolay<30) {step=1; countLihat=0;}
// 	}
// 	else
// 	{
// 		if(Kick==1)
// 		{

// 			countLihat++;
// 			if(countLihat>200) {step++; countLihat =  countgerak = 0;}else{motion=0;}
// 			stepT = 1;
// 			fprintf(stderr,"TES1!!!! ");
// 		}
// 		else
// 		{
// 			Kick=0;
// 			fprintf(stderr,"TES2!!!! ");
// 			countLihat=0;
// 			CountLost++;
// 			//if(CountLost < 100){
// 			//	xBall = lastXBall;
// 			//	yBall = lastYBall;
// 			//}
// 			if(CountLost > 400) {
// 				countertimeout = countgerak = 0;
// 				step = 0;
// 				CountLost = 0;
// 			}
// 			//step = 1;
// 		}
// 	}

// 	motionAct(sdtblax,sdtexcute,motion,0);
// 	fprintf(stderr,"NB(%d)Kick(%d)CG(%d)batas(%d)kaki(%d)motion(%d)\n",nBall,Kick,countgerak,batascounttendang, kaki, motion);
// }

// void TaktikFindGoal3(){
// 	unsigned char kplKa,kplKi,kplTe;
// 	int countLookGKa,countLookGKi,countLookGTe;
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	if(posRobot == 2){
// 		stepH = step = 1; posRobot = 0;
// 	}

// 	if(nextstep > 0){
// 		step = nextstep;
// 	} else if(nextstep == 0){
// 		if(motion >= 20){
// 			motion = 20;
// 			countjalan--;
// 			if(countjalan <= 0)motion = 10;
// 		} else if(motion < 20){
// 			motion = 10;
// 		} else motion = 10;
// 		yBall = 2301;
// 		if(stepG == 1){
// 			lastbolay = lastsdtbolay;
// 			yBall = lastYball;
// 			cariGawangOut=0;
// 			stepG = 2;
// 			kplKa=kplKi=kplTe=0;
// 			countLookGKa=countLookGKi=countLookGTe=0;
// 			servoXG = GoalXservoG = sdtblax;
// 		} else if(stepG == 2){
// 			xBall = 2550;

// 			cariGawangOut++;

// 			if(posTiang == 1 || posTiang == 5 || posTiang == 3 || posTiang == 2 ){
// 				countLookGKi++;
// 				if(countLookGKi >= 60)countLookGKi = 60;
// 			} else {
// 				countLookGKi--;
// 				if(countLookGKi<= 0)countLookGKi=0;
// 			}

// 			if(cariGawangOut >= 99){
// 				if(countLookGKi >= 20)kplKi = 1;
// 				else kplKi = 0;
// 				cariGawangOut=0;
// 				stepG++;
// 			}
// 		} else if(stepG == 3){
// 			xBall = 2050;
// 			cariGawangOut++;

// 			if(posTiang == 1 || posTiang == 5 || posTiang == 3 || posTiang == 2){
// 				countLookGTe++;
// 				if(countLookGTe >= 60)countLookGTe = 60;
// 			} else {
// 				countLookGTe--;
// 				if(countLookGTe<= 0)countLookGTe=0;
// 			}

// 			if(cariGawangOut >= 99){
// 				if(countLookGTe >= 20)kplTe = 1;
// 				else kplTe = 0;
// 				cariGawangOut=0;
// 				stepG++;
// 			}
// 		} else if(stepG == 4){
// 			xBall = 1550;
// 			cariGawangOut++;

// 			if(posTiang == 1 || posTiang == 5 || posTiang == 3 || posTiang == 2){
// 				countLookGKa++;
// 				if(countLookGKa >= 60)countLookGKa = 60;
// 			} else {
// 				countLookGKa--;
// 				if(countLookGKa<= 0)countLookGKa=0;
// 			}

// 			if(cariGawangOut >= 99){
// 				if(countLookGKa >= 20)kplKa = 1;
// 				else kplKa = 0;
// 				cariGawangOut=0;
// 				stepG++;
// 			}
// 		} else if(stepG == 5){
// 			if(kplKa == 0 && kplTe == 0 && kplKi == 0){
// 				sdtGWX = 0;
// 			} else if(kplKa == 0 && kplTe == 0 && kplKi == 1){
// 				sdtGWX = -69;
// 			} else if(kplKa == 0 && kplTe == 1 && kplKi == 1){
// 				sdtGWX = -43;
// 			} else if(kplKa == 0 && kplTe == 1 && kplKi == 0){
// 				sdtGWX = 0;
// 			} else if(kplKa == 1 && kplTe == 1 && kplKi == 0){
// 				sdtGWX = 43;
// 			} else if(kplKa == 1 && kplTe == 0 && kplKi == 0){
// 				sdtGWX = 68;
// 			}else if(kplKa == 1 && kplTe == 0 && kplKi == 1){
// 				sdtGWX = 0;
// 			}
// 			stepG++;
// 		} else if(stepG == 6){
// 			countTrackG=stepFD=countLookG=0;
// 			xBall = xBall1 = sdtblax;
// 			if(lastYball <= sdtexcute)lastYball = sdtexcute;
// 			yBall = yBall1 = lastYball;
// 			stepG = 1;
// 			stepFD = 0;

// 			step++;
// 			//fprintf(stderr,"S(%d)A(%d)SD(%d)CLKi(%d)CLTe(%d)CLKa(%d)Ki(%d)Te(%d)Ka(%d)\n",stepG,arahRobot,sdtGWX,countLookGKi,countLookGTe,countLookGKa,kplKi,kplTe,kplKa);
// 		}
// 		//===//
// 		//else {
// 		//	TaktikNggiring();	
// 		//}
// 		//==//
// 		motionAct(xBall, yBall, 0, 10);
// 		//fprintf(stderr,"S(%d)A(%d)SD(%d)CLKi(%d)CLTe(%d)CLKa(%d)Ki(%d)Te(%d)Ka(%d)\n",stepG,arahRobot,sdtGWX,countLookGKi,countLookGTe,countLookGKa,kplKi,kplTe,kplKa);
// 		//fprintf(stderr,"S(%d)X(%d)Y(%d)PT(%d)SD(%d)Y(%d)E(%d)N(%d)\n",step,dataXG,dataYG,posTiang,sdtGWX,yBall,ErrorG,nGawang);
// 	}
// }

// void TaktikToBALL_free(){
// 	static int sdtbolaLurus, GeserFTBALL;
// 	if(posRobot <= 0 || posRobot == 4 || posRobot == 3){
// 		arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 		nBall = ftBall3(dataXB,dataYB,GeserFTBALL);
// 		if(stepFD == 0 && nBall == 1){
// 			GeserFTBALL = 0;
// 			if(nBall == 1){
// 				cariGawangOut++;
// 				if(motion >= 10 && sdtbolay >= 35 && cariGawangOut>=10){
// 					sdtbolaLurus = (sdtbolay-35)/2;
// 					cariGawangOut = 0;
// 					stepFD++;
// 				} else if(motion >= 10 && sdtbolay < 35 && cariGawangOut>=10){
// 					 cariGawangOut = 0;
// 					 sdtbolaLurus = 0;
// 					 stepFD++;
// 				}
// 			}
// 			motion = 10;
// 		} else if(stepFD == 1 && nBall == 1){
// 			if(sdtGWX < -20){
// 				sdtkplX = sdtGWX - sdtbolaLurus;
// 			} else if(sdtGWX > 20){
// 				sdtkplX = sdtGWX + sdtbolaLurus;
// 			} else {
// 				sdtkplX = sdtGWX;
// 			}
// 			if(SafeBall == 1){
// 				Hit_Ball_SafeBall();
// 				Hit_Ball_SafeBall();
// 			} else if(Strategiserang == 1){
// 				Hit_Ball_Shoot_Target2();
// 				Hit_Ball_Shoot_Target2();
// 				//Hit_Ball_Shoot_Target();
// 				//Hit_Ball_Shoot_Target();
// 			} else if(Strategiserang == 2){
// 				Hit_Ball_Dribble_Shoot_Target();
// 				Hit_Ball_Dribble_Shoot_Target();
// 			} else if(Strategiserang == 3){
// 				Hit_Ball_Dribble_Target();
// 				Hit_Ball_Dribble_Target();
// 			} else if(Strategiserang == 4){
// 				Hit_Ball_Swing_Target();
// 				Hit_Ball_Swing_Target();
// 			} else if(Strategiserang == 5){
// 				Strategi_Ngiring();
// 				Strategi_Ngiring();
// 			}

// 			if(kaki == 1 || kaki == 5 || kaki == 156 || kaki == 4){
// 				GeserFTBALL = -30;
// 			} else if(kaki == 2 || kaki == 6 || kaki == 155 || kaki == 3){
// 				GeserFTBALL = 30;
// 			} else GeserFTBALL = 0;
// 			stepFD++;
// 			motion = 10;

// 		}else if(stepFD == 2 && nBall == 1){
// 			if (nBall == 1 && sdtbolay <= 75) {
// 				cariGawang++;
// 				motion = GerakArahBolaLurus(sdtbolax, sdtbolay);
// 				if(cariGawang >= 30){
// 					cariGawang = 30;
// 					if(motion == 10){
// 						if(kaki == 155 || kaki == 156){
// 							step = 4;
// 						}
// 						//sumsdtputar = 0;
// 						sudutTendang = 2;
// 						if(posRobot == 3)posRobot = 3;
// 						else posRobot = 2;
// 						step++;
// 						GeserFTBALL = 0;
// 						cariGawangOut=0;
// 					}
// 				}
// 			} else if(nBall == 1 && sdtbolay > 75){
// 				setArahRobot = countGawang = cariGawangOut =  flagPutar = sdtbolax = kaki = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				step = stepH = 1;
// 				sdtbolaLurus=stepFD=0;
// 				cariGawang = 0;
// 				cariGawangOut = 0;
// 			}
// 		} else if (nBall == 0){
// 			cariGawang++;
// 			if(motion >= 10)motion = 10;
// 			if(cariGawang >= 100){
// 				setArahRobot = countGawang = cariGawangOut =  flagPutar = sdtbolax = kaki = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				step = stepH = 1;
// 				cariGawang = 0;
// 				cariGawangOut = 0;
// 			} else if(cariGawang < 100){
// 				if(cariGawang >80){
// 					sdtbolaLurus=stepFD=0;
// 					GeserFTBALL = 0;
// 				}
// 				xBall = sdtblax;
// 				yBall = lastYball-10;
// 				if(yBall >= sdtexcute){
// 					lastYball = yBall;
// 				}
// 				if(yBall <= sdtexcute){
// 					yBall = sdtexcute;
// 				}
// 			}
// 			motion = 10;
// 		} else cariGawang=0;
// 	}
// 	fprintf(stderr,"S(%d)SB(%d)PR(%d)N(%d)M(%d)LB(%d)SBL(%d)SFD(%d)K(%d)SK(%d)\n",step,SafeBall,posRobot,nBall,motion,lastbolay,sdtbolaLurus,stepFD,kaki,sdtkplX);
// 	motionAct(xBall, yBall, motion, 10);
// }

// void TaktikToBALL_putar_parabola(){
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	if(hadapGawang > 20 && sudutTendang >= 1 && sudutTendang <= 3 && posRobot >= 1){
// 		if(kaki == 5 || kaki == 6 || kaki == 1 || kaki == 2){
// 			hadapGawang -= 8;
// 			sudutTendang = 33;
// 		} else if(kaki == 7 || kaki == 8 || kaki == 3 || kaki == 4){
// 			hadapGawang -= 13;
// 			sudutTendang = 33;
// 		}  else if(stepH == 3){
// 			hadapGawang -= 15;
// 			sudutTendang = 33;
// 		}
// 		sumsdtKick = 0;
// 	} else if(hadapGawang <= 20 && sudutTendang >= 1 && sudutTendang <= 3 && posRobot >= 1){
// 		hadapGawang *=5;
// 		sudutTendang = 0;
// 		sumsdtKick = 0;
// 	}

// 	if(posRobot == 1){
// 		if(flagPutar == 2){
// 			if(sudutTendang > 0){
// 				motion = 28;
// 				sudutTendang--;
// 			} else if(sudutTendang <= 0){
// 				motion = 218;
// 				hadapGawang--;
// 			}
// 		} else if(flagPutar == 1){
// 			if(sudutTendang > 0){
// 				motion = 27;
// 				sudutTendang--;
// 			} else if(sudutTendang <= 0){
// 				motion = 217;
// 				hadapGawang--;
// 			}
// 		}
// 		if(hadapGawang <= 0){
// 			step++;
// 			xBall = xBall1 = sdtblax;
// 			yBall = yBall1 = 1670;

// 			motion = 10;
// 		}
// 	} else if(posRobot >= 2){
// 		if(flagPutar == 2){
// 			if(sudutTendang > 0){
// 				motion = 18;
// 				sudutTendang--;
// 			} else if(sudutTendang <= 0){
// 				motion = 218;
// 				hadapGawang--;
// 			}
// 		} else if(flagPutar == 1){
// 			if(sudutTendang > 0){
// 				motion = 17;
// 				sudutTendang--;
// 			} else if(sudutTendang <= 0){
// 				motion = 217;
// 				hadapGawang--;
// 			}
// 		}
// 		if(hadapGawang <= 0){
// 			if(posRobot == 2){
// 				step++;
// 				xBall = xBall1 = sdtblax;
// 				yBall = yBall1 = sdtexcute;
// 				motion = 10;
// 			} else if(posRobot == 3){
// 				stepH = 1;
// 				nextstep = 0;
// 				posRobot = 4;
// 				step = 2;
// 			} else if(posRobot == 4){
// 				stepH = 1;
// 				nextstep = 0;
// 				posRobot = 0;
// 				yBall = yBall1 = sdtexcute;
// 				step++;
// 				motion = 10;
// 			}
// 		}
// 	}
// 	arahdir = arahRobot;
// 	Kick = stepE = cekTdgSkaa = 0;
// 	motionAct(sdtblax, yBall,motion, 10);
// 	fprintf(stderr,"S = %d | HG = %d | SH = %d | F = %d | K = %d | M = %d\n",step,hadapGawang,sudutTendang,flagPutar,kaki,motion );
// }

// int cekArahExc(int kaki){
// 	int kondisi=0;

// 	if((kaki==1 || kaki==2 || kaki==5 || kaki==6)
// 			&& (arahRobot==5 || arahRobot==6|| arahRobot==7 || arahRobot==8|| arahRobot==9)){}
// 	else if((kaki==4 || kaki==7) && (arahRobot==8 || arahRobot==9 || arahRobot==10 || arahRobot==11)){}
// 	else if((kaki==3 || kaki==8) && (arahRobot==6 || arahRobot==5 || arahRobot==4 || arahRobot==3)){}
// 	else{kondisi=1;}
// 	return kondisi;
// }

// void TaktikToExcute(){
// 	nBall = ftBall3(dataXB,dataYB,0);
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	if(cekTdgSkaa==0) {okTdg=0;}
// 	if(cekTdgSkaa<=20){
// 		cekTdgSkaa++;
// 		Kick = 0;
// 		if(cekTdgSkaa >= 11 && cekTdgSkaa <= 20 ){
// 			sumsdtKick += sudutHadap;
// 		}
// 		if(cekTdgSkaa >= 20){
// 			sdtKick = sumsdtKick/10;
// 		}
// 		motionAct(xBall, yBall,10, 10);
// 		okTdg += cekArahExc(kaki);
// 	}
// 	if(hitungtendangkecil>=(ctendangkecil*2)) {step=1;cekTdgSkaa=0; posRobot = 0;hitungtendangkecil=0; CountSafeBall=0;}

// 	if(cekTdgSkaa>20 && okTdg<=3 && play != 2){step=1;cekTdgSkaa=0; posRobot = 0;}
// 	else if(okTdg>3 || play == 2){
// 		if((lastskel>0 && lastskel<10 && motion>=10) || (skelTdg>0 && skelTdg<10)){
// 			yBall = sdtexcute;
// 			xBall = sdtblax;
// 			motionAct(xBall, yBall,10, 10);
// 			skelTdg++;
// 			Kick = 0;
// 			lastskel=0;
// 		} else if (nBall == 2 && Kick == 1 ){
// 			yBall = sdtexcute;
// 			xBall = sdtblax;
// 			kakie = 0;
// 			motion = GerakOverShoot(dataXB, dataYB, sdtKick, sudutHadap);
// 			motionAct(xBall, yBall,motion, 10);
// 			if(motion == 10){
// 				countGawang++;
// 				if(countGawang >= 7){
// 					Kick = 0;
// 					countGawang = 0;
// 					Objectlock = 0;
// 				}
// 			} else if(motion != 10)countGawang=0;
// 			skelTdg=0;
// 		} else if (nBall == 1 && ModeKickOFF >= 6){
// 			yBall = sdtexcute;
// 			xBall = sdtblax;
// 			kakie = 0;
// 			motion = GerakTendangDribb(dataXB, dataYB, kaki, arahdir);
// 			kakie = hadapGawang = 0;
// 			if(motion == 40 || motion == 50){
// 				Objectlock++;
// 				if(Objectlock >= 5){
// 					Kick = 1;
// 					Objectlock = 6;
// 				}
// 			}
// 			if(countDribb > 0)countDribb--;
// 			else if(countDribb == 0)countDribb=0;
// 			if(ModeKickOFF >= 8 && countDribb==0) {
// 				Kick = 3;
// 				ModeKickOFF = 1;
// 			}
// 			//fprintf(stderr,"(%d)(%d)",countDribb,Kick);
// 			motionAct(xBall, yBall,motion, 10);
// 			skelTdg=0;
// 		} else if (nBall == 1 && ((kaki >= 5 && kaki <= 9) || kaki == 155 || kaki == 156) ){

// 			/*
// 			yBall = sdtexcute;
// 			xBall = sdtblax;
// 			//if(dataXB<150) kaki = 5; ////////////////////
// 			//else kaki = 6; //////////////////////////////

// 			motion = GerakTendang(dataXB, dataYB, kaki, sdtKicking);*/

// 			motion = GerakTendangjauhfast(sdtbolax,sdtbolay);


// 			kakie = hadapGawang = 0;
// 			if(motion == 155 || motion == 156){
// 				Objectlock++;
// 				if(Objectlock >= 10){
// 					Kick = 1;
// 					kaki = 5;
// 					Objectlock = 6;
// 				}
// 			}
// 			motionAct(xBall, yBall,motion, mode);
// 			skelTdg=0;
// 		} else if (nBall == 1 && kaki >= 1 && kaki <= 4 ){

// 			yBall = sdtexcute;
// 			xBall = sdtblax;
// 			motion = GerakTendangKecil(dataXB, dataYB, kaki);
// 			kakie = hadapGawang = 0;
// 			if(motion < 10){
// 				Objectlock++;
// 				if(Objectlock >= 8){
// 					Kick = 1;
// 					Objectlock = 8;
// 				}
// 			}
// 			else Kick = 0;
// 			if(Kick==1 && lastKick==0 && CountSafeBall<=0) {hitungtendangkecil++; CountSafeBall=30;}
// 			lastKick = Kick;
// 			CountSafeBall--;
// 			if(CountSafeBall<0) CountSafeBall=0;
// 			motionAct(xBall, yBall,motion, 10);
// 			skelTdg=0;
// 		} else if (nBall == 1 && (kaki == 20 || kaki <= 30 || kaki == 27 || kaki == 28) ){
// 			yBall = sdtexcute-100;
// 			xBall = sdtblax;
// 			//kakie = 0;
// 			motion = GerakTendangDribb(dataXB, dataYB, kaki, arahdir);
// 			kakie = hadapGawang = 0;
// 			//if(motion == 40 || motion == 50){
// 			//	Objectlock++;
// 			//	if(Objectlock >= 5){
// 			//		//Kick = 1;
// 			//		Objectlock = 6;
// 			//	}
// 			//}
// 			if(countDribb > 0)countDribb--;
// 			else if(countDribb == 0)countDribb=0;
// 			if(countDribb<=0) {
// 				step=1;
// 				stepH=1;
// 				countDribb=0;
// 			}
// 						//fprintf(stderr,"(%d)(%d)",countDribb,Kick);
// 			motionAct(xBall, yBall,motion, 10);
// 			skelTdg=0;
// 		}
// 		else {
// 			kakie++;
// 			if(kakie <= 40){
// 				countKick = hadapGawang = 0;
// 				xBall1 = sdtblax;
// 				yBall1 = sdtexcute;
// 			} else if(kakie >= 60){
// 				Kick = 2;
// 			} else if(kakie > 40 && kakie < 60){
// 				if (Kick == 1) {
// 					lastErrY = 10;
// 					if (kaki == 8||kaki == 3) {
// 						xBall1 = 1200; motion = 22;
// 						yBall1 = sdtblamx;
// 						arahX = 1;
// 					} else if (kaki == 5||kaki == 1||kaki == 6||kaki == 2) {
// 						xBall1 = sdtblax; motion = 20;
// 						yBall1 = sdtblamx;
// 						if(kakie >= 53){
// 							Kick = 2;
// 						}
// 					} else if (kaki == 7||kaki == 4) {
// 						xBall1 = 2900; motion = 21;
// 						yBall1 = sdtblamx;
// 						arahX = 0;
// 					} else if (kaki == 9){
// 						CountTrack = 350;
// 						arahX = 1;
// 					}
// 				}
// 			}
// 			if(Kick == 2){
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = Objectlock = 0;
// 				nextstep = sdtGWX = sdtkplX = Kick = stepE = cekTdgSkaa = 0;
// 				stepH = stepTB = 1;
// 				servoX = GoalXservo = xBall = xBall1;
// 				yBall = yBall1;
// 				if(kaki==20||kaki==30) step=1;
// 				else step++;
// 			}

// 			motionAct(xBall1, yBall1,motion, 10);
// 			skelTdg=0;
// 		}
// 		if(Kick == 3){
// 			setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = Objectlock = 0;
// 			nextstep = sdtGWX = sdtkplX = Kick = stepE = cekTdgSkaa = 0;
// 			stepH = stepTB = 1;
// 			servoX = GoalXservo = xBall = xBall1;
// 			yBall = yBall1;
// 			step++;
// 		}
// 	}
// 	lastskel = motion;
// 	//fprintf(stderr,"(%d)-(%d)-(%d)-(%d)-(%d)-(%d)-(%d)-(%d)\n",step,kaki,Kick,motion,lapangan,kaki,okTdg,sdtKicking);

// 	fprintf(stderr,"S = %d | M = %d | K = %d | Ki = %d | SH = %d\n",sdtKick, motion, kaki, Kick,sudutHadap);
// 	//fprintf(stderr,"S = %d | A = %d | SA = %d | M = %d | K = %d | H = %d\n",step, arahRobot, setArahRobot, motion, kaki, hadapGawang);
// }

// void TaktikTrackBall(){
// 	nBall = ftBall3(dataXB, dataYB, 0);
// 	if(nBall == 1){
// 		if(stepTB == 1){
// 			countGW++;
// 			if(sdtbolay <= 70 && countGW >= 10){
// 				Tbolax = sdtbolax;
// 				Tbolay = sdtbolay;
// 				if(kaki == 1 || kaki == 2 || kaki == 5 || kaki == 6){
// 					stepTB = 2;
// 					countGW = 0;
// 				}
// 			}
// 			motion = 10;
// 		} else if(stepTB == 2){
// 			if(Tbolax >= -15 && Tbolax <= 15){
// 				hadapGawang = 0;
// 			} else if(Tbolax < -15 || Tbolax > 15){
// 				hadapGawang = Tbolay - 25;
// 				if(Tbolax >= 0) flagPutar = 2;
// 				else if(Tbolax < 0) flagPutar = 1;
// 			}
// 			if(Tbolax < 0) kaki = 5;
// 			else if(Tbolax >= 0) kaki = 6;
// 			stepTB = 4;
// 		} else if(stepTB == 4){
// 			motion = GerakArahBolaLurus(sdtbolax, sdtbolay);
// 			if(motion == 10){
// 				countGW++;
// 				if(countGW >= 3){
// 					posRobot = 4;
// 					sudutTendang = 2;
// 					step = 4;
// 					hitungtendangkecil++;
// 					stepTB = 1;
// 				}
// 			} else countGW = 0;
// 		}

// 		if (sdtbolay > 70 ||kaki == 8 || kaki == 7 || kaki == 3 || kaki == 4 ||(sdtbolax <= -45 || sdtbolax >= 45)){
// 			step = 1;
// 			stepTB = 1;
// 			stepH = 1;
// 			Tbolax = Tbolay = 0;
// 		}
// 	} else {
// 		if(motion == 12 || motion == 11){
// 			countGW++;
// 			if(countGW >= 10){
// 				step = 1;
// 				stepH = 1;
// 				stepTB = 1;
// 				countGW = 0;
// 			}
// 		}

// 	}
// 	motionAct(xBall, yBall,motion, 10);
// 	fprintf(stderr,"S = %d | ST = %d | Y = %d | TX = %d | TY = %d | K = %d | M = %d | H = %d | F = %d\n", step,stepTB,sdtbolay,Tbolax,Tbolay, kaki, motion, hadapGawang,flagPutar);
// }

// void TaktikToBALL_putar(){
// 	nBall = ftBall3(dataXB,dataYB,0);

// 	if (nBall == 1) {
// 		motion = GerakCoverBallSlow(sdtbolax, sdtbolay);
// 		if(motion == 10){
// 			cariGawangOut++;
// 			if(cariGawangOut >= 20){
// 				step++;
// 				cariGawangOut=0;
// 			}
// 		}
// 	} else {
// 		if(CountTrack >= 50){
// 			play = 1;
// 		} else if(CountTrack < 50){
// 			play = 2;
// 		}
// 	}
// 	//writeIPC(1,xBall,yBall,motion);
// 	motionAct(xBall, yBall, motion, 10);
// }

// void TaktikMinggir(){
// 	int mGeser;
// 	nBall = ftBall3(dataXB, dataYB, 0);
// 	if(flagMinggir == 1){
// 		mGeser = 32;
// 	} else if(flagMinggir == 2){
// 		mGeser = 31;
// 	}

// 	countGeser --;
// 	if(countGeser <= 0){
// 		countGeser=0;
// 		mGeser = 10;
// 		step++;
// 	}
// 	motion = mGeser;
// 	motionAct(xBall, yBall,motion, 10);
// }

// void TaktikTunggubola()
// {
// 	aktifkansearching();
// 	if(nBall == 1)
// 	{
// 		fprintf(stderr,"Lihat BOLA1 -> ");
// 		motion = GerakUncoverBallDimas(sdtbolax, sdtbolay);
// 		//dataTm = 1;
// 		//if(sdtbolay>70 && sdtbolay<=86 && motion == 10){
// 		//	cariGawangOut++;
// 		//	if(cariGawangOut>20) { step=2; cariGawangOut=0;}
// 		//}
// 		//else cariGawangOut=0;
// 	}
// 	motionAct(xBall, yBall, motion, 0);
// }

// int TaktikStayBALL(){
// 	//dtJob=1;
// 	//nBall = ftBall3(dataXB,dataYB,0);
// 	aktifkansearching();
// 	if(nBall == 1)
// 	{
// 		fprintf(stderr,"Lihat BOLA1 -> ");
// 		motion = GerakUncoverBallDimas(sdtbolax, sdtbolay);
// 		//daTm = 1;
// 		if(sdtbolay>70 && sdtbolay<=86 && motion == 10){
// 			cariGawangOut++;
// 			if(cariGawangOut>20) { step=2; cariGawangOut=0;}
// 		}
// 		else cariGawangOut=0;
// 	}
// 	else //dataTm = 0;
// 	cariGawang = 0;
// 	motionAct(xBall, yBall, motion, 10);
// 	//writeIPC(1,xBall,yBall,motion,arahRobot);
// 	printf("mo=%d__sdtBol=%d\n",motion,sdtbolay);
// }

// void tunggudeff()
// {
// 	//dtJob = 1;
// 	//nBall = ftBall3(dataXB,dataYB,0);
// 	aktifkansearching();
// 	arahRobot = CariSudut(0,myGcol);
// 		if(nBall == 1)
// 		{
// 			//dataTm = 1;
// 			fprintf(stderr,"Lihat BOLA2 -> ");
// 			if(arahRobot >= 6 && arahRobot <= 9)
// 			{
// 				if(arahRobot<7) //hadap kanan
// 				{
// 					if	(sdtbolax < -30) motion = 11;
// 					else motion = 16;
// 				}
// 				else //hadap kiri
// 				{
// 					if	(sdtbolax > 30) motion = 12;
// 					else motion = 15;

// 				}
// 			}
// 			else{
// 			if(sdtbolax <-20) motion = 13;
// 			else if	(sdtbolax > 20) motion = 14;
// 			else motion = 10;
// 			}
// 			if(sdtbolay<70 || sdtbolay>86) cariGawang++;
// 			else cariGawang=0;

// 			if(cariGawang>30) {step = 1; cariGawang = 0;}
// 		}
// 		else
// 		{
// 			//dataTm = 0;
// 			cariGawang++;
// 			if(cariGawang>30) {step = 1; cariGawang = 0;}
// 			stepH = 1;
// 		}
// 		motionAct(xBall, yBall, motion, 10);
// }

// /* ==================== Pilih taktik tendangan ============================= */
// void Hit_Ball_SafeBall(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 30){
// 			kaki = 5;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 		} else if(sdtkplX > 30 && sdtkplX <= 90){
// 			kaki = 3;
// 			flagPutar = 2;
// 			hadapGawang = (90 - sdtkplX);
// 		} else if(sdtkplX > 90){
// 			kaki = 3;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX-90);
// 		}
// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -30){
// 			kaki = 6;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX*-1);
// 		} else if(sdtkplX < -30 && sdtkplX >= -90){
// 			kaki = 4;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX + 90);
// 		} else if(sdtkplX < -90){
// 			kaki = 4;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX+90)*-1;
// 		}
// 	}
// 	//fprintf(stderr,"Hg = %d | Sk =%d | Fp = %d | K = %d | P = %d\n",hadapGawang,sdtkplX,flagPutar,kaki,posRobot);
// 	sudutTendang = 2;
// }

// void Hit_Ball_Shoot_Target(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 60){
// 			if(servoYG <= sdtblaGmn + 500){
// 				kaki = 5;
// 				flagPutar = 1;
// 				hadapGawang = sdtkplX;
// 			} else if(servoYG > sdtblaGmn + 500){
// 				kaki = 1;
// 				flagPutar = 1;
// 				hadapGawang = sdtkplX;
// 			}
// 		} else if(sdtkplX > 60 && sdtkplX <= 90){
// 			kaki = 8;
// 			flagPutar = 2;
// 			hadapGawang = (90 - sdtkplX);
// 		} else if(sdtkplX > 90){
// 			kaki = 8;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX-90);
// 		}
// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -60){
// 			if(servoYG <= sdtblaGmn + 500){
// 				kaki = 6;
// 				flagPutar = 2;
// 				hadapGawang = (sdtkplX*-1);
// 			} else if(servoYG > sdtblaGmn + 500){
// 				kaki = 2;
// 				flagPutar = 2;
// 				hadapGawang = (sdtkplX*-1);
// 			}
// 		} else if(sdtkplX < -60 && sdtkplX >= -90){
// 			kaki = 7;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX + 90);
// 		} else if(sdtkplX < -90){
// 			kaki = 7;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX+90)*-1;
// 		}
// 	}
// 	//fprintf(stderr,"Hg = %d | Sk =%d | Fp = %d | K = %d | P = %d\n",hadapGawang,sdtkplX,flagPutar,kaki,posRobot);
// 	sudutTendang = 2;
// }

// void Hit_Ball_Shoot_Target2(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 70){
// 			kaki = 5;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 		}
// 		else{
// 			kaki = 5;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX+20;
// 		}

// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -60){
// 			kaki = 6;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX*-1);
// 		}
// 		else{
// 			kaki = 6;
// 			flagPutar = 2;
// 			hadapGawang = ((sdtkplX+20)*-1);
// 		}


// 	}
// 	//fprintf(stderr,"Hg = %d | Sk =%d | Fp = %d | K = %d | P = %d\n",hadapGawang,sdtkplX,flagPutar,kaki,posRobot);
// 	sudutTendang = 2;
// }

// void Hit_Ball_Dribble_Shoot_Target(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 70){
// 			kaki = 1;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 		} else if(sdtkplX > 70 && sdtkplX <= 90){
// 			kaki = 8;
// 			flagPutar = 2;
// 			hadapGawang = (90 - sdtkplX);
// 		} else if(sdtkplX > 90){
// 			kaki = 8;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX-90);
// 		}
// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -70){
// 			kaki = 2;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX*-1);
// 		} else if(sdtkplX < -70 && sdtkplX >= -90){
// 			kaki = 7;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX + 90);
// 		} else if(sdtkplX < -90){
// 			kaki = 7;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX+90)*-1;
// 		}
// 	}
// 	//fprintf(stderr,"Sp = %d | Sg =%d | St = %d | K = %d | P = %d\n",sdtputar,sdtGWXptr,sdtTndang,kaki,posRobot);
// 	sudutTendang = 2;

// }

// void Hit_Ball_Dribble_Target(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 70){
// 			kaki = 1;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 		}  else if(sdtkplX > 70 && sdtkplX <= 90){
// 			kaki = 8;
// 			flagPutar = 2;
// 			hadapGawang = (90 - sdtkplX);
// 		} else if(sdtkplX > 90){
// 			kaki = 8;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX-90);
// 		}
// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -70){
// 			kaki = 2;
// 			flagPutar = 2;
// 			hadapGawang = ((sdtkplX)*-1);
// 		} else if(sdtkplX < -70 && sdtkplX >= -90){
// 			kaki = 7;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX + 90);
// 		} else if(sdtkplX < -90){
// 			kaki = 7;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX+90)*-1;
// 		}
// 	}

// 	//fprintf(stderr,"Sp = %d | Sg =%d | St = %d | K = %d | P = %d\n",sdtputar,sdtGWXptr,sdtTndang,kaki,posRobot);
// 	sudutTendang = 2;

// }

// void Hit_Ball_Swing_Target(){
// 	if(sdtkplX >= 0){
// 		if(sdtkplX <= 40){
// 			kaki = 5;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 		} else if(sdtkplX > 40 && sdtkplX <= 50){
// 			kaki = 155;
// 			flagPutar = 1;
// 			sdtKicking = hadapGawang = 110+sdtkplX;
// 		}else if(sdtkplX > 50 && sdtkplX <= 90){
// 			kaki = 8;
// 			flagPutar = 2;
// 			hadapGawang = (90 - sdtkplX);
// 		} else if(sdtkplX > 90){
// 			kaki = 8;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX-90);
// 		}
// 	} else if(sdtkplX < 0){
// 		if(sdtkplX >= -40){
// 			kaki = 6;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX*-1);
// 		} else if(sdtkplX < -40 && sdtkplX >= -50){
// 			kaki = 156;
// 			flagPutar = 2;
// 			sdtKicking = hadapGawang = 110+(sdtkplX*-1);
// 		} else if(sdtkplX < -50 && sdtkplX >= -90){
// 			kaki = 7;
// 			flagPutar = 1;
// 			hadapGawang = (sdtkplX + 90);
// 		} else if(sdtkplX < -90){
// 			kaki = 7;
// 			flagPutar = 2;
// 			hadapGawang = (sdtkplX+90)*-1;
// 		}
// 	}
// 	//fprintf(stderr,"Hg = %d | Sk =%d | Fp = %d | K = %d | P = %d\n",hadapGawang,sdtkplX,flagPutar,kaki,posRobot);
// 	sudutTendang = 2;
// }

// void Strategi_Ngiring()
// {
// 	if(sdtkplX >= 0){
// 			kaki = kakidrible;
// 			flagPutar = 1;
// 			hadapGawang = sdtkplX;
// 	} else if(sdtkplX < 0){
// 			kaki = kakidrible;
// 			flagPutar = 2;
// 			hadapGawang = ((sdtkplX)*-1);
// 	}
// 	countDribb=1500;
// }

// void PathPlanning2()
// {
// 	// Bismillahirahmanirahim
// 	static short langkah = 1;
// 	static double DF = 20, SP, SL, JT, SKT;
// 	switch(langkah)
// 	{
// 		case 1:
// 		{
// 			arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 			arahLihat = CariArahLihat(sdtbolax,myGcol);
// 			//nBall = ftBall3(dataXB,dataYB,0);
// 			aktifkansearching();
// 			if(nBall == 1)
// 			{
// 				if(sdtbolax > 40) motion = 26;
// 				else if(sdtbolax < -40) motion = 25;
// 				else if(sdtbolax > 20) 	motion = 12;
// 				else if(sdtbolax < -20) motion = 11;
// 				else if(sdtbolax > 6) 	motion = 212;
// 				else if(sdtbolax < -6) motion = 211;
// 				else if(arahRobot>=1 && arahRobot<5)
// 				{
// 					motion = 10;
// 					if(sdtbolay<50)
// 					{
// 						if(countWait++ < 10) {motion = 10;}
// 						else {
// 							countWait = 0; langkah=4;
// 						}
// 					}
// 					else if(arahRobot==1 || arahRobot==2)
// 					{
// 						if(countWait++ < 10) {motion = 10;}
// 						else {
// 							countWait = 0; langkah=4;
// 						}
// 					}
// 					else
// 					{
// 						if(countWait++ < 100) {motion = 10;}
// 						else {
// 							JT = sqrt( pow(jrkbolay,2) + pow(DF,2) - 2*DF*jrkbolay*cosd(sudutHadap) );
// 							SP = asin( DF*sind(sudutHadap) / JT ) *180/M_PI;
// 							SL = sudutHadap+SP;
// 							SKT = sudutHadap+SP;
// 							countWait = 0; langkah=3;
// 						}
// 					}
// 				}
// 				else { motion = 10; countWait = 0;}
// 			}
// 			break;
// 		}
// 		case 2 :
// 		{
// 			aktifkansearching();
// 			arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 			//nBall = ftBall3(dataXB,dataYB,0);
// 			arahLihat = CariArahLihat(sdtbolax,myGcol);
// 			if(nBall == 1)
// 			{
// 				if(SKT>0 && SKT<180)
// 				{
// 					if(sudutHadap < (int)SKT) motion = 212;
// 					else if(sudutHadap > (int) SKT+10) motion=211;
// 					else
// 					{
// 						if(countWait++ < 200) {
// 							motion =0;
// 						}
// 						else langkah++;
// 					}
// 				}
// 			}
// 			else {
// 				CountLost++;
// 				if(CountLost > 50) {
// 					langkah = 1;
// 					CountLost = 0;
// 				}
// 			}
// 		} break;
// 		case 3 :
// 		{
// 			aktifkansearching();
// 			arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 			//nBall = ftBall3(dataXB,dataYB,0);
// 			arahLihat = CariArahLihat(sdtbolax,myGcol);
// 			if(nBall == 1)
// 			{
// 				if(arahLihat>6 && arahLihat<12)
// 				{
// 					langkah=1;
// 					motion = 10;
// 				}
// 				else if(arahLihat>4)
// 				{
// 					langkah=1;
// 					motion = 10;
// 				}
// 				else if((arahLihat == 1 || arahLihat == 12) && dataCMPS!=50)
// 				{
// 					langkah++;
// 					motion = 10;

// 				}
// 				else
// 				{
// 					if(arahLihat == 2)
// 					{
// 						if(countWait++ > 333) {
// 							motion = 10;
// 							langkah++;
// 							countWait = 0;
// 						}
// 					}
// 					if(SKT>0 && SKT<180)
// 					{
// 						if(sudutHadap < (int)SKT) motion = 32;
// 						else if(sudutHadap > (int) SKT+10) motion=31;
// 						else
// 						{
// 							motion = 30;
// 						}
// 					}
// 					else langkah = 1;
// 				}
// 			}
// 			else {
// 				CountLost++;
// 				if(CountLost > 50) {
// 					langkah = 1;
// 					CountLost = 0;
// 				}
// 			}
// 			break;
// 		}
// 		case 4:
// 		{
// 			aktifkansearching();
// 			arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 			arahLihat = CariArahLihat(sdtbolax,myGcol);
// 			//nBall = ftBall3(dataXB,dataYB,0);
// 			if(nBall == 1)
// 			{
// 				if(sdtbolay > 30 || arahRobot !=1 )
// 				{
// 					motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,1);
// 				}
// 				else {

// 					if(countWait++ < 50) {
// 						motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,1);
// 					}
// 					else
// 					{
// 						langkah++;
// 						countWait = 0;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				CountLost++;
// 				if(CountLost > 50) {
// 					langkah = 1;
// 					CountLost = 0;
// 				}
// 			}
// 			break;
// 		}
// 		case 5:
// 		{
// 			aktifkansearching();
// 			motion = 0;
// 			if(nBall == 1)
// 			{

// 			}
// 			else
// 			CountLost++;
// 			if(CountLost > 50) {
// 				langkah = 1;
// 				CountLost = 0;
// 			}

// 			break;
// 		}
// 	}
// 	motionAct(xBall,yBall,motion,0);
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"X:%d Y:%d AR:%d AL:%d L:%d CW:%d\n", sdtbolax, sdtbolay, arahRobot, arahLihat, langkah,countWait);
// 	usleep(10);
// }

// void Taktikcekpinggir()
// {
// 	enablevision = 0b110000;
// 	ethreadsearching = 0;

// 	if(countertimeout<=300)
// 	{
// 		yBall = generatesdtY(65);
// 		xBall = sdtblax;
// 		motion = 0;
// 		if(countertimeout>500) step++;
// 	}
// 	else if(lokasirobot == 15 && countertimeout>300)
// 	{
// 		step=5;
// 		countertimeout = 0;
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 	}
// 	else if(lokasirobot == 14 && countertimeout>300)
// 	{
// 		step++;
// 		countertimeout = 0;
// 		stepG = 1;
// 	}
// 	else
// 	countertimeout++;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"LO : %d",lokasirobot);
// }

// void TaktikcekGw()
// {
// 	static short suduttolehx,arahtoleh;
// 	static int countertimeouttmbh = 0;
// 	if(stepG==1) enablevision = 0b1100000;
// 	else enablevision = 0b100000;
// 	ethreadsearching = 0;

// 	if(stepG == 1)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(75);
// 		xBall = sdtblax;

// 		if(posrobotY>=4 && flagsudahdekat==0) //kalo masih jauh
// 		{
// 			if(countertimeout>120)
// 			{
// 				araheksekusi =1; suduteksekusi = 0; piliheksekusi = 2;
// 				stepG=4;
// 				countertimeout = 0;
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				countDribb = 500;
// 			}
// 		}
// 		else
// 		{
// 			if(posTiang == 1) { counterpos1++; countertimeouttmbh+=2; }
// 			else if(posTiang == 2) { counterpos2++; countertimeouttmbh+=2; }

// 			if (counterpos2 > 50) { simpanposisiGW = 2;sudutxTiang = (dataXG-frametengah)/6;} //X2X
// 			else if(counterpos1 > 50) { simpanposisiGW = 1; sudutxTiang = (dataXG-frametengah)/6; if(sudutxTiang>0) arahtoleh=kanan; else arahtoleh=kiri;} //X1X
// 			else if(countertimeout>120) { simpanposisiGW = 0; sudutxTiang = 0;}  //X0X

// 			if(countertimeout>120+countertimeouttmbh || counterpos2 > 50 || counterpos1 > 50) {
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 				if(arahtoleh==kiri) suduttolehx = -60; else suduttolehx = 60;
// 				flagsudahdekat = 1;
// 			}
// 			piliheksekusi = 1;
// 		}
// 		countertimeout++;

// 		fprintf(stderr,"RobotY (%d) ",posrobotY);

// 		else
// 		{
// 			if(countertimeout > 75 && posTiang<1 && (counterpos1<37 || counterpos2<37))
// 			{
// 				if(suduttolehx>0 && countertimeout%3 == 0) suduttolehx++;
// 				else if(countertimeout%3 == 0) suduttolehx--;
// 			}

// 			if(posTiang>0)
// 			{
// 				if((dataXG-160)>6 && countertimeout%3 == 0)suduttolehx++;
// 				else if((dataXG-160)<-6 && countertimeout%3 == 0)suduttolehx--;
// 			}
// 		}
// 	}
// 	else if(stepG == 2)
// 	{
// 		motion = motioncekgw;

// 		if(posTiang == 1 && countertimeout>30) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>30) { counterpos2++; countertimeouttmbh+=2; }
// 		countertimeout++;

// 		if(simpanposisiGW == 0)
// 		{
// 			motion = motioncekgw;
// 			yBall = generatesdtY(75);
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > 40) { simpanposisiGW2 = 2; sudutxTiang2 = (suduttolehx + ((dataXG-frametengah)/6))*4/3;} //X02
// 			else if(counterpos1 > 40) { simpanposisiGW2 = 1; sudutxTiang2 = suduttolehx + ((dataXG-frametengah)/6);} //X01
// 			else if(countertimeout>111) { simpanposisiGW2 = 0; sudutxTiang2 = 0; } //X00

// 			if(countertimeout>111 + countertimeouttmbh || counterpos2 > 40 || counterpos1 > 40)
// 			{
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 				if(arahtoleh==kanan) suduttolehx = -60; else suduttolehx = 60;
// 			}
// 		}
// 		else if(simpanposisiGW == 1)
// 		{
// 			motion = motioncekgw;
// 			yBall = generatesdtY(75);
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > 40) { simpanposisiGW2 = 4; sudutxTiang2 = (suduttolehx + ((dataXG-frametengah)/6)) * 4/3;} //X12
// 			else if(counterpos1 > 40) { simpanposisiGW2 = 3; sudutxTiang2 = (sudutxTiang + (suduttolehx + ((dataXG-frametengah)/6)))/2;} //X11
// 			else if(countertimeout>111) { simpanposisiGW2 = 1; sudutxTiang2 = sudutxTiang;} //X10

// 			if(countertimeout>111 + countertimeouttmbh || counterpos2 > 40 || counterpos1 > 40)
// 			{
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 				if(arahtoleh==kanan) suduttolehx = -60; else suduttolehx = 60;
// 			}
// 		}
// 		else if(simpanposisiGW == 2)
// 		{
// 			sudutxTiang2 = sudutxTiang; //2
// 			simpanposisiGW2 = 2;
// 			countertimeout = counterpos1 = counterpos2 = 0;
// 			stepG++;
// 		}
// 		/*
// 		if(simpanposisiGW != 2)
// 		{
// 			if(countertimeout > 75 && posTiang<1 && (counterpos1<37 || counterpos2<37))
// 			{
// 				if(suduttolehx>0 && countertimeout%2 == 0) suduttolehx++;
// 				else if(countertimeout%2 == 0)suduttolehx--;
// 			}
// 		}*/
// 			/*
// 			if(posTiang == 1)
// 			{
// 				if(suduttolehx>0 && (dataXG-160)>3) suduttolehx++;
// 				else if(suduttolehx<0 && (dataXG-160)<-3) suduttolehx--;
// 			}*/
// 			/*if(posTiang>0)
// 			{
// 				if((dataXG-160)>6 && countertimeout%3 == 0)suduttolehx++;
// 				else if((dataXG-160)<-6 && countertimeout%3 == 0)suduttolehx--;
// 			}*/

// 	}
// 	else if(stepG==3)
// 	{
// 		motion = motioncekgw;
// 		if(posTiang == 1 && countertimeout>30) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>30) { counterpos2++; countertimeouttmbh+=2; }
// 		countertimeout++;

// 		if(simpanposisiGW2 == 0)
// 		{
// 			motion = motioncekgw;
// 			yBall = generatesdtY(75);
// 			xBall = generatesdtX(suduttolehx);

// 			if(counterpos2 > 40) { suduteksekusi = (suduttolehx + ((dataXG-frametengah)/6))*4/3;} //200
// 			else if(counterpos1 > 40) {
// 				suduteksekusi = ((suduttolehx) + ((dataXG-frametengah)/6))*3/2;

// 				if(suduteksekusi > 90){ suduteksekusi+=5; disampinggw = 1;}
// 				else if(suduteksekusi < -90){ suduteksekusi-=5; disampinggw = 2;}
// 				else disampinggw = 0;

// 				if(suduteksekusi > 0) suduteksekusi+=5;
// 				else if(suduteksekusi < 0) suduteksekusi-=5;

// 				if(suduteksekusi > 100) suduteksekusi = 100;
// 				else if(suduteksekusi <-100) suduteksekusi =-100;
// 			} //100
// 			else if(countertimeout>150) { suduteksekusi = 0; } //000

// 			if(countertimeout>150 + countertimeouttmbh || counterpos2 > 40 || counterpos1 > 40) {
// 				//step++;
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 				stepG++;
// 				//heading_skr = 0;
// 			}
// 		}
// 		else if(simpanposisiGW2 == 1)
// 		{
// 			motion = motioncekgw;
// 			yBall = generatesdtY(75);
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > 40) { suduteksekusi = (suduttolehx+ ((dataXG-frametengah)/6))*4/3;} //200
// 			else if(counterpos1 > 40) { suduteksekusi = (suduttolehx + sudutxTiang2 )/2; } //101 or 110
// 			else if(countertimeout>150) {
// 				suduteksekusi = (sudutxTiang2*3/2);
// 				if(suduteksekusi > 100) suduteksekusi = 100;
// 				else if(suduteksekusi <-100) suduteksekusi =-100;
// 			} //001


// 			if(countertimeout>150 || counterpos2 > 40 || counterpos1 > 40) {
// 				//step++;
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 				stepG++;
// 			}
// 		}
// 		else if(simpanposisiGW2 >= 2)
// 		{

// 			suduteksekusi = sudutxTiang2;
// 			//step++;
// 			if(suduteksekusi>10) suduteksekusi-=15;
// 			else if(suduteksekusi<10) suduteksekusi+=15;
// 			xBall = lastXBall;
// 			yBall = lastYBall;
// 			countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = 0;
// 			stepG++;
// 			//heading_skr = 0;
// 		}
// 		/*if(simpanposisiGW2 < 2)
// 		{
// 			if(countertimeout > 75 && posTiang<1 && (counterpos1<20 || counterpos2<20))
// 			{
// 				if(suduttolehx>0 && countertimeout%2 == 0) suduttolehx++;
// 				else if(countertimeout%2 == 0)suduttolehx--;
// 			}
// 		}*/
// 			/*(if(posTiang == 1)
// 			{
// 				if(suduttolehx>0 && (dataXG-160)>3) suduttolehx++;
// 				else if(suduttolehx<0 && (dataXG-160)<-3) suduttolehx--;
// 			}*/

// 			/*if(posTiang>0)
// 			{
// 				if((dataXG-160)>6 && countertimeout%3 == 0)suduttolehx++;
// 				else if((dataXG-160)<-6 && countertimeout%3 == 0)suduttolehx--;
// 			}*/

// 	}
// 	else if(stepG == 4)
// 	{
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 		motion = 10;
// 		countertimeout++;
// 		heading_skr = 0;
// 		if(countertimeout>33) {countertimeout = stepG = 1; step++; }
// 	}
// 	araheksekusi = cariarahRobot(suduteksekusi, myGcol);
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"Step(%d)Pos(%d)dataXG(%d)XG(%d)SGW(%d)SGW1(%d)ST(%d)ST1(%d)SE(%d)AE(%d)\n",stepG,posTiang,dataXG,SudutBolaX(),simpanposisiGW,simpanposisiGW2,sudutxTiang,sudutxTiang2,suduteksekusi,araheksekusi);
// }

// void TaktikcekGw2()
// {
// 	static short suduttolehx,arahtoleh;
// 	static int countertimeouttmbh = 0;
// 	static short hasilgw[3];
// 	static unsigned char simpanposlihat[3];
// 	static unsigned char simpanobs[3];
// 	static int sudutY;
// 	static int sudutX = 73; // sudutY = 75;
// 	int batascountertimeout = 80, bataslihat = 35, delaytoleh = 35;
// 	int batasjarakjauh;

// 	if(pilihstrategi == 1) batasjarakjauh = 4;
// 	else if(pilihstrategi == 2) batasjarakjauh = 3;
// 	//if(stepG==1) enablevision = 0b1100000;
// 	//else enablevision = 0b100000;
// 	sudutY = carisudutbolay(sdtdefy);
// 	enablevision = 0b1100100;//0b1100000;

// 	if(player == 3) {batascountertimeout = 180; bataslihat = 70; delaytoleh = 70;} //sudutY = 83; }
// 	if(player == 2) {batascountertimeout = 200; bataslihat = 80; delaytoleh = 80;} //sudutY = 79; }
// 	ethreadsearching = 0;

// 	if(datakompc!=0 && stepG<4 && stepG>1) {
// 		stepG = 4;
// 		countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 	}

// 	if(stepG == 1)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);
// 		xBall = sdtblax;

// 		if(posrobotY>=batasjarakjauh && flagsudahdekat==0) //kalo masih jauh
// 		{
// 			if(posTiang == 2) { counterpos2++; countertimeouttmbh+=2; }
// 			if (counterpos2 > bataslihat) { simpanposisiGW = 2; hasilgw[1] = 2; sudutTiang[1] = (dataXG-frametengah)/6;} //X2X

// 			if(countertimeout>batascountertimeout)
// 			{
// 				//araheksekusi =1;
// 				//suduteksekusi = 0;
// 				piliheksekusi = 2;
// 				stepG=4;
// 				countertimeout = 0;
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 				countDribb = 500;
// 			}
// 		}
// 		else
// 		{
// 			if(posTiang == 1) { counterpos1++; countertimeouttmbh+=2; }
// 			else if(posTiang == 2) { counterpos2++; countertimeouttmbh+=2; }
// 			if(dataOBS[0]!=0) {counterobs++; fprintf(stderr,"cobs (%d) ",counterobs);}

// 			if (counterpos2 > bataslihat) { simpanposisiGW = 2; hasilgw[1] = 2; sudutTiang[1] = (dataXG-frametengah)/6;} //X2X
// 			else if(counterpos1 > bataslihat) { simpanposisiGW = 1; hasilgw[1] = 1; sudutTiang[1] = (dataXG-frametengah)/6; if(sudutxTiang>0) arahtoleh=kiri; else arahtoleh=kanan;} //X1X
// 			else if(countertimeout>batascountertimeout) { simpanposisiGW = 0; hasilgw[1] = 0; sudutTiang[1] = 0;}  //X0X

// 			if(countertimeout>batascountertimeout+countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat) {
// 				if(posrobotY == 1) sudutX = 73;
// 				else if(posrobotY == 2) sudutX = 60;
// 				else sudutX = 60;
// 				simpanposlihat[1]= jrkGwang;
// 				if(arahtoleh==kiri) suduttolehx = -sudutX; else suduttolehx = sudutX;
// 				flagsudahdekat = 1;
// 				if(counterobs > countertimeout/2) simpanobs[1] = 1; else simpanobs[1] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;

// 			}
// 			piliheksekusi = 1;
// 		}
// 		countertimeout++;
// 		simpanposrobotY = posrobotY;
// 		fprintf(stderr,"RobotY (%d) ",posrobotY);
// 	}
// 	else if(stepG == 2)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);

// 		if(posTiang == 1 && countertimeout>delaytoleh) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>delaytoleh) { counterpos2++; countertimeouttmbh+=2; }
// 		if(dataOBS[0]!=0 && countertimeout>delaytoleh) {counterobs++; fprintf(stderr,"cobs (%d) ",counterobs);}

// 		countertimeout++;

// 		if(simpanposisiGW < 2)
// 		{
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > bataslihat) { simpanposisiGW2 = hasilgw[arahtoleh] = 2; sudutTiang[arahtoleh] = suduttolehx + (dataXG-frametengah)/6;}
// 			else if(counterpos1 > bataslihat) { simpanposisiGW2 = hasilgw[arahtoleh] = 1; sudutTiang[arahtoleh] = suduttolehx + ((dataXG-frametengah)/6);}
// 			else if(countertimeout>batascountertimeout) { simpanposisiGW2 = hasilgw[arahtoleh] =  sudutTiang[arahtoleh] = 0; }


// 			if(countertimeout>batascountertimeout + countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat)
// 			{
// 				if(counterobs > ((countertimeout-delaytoleh)*3/4)) simpanobs[arahtoleh] = 1; else simpanobs[arahtoleh] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 				simpanposlihat[arahtoleh]= jrkGwang;
// 				if(simpanposisiGW > 0 && simpanposisiGW2 >0) simpanposisiGW2 = 2;
// 				if(arahtoleh==kiri) arahtoleh = kanan; else arahtoleh = kiri;
// 				if(arahtoleh==kiri) suduttolehx = -sudutX; else suduttolehx = sudutX;
// 			}
// 		}
// 		else if(simpanposisiGW >= 2)
// 		{
// 			simpanposisiGW2 = 2;
// 			countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 			stepG++;
// 		}
// 	}
// 	else if(stepG == 3)
// 	{
// 		motion = motioncekgw;
// 		yBall = generatesdtY(sudutY);

// 		if(posTiang == 1 && countertimeout>delaytoleh) { counterpos1++; countertimeouttmbh+=2; }
// 		else if(posTiang == 2 && countertimeout>delaytoleh) { counterpos2++; countertimeouttmbh+=2; }
// 		if(dataOBS[0]!=0 && countertimeout>delaytoleh) {counterobs++;}//fprintf(stderr,"cobs (%d) ",counterobs);}

// 		countertimeout++;

// 		if(simpanposisiGW2 < 2)
// 		{
// 			xBall = generatesdtX(suduttolehx);

// 			if (counterpos2 > bataslihat) { hasilgw[arahtoleh] = 2; sudutTiang[arahtoleh] = suduttolehx + (dataXG-frametengah)/6;}
// 			else if(counterpos1 > bataslihat) { hasilgw[arahtoleh] = 1; sudutTiang[arahtoleh] = suduttolehx + ((dataXG-frametengah)/6);}
// 			else if(countertimeout>batascountertimeout) { hasilgw[arahtoleh] = sudutTiang[arahtoleh] = 0; }

// 			if(countertimeout>batascountertimeout + countertimeouttmbh || counterpos2 > bataslihat || counterpos1 > bataslihat)
// 			{
// 				if(counterobs > ((countertimeout-delaytoleh)*3/4)) simpanobs[arahtoleh] = 1; else simpanobs[arahtoleh] = 0;
// 				stepG++; countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 				simpanposlihat[arahtoleh]= jrkGwang;
// 			}
// 		}
// 		else if(simpanposisiGW2 >= 2)
// 		{
// 			countertimeout = counterpos1 = counterpos2 = countertimeouttmbh = counterobs = 0;
// 			stepG++;
// 		}
// 	}
// 	else if(stepG == 4)
// 	{
// 		motion = 10;
// 		suduteksekusi = 0;

// 		for(int i = 0; i<3; i++)
// 		{
// 			if(hasilgw[i] == 1) {
// 				suduteksekusi = sudutTiang[i]*3/2; fprintf(stderr,"Lihat1gwng \n");

// 				if(i == 1) {
// 					suduteksekusi = sudutTiang[i];
// 					if(simpanobs[0] != 0) suduteksekusi-= 20;
// 					if(simpanobs[2] != 0) suduteksekusi+= 20;
// 					fprintf(stderr,"Lihat1gwngdtengah \n");
// 					fprintf(stderr,"obskiri : %d | obskanan : %d | \n",simpanobs[0],simpanobs[2]);

// 					/*if(simpanposlihat[0] > simpanposlihat[2]) suduteksekusi = sudutTiang[i]-20;
// 					else if(simpanposlihat[0] < simpanposlihat[2]) suduteksekusi = sudutTiang[i]+20;
// 					fprintf(stderr,"Lihat1gwngdtengah \n");
// 					fprintf(stderr,"jarakkiri : %d | jarakkanan : %d | \n",simpanposlihat[0],simpanposlihat[2]);
// 					*/
// 				}
// 			}
// 		}


// 		for(int i = 0; i<2; i++)
// 		{
// 			for(int j = i+1; j<3; j++)
// 			{
// 				if(hasilgw[i] == 1 && hasilgw[j] == 1) {suduteksekusi = (sudutTiang[i]  + sudutTiang[j]) / 2; fprintf(stderr,"Lihat1dan1 \n"); break;}
// 			}
// 		}

// 		for(int i = 0; i<3 ; i++)
// 		{
// 			if(hasilgw[i] == 2) {suduteksekusi = sudutTiang[i]; if(posrobotY<2) {if(suduteksekusi>0) suduteksekusi+=5; else suduteksekusi-=5;}   fprintf(stderr,"lihat2langsung \n"); break;} // Lihat 2 Tiang langsung
// 		}

// 		if(suduteksekusi > 90){ suduteksekusi+=5; disampinggw = 1;}
// 		else if(suduteksekusi < -90){ suduteksekusi-=5; disampinggw = 2;}
// 		else disampinggw = 0;

// 		if(suduteksekusi > 100) suduteksekusi = 100;
// 		else if(suduteksekusi <-100) suduteksekusi =-100;

// 		switch(datakompc)
// 		{
// 			case 1: suduteksekusi = 80; break;
// 			case 2: suduteksekusi = 60; break;
// 			case 3: suduteksekusi = 40; break;
// 			case 4: suduteksekusi = 20; break;
// 			case 5: suduteksekusi = 0; break;
// 			case 6: suduteksekusi = -20; break;
// 			case 7: suduteksekusi = -40; break;
// 			case 8: suduteksekusi = -60; break;
// 			case 9: suduteksekusi = -80; break;
// 		}

// 		fprintf(stderr,"======================================================");
// 		for(int i = 0; i<3 ; i++) {fprintf(stderr,"%d(%d)(%d) ",i,hasilgw[i],sudutTiang[i]); hasilgw[i] = sudutTiang[i] = simpanposlihat[i] = 0;}
// 		fprintf(stderr,"SE(%d)PC(%d)\n",suduteksekusi,datakompc);
// 		fprintf(stderr,"======================================================");

// 		stepG++;
// 	}
// 	else if(stepG == 5)
// 	{
// 		xBall = lastXBall;
// 		yBall = lastYBall;
// 		motion = 10;
// 		countertimeout++;
// 		simpansuduteksekusi = suduteksekusi;
// 		heading_skr = countLihatblg = 0;
// 		if(player == 2 || player == 3) {if(countertimeout>233) {countertimeout = stepG = 1; step++; }}
// 		else if(countertimeout>33) {countertimeout = stepG = 1; step++; }
// 	}
// 	fprintf(stderr,"StepG(%d)Pos(%d)dataXG(%d)\n",stepG,posTiang,dataXG);
// 	araheksekusi = cariarahRobot(suduteksekusi, myGcol);
// 	motionAct(xBall,yBall,motion,0);

// }

// /*void TaktikkeBolaBcakup(int arahhadap)
// {
// 	//nBall = ftBall3(dataXB, dataYB, 0);
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	kaki = 0;

// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;
// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 		if(sdtbolay <= 36 && sdtbolax<=20 && sdtbolax>=-20 && arahRobot == arahhadap)
// 		{
// 			countertimeout++;
// 			if(countertimeout>40) { step++; countertimeout =0; motion = 10;}
// 		}
// 		else countertimeout--;

// 		if(arahLihat > 5 && arahLihat < 17) {countLihatblg++;}
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		else if(countLihatblg>50) countLihatblg=50;

// 		if(countLihatblg>=40) flagsudahdekat=0;
// /*		if(sdtbol-ay <= 30 && sdtbolax <= 20 && sdtbolax >= -20)
// 		{
// 			if(arahRobot>2 && arahRobot<=6) motion = 18;
// 			else if(arahRobot>6 && arahRobot<10) motion = 17;
// 			else step++;
// 		}*/
// /*		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			//if(CountLost < 50){
// 			//	xBall = lastXBall;
// 			//	yBall = lastYBall;
// 			//}
// 			if(CountLost > 200) {
// 				step = 1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr," dataX(%d)dataY(%d)\n",dataXB,dataYB);
// }*/

// void TaktikkeBola(int arahhadap)
// {
// 	nBall = ftBall3(dataXB, dataYB, 0);
// 	fprintf(stderr, "nB=%d | ",nBall);
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	kaki = 0;

// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;
// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 		if(sdtbolay <= 5 && sdtbolax<=20 && sdtbolax>=-20)//arahRobot == arahhadap)
// 		{
// 			countertimeout++;
// 			if(countertimeout>50) { step++; countertimeout =0; motion = 10;}
// 		}
// 		else countertimeout--;

// 		if(arahLihat > 5 && arahLihat < 17) {countLihatblg++;}
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		else if(countLihatblg>50) countLihatblg=50;

// 		if(countLihatblg>=40) flagsudahdekat=0;
// 		/*if(sdtbolay <= 30 && sdtbolax <= 20 && sdtbolax >= -20)
// 		{
// 			if(arahRobot>2 && arahRobot<=6) motion = 18;
// 			else if(arahRobot>6 && arahRobot<10) motion = 17;
// 			else step++;
// 		}*/
// 		CountLost=0;
// 	}
// 	else{
// 		countertimeout-=2;
// 		CountLost++;
// 		if(CountLost < 50){
// 			xBall = lastXBall;
// 			yBall = lastYBall;
// 		}
// 		if(CountLost > 200){
// 			step = 0;
// 			CountLost = 0;
// 			flagsudahdekat=0;
// 		}
// 	}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// 	//fprintf(stderr,"NB(%d) | dataX(%d)dataY(%d) | Motion = %d\n",nBall,dataXB,dataYB,motion);
// }

// void TaktikkeBolabeta(int arahhadap)
// {
// 	//nBall = ftBall3(dataXB, dataYB, 0);
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	kaki = 0;

// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;

// 		if(stepH == 1)
// 		{
// 			motion = GerakLurusArah(sdtbolax,sdtbolay,arahRobot,7);
// 			fprintf(stderr," lurus ");
// 		}
// 		else
// 		{
// 			motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 			fprintf(stderr," putar ");
// 		}
// 		if(sdtbolay <= 35 && sdtbolax<20 && sdtbolax>-20 && arahRobot == arahhadap)
// 		{
// 			countertimeout++;
// 			if(countertimeout>40) { step++; countertimeout = countLihatblg = 0; motion = 10;}
// 		}
// 		else countertimeout--;

// 		if(arahLihat > 5 && arahLihat < 15) countLihatblg++;
// 		else countLihatblg--;

// 		if(countLihatblg<0) {countLihatblg=0; stepH =0; }
// 		else if(countLihatblg>50) {countLihatblg=50; stepH = 1;}

// 		if(countLihatblg>=40) flagsudahdekat=0;
// /*		if(sdtbolay <= 30 && sdtbolax <= 20 && sdtbolax >= -20)
// 		{
// 			if(arahRobot>2 && arahRobot<=6) motion = 18;
// 			else if(arahRobot>6 && arahRobot<10) motion = 17;
// 			else step++;
// 		}*/
// 		CountLost=0;
// 	}
// 	else {
// 			countertimeout-=2;
// 			CountLost++;
// 			//if(CountLost < 50){
// 			//	xBall = lastXBall;
// 			//	yBall = lastYBall;
// 			//}
// 			if(CountLost > 200) {
// 				step = 1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"sdtX(%d)sdtY(%d)M(%d)AR(%d)AL(%d)\n",sdtbolax,sdtbolay,motion,arahRobot,arahLihat);
// 	//fprintf(stderr,"Kebola motion = %d\n",motion);
// }

// void TaktikTentukanArahKickOff(){
// 	int batascountertimeout=200;
// 	unsigned char posisiobs;
// 	posisiobs = checkobstacle(0);
// 	if((posisiobs & 0b001) > 0 && flagaktifkanobs) {
// 		fprintf(stderr,"Ada Obs di Kanan !\n");
// 		counterobs1++;
// 		if(counterobs1 >100) {
// 			counterobs1 = 100;			
// 		}
// 	}
// 	else counterobs1--;
// 	if((posisiobs & 0b010) > 0 && flagaktifkanobs) {
// 		fprintf(stderr,"Ada Obs di tengah !\n");
// 		counterobs2++;
// 		if(counterobs2 >100) {
// 			counterobs2 = 100;
// 		}
// 	}
// 	else counterobs2--;
// 	if((posisiobs & 0b100) > 0 && flagaktifkanobs) {
// 		fprintf(stderr,"Ada Obs di Kiri !\n");
// 		counterobs3++;
// 		if(counterobs3 >100) {
// 			counterobs3 = 100;
// 		}
// 	}
// 	else counterobs3--;
// 	if(counterobs1<0) counterobs1 = 0;
// 	if(counterobs2<0) counterobs2 = 0;
// 	if(counterobs3<0) counterobs3 = 0;
	
// 	//Perbandingan
// 	if((counterobs3==100 && counterobs2==100 && counterobs1!=100) || (counterobs3==100 && counterobs1!=100 && counterobs2!=100)){//ada obs kiri tengah
// 		arahNggiring=3;
// 		suduteksekusi=30;
// 		countDribb=countDribbChange=200;
// 	}else 
// 	if((counterobs1==100 && counterobs2==100 && counterobs3!=100) || (counterobs1==100 && counterobs2!=100 && counterobs3!=100)){//ada obs kanan tengah
// 		arahNggiring=19;
// 		suduteksekusi=-30;
// 		countDribb=countDribbChange=200;
// 	}else
// 	if(counterobs2==100 && counterobs1!=100 && counterobs3!=100){
// 		arahNggiring=3;
// 		suduteksekusi=30;
// 		countDribb=countDribbChange=200;
// 	}else{
// 		arahNggiring=1;
// 		suduteksekusi=0;
// 		countDribb=350;
// 	}
	
// 	countertimeout++;
// 	if(countertimeout>batascountertimeout){
// 		countertimeout=0;
// 		xBall=sdtblax;
// 		yBall=sdtexcute;
// 		step++;	
// 	}
// 	motionAct(sdtblax,generatesdtY(70),0,10);
// }

// void TaktikEksekusiDribble()
// {
// 	unsigned char posisiobs;
// 	short modeeksekusi;
// 	aktifkansearching();
// 	enablevision = 0b0100101;
// 	int modetendang=1;
// 	if(nBall==1)
// 	{
// 			fprintf(stderr,"Ada ball\n");
// 		posisiobs = checkobstacle(0);
// 		if((posisiobs & 0b010) > 0 && flagaktifkanobs) {
// 			fprintf(stderr,"Ada Obs di tengah !\n");
// 			counterobseks++;
// 			if(counterobseks >50) {
// 				counterobseks = 51;
// 				modeeksekusi = 4;
// 			}
// 			else modeeksekusi = modetendang;
// 		}
// 		else counterobseks--;
// 		if(counterobseks<0) counterobseks = 0;

// 		motion = GerakEksekusiDribble(dataXB,dataYB);
// 		if(countDribb > 0 && motion >= 20)countDribb--;
// 		else if(countDribb == 0)countDribb=0;
// 		if(countDribb<=0) {
// 			step=2; //Cek Gawang
// 			stepT = 1;
// 			countDribb=0;
// 		}
// 		CountLost = 0;
// 	}
// 	else
// 	{
// 		motion = 10;
// 		countLihat=0;
// 		CountLost++;
// 		if(CountLost < 50){
// 			xBall = lastXBall;
// 			yBall = lastYBall;
// 		}
// 		if(CountLost > 200) {
// 			countertimeout = countgerak = 0;
// 			step = 1;
// 			CountLost = 0;
// 		}
// 	}
// 	motionAct(sdtblax, sdtexcute,motion, mode);
// 	//fprintf(stderr,"NB(%d)Kick(%d)CG(%d)batas(%d)kaki(%d)motion(%d)COBS(%d)\n",nBall,Kick,countgerak,batascounttendang, kaki, motion,counterobseks);

// }





// void TaktikEksekusiwide(short modetendang)
// {
// 	int batascounttendang = 50;
// 	unsigned char posisiobs;
// 	short modeeksekusi;
// 	if(player == 2) batascounttendang = 150;
// 	else if(player == 3) batascounttendang = 50;
// 	aktifkansearching();
// 	enablevision = 0b0100101;
// 	modeeksekusi = modetendang;
// 	//ethreadsearching = 0;

// 	if(nBall==1)
// 	{
// 		posisiobs = checkobstacle(0);

// 		if((posisiobs & 0b010) > 0 && flagaktifkanobs){
// 			//fprintf(stderr,"Ada Obs di tengah !\n");
// 			counterobseks++;
// 			if(counterobseks >50) {
// 				counterobseks = 51;
// 				modeeksekusi = 4;
// 			}
// 			else modeeksekusi = modetendang;
// 		}
// 		else counterobseks--;
// 		if(counterobseks<0) counterobseks = 0;

// 		motion = GerakEksekusiwide(dataXB,dataYB,modeeksekusi);//GerakTendangjauhfast(sdtbolax,sdtbolay);
// 		if(motion < 10)  { Kick = 1; kaki = motion; countgerak++; motion = 10; }
// 		else countgerak--;

// 		if(countgerak>batascounttendang) { motion = kaki; if(motion==0) motion =20; countgerak = batascounttendang+1; }
// 		else if(countgerak<0) countgerak=0;

// 		if(modetendang == 2)
// 		{
// 			if(countDribb > 0 && motion >= 20)countDribb--;
// 			else if(countDribb == 0)countDribb=0;
// 			if(countDribb<=0) {
// 				step++;
// 				stepT = 1;
// 				countDribb=0;
// 			}
// 		}

// 		if(Kick==1&&dataYB<128)
// 		{

// 			countLihat++;
// 			if(countLihat>5) {step++; countLihat =  countgerak = 0;}
// 			stepT = 1;
// 			fprintf(stderr,"TES1!!!! ");
// 		}

// 		/*if(sdtbolay>60 || sdtbolax > 40 || sdtbolax < -40) {
// 			countLihat++;
// 			if(countLihat>10) {step = 1; countLihat=0;}
// 		}*/
// 		CountLost = 0;

// 		//if(arahRobot>6 && arahRobot<8 && sdtbolay<30) {step=1; countLihat=0;}

// 	}
// 	else
// 	{
// 		motion = 10;
// 		if(Kick==1)
// 		{

// 			countLihat++;
// 			if(countLihat>5) {step++; countLihat =  countgerak = 0;}
// 			stepT = 1;
// 			fprintf(stderr,"TES1!!!! ");
// 		}
// 		else
// 		{
// 			Kick=0;
// 			fprintf(stderr,"TES2!!!! ");
// 			countLihat=0;
// 			CountLost++;
// 			/*if(CountLost < 50){
// 				xBall = lastXBall;
// 				yBall = lastYBall;
// 			}*/
// 			if(CountLost > 200) {
// 				countertimeout = countgerak = 0;
// 				step = 1;
// 				CountLost = 0;
// 			}
// 			//step = 1;
// 		}
// 	}
// 	motionAct(sdtblax, sdtexcute,motion, mode);
// 	fprintf(stderr,"NB(%d)Kick(%d)CG(%d)batas(%d)kaki(%d)motion(%d)COBS(%d)\n",nBall,Kick,countgerak,batascounttendang, kaki, motion,counterobseks);

// }

// void TaktikNggiring2(short arah)
// {
// 	aktifkansearching();
// 	//enablevision = 0b00001;
// 	//ethreadsearching = 1;
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	//yBall = sdtexcute-100;
// 	//xBall = sdtblax;
// 	if (nBall == 1 ){ //&& (kaki == 20 || kaki <= 30 || kaki == 27 || kaki == 28)
// 		//kakie = 0;
// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arah);//GerakTendangjauhfast(sdtbolax,sdtbolay+35);
// 		if(arahRobot == arah) motion = GerakNggiring(sdtbolax,sdtbolay);
// 		if(countDribb > 0 && motion >= 20)countDribb--;
// 		else if(countDribb == 0)countDribb=0;
// 		if(countDribb<=0) {
// 			step++;
// 			stepT = 1;
// 			countDribb=0;
// 		}
// 	}
// 	else {
// 		//yBall = sdtexcute;
// 		//xBall = sdtblax;
// 		motion = 10;
// 		CountLost++;
// 		if(CountLost > 50) {
// 			step = 1;
// 			CountLost = 0;
// 		}
// 	}
// 		fprintf(stderr,"(%d)(%d)(%d)(%d)(%d)\n",countDribb,Kick,sdtbolax,sdtbolay,motion);

// 		motionAct(xBall, yBall,motion, 10);
// 		skelTdg=0;
// }

// void TaktikLuruskanGW(int sudutputar)
// {
// 	aktifkansearching();
// 	//arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	//kaki = 0;
	
// 	if(nBall == 1)
// 	{
// 		lastXBall = xBall;
// 		lastYBall = yBall;
// 		if(sdtbolax> -20 && sdtbolax< 20) motion = GerakArahBolaLurus(sdtbolax,sdtbolay);
// 		else  motion =  GerakHadapBola(sdtbolax,sdtbolay,arahRobot,araheksekusi);

// 		if(arahLihat > 6 && arahLihat < 16) {if(sdtbolay > 60) countLihatblg++;}
// 		else countLihatblg--;

// 		if(countLihatblg<0) countLihatblg=0;
// 		if(countLihatblg>100){
// 			step=1;
// 			CountLost = 0;
// 			countLihatblg=0;
// 			flagsudahdekat=0;
// 		}

// 		if(sdtbolay <= 35 && sdtbolax<20 && sdtbolax>-20)
// 		{
// 			if(sudutputar>=0)
// 			{
// 				if(heading_skr<sudutputar) motion = 17;
// 				else {
// 					if(arahRobot!=arahLihat){
// 						motion = GerakCoverBallSlow(sdtbolax,sdtbolay);
// 					}else{
// 						step++; motion = 10;
// 						suduteksekusi = countLihatblg = 0;
// 						countDribb = 500; if(player == 2 | player == 3) countDribb = 1500;
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(heading_skr>sudutputar) motion = 18;
// 				else {
// 				 if(arahRobot!=arahLihat){
// 						motion = GerakCoverBallSlow(sdtbolax,sdtbolay);
// 					}else{step++; motion = 10; 
// 								suduteksekusi = countLihatblg = 0; countDribb = 500; if(player == 2 | player == 3) countDribb = 1500;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else {
// 			CountLost++;
// 			if(CountLost > 300) {
// 				step = 1;
// 				CountLost = 0;
// 				flagsudahdekat=0;
// 			}
// 		}
// 	if(countertimeout<0) countertimeout=0;
// 	motionAct(xBall,yBall,motion,0);
// 	fprintf(stderr,"Motion(%d) Heading(%d) eks(%d)\n",motion,heading_skr,suduteksekusi);
// }
// int lar=0;
// void lari(int mode){
// 	if(mode==0){
// 		if(lar<=200) motion=14;
// 		else if(lar>200 && lar<=400) motion=20;
// 		else if(lar>400 && lar<=600) motion=15;
// 		else { mode=1; lar=0; }
// 	}
// 	if(mode==1){
// 		if(lar<=200) motion=29;
// 		if(lar>200 && lar<=400) motion=14;
// 		else { lar=0; step++; }
// 	}
// 	else if(mode==2){
// 		if(lar<=700){ motion=30;}
// 		else { step++; lar=0; }
// 	}
// 	lar++;
// 	motionAct(xBall, yBall,motion, 10);
// 	fprintf(stderr, "Mode = %d\t | \tMotion=%d | Count = %d\n",mode,motion, lar);
// }

// void trackdefense()
// {
// 	aktifkansearching();
// 	arahRobot = CariSudut(0,myGcol);
// 	arahLihat = CariArahLihat(SudutBolaX(),myGcol);

// 	if(nBall == 1 && arahLihat >= 6 && arahLihat <= 16)
// 	{
// 		printf("bola samping belakang");
// 		step++;
// 		//motion = 10;
// 	}
// 	else if(nBall == 1 && ((1 <= arahLihat <= 5) || (17 <= arahLihat <= 20)) && sdtbolay <= 55)
// 	{
// 		printf("bola depan");
// 		step++;
// 		//motion = 10;
// 	}
// 	else
// 	{
// 		//tunggudeff();
// 		//TaktikTunggubola();
// 		int countdefense;
// 		//TaktikStayBALL();
// 		//stepH=1;
// 		//motion = 10;
// 		countdefense++;

// 		if(countdefense > 100)
// 		{
// 			step = 1;
// 			countdefense = 0;
// 		}

// 	}
// 	motionAct(xBall, yBall, motion, 10);
// }

// void TaktikkeBolaDef(int arahhadap)
// {
// 	aktifkansearching();
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	arahLihat = CariArahLihat(SudutBolaX(),myGcol);

// 	if(nBall == 1)
// 	{
// 		motion = GerakHadapBola(sdtbolax,sdtbolay,arahRobot,arahhadap);
// 		if(sdtbolay <= 30 && sdtbolax<=10 && sdtbolax>=-10 && arahRobot == arahhadap)
// 		{
// 			step++;
// 		}
// /*
//  	 	if(sdtbolay <= 30 && sdtbolax <= 20 && sdtbolax >= -20)
// 		{
// 			if(arahRobot>2 && arahRobot<=6) motion = 18;
// 			else if(arahRobot>6 && arahRobot<10) motion = 17;
// 			else step++;
// 		}
// */
// 		CountLost=0;
// 	}
// 	else
// 	{
// 		CountLost++;
// 		if(CountLost > 500)
// 		{
// 			step = 1;
// 			CountLost = 0;
// 		}
// 	}

// 	motionAct(xBall,yBall,motion,0);
// 	lastXBall = xBall;
// 	lastYBall = yBall;
// }
// void TaktikEksekusiDef()
// {
// 	aktifkansearching();
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	arahLihat = CariArahLihat(SudutBolaX(),myGcol);

// 	if(nBall==1)
// 	{
// 		motion = GerakTendangjauhfast(sdtbolax,sdtbolay);
// 		if(motion<10)
// 		{
// 			Kick = 1;
// 			//dtJob = 0;
// 			step=1;
// 		}
// 	}
// 	if(sdtbolay > 40)
// 	{
// 		countLock++;
// 		if(countLock > 200)
// 		{
// 			step = 2;
// 			countLock = 0;
// 		}
// 	}
// 	else
// 	{
// 		if(Kick==1)
// 		{
// 			//dtJob = 0;
// 			countLock++;
// 			if(countLock>5)
// 			{
// 				step=1;
// 				countLock = 0;
// 			}
// 			//step = 2;
// 			//step = 4;
// 		}
// 		else
// 		{
// 			Kick=0;
// 			countLock=0;
// 			//step = 4;
// 			step=1;
// 			//step = 1;
// 		}
// 	}
// 	motionAct(xBall,yBall,motion,0);
// }

// void TaktikKembali()
// {
// 	aktifkansearching();
// 	arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 	arahLihat = CariArahLihat(SudutBolaX(),myGcol);
// 	motion = firstdefense(7);
// 	motionAct(xBall, yBall, motion, 10);
// }

// int firstdefense(int pedoman)
// {

// 	aktifkansearching();
// 	arahRobot = CariSudut(0,myGcol);
// 	arahLihat = CariArahLihat(SudutBolaX(),myGcol);
// 	if(nBall == 0)
// 	{
// 			if(pedoman >=1 && pedoman <= 7)
// 			{
// 				if(arahRobot > pedoman && arahRobot <= 12)
// 				{motion = 11;}
// 				else if(arahRobot < pedoman && arahRobot <= 12)
// 				{motion = 12;}
// 				else
//  				{motion = 0;}
// 			}
// 			else if(pedoman > 7 && pedoman <= 12)
// 			{
// 				if(arahRobot < pedoman && arahRobot <= 12)
// 				{motion = 11;}
// 				else if(arahRobot > pedoman && arahRobot <=12)
// 				{motion = 12;}
// 				else
// 				{motion = 0;}
// 			}
// 			return(motion);

// 	}
// 	step++;
// 	fprintf(stderr,"arah Robot : %d | Motion : %d \n",arahRobot,motion);
// 	motionAct(xBall, yBall, motion, 10);
// }

// void kirimjarak()
// {
// 		enablevision = 0b1100000;
// 		ethreadsearching = 0;
// 		yBall = generatesdtY(75);
// 		xBall = sdtblax;
// 		countertimeout++;

// 		if(countertimeout>100)
// 		{
// 			printf("Masuk Step Akhir");
// 			if((posrobotX == 1 || posrobotX == 2) && countertimeout>200)
// 			{
// 				if(posrobotY == 5 || posrobotY == 6)
// 				{
// 					motion = firstdefense(6);
// 					countertimeout = 0;
// 				}
// 				else if(posrobotY == 7)
// 				{
// 					motion = firstdefense(4);
// 					countertimeout = 0;
// 				}
// 			}
// 			else if((posrobotX == 4 || posrobotX == 5) && countertimeout>200)
// 			{
// 				if(posrobotY == 5 || posrobotY == 6)
// 				{
// 					motion = firstdefense(8);
// 					countertimeout = 0;
// 				}
// 				else if(posrobotY == 7)
// 				{
// 					motion = firstdefense(10);
// 					countertimeout = 0;
// 				}
// 			}
// 			else if((posrobotX == 3) && countertimeout>200)
// 			{
// 				if(posrobotY == 5 || posrobotY == 6)
// 				{
// 					motion = firstdefense(7);
// 					countertimeout = 0;
// 				}
// 				else if(posrobotY == 7)
// 				{
// 					motion = firstdefense(1);
// 					countertimeout = 0;
// 				}
// 			}
// 			motionAct(xBall, yBall, motion, 10);
// 			step++;
// 		}
// }

// void MajuBelakang()
// {
// 	int countermm;
// 	countermm++;
// 	motion = 20;
// 	if(countermm == 20)
// 	{
// 			aktifkansearching();
// 			printf("counter = %d",countermm);
// 			arahRobot = CariSudut(COMPASS_Blue,myGcol);
// 			arahLihat = CariArahLihat(SudutBolaX(),myGcol);
// 			motion = firstdefense(1);
// 			countermm = 0;
// 	}
// 	motionAct(xBall, yBall, motion, 10);
// }

// int TaktikStayBALLDimas()
// {
// 	//dtJob=1;
// 	//nBall = ftBall3(dataXB,dataYB,0);
// 	aktifkansearching();
// 	if(nBall == 1)
// 	{
// 		fprintf(stderr,"Lihat BOLA1 -> ");
// 		motion = 10;
// 		//dataTm = 1;
// 		if(sdtbolay>70 && sdtbolay<=86 && motion == 10){
// 			cariGawangOut++;
// 			if(cariGawangOut>20) { step=2; cariGawangOut=0;}
// 		}
// 		else cariGawangOut=0;
// 	}
// 	else //dataTm = 0;
// 	cariGawang = 0;
// 	motionAct(xBall, yBall, motion, 10);
// 	//writeIPC(1,xBall,yBall,motion,arahRobot);
// 	printf("mo=%d__sdtBol=%d\n",motion,sdtbolay);
// 	step++;
// }

// void DirectPass(){
// 	//aktifkansearching();
// 	enablevision = 0b00001;
// 	xBall=sdtblax+500;
// 	yBall=sdtexcute+300;
// 	if(dataXB!=0 && dataYB!=0){
// 		if((dataXB-lastXBall>5 || dataXB-lastXBall<100) && lastXBall!=0 && speedX<dataXB-lastXBall)speedX=dataXB-lastXBall;else CountDelay++;
// 		if((dataYB-lastYBall>5 || dataYB-lastYBall<100) && lastYBall!=0 && speedY<dataYB-lastYBall)speedY=dataYB-lastYBall;else CountDelay++;
// 		if(CountDelay>20){speedX=0;speedY=0;CountDelay=0;}
// 		fprintf(stderr,"speedX=%4d, speedY=%4d\n",speedX,speedY);
// 		if(dataXB>220 && dataYB>290)motion=6;
// 		lastXBall=dataXB;
// 		lastYBall=dataYB;
// 	}else{
// 		motion=0;
// 		//lastXBall=0;
// 		//lastYBall=0;
// 		CountLost++;
// 		if(CountLost>20){speedX=0;speedY=0;CountLost=0;}
// 	}
// 	motionAct(xBall,yBall,motion,10);
// }
