//============================================================================
// Name        : thread.cpp / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include "eheader.h"
using namespace std;

int xdirect=0,ydirect=0,ydirectpos=0;
int sdtxmin=1200,sdtxmax=2900,sdtycenter=40;
int sdtxmin1,sdtxmax1;

void aktifkansearching()
{
	enablevision = 0b00001;
	xBallvision = dataXB;
	yBallvision = dataYB;
	ethreadsearching = 1;
}

void *ftBall5(void *arg){
	int MAX_X=2900,MIN_X=1200;
	int ErrFrameY = 0, ErrFrameX = 0, MM, limitLook = 10;//20;
	int Kons[3];
	Kons[0]=7; //Fast;
	Kons[1]=5; //Med;
	Kons[2]=3; //Slow;
	while(1){
		if(ethreadsearching == 1){
			if (dataXB == 0 && dataYB == 0) {
				 sKpl = 1;//1;//2;
				 CountLook--;
				 if(CountLook>0){
				 	if(lastErrY<=0){
				 		ydirect=1;
				 	}else{
				 		ydirect=-1;
				 	}
				 	if(!flagPEX){
				 		if(lastErrX<=0){
					 		xdirect=1;
					 	}else{
					 		xdirect=-1;
					 	}
					 	if(xdirect==1)xBall+=2;
				 		else xBall-=2;
				 	}

				 	if(ydirect==1)yBall+=3;
				 	else yBall-=3;

				 	if (yBall>sdtblamx)yBall=sdtblamx;
				 	if (yBall<sdtblamn)yBall=sdtblamn;
				 	if(xBall>MAX_X)xBall=MAX_X;
				 	if(xBall<MIN_X)xBall=MIN_X;
				 	if(yBall>generatesdtY(sdtycenter+40))ydirect=-1;
				 	else if(yBall<generatesdtY(sdtycenter-30))ydirect=1;
				 }
				 if(CountLook <= 0){
				 	CountLook = MM = FlagLook = 0;CountTrack++;
				 	sdtxmin=MIN_X;
				 	sdtycenter=40;
				 	sdtxmax=MAX_X;//*
				 	//fprintf(stderr, "ikii %d\n",CountTrack);
				 }
				 else if(CountLook <= limitLook && CountLook > 0){ MM = 1; FlagLook = 2;}
				 else if(CountLook > limitLook){ MM = 1; FlagLook = 1;}
			}else {
				CountLook++;
				if (CountLook >= 2) { //6
					if(CountLook < limitLook) { MM = 1; FlagLook = 2;}
					else if(CountLook >= limitLook) { MM = 1; FlagLook = 1;}
					CountTrack = 0;
					sdtxmin=xBall-abs(dataXB-212)*2;
					sdtxmax=xBall+abs(dataXB-212)*2;
					sdtycenter=sdtbolay;
					//xdirect*=-1;//**
					if(CountLook >= 300){
						CountLook = 300;
					}
				}
				ErrFrameX = xBallvision - 212;
				//ErrFrameX = xBallvision - 216 - GeserVision;
				//ErrFrameX = xBallvision - 160 - GeserVision;
				ErrFrameY = yBallvision - 120;
				//ErrFrameY = yBallvision - 120;

				if(ErrFrameX <= 2 && ErrFrameX >= -2) ErrFrameX = 0;
				if(ErrFrameY <= 2 && ErrFrameY >= -2) ErrFrameY = 0;
				if(sdtbolay<45 && CountLook >= 66){
					if(use_gui)
						xBall -= ((ErrFrameX) / TKPX1_GUI) + ((ErrFrameX - lastErrX)/TKDX1_GUI) + ((ErrFrameX + lastErrX) / TKIX1_GUI);
					else
						xBall -= ((ErrFrameX) / TKPX1) + ((ErrFrameX - lastErrX)/TKDX1) + ((ErrFrameX + lastErrX) / TKIX1);
				}else{
					if(use_gui)
						xBall -= ((ErrFrameX) / TKPX2_GUI) + ((ErrFrameX - lastErrX)/TKDX2_GUI) + ((ErrFrameX + lastErrX) / TKIX2_GUI);
					else
						xBall -= ((ErrFrameX) / TKPX2) + ((ErrFrameX - lastErrX)/TKDX2) + ((ErrFrameX + lastErrX) / TKIX2);
				}
				if (use_gui)
					yBall -= ((ErrFrameY) / (TKPY_GUI*(90-sdtbolay)/9)) + ((ErrFrameY - lastErrY)/(TKDY_GUI));
				else
					yBall -= ((ErrFrameY) / (TKPY*(90-sdtbolay)/9)) + ((ErrFrameY - lastErrY)/(TKDY));

				if(motion==25 || motion==11){
					xBall-=8;
				}else if(motion==26 || motion==12){
					xBall+=8;
				}
				if (xBall >= 3500)
					xBall = 3500;
				else if (xBall <= 600)
					xBall = 600;
				if (yBall >= sdtblamx+100)
					yBall = sdtblamx+100;
				else if (yBall <= sdtblamn)//+400)
					yBall = sdtblamn;//+400;

				lastXball = servoX = xBall;
				GoalXservo = xBall;
				servoY = yBall;
				lastYball = yBall;
				lastErrX = ErrFrameX;
				lastErrY = ErrFrameY;
				lastMotion = motion;
				//MotionHeadPrediction();


				if ((sdtbolax >= 3 && ErrFrameX > -5) || (sdtbolax <= 3 && ErrFrameX > 5)) {
					arahX = lastarahX = 1;
				} else if ((sdtbolax <= 3 && ErrFrameX < 5) || (sdtbolax >= 3 && ErrFrameX < -5))
					arahX = lastarahX = 0;
				else arahX = 0;
				if ((sdtbolay >= 3 && ErrFrameY > -5) || (sdtbolay <= 3 && ErrFrameY > 5)) {
					arahY = lastarahY = 0;
				} else if ((sdtbolay <= 3 && ErrFrameY < 5) || (sdtbolay >= 3 && ErrFrameY < -5))
					arahY = lastarahY = 1;
				else arahY = 0;

				SudutBolaX();
				SudutBolaY();
				lastsdtbolax = sdtbolax;
				lastsdtbolay = sdtbolay;
			}//****

			if(CountTrack>10){
				if(CountLook>20)CountLook=20;
				if(xdirect==0 && ydirect==0){ //awal
					yBall=sdtblamx;
					xBall=sdtxmin;
					xdirect=1;
					ydirect=-1;
					ydirectpos=2;
				}else if(xdirect==1){ //nganan
					if(CountTrack>400){
						xBall+=Kons[0]-CountLook;
					}else{
						xBall+=Kons[1]-CountLook;
					}
					if(xBall>sdtxmax){
						xdirect=-1;
					}
					
				}else if(xdirect==-1){ //ngiri
					if(CountTrack>300){
						xBall-=Kons[0]-CountLook;
					}else{
						xBall-=Kons[1]-CountLook;
					}
					xBall-=7-CountLook;
					if(xBall<sdtxmin){
						xdirect=1;
					}
				}

				if(flagsudahdekat){
					if(flagrotasilostball){
						if(xBall<sdtxmin || xBall>sdtxmax){
							if(sdtycenter<40)sdtycenter=40;
							switch(ydirectpos){
								case 0: yBall=generatesdtY(sdtycenter); break;
								case 1: yBall=generatesdtY(sdtycenter+20); break;
								case 2: yBall=generatesdtY(sdtycenter+40); break;
							}

							if(ydirect==-1)ydirectpos--;
							else ydirectpos++;

							if(ydirectpos>2)ydirectpos=0;
							else if(ydirectpos<0)ydirect=1;
						}
					}else{
						if(xBall<sdtxmin || xBall>sdtxmax){
							if(sdtycenter<40)sdtycenter=40;
							switch(ydirectpos){
								case 0: yBall=generatesdtY(sdtycenter-30); break;
								case 1: yBall=generatesdtY(sdtycenter-15); break;
								case 2: yBall=generatesdtY(sdtycenter); break;
							}

							if(ydirect==-1)ydirectpos--;
							else ydirectpos++;

							if(ydirectpos>2)ydirectpos=0;
							else if(ydirectpos<0)ydirect=1;
						}
					}
				}else{
					if(dtComm==1){
						if(xBall<sdtxmin || xBall>sdtxmax){
							if(sdtycenter<40)sdtycenter=40;
							switch(ydirectpos){
								case 0: yBall=generatesdtY(sdtycenter); break;
								case 1: yBall=generatesdtY(sdtycenter+20); break;
								case 2: yBall=generatesdtY(sdtycenter+40); break;
								//case 3: yBall=generatesdtY(sdtycenter+20); break;
								//case 4: yBall=generatesdtY(sdtycenter+40); break;
							}

							if(ydirect==-1)ydirectpos--;
							else ydirectpos++;

							if(ydirectpos>2)ydirectpos=0;
							else if(ydirectpos<0)ydirect=1;
						}
					}else{
						if(xBall<sdtxmin || xBall>sdtxmax){
							if(sdtycenter<40)sdtycenter=40;
							switch(ydirectpos){
								case 0: yBall=generatesdtY(sdtycenter-30); break;
								case 1: yBall=generatesdtY(sdtycenter-15); break;
								case 2: yBall=generatesdtY(sdtycenter); break;
								case 3: yBall=generatesdtY(sdtycenter+20); break;
								case 4: yBall=generatesdtY(sdtycenter+40); break;
							}

							if(ydirect==-1)ydirectpos--;
							else ydirectpos++;

							if(ydirectpos>4)ydirectpos=0;
							else if(ydirectpos<0)ydirect=1;
						}
					}
				}
				usleep(3000);
			}

			if(CountTrack >= 2000)CountTrack=2000;
		}else usleep(5000);
		nBall = MM;
		usleep(9000);
		//fprintf(stderr, "nB[%1d]CL[%3d]CT[%4d]lX[%3d]lY[%3d]\n",nBall,CountLook,CountTrack,lastErrX,lastErrY);
		//fprintf(stderr,"nB(%d)X(%d)Y(%d)XS(%d)YS(%d)PT(%d)XB(%d)YB(%d)DBY(%d)JBL(%f)\n",nBall, dataXB,dataYB,sdtbolax, sdtbolay,posTiang , xBall,yBall, dataYbl,jrkbolay);
	}
}