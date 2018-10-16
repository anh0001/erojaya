//============================================================================
// Name        : strategy.cpp / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include "eheader.h"

void bufferTendang_Generator(){
	bufferTendang=rotasiarah((-1*((int)(dtaXPOS/BUFFERTENDANG_CONST)))+arahTendang+arahInitial);
	if(bufferTendang>5 && bufferTendang<11)bufferTendang=4;
	else if(bufferTendang<17 && bufferTendang>=11)bufferTendang=18;
}

//Strategi Default Normal
void strategi_serang_WIDE3(){
	if(dtaFall==0){step=0;mainx=0;}
	else if(dtaFall==1){step=1;CountLook=0;}  //Saat robot jatuh
	else if(dtaFall==3){step=7;countLihat=0;} //Saat robot menendang
	dtJob=step;
	switch(step)
	{
		case 0:  Initialize();break;
		case 1:  lostball();break;
		case 2:  if(initialPosition>5)TaktikkeBola3(1);
				 else TaktikkeBola3(arahTendang);
				 break;//
		case 3:  PositionGenerator();break;
		case 4:  TaktikNggiring3();break;
		case 5:  TaktikLuruskanGW_wide();break;
		case 6:  TaktikEksekusi4();break;
		case 7:  TaktikPEksekusi();break;
		case 8:  TaktikEksekusiSamping();break;
		case -1: lostball_positioning();break;
		case -2: 	aktifkansearching();
					motionAct(xBall,yBall,0,0);
					if(dtaFall==2){
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
					else 
					if(dtaFall==6)step=-2;
				 break;
		case -3: step=1;break;
		default: Initialize();break;
	}
}

void strategi_serang_WIDE4(){
	if(dtaFall==0)step=0;
	else if(dtaFall==1){step=1;CountLook=0;}  //Saat robot jatuh
	else if(dtaFall==3){step=7;countLihat=0;} //Saat robot menendang
	
	if ((laststep==6 && step==7) || (laststep==9 && step==7)){
		arahInitial=0;
		bufferTendang=rotasiarah((-1*((int)(dtaXPOS/BUFFERTENDANG_CONST)))+arahTendang);
		if(bufferTendang>4 && bufferTendang<11)bufferTendang=4;
		else if(bufferTendang<18 && bufferTendang>=11)bufferTendang=18;
		arahTendang=bufferTendang;
		resetOdometry=true;
	}

	dtJob=step;
	switch(step)
	{
		case 0: Initialize();break;
		case 1: bufferTendang_Generator();
				lostball();break;
		case 2: bufferTendang_Generator();
				TaktikkeBola4(bufferTendang);
				break;
		case 3: PositionGenerator();break;
		case 4: TaktikNggiring3();break;
		case 5: bufferTendang_Generator();
				TaktikLuruskanGW_wide();
				break;
		case 6:  TaktikEksekusi4();break;
		case 7:  TaktikPEksekusi();break;
		case 8: bufferTendang_Generator();
				TaktikLuruskanGW_wide_rokh();break;
		case 9:  TaktikEksekusiSamping();break;
		case -1: lostball_positioning();break;
		case -2: 	aktifkansearching();
					motionAct(xBall,yBall,0,0);
					if(dtaFall==2){
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
					else 
					if(dtaFall==6)step=-2;
				 break;
		default: Initialize();break;
	}
	laststep=step;
}

void strategi_bertahan_cpp(){
	if(dtaFall==0){step=0;mainx=0;}
	else if(dtaFall==1){step=1;CountLook=0;}
	else if(dtaFall==3){step=7;countLihat=0;}

	if(step==6 || step==8 || step==9)step=2;
	dtJob=step;
	switch(step)
	{
		case 0: Initialize();break;
		case 1: bufferTendang_Generator();
				lostball_bertahan();break;
		case 2: bufferTendang_Generator();
				TaktikBertahan(); break;
		case 7:  TaktikPEksekusi();break;
		case -1: lostball_positioning();break;
		case -2: 	aktifkansearching();
					motionAct(xBall,yBall,0,0);
					if (dtaFall==2){
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
				break;
		case -3:motionAct(sdtblax,sdtexcute,0,0);
				break;
		default: Initialize();break;
	}
	if(dtflagsama==1 && diving_mode)catchflagsama=1;
}

/*===================================strategi penalty=======================================*/
void strategi_penalty_WIDE(){
	if(dtaFall==0)step=0;
	else if(dtaFall==1){step=1;CountLook=0;}  //Saat robot jatuh
	else if(dtaFall==3){step=7;countLihat=0;} //Saat robot menendang
	
	if ((laststep==6 && step==7) || (laststep==9 && step==7)){
		arahInitial=0;
		bufferTendang=rotasiarah((-1*((int)(dtaXPOS/450)))+arahTendang);
		arahTendang=bufferTendang;
		resetOdometry=true;
	}
	if(step==8 || step==9)step=5;
	dtJob=step;
	//if (step==7)dtJob = 1;
	switch(step)
	{
		case 0:  InitializePen();break;
		case 1: bufferTendang_Generator();
				lostball();break;
		case 2: bufferTendang_Generator();
				TaktikkeBola4(bufferTendang);
				break;
		case 3: PositionGenerator();break;
		case 4: TaktikNggiring3();break;
		case 5: bufferTendang_Generator();
				TaktikLuruskanGW_wide();
				break;
		case 6: TaktikEksekusi4();break;
		case 7: TaktikPEksekusi();break;
		case 8: bufferTendang_Generator();
				TaktikLuruskanGW_wide_rokh();break;
		case 9:  TaktikEksekusiSamping();break;
		case -1: lostball_positioning();break;
		case -2: 	aktifkansearching();
					motionAct(xBall,yBall,0,0);
					if(dtaFall==2){
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
					else 
					if(dtaFall==6)step=-2;
				 break;
		default: InitializePen();break;
	}
	laststep=step;
}

/*===================================GOAL KICKOFF=======================================*/
void strategi_goal_kickoff(){
	if(dtaFall==0){step=0;mainx=0;}
	else if(dtaFall==1){step=1;CountLook=0;}  //Saat robot jatuh
	else if(dtaFall==3){step=7;countLihat=0;} //Saat robot menendang
	dtJob=step;
	switch(step)
	{
		case 0: Initialize();break;
		case 1: Goal_Teknik();break;
		case -1: 	motionAct(xBall,yBall,0,0);
					if(dtaFall==2){
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
					else 
					if(dtaFall==6)step=-2;
				 break;
		default: Initialize();break;
	}
	laststep=step;
}


//Dengan Game Controller======================================================
void init(){
	resetOdometry=true;
	play = dtJob = 0;
	step = 1;
	countertimeout = 0;
	countMM = CountKF = 0;
	lastrefree = BackHome = 1;
	hadapGawang = nextstep = kaki = posRobot = lihatGoal =countWait=countLock= 0;
	setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
	Kick = 0;
	stepTB = 1;
	servoX = GoalXservo = sdtblax;
	heading_skr = 0;
	stepG=counterpos1=0;
	//flagGetPosition=0; //reset flag pada taktik PositionGenerator
	initialPosition=10;
	batasGiringY=2000;
	flagKickOff=false;
	//flagStopDribble=false;
	arahNggiring=arahTendang=bufferTendang=1;
	countLock=0;
	flagPEX=0;
	countertimeout=CountLost=countLihat=countn=CountAda=0; //reset all counter
	stepG=stepK=majulost=majuloss=0; //reset all variable to zero
	flagsudahdekat=0;
	flaginposition=0;
	flagPuterKO=false;
}

void kanankiri(){
	if(dtaFall>=40 && dtaFall<=49){
		mainx=KANAN;
	}else 
	if(dtaFall>=50 && dtaFall<=59){
		mainx=KIRI;
	}
}

void bermain(){
	refree = Tasking();
	state2nd=Tasking2();
	switch (refree){
		case 0:
		{
			strategi_serang_WIDE4();
		}break;
		case 1 : // initial state
		{
			mainx=0;
			if(firstHit==false){ //reset variable first
				init();
				firstHit=true;
			}

			//tombol harus ditekan saat melakukan positioning
			if((dtaFall>=40 && dtaFall<=49) ||(dtaFall>=50 && dtaFall<=59)){
				//flagStrategi=false;

				//kondisi titik belum terpenuhi
				if(flagPuterKO==false){
					init_positioning();
				}else{
					//terpenuhi dan harus hadap arah 1
					CheckArahMusuh();
				}
			}else{
				resetOdometry=true;
				flagPuterKO=false;
				firstHit=false;
			}			
		}break;

		case 2 : //Playing (Saat Musuh KickOff)
		{
			if(lastrefree==7 || lastrefree==8){
				flaginposition=true;
				strategi_penalty_WIDE();
			}else{
				mainx=0;
				//lastrefree = refree;
				if(play==0){
					if(dtaYPOS<1500){
						aktifkansearching();
						if(nBall==1){
							motion=GerakHadapBola(sdtbolax,sdtbolay,arahRobot,1);
							if(motion>=40 && motion<=49)motion-=10;
							else if(motion>=50 && motion<=59)motion-=20;
						}else{
							if(arahRobot<11 && arahRobot>1)motion=31;
							else if(arahRobot>11 && arahRobot<=20)motion=32;
							else motion=30;
						}
						motionAct(xBall,yBall,motion,0);
					}else waitball();
				}
				if(play==0)sek=0;
				else sek=1;
				step = 1;
			}

		}break;

		case 3: //State Play (saat EROS kickoff)
		{
			kanankiri();
			if(sek == 0){				
				sek=1; 
				//step=1;
				aktifkansearching();
				play = 1;
			}

		}break;

		case 4: //State Finish
		{
			init();
			firstHit=false;
			motionAct(sdtblax, sdtexcute, 0, 10);
		}break;

		case 5: //State Ready for Kickoff
		{
			mainx=0;

			//jika titik belum terpenuhi, GC pindah dari initial->ready, arahTendang baru tergenerate otomatis
			if(dtaXPOS!=0 && resetOdometry==false)bufferTendang=rotasiarah((-1*((int)(dtaXPOS/BUFFERTENDANG_CONST)))+arahTendang);
			resetOdometry=true;
			aktifkansearching();
			play=sek=0;
			motion = 0;
			motionAct(xBall, yBall, 0, 10);
		}break;

		case 6: //State Ready for Defense
		{
			mainx=0;

			//jika titik belum terpenuhi, GC pindah dari initial->ready, arahTendang baru tergenerate otomatis
			if(dtaXPOS!=0 && resetOdometry==false)bufferTendang=rotasiarah((-1*((int)(dtaXPOS/BUFFERTENDANG_CONST)))+arahTendang);
			resetOdometry=true;
			flagsudahdekat=false;
			aktifkansearching();
			play=sek=0;
			motion = 0;
			motionAct(xBall, yBall, 0, 10);			
		}break;

		case 7: //State Ready atau Set Persiapan Penalty
		{
			lastrefree=refree;
			flaginposition=true;
			lastrefree=refree;
			aktifkansearching();
			// if(nBall == 1){
				//if(yBall < ((sdtexcute+sdtdefy)/2)){
			ModeKickOff2(); 
				// }else{
				// 	arahNggiring=arahTendang=1;
				// }
			// 
			play=sek=0;
			motion = 0;
			motionAct(xBall, yBall, 0, 90);
		}break;

		case 8: //State Ready atau Set Persiapa Musuhnya Penalty
		{
			lastrefree=refree;
			ethreadsearching = 0;
			xBall=sdtblax;
			yBall=sdtdefy;
			step=1;
			play=sek=0;
			motionAct(xBall, yBall, 0, 90);
		}break;

		case 9: //State set for Kickoff
		{
			lastrefree=refree;
			mainx=0;//reset mainx
			aktifkansearching();
			resetOdometry=true;
			arahTendang=bufferTendang;
			ModeKickOff2();
			flaginposition=true;
			play=0;
			if(nBall && sdtbolay<50){
				step=5;
			}else{
				step=1;
			}
			motion = 0;
			motionAct(xBall, yBall, 0, 10);
		}break;

		case 10: //State Set for Defense
		{
			lastrefree=refree;
			mainx=0;
			resetOdometry=true;
			flagsudahdekat=false;
			arahTendang=bufferTendang;
			ModeDefend2();
			aktifkansearching();
			flaginposition=true;
			if(nBall == 1){
				KickOff=1;
				if(yBall < ((sdtexcute+sdtdefy)/2)){			
					dtJob=2;
				}else{
					dtJob=1;
				}
			}
			play=sek=0;
			step=1;
			motion = 0;
			motionAct(xBall, yBall, 0, 10);
		}break;
	}

	if(play >= 1){
		playing();
	}

	//fprintf(stderr,"dCom[%d]aR[%3d]ref[%2d]2nd[%2d]dJ[%2d]P[%d]nBall[%d]KO[%d]aN[%d]\n",dtComm,arahRobot,refree,state2nd,dtJob, play, nBall, KickOff,arahNggiring);
}

void playing(){
	if(play == 1 || play == 2){
		if(dtComm!=lastDtComm){
			if(lastDtComm == 9)	step = 0;
			else {
				if(flaginposition){
					//if(step<2)step = 1;
				}
				else step=-1;
			}
			countertimeout = 0;
			CountLost=0;
		}
		lastDtComm = dtComm;

		comKickOff=0;
		if(state2nd!=0 && dtComm!=9){
			switch(state2nd){
				case 1:case 3:case 5: strategi_serang_WIDE4(); break;
				case 2:case 4:case 6: strategi_bertahan_cpp(); break;
				default:strategi_serang_WIDE4();break;
			}	
		}else{
			switch(dtComm){
				case (0 || 1 || 3 || 4): 
						 strategi_serang_WIDE4(); break;
				case 2 : strategi_bertahan_cpp(); break;
				case 9 : step=-1; catchflagsama=0; dtJob=0; motionAct(sdtblax,sdtdefy,0,0); break;
				default: strategi_serang_WIDE4(); break;
			}
		}
	}
}

void ModeKickOff2(){
	if(dtaFall>=50 && dtaFall<=55){
		if(dtaFall==50){
			arahInitial=0;
		}else
		if(dtaFall==51){
			//arahNggiring=arahTendang=19;
			arahInitial=-2;
		}else 
		if(dtaFall==52){
			//arahNggiring=arahTendang=20;
			arahInitial=-1;
		}else
		if(dtaFall==53){
			//arahNggiring=arahTendang=1;
			arahInitial=0;
		}else
		if(dtaFall==54){
			//arahNggiring=arahTendang=2;
			arahInitial=1;
		}else
		if(dtaFall==55){
			//arahNggiring=arahTendang=3;
			arahInitial=2;
		}
	}else
	if(dtaFall>=40 && dtaFall<=45){
		if(dtaFall==40){
			arahInitial=0;
		}else
		if(dtaFall==41){
			//arahNggiring=arahTendang=19;
			arahInitial=-2;
		}else 
		if(dtaFall==42){
			//arahNggiring=arahTendang=20;
			arahInitial=-1;
		}else
		if(dtaFall==43){
			//arahNggiring=arahTendang=1;
			arahInitial=0;
		}else
		if(dtaFall==44){
			//arahNggiring=arahTendang=2;
			arahInitial=1;
		}else
		if(dtaFall==45){
			//arahNggiring=arahTendang=3;
			arahInitial=2;
		}
	}
}

void ModeDefend2(){
	arahInitial=0;
	if(dtaFall>=50 && dtaFall<=55){
		if(dtaFall==50){
			arahTendang=1;
		}else
		if(dtaFall==51){
			arahTendang=19;
		}else 
		if(dtaFall==52){
			arahTendang=20;
		}else
		if(dtaFall==53){
			arahTendang=1;
		}else
		if(dtaFall==54){
			arahTendang=2;
		}else
		if(dtaFall==55){
			arahTendang=3;
		}
	}else
	if(dtaFall>=40 && dtaFall<=45){
		if(dtaFall==40){
			arahTendang=1;
		}else
		if(dtaFall==41){
			arahTendang=19;
		}else 
		if(dtaFall==42){
			arahTendang=20;
		}else
		if(dtaFall==43){
			arahTendang=1;
		}else
		if(dtaFall==44){
			arahTendang=2;
		}else
		if(dtaFall==45){
			arahTendang=3;
		}
	}
}

void waitball(){
	aktifkansearching();
	servoX = xBall;
	GoalXservo = xBall;
	servoY = yBall;
	dComm = 1;
	motion=0; //tambahan
	motionAct(xBall, yBall, 0, 10);
}

int Tasking(){
	static int BTask;

	if(myTask != BTask){
		countTask--;
		if(countTask <= 0)countTask=0;
	} else if(myTask == BTask){
		countTask++;
		if(countTask >= 5)countTask=5;
	}
	if(countTask<=0){
		BTask = myTask;
	}

	return BTask;
}

int Tasking2(){
	static int BTask;

	if(myTask2 != BTask){
		countTask2--;
		if(countTask2 <= 0)countTask2=0;
	} else if(myTask == BTask){
		countTask2++;
		if(countTask2 >= 5)countTask2=5;
	}
	if(countTask2<=0){
		BTask = myTask2;
	}

	return BTask;
}

void CheckArahMusuh(){
	ethreadsearching = 0;
	dtJob = 0;
	xBall = sdtblax;
	yBall = sdtdefy;
	resetOdometry=true;
	arahTendang=1;
	motion = GerakPutarArah(arahRobot,1);
	motionAct(xBall, yBall, motion, 10);
}

void init_positioning(){
	int MIN_MAIN_KIRI=-3100,MAX_MAIN_KIRI=500;
	int MIN_MAIN_KANAN=-500,MAX_MAIN_KANAN=3100;
	
	if(arahRobot<=20 && arahRobot>=11){ // Lepaskan robot dari kanan
		//16
		arahTendang=17;
		initialPosition=10;
		if(arahRobot>16)motion=21;
		else
		if(arahRobot<16)motion=22;
		else {
			motion=20;
		}
		if(dtaXPOS<MIN_MAIN_KIRI){
			flagPuterKO=true;
		}
		sprintf(debug_print,"[LostballPos] :: Going to left [%d][%d]",dtaXPOS,MIN_MAIN_KIRI);
		DebugAI(debug_print);
	}else 
	if(arahRobot>=1 && arahRobot<11){ //Lepaskan robot dari kiri
		//6
		arahTendang=5;
		initialPosition=9;
		if(arahRobot>6)motion=21;
		else
		if(arahRobot<6)motion=22;
		else {
			motion=20;
		}
		if(dtaXPOS>MAX_MAIN_KANAN){
			flagPuterKO=true;
		}
	}
	sprintf(debug_print,"[LostballPos] :: Going to left [%d][%d]",dtaXPOS,MAX_MAIN_KIRI);
	DebugAI(debug_print);

	//fprintf(stderr, "xBall=%d | yBall=%d | motion=%d\n",xBall,yBall,motion);
	motionAct(sdtblax, sdtdefy,motion, 10);
}











//Strategi Utama==============================================================
//SEMANGAT NASIONAL Bismillah Juara 1

//Strategi KickOff Aggressive
// void strategi_kickoff3(){
// 	switch(step)
// 	{
// 		dtJob = step+1;
// 		dataTm = step+1;
// 		if(step == 2){
// 			dtJob = 3;
// 		}
// 		case 1:	KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				step++;break;
// 		case 2: TaktikTentukanArahKickOff();break;
// 		case 3: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 4:	TaktikEksekusi3(); break;
// 		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				step = play = 1;
// 				step = play = 1;
// 				step = play = 1;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }

//Strategi KickOff Normal
// void strategi_kickoff2(){
// 	switch(step){
// 		dtJob = step+1;
// 		dataTm = step+1;
// 		if(step == 2){
// 			dtJob = 3;
// 		}
// 		case 1:	KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				step++;break;
// 		case 2: TaktikTentukanArahKickOff();break;
// 		case 3: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 4:	TaktikNggiringKickOff(); break;
// 		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				step = play = 1;
// 				step = play = 1;
// 				step = play = 1;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }

//Strategi KickOff OneTouch==============================================================================
// void strategi_kickoff4L(){
// 	dtJob = step+1;
// 	dataTm = step+1;
// 	if(step == 2){
// 		dtJob = 3;
// 	}
// 	switch(step){
// 		case 1:	KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				countDribb=300;
// 				step++;break;
// 		case 2: suduteksekusi=30;
// 				motionAct(sdtblax,sdtexcute,20,10);
// 				countDribb--;
// 				if(countDribb<=0){step++;countDribb=0;}
// 				break;
// 		case 3: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 4:	TaktikEksekusi3(); break;
// 		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }
// void strategi_kickoff4R()
// {
// 	dtJob = step+1;
// 	dataTm = step+1;
// 	if(step == 2){
// 		dtJob = 3;
// 	}
// 	switch(step)
// 	{
// 		case 0: play = 1;
// 		case 1:	enablevision = 0b00001;
// 				KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				suduteksekusi=-14;
// 				countDribb--;
// 				motionAct(sdtblax,sdtdefy,0,10);
// 				if(countDribb<=0){step++;countDribb=1000;}break;
// 		case 2: countDribb--;
// 				motionAct(sdtblax,sdtdefy,40,10);
// 				if(countDribb<=0){step++;countDribb=0;}break;
// 		case 3: TaktikkeBola3(1);break;
// 		case 4: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 5:	TaktikEksekusi3(); break;
// 		case 6: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }

// void strategi_kickoff5R()
// {
// 	dtJob = step+1;
// 	dataTm = step+1;
// 	if(step == 2){
// 		dtJob = 3;
// 	}
// 	switch(step)
// 	{
// 		case 1:	KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				step++;break;
// 		case 2: suduteksekusi=-30;step++;break;
// 		case 3: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 4:	TaktikEksekusi3(); break;
// 		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }
// void strategi_kickoff5L()
// {
// 	dtJob = step+1;
// 	dataTm = step+1;
// 	if(step == 2){
// 		dtJob = 3;
// 	}
// 	switch(step)
// 	{
// 		case 0: play = 1;
// 		case 1:	enablevision = 0b00001;
// 				KickOff=1; 
// 				counterobs1=counterobs2=counterobs3=0;
// 				countertimeout=0;
// 				counterobseks=0;
// 				CountLost=0;
// 				suduteksekusi=14;
// 				countDribb--;
// 				motionAct(sdtblax,sdtdefy,40,10);
// 				if(countDribb<=0){step++;countDribb=0;}break;
// 		case 2: TaktikkeBola3(1);break;
// 		case 3: TaktikLuruskanKickOff(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
// 		case 4:	TaktikEksekusi3(); break;
// 		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 				nextstep = sdtGWX = Kick = stepE = 0;
// 				stepH = stepTB = step = 1;
// 				servoX = GoalXservo = sdtblax;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				play = 1; step=4;
// 				dtJob = 1;
// 				dtJob = 1;
// 				break;
// 	}
// 	writeIPC(1,xBall,yBall,motion,arahRobot);
// 	fprintf(stderr,"Step KO = %d ",step);
// }
//======================================================================================================

//Strategi Default Aggressive
// void strategi_serang_WIDE2()
// {
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step)
// 	{
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola3(1); fprintf(stderr,"1 "); break;//
// 		case 2: Taktikcekjarak2(); fprintf(stderr,"2 ");break;// 
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide2(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: TaktikEksekusi2(); fprintf(stderr,"6 "); break; //
// 		case 6: TaktikTrackBall2(); break;
// 		case 7: break;
// 	}

// 	if(laststep2 != step) laststep = laststep2;
// 	laststep2 = step;

// 	//fprintf(stderr," S(%d)A(%d)X(%d)Y(%d)dtX(%d)dtY(%d)\n",step,arahRobot, sdtbolax, sdtbolay,dataXB,dataYB);
// 	//fprintf(stderr," pSt: %d|pE: %d",pilihstrategi,piliheksekusi);
// }

// void strategi_serang_WIDE4()
// {
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step)
// 	{
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola4(1); fprintf(stderr,"1 "); break;//
// 		case 2: Taktikcekjarak3(); fprintf(stderr,"2 "); break; //step++;break;//Taktikcekjarak3(); fprintf(stderr,"2 ");break;// 
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide4(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: step++;break;//TaktikNggiring3(); fprintf(stderr,"5 ");break; //
// 		case 6: TaktikEksekusi4(); fprintf(stderr,"6 "); break; //
// 		case 7: {
// 					if(Kick == 1) 
// 					{
// 						countertimeout++;
// 						if(countertimeout < 100)
// 						{
// 							yBall = generatesdtY(70);
// 							xBall = sdtblax;
// 							motion = 10;
// 						}
// 						else
// 						{						
// 							if(CountUlang>0) CountUlang--; 
// 							countertimeout=0;
// 							step=1;counterobseks=0;sudahlihatGW=0;
// 						}						
// 					}
// 					else
// 					{
// 						countertimeout=0;
// 						step=1;counterobseks=0;sudahlihatGW=0;  break;
// 					}
// 				}
// 		case 8: break;
// 	}

// 	if(laststep2 != step) laststep = laststep2;
// 	laststep2 = step;

// 	//fprintf(stderr," S(%d)A(%d)X(%d)Y(%d)dtX(%d)dtY(%d)\n",step,arahRobot, sdtbolax, sdtbolay,dataXB,dataYB);
// 	//fprintf(stderr," pSt: %d|pE: %d",pilihstrategi,piliheksekusi);
// }

// void strategi_serang_WIDE5(){
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step){
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola5(1); fprintf(stderr,"1 "); break;
// 		case 2: step++;break;//Taktikcekjarak3(); fprintf(stderr,"2 ");break;//
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide4(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: step++;break;//TaktikNggiring3(); fprintf(stderr,"5 ");break; //
// 		case 6: TaktikEksekusi4(); fprintf(stderr,"6 "); break; //
// 		case 7: step=1;suduteksekusi=0;counterobseks=0;sudahlihatGW=0; break;
// 		case 8: break;
// 	}
// }

// void strategi_serang_WIDE6(){
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step){
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola6(1); fprintf(stderr,"1 "); break;
// 		case 2: step++;break;//Taktikcekjarak3(); fprintf(stderr,"2 ");break;//
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide4(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: step++;break;//TaktikNggiring3(); fprintf(stderr,"5 ");break; //
// 		case 6: TaktikEksekusi4(); fprintf(stderr,"6 "); break; //
// 		case 7: step=1;suduteksekusi=0;counterobseks=0;sudahlihatGW=0; break;
// 		case 8: break;
// 	}
// }

// void strategi_serang_WIDE7(){
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step){
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola7(1); fprintf(stderr,"1 "); break;
// 		case 2: step++;break;//Taktikcekjarak3(); fprintf(stderr,"2 ");break;//
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide4(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: step++;break;//TaktikNggiring3(); fprintf(stderr,"5 ");break; //
// 		case 6: TaktikEksekusi4(); fprintf(stderr,"6 "); break; //
// 		case 7: step=1;suduteksekusi=0;counterobseks=0;sudahlihatGW=0; break;
// 		case 8: break;
// 	}
// }

// void strategi_serang_WIDE8(){
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step){
// 		case 0: lostball(); fprintf(stderr,"0 ");break;
// 		case 1: TaktikkeBola8(1); fprintf(stderr,"1 "); break;
// 		case 2: step++;break;//Taktikcekjarak3(); fprintf(stderr,"2 ");break;//
// 		case 3: TaktikcekGW_wide(); lastposrobot=CariSudut(COMPASS_Blue,0); fprintf(stderr,"3 ");break;//
// 		case 4: TaktikLuruskanGW_wide4(suduteksekusi); fprintf(stderr,"4 ");break;//
// 		case 5: step++;break;//TaktikNggiring3(); fprintf(stderr,"5 ");break; //
// 		case 6: TaktikEksekusi4(); fprintf(stderr,"6 "); break; //
// 		case 7: step=1;suduteksekusi=0;counterobseks=0;sudahlihatGW=0; break;
// 		case 8: break;
// 	}
// }

//Strategi Bertahan Normal
// void strategi_bertahan()
// {
// 	//if(dataXB!=0 || dataYB!=0) 
// 	dtJob = step;
// 	if(step>1)step=1;
// 	switch(step){
// 		case 0: fprintf(stderr,"0 ");lostballdef();break;
// 		case 1: fprintf(stderr,"1 ");Bertahan(); break;
// 	}
// }

//Strategi Utama End==========================================================

//============================================================================

//============================================================================

/*void strategi_kickoff()
{
	switch(step)
	{
		dtJob = step+1;
		dataTm = step+1;
		if(step == 2){
			dataTm = dtJob = 3;
		}
		case 1: Modekickoff();
				nBall = ftBall3(dataXB,dataYB,0);
				sudutTendang = 2;
				heading_skr = 0;
				motion = 10;
				motionAct(xBall, yBall, motion, 10);
				step++;break;
		case 2: Modekickoff(); TaktikLuruskanGW_wide(suduteksekusi); break; //TaktikLuruskanGW(suduteksekusi);  break;
		case 3: TaktikEksekusiwide(1); break;
		case 4:	step++; break;
		case 5: hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
				setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
				nextstep = sdtGWX = Kick = stepE = 0;
				stepTB = step = 1;
				servoX = GoalXservo = sdtblax;
				step = play = 1;
				step = play = 1;
				step = play = 1;
				dataTm = dtJob = 1;
				dataTm = dtJob = 1;
				break;
	}
	writeIPC(1,xBall,yBall,motion,arahRobot);
	fprintf(stderr,"Step KO = %d ",step);
}*/


//Strategi Default 1
/*void strategi_serang_WIDE()
{
	if(dataXB!=0 || dataYB!=0) dtJob = step;
	switch(step)
	{
		case 0: lostball(); fprintf(stderr,"0 "); break;
		case 1: TaktikkeBola3(1); fprintf(stderr,"1 "); break;
		case 2: TaktikcekGW_wide(); fprintf(stderr,"2 "); 	lastposrobot=CariSudut(COMPASS_Blue,myGcol);break;//heading_skr = 0; break;
		case 3: TaktikLuruskanGW_wide(suduteksekusi); fprintf(stderr,"3 "); break;
		case 4: TaktikNggiring(); fprintf(stderr,"4 "); break;
		case 5: TaktikEksekusi(); fprintf(stderr,"5 "); break;
		case 6: TaktikTrackBall2(); break;
		case 7: break;
	}

	if(laststep2 != step) laststep = laststep2;
	laststep2 = step;

	//fprintf(stderr," S(%d)A(%d)X(%d)Y(%d)dtX(%d)dtY(%d)\n",step,arahRobot, sdtbolax, sdtbolay,dataXB,dataYB);
	//fprintf(stderr," pSt: %d|pE: %d",pilihstrategi,piliheksekusi);
}*/

// void pilihstrategieksekusi_wide()
// {
// 	switch(pilihstrategi)
// 	{
// 		case 1: TaktikEksekusiwide(1); break;
// 		case 2: if(piliheksekusi == 1) { TaktikEksekusiwide(1); } 
// 				else if(piliheksekusi == 2) {TaktikEksekusiwide(2);} 
// 				else if (piliheksekusi == 3) {TaktikEksekusiwide(4);} break;
// 		case 3: TaktikEksekusiwide(2); break;
// 	}
// }

// int strategi_Deff()
// {
// 	if((dataXB!=0 && dataYB!=0)) dtJob = 1;
// 	switch(step)
// 	{
// 	case 1: TaktikStayBALLDimas(); break;
// 	//case 1: TaktikkeBola3(1); break;
// 	//case 2: TaktikStayBALLDimas(); stepH=1;break;
// 	//case 2: tunggudeff(); break; //CheckGawang();break;
// 	//case 3: TaktikStayBALL1();break;
// 	default : step = 1; break;
// 	}
// 	//printf("step %d\n",step);
// 	//writeIPC(1,xBall,yBall,motion,arahRobot);
// 	return 0;
// 	//mundur & cari bola
// 	//mundur & lihat gawang
// 	//jlan d temapat & cari bola
// }

// //Strategy Utility============================================================

// void Modekickoff(){
// 	//Sementara karena saya bingung maka ModeKickOFF nya saya hardcode ke 0
// 	ModeKickOFF=0;
// 	if(ModeKickOFF == 1 || ModeKickOFF == 0){
// 		hadapGawang = 0;
// 		sdtGWX = 0;
// 		kaki = 5;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countGeser = 0;
// 		suduteksekusi = 0;
// 	} else if(ModeKickOFF == 2){
// 		hadapGawang = 60;
// 		sdtGWX = 0;
// 		kaki = 5;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countGeser = 150;
// 		suduteksekusi = 30;
// 	} else if(ModeKickOFF == 3){
// 		hadapGawang = 60;
// 		sdtGWX = 0;
// 		kaki = 6;
// 		flagPutar = flagMinggir = 2;
// 		posRobot = 2;
// 		countGeser = 150;
// 		suduteksekusi = -30;
// 	} else if(ModeKickOFF == 4){
// 		hadapGawang = 80;
// 		sdtGWX = 0;
// 		kaki = 6;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countGeser = 50;
// 		suduteksekusi = 45;
// 	} else if(ModeKickOFF == 5){
// 		hadapGawang = 80;
// 		sdtGWX = 0;
// 		kaki = 5;
// 		flagPutar = flagMinggir = 2;
// 		posRobot = 2;
// 		countGeser = 50;
// 		suduteksekusi = -45;
// 	} else if(ModeKickOFF == 6){
// 		hadapGawang = 40;
// 		sdtGWX = 0;
// 		kaki = 20;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countDribb = 700;
// 		countGeser = 0;
// 		suduteksekusi = 60;
// 	} else if(ModeKickOFF == 7){
// 		hadapGawang = 40;
// 		sdtGWX = 0;
// 		kaki = 30;
// 		flagPutar = flagMinggir = 2;
// 		posRobot = 2;
// 		countDribb = 700;
// 		countGeser = 0;
// 		suduteksekusi = -60;
// 	} else if(ModeKickOFF == 8){
// 		hadapGawang = 0;
// 		sdtGWX = 0;
// 		kaki = 20;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countDribb = 700;
// 		countGeser = 0;
// 		suduteksekusi = 10;
// 	} else if(ModeKickOFF == 9){
// 		hadapGawang = 0;
// 		sdtGWX = 0;
// 		kaki = 30;
// 		flagPutar = flagMinggir = 2;
// 		posRobot = 2;
// 		countDribb = 500;
// 		countGeser = 0;
// 		suduteksekusi = -10;
// 	} else if(ModeKickOFF == 10){
// 		hadapGawang = 40;
// 		sdtGWX = 0;
// 		kaki = 20;
// 		flagPutar = flagMinggir = 1;
// 		posRobot = 2;
// 		countDribb = 400;
// 		countGeser = 0;
// 	}
// }



// void CheckGawang()
// {

// 	dtJob = 2;
// 	dataTm = 0;
// 	if(stepH == 1){ // Putar ke arah gawang musuh
// 		ktmGwng=0;
// 		arahRobot = CariSudut(0,myGcol);
// 		if(arahRobot == 1){
// 			motion = 10;
// 			cariGawangOut++;
// 			if(cariGawangOut >= 10){
// 				cariGawangOut = 0;
// 				stepH = 2;
// 			}
// 		} else if(arahRobot >= 4 && arahRobot <= 10){
// 			if(arahRobot <= 7)motion = 11;
// 			else if(arahRobot > 7) motion = 12;
// 		} else if(arahRobot < 4 || arahRobot > 10){
// 			if(arahRobot > 10 )motion = 212;
// 			else if(arahRobot < 4) motion = 211;
// 		}

// 		motionAct(xBall, yBall, motion, 10);
// 	} else if(stepH == 2){ //posisi duduk hemat baterai Mode 30 duduk
// 		motionAct(xBall, yBall, 0, 10);
// 	}
// 	//fprintf(stderr," A = %d | S = %d | J = %d\n",arahRobot,stepH,dtJob);
// }

// void Reset()
// {
// 	countMM = 2;
// 	hadapGawang = nextstep = kaki = posRobot = lihatGoal = 0;
// 	setArahRobot = countGawang = flagPutar = sdtbolax = posRobot = 0;
// 	nextstep = sdtGWX = Kick = stepE = 0;
// 	stepH = stepTB = 1;
// 	servoX = GoalXservo = sdtblax;
// 	//lastrefree = 0;
// 	motion = 0;
// }

// void PilihModeKickOFF(){
// 	if(PilihMode == 110){
// 		ModeKickOFF = 1;
// 	} else if(PilihMode == 120){
// 		ModeKickOFF = 2;
// 	} else if(PilihMode == 130){
// 		ModeKickOFF = 3;
// 	} else if(PilihMode == 140){
// 		ModeKickOFF = 4;
// 	} else if(PilihMode == 150){
// 		ModeKickOFF = 5;
// 	} else if(PilihMode == 160){
// 		ModeKickOFF = 6;
// 	} else if(PilihMode == 170){
// 		ModeKickOFF = 7;
// 	} else if(PilihMode == 180){
// 		ModeKickOFF = 8;
// 	} else if(PilihMode == 190){
// 		ModeKickOFF = 9;
// 	} else if(PilihMode == 200){
// 		ModeKickOFF = 10;
// 	}
// 	else ModeKickOFF = 0;
// }

// void PilihModeRobot(){
// 	PilihMode = bacaMode();
// 	//Strategi();
// 	PilihModeKickOFF();
// 	//fprintf(stderr," (%d)(%d)(%d)\n",Strategiserang,ModeKickOFF,PilihMode);
// }

/*void KepalaDiamAtas(){
	//nBall = ftBall3(dataXB, dataYB, 0);
	xBall=sdtblax;
	yBall=sdtdefy;
	servoX = xBall;
	GoalXservo = xBall;
	servoY = yBall;
}

void KepalaDiamBawah(){
	//nBall = ftBall3(dataXB, dataYB, 0);
	xBall=sdtblax;
	yBall=sdtexcute;
	servoX = xBall;
	GoalXservo = xBall;
	servoY = yBall;
}*/

// void strategidemo()
// {
// 	switch(step)
// 	{
// 		case 1 : TaktikkeBola(1); countDribb = 1000; break;
// 		case 2 : TaktikNggiring(); stepG = 1; break;
// 		case 3 : TaktikFindGoal3(); ethreadsearching = 0; break;
// 		case 4 : TaktikkeBola(3); break;
// 		case 5 : kalibrasiTendang(); break;

// 	}
// }

// int positioningdefense()
// {
// 	if(dataXB!=0 || dataYB!=0) dtJob = step;
// 	switch(step)
// 	{
// 	case 0: lostball();break;
// 	//case 1: trackdefense();break;
// 	//case 1: TaktikkeBolaDef(1);break;
// 	case 1: TaktikkeBola3(1);break;
// 	//case 2: TaktikkeBola2(1);break;
// 	//case 3: TaktikEksekusiDef();break;
// 	case 2: TaktikEksekusiwide(1);break;
// 	//case 2: TaktikEksekusiwide(1);break;
// 	//case 3: TaktikEksekusi();break;
// 	case 3: TaktikKembali();break;
// 	//case 4: kirimjarak();break;
// 	case 4: MajuBelakang();break;
// 	default: step = 1;break;
// 	}
// 	if(laststep2 != step) laststep = laststep2;
// 	laststep2 = step;
// }
//===================================================================

