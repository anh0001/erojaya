//============================================================================
// Name        : main.cpp / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================
#include "eheader.h"
using namespace std;

float radSudutY, jarakBola;


int main(int argc, char **argv){
	pthread_t threadsearching,threadlocalization;
	struct timeval t1, t2;
	char system_check[100];
	double elapsedTime;
	unsigned char param1,param2,param3;
	int setX2,setY2,setX,setY,setH,eX,eY,eH;
	int xpos2,ypos2;
	int ct;

	//=====================Data kepala lihat==========
	//short sdtlihatkpl;

	cout << "Bismillah" << endl;
	
	expData();
	initplayer();
	
	if(impUSBRX() == 1){
		fprintf(stderr,".. USBRX.eros Loaded..\n");
	}
	else fprintf(stderr,".. USBRX.eros failed to load..\n");
	
	sprintf(system_check,"stty -F %s 57600",USBRX);
	fprintf(stderr,"%s\n\n",system_check);
	system(system_check);
	system(system_check);
	system(system_check);
	

	//Load Data Intialize ================================================
	if(impDatakpl() == 1){
		fprintf(stderr," Alhamdulillah data kalibrasi kepala berhasil di load\n");
	}
	else fprintf(stderr," Astaghfirullah data kalibrasi kepala berhasil di load\n");



	//ROS Publish/Subscribe Intialize ====================================
	ros::init(argc, argv, "intelligent");
	ros::NodeHandle n;

	//serial/serialcom.cpp
	ros::Subscriber serial_it = n.subscribe("serial_it", 100, SerialItCallback);
	ros::Publisher  it_serial = n.advertise<std_msgs::Int32MultiArray>("it_serial", 100);

	//referee/referee.cpp
	ros::Subscriber referee_it = n.subscribe("referee_it", 100, RefereeItCallback);

	//localization/localization.cpp
	ros::Subscriber vision_local = n.subscribe("vision_local", 100, VisionLocalCallback);

	//communication/clientrec.cpp
	ros::Subscriber rec_it = n.subscribe("rec_it", 100, RecItCallback);
	ros::Publisher  it_rec = n.advertise<std_msgs::Int32MultiArray>("it_rec", 100);

	//vision/vision.cpp
	ros::Subscriber vision_it = n.subscribe("vision_it", 100, VisionItCallback);
	ros::Publisher  it_vision = n.advertise<std_msgs::Int32MultiArray>("it_vision", 100);

	n.getParam("use_gui",use_gui);
	ROS_INFO("Set PID for GUI: [%d]",use_gui);

	pthread_create(&threadsearching,NULL,ftBall5,NULL);
	//pthread_create(&threadlocalization,NULL,localization,NULL);
	
	
	n.getParam("/robot_pos/xpos",setX);
	n.getParam("/robot_pos/ypos",setY);
	n.getParam("/robot_pos/hpos",setH);
	//Dummy
	setX=setY=setH=0;
	ROS_INFO("Robot Position:[%d][%d][%d]",setX,setY,setH);
	//Nilai default variable =============================================
	//modePlay = 1;
	//modeTaktik = 2;
	//stepG = 1;
	stepTB = BackHome = stepH = stepK = 0;
	kaki = play =  0;
	flagsudahdekat=0;
	enablevision = 0b0001;
	lastXBall = sdtblax;
	lastYBall = sdtblay;
	//countDribb=1500;
	setX=-2250;setY=2250; setH=-90;
	flagStopDribble=0;
	initialPosition=5;
	//KNN_Basic(3,25,377,146,1);
	stepG=counterpos1=0;
	init();
	while (ros::ok()){
		//flagLocalize=1;
		//gettimeofday(&t1, NULL);
		//dtaYPOS=650;
		/*xpos2 = dtaXPOS*cosd(dtaHeading)-dtaYPOS*sind(dtaHeading);
		ypos2 = dtaXPOS*sind(dtaHeading)+dtaYPOS*cosd(dtaHeading);
		setX2 = setX * cosd(dtaHeading) - setY * sind(dtaHeading);
		setY2 = setX * sind(dtaHeading) + setY * cosd(dtaHeading);
		eX=setX2-xpos2;
		eY=setY2-ypos2;
		eH=dtaHeading-setH;
		
		//Fuzzyfikasi
		if(eY>700)			param1=40;
		else if(eY>400)	param1=30;
		else if(eY>100)	param1=20;
		else 						param1=10;
		
		if(eX>400)			param2=6;
		else if(eX>100)	param2=4;
		else if(eX<-400)param2=5;
		else if(eX<-100)param2=3;
		else						param2=0;
		
		if(eH>10)				param3=1;
		else if(eH<-10)	param3=2;
		else						param3=0;
		
		//Defuzzyfikasi
		if(param3==0){
			motion=param1+param2;
		}else{
			motion=param1+param3;
		}
		//if(motion==10)ct++;
		//if(ct>300){setX=0;setY=0;setH=0;}
		//if(motion==45)motion=35;
		//if(motion==46)motion=36;
		motionAct(2048,2048,motion,0);*/


		//MAIN LOOP ---------------------------------------------------------------------------------------------------------------------
		
		arahRobot = cariarahRobot(dtaHeading,0);
		arahLihat = CariArahLihat(SudutBolaX(),0);
		//bermain();
		lostball_keeping();
		//strategi_keeping1();
		//lostball();
		//strategi_serang_WIDE3();
		//PositionGenerator();
		//aktifkansearching();
		//TaktikkeBola3(1);
		//arahNggiring=1;
		//TaktikNggiring3();
		//DEBUG STRATEGY
		//ROS_INFO("(%d)X(%d)Y(%d)sX(%d)sY(%d)m(%d)xB(%d)yB(%d)aR(%d)",nBall,dataXB,dataYB,sdtbolax,sdtbolay,motion,xBall,yBall,arahRobot);

		//DEBUG TACTIC
		fprintf(stderr,"%d(%d)X(%d)Y(%d)sX(%d)sY(%d)m(%d)aR(%d)aL(%d)X(%d)Y(%d)(%d)(%d)\n",step,nBall,dataXB,dataYB,sdtbolax,sdtbolay,motion,arahRobot,arahLihat,dtaXPOS,dtaYPOS,sdtbolay,stepK);
		
		//DEBUG I/O
		//ROS_INFO("(%d)X(%3d)Y(%3d)sX(%2d)sY(%2d)m(%2d)xB(%4d)yB(%4d)aR(%2d)aL(%2d)\n",nBall,dataXB,dataYB,sdtbolax,sdtbolay,motion,xBall,yBall,arahRobot,arahLihat);
		//ROS_INFO("(%d)aR(%2d)aL(%2d)m(%2d)L(%d)mLS(%d)mLT(%d)dtJob(%d)",nBall,arahRobot,arahLihat,motion,langkah,majuloss,majulost,dtJob);
		//printf("Hasil = %d", sdtlihatkpl);
		

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//motionAct(xBall,yBall,0,0);
		if(dtaYPOS==0 && dtaXPOS==0)resetOdometry=false;
		VisionPublish(yBall,SudutBolaY(),arahLihat,dtJob,enablevision);
		SerialPublish(resetOdometry,1,0,0);
		it_serial.publish(dtaPublishSERIAL);
		it_rec.publish(dtaPublishREC);
		it_vision.publish(dtaPublishVISION);
		ros::spinOnce();

		//gettimeofday(&t2, NULL);
		//elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		//elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		//fprintf(stderr," Time : %f \n",elapsedTime);
	}
}
