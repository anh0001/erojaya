//Kill modified
//============================================================================
// Name        : visionEROS.cpp
// Author      : Cipta Priambodo AKA CPP_REV
// Version     : 2.3
// Created on  : Jan 10, 2018
// Copyright   : Bismillahirrahmanirrahim
// Description : EROS Juara 1 KRI Regional, Nasional, Robocup 2018
// comment     : Jenggirat Tangi Lare!!!!
//============================================================================
#include <cstring>
#include <ctime>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>

#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

#define MAX_VISION_IT 13
#define MAX_IT_VISION 5

using namespace cv;
using namespace cv::ml;
using namespace std;
#define lebar_frame 416
#define tinggi_frame 240

/************ Machine Learning Lib ************/
#ifdef _DEBUG
#pragma comment(lib, "opencv_core247d.lib")
#pragma comment(lib, "opencv_imgproc247d.lib")   //MAT processing
#pragma comment(lib, "opencv_objdetect247d.lib") //HOGDescriptor
//#pragma comment(lib, "opencv_gpu247d.lib")
//#pragma comment(lib, "opencv_features2d247d.lib")
#pragma comment(lib, "opencv_highgui247d.lib")
#pragma comment(lib, "opencv_ml247d.lib")
//#pragma comment(lib, "opencv_stitching247d.lib");
//#pragma comment(lib, "opencv_nonfree247d.lib");

#else
#pragma comment(lib, "opencv_core247.lib")
#pragma comment(lib, "opencv_imgproc247.lib")
#pragma comment(lib, "opencv_objdetect247.lib")
//#pragma comment(lib, "opencv_gpu247.lib")
//#pragma comment(lib, "opencv_features2d247.lib")
#pragma comment(lib, "opencv_highgui247.lib")
#pragma comment(lib, "opencv_ml247.lib")
//#pragma comment(lib, "opencv_stitching247.lib");
//#pragma comment(lib, "opencv_nonfree247.lib");
#endif
/**********************************************/

//======IPC Code======================
#define header 		5000
//vision
/*#define xBall 		5001
#define dGoal 		5002
#define yBall 		5003
#define tGoal 		5004
#define xGoal		5005
#define jGoal		5006
#define yGoal		5007
#define wGoal		5008
#define obstacle1   5050
#define obstacle2   5051
#define obstacle3   5052
#define EVISION		5045
#define ROVISION	5046
#define XposRBT		5047
#define YposRBT		5048
#define LineEx		5049
#define EstJ		5069
//head
#define xHead		5009 // dataservo
#define xHead1		5010
#define yHead		5011
#define yHead1		5012
//motion
#define motionR		5013
//initial
#define iTeam 		5015
#define iGCol 		5016
#define iTask		5017
//musuh
#define xMusuh 		5019
#define pMusuh 		5020
#define yMusuh		5021
//request
#define rColor 		5022
//position
#define xRbt 		5023
#define yRbt 		5024
#define xBla 		5025
#define yBla 		5026
//bola
#define dBall 		5027
//Compass
#define dataCMPS	5030
#define dtaArah		5031
#define dtaArahkpl	5032
//Comm
#define dtJob		5033
#define dtComm		5035
//jarakbola
#define ordoM 		5028
*/
#define al			22

#define H			0
#define S			1
#define V			2

#define erSize		0
#define diSize		1
#define clSize		2

#define RECT		0
#define CROSS		1
#define ELIPSE		2

#define BALL		1
#define GOAL		2
#define FIELD		3
#define LINE		4
#define OBSTACLE	5
#define PI 			3.14159265

VideoCapture capture = VideoCapture(0);
Mat frames,preframes,pureframes;
Mat frameBall,frameGoal,frameLine,frameField;
Mat fieldk, fieldfill, fieldfill_inv, field_out, drawing, mask; //Hull
Mat cropBall(frameBall.rows, frameBall.cols, CV_8UC3);

Rect K1;
Rect K2;
Rect K3;

Rect FF1;
Rect FF2;
Rect FF3;
//======HOG + SVM ====== ------------------------------------------------------------------------------------------------
Ptr<SVM> svm = SVM::create();
//HOGDescriptor d( Size(32,16), Size(8,8), Size(4,4), Size(4,4), 9);
HOGDescriptor d( Size(32,32), Size(8,8), Size(4,4), Size(4,4), 9);
char namajendela[10]="calon";
//======variable EV ======
bool findBall,findGoal,findObs,findLine,findCalcF,newgoal,findPOSY,findPOSXki,findPOSXka;

//======== flag DEBUGGING  ===========================================//
bool EXITNOW=false;
bool debb=false;
bool sball=false,sgoal=false,sobs=false,selapsed=false,sgrid=false,sline=false,scalcf=false,shog=false;
bool show=false,cetak=false;
//======variable global ======
int XB=0, YB=0, XG=0, YG=0, TG=0, JG=0, YGG=0,LE=0;
int dataEV=0;
int sepertigaframe=0,sepertigaframet=0;
int sepersepuluhframe=0;
int luasminlap=0;
int minlinelong=0;
int minBall=0;
int luasnorm=0,luasabnorm=0;
int tembak=0;
int macrotinggiframe=0;
int dataObs[3]={0,0,0};
int dataXH=90, dataXH1=0, dataYH=45, dataYH1=0, dataCP=0;
int dataIT=0, dataGC=0, dataCL=0, dataJB=0, dataOM=0;
int distBall=1,flagLH=1;
static int *resultXB, *resultYB;
static int *resultLE;
static int *resultXG, *resultYG, *resultTG, *resultJG;
static int *resultobs1, *resultobs2,*resultobs3;
static int *resultROVISION,*resultXPOS,*resultYPOS;
static int *resultCalcJ;

int f1,f2,f3;
int l1,l2,l3;
int sdtblax, sdtblay, sdtdefy, sdtexcute, sdtblamx, sdtblamn; 	// Kalibrasi Kepala
int suduteksekusi=0,sudutcek=0;
//======== Variabel Range HSV  ===========================================//
int BDMin[3], BDMax[3],	BDed[3];	// BD = Bola Dekat
int BSMin[3], BSMax[3],	BSed[3];	// BS = Bola Sedang
int BJMin[3], BJMax[3],	BJed[3];	// BJ = Bola Jauh
int GwMin[3], GwMax[3],	Gwed[3];	// Gw = Gawang
int CyMin[3], CyMax[3],	Cyed[3];	// Cy = Cyan
int MgMin[3], MgMax[3],	Mged[3];	// Mg = Magenta
int GPMin[3], GPMax[3],	GPed[3];	// GP = Gawang Putih
int LHMin[3], LHMax[3],	LHed[3];	// LH = Lapangan Hijau
int LHMin1[3], LHMax1[3], LHed1[3];	// LH = Lapangan Hijau Dekat
int ObsMin[3], ObsMax[3], ObsED[3];	// Obs = Lapangan + line + bola
int JF[4];
int rMin[3] = {0,0,0};
int rMax[3] = {255,255,255};
int rED[3]  = {0,0,0};
int kalibjlY=0,kalibjlXi=0,kalibjlXa=0,Mrow,Mcol,Mcolka,Mcolki;
char file[] = {"/home/eros/param/vdata0.eros"};
char filekalib[]={"/home/eros/param/vdata.eros"};
float estjarak=0;
int nnn=0, ppp=0;
char FullFileName[10]="Bismillah";
char FirstFileName[100]="/home/ilmnss/Documents/CobaV/positivesample/pos"; //"./NegaImages/Negative";      //
int dtaFromIntelligent[MAX_IT_VISION];
std_msgs::Int32MultiArray dta_vision_it;
std::vector<Rect> ball;
unsigned int i;
String dekat_cascade_name = "/home/eros/param/dekat_cascade.xml"; //lokasi file ball_cascade
String sedang_cascade_name = "/home/eros/param/sedang_cascade.xml";
String jauh_cascade_name = "/home/eros/param/jauh_cascade.xml";
CascadeClassifier jauh_cascade; //class for load classifier from file (ball_cascade)
CascadeClassifier sedang_cascade; //class for load classifier from file (ball_cascade)
CascadeClassifier dekat_cascade; //class for load classifier from file (ball_cascade)

int frameCounter = 0;
int tick = 0;
int fps;
time_t timeBegin = time(0);

void ItVisionCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
  	int i=0;
  	for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
  		dtaFromIntelligent[i]=*it;
  		i++;
  	}
  	dataYH=dtaFromIntelligent[0];
  	dataYH1=dtaFromIntelligent[1];
  	//dataJB=dtaFromIntelligent[2];
  	//dataCP=dtaFromIntelligent[3];
  	dataEV=dtaFromIntelligent[4];
	//ROS_INFO("I heard: [%d][%d][%d][%d][%d]", dataYH,dataYH1,dataJB,dataCP,dataEV);
  	//ping=100000;
}

void VisionPublish(){
	dta_vision_it.data.clear();
	dta_vision_it.data.push_back(XB);
	dta_vision_it.data.push_back(YB);
	dta_vision_it.data.push_back(LE);
	dta_vision_it.data.push_back(XG);
	dta_vision_it.data.push_back(YG);
	dta_vision_it.data.push_back(TG);
	dta_vision_it.data.push_back(JG);
	dta_vision_it.data.push_back(dataObs[0]);
	dta_vision_it.data.push_back(dataObs[1]);
	dta_vision_it.data.push_back(dataObs[2]);
	dta_vision_it.data.push_back(Mrow);
	dta_vision_it.data.push_back(Mrow);
	dta_vision_it.data.push_back(Mrow);
    ROS_INFO("I send: [%d][%d][%d][%d]",XB,YB,distBall,dataYH1);
}

void detectAndDisplay( Mat frames )
{
    if(!sball){
        if(dataYH1<=(suduteksekusi+5)){distBall = 1;} //dekat
        else if(dataYH1>(suduteksekusi+5) && dataYH1<=(sudutcek)){distBall = 2;} //sedang
        else if(dataYH1>(sudutcek)){distBall = 3;} //jauh
        else distBall = 1;/**/
    }
    Mat frame_gray;
    cvtColor( frames, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    if(distBall==1){
    	dekat_cascade.detectMultiScale( frame_gray, ball, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(64, 64));
    }else if(distBall==2){
    	sedang_cascade.detectMultiScale( frame_gray, ball, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(32, 32),Size(100,100));
    }else{
    	jauh_cascade.detectMultiScale( frame_gray, ball, 1.1, 3, 0|CASCADE_SCALE_IMAGE, Size(8, 8), Size(64,64));
    }
    XB=0;
    YB=0;
    for ( i = 0; i < ball.size(); i++ )
    {
    	if((ball[i].x + ball[i].width/2)<0 || (ball[i].y + ball[i].height/2)<0 ){
			continue;
		}
		if((ball[i].x + ball[i].width/2)>lebar_frame || (ball[i].y + ball[i].height/2)> tinggi_frame){
			continue;
		}
		int counter1=0, flag_atas=0, flag_kiri=0, flag_kanan=0;
		if(distBall != 1){
			if((ball[i].x+ball[i].width+10)<lebar_frame){ //cek sudut 90
                Scalar scale0 = frameField.at<uchar>(ball[i].y+(ball[i].height/2),(ball[i].x+ball[i].width+10));
				if(scale0.val[0]==255) counter1++;
				else flag_kanan=1;
			}
			if((ball[i].y+ball[i].height+10)<tinggi_frame){ //cek sudut 0
                Scalar scale0 = frameField.at<uchar>(ball[i].y+ball[i].height+10,ball[i].x+(ball[i].width/2));
				if(scale0.val[0]==255) counter1++;
			}
			if((ball[i].x-10)>0){ //cek sudut 270
                Scalar scale0 = frameField.at<uchar>(ball[i].y+(ball[i].height/2),ball[i].x-10);
				if(scale0.val[0]==255) counter1++;
				else flag_kiri=1;
			}
			if((ball[i].x-9)>0 && (ball[i].y-9)>0){ //cek sudut 315
                Scalar scale0 = frameField.at<uchar>(ball[i].y-9,ball[i].x-9);
				if(scale0.val[0]==255) counter1++;
			}
			if((ball[i].x-9)>0 && (ball[i].y+ball[i].height+9)<tinggi_frame){ //cek sudut 225
                Scalar scale0 = frameField.at<uchar>(ball[i].y+ball[i].height+9,ball[i].x-9);
				if(scale0.val[0]==255) counter1++;
			}
			if((ball[i].x+ball[i].width+9)<lebar_frame && (ball[i].y-9)>0){ //cek sudut 45
                Scalar scale0 = frameField.at<uchar>(ball[i].y-9,ball[i].x+ball[i].width+9);
				if(scale0.val[0]==255) counter1++;
			}
			if((ball[i].x+ball[i].width+9)<lebar_frame && (ball[i].x+ball[i].width+9)<tinggi_frame){ //cek sudut 13
                Scalar scale0 = frameField.at<uchar>(ball[i].x+ball[i].width+9,ball[i].x+ball[i].width+9);
				if(scale0.val[0]==255) counter1++;
			}
			if(counter1<3){
				continue;
			}
			counter1=0;
		}
		XB=ball[i].x + ball[i].width/2;
		YB=ball[i].y + ball[i].height/2;
        Point center( XB, YB );
        ellipse( frames, center, Size( ball[i].width/2, ball[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        Mat ballROI = frame_gray( ball[i] );
        //VisionPublish();
        fprintf(stderr,"XB = %d | YB = %d\tHeight = %d | Width = %d\n", ball[i].x, ball[i].y, ball[i].height, ball[i].width);
    }
    frameCounter++;
    time_t timeNow = time(0) - timeBegin;

    if (timeNow - tick >= 1)
    	{
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }
    cout << "Average FPS = " << fps << endl;        
    //ROS_INFO("BAll[%d][%d]",distBall,suduteksekusi);
    if(show)imshow( "Detect Ball", frames );
}

int SudutBolaY(int nilaiservo){
	int sdtbolay = (((nilaiservo-sdtblay)*90)/1025);
	return sdtbolay;
}

/*void initIPC(){
	//share ball position
	int sharedSpaceXB = shmget(xBall, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultXB = (int*)shmat(sharedSpaceXB, NULL, 0);

	int sharedSpaceYB = shmget(yBall, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultYB = (int*)shmat(sharedSpaceYB, NULL, 0);

	//share ball position
	int sharedSpaceLE = shmget(LineEx, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultLE = (int*)shmat(sharedSpaceLE, NULL, 0);

	//share Goal position
	int sharedSpaceXG = shmget(xGoal, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultXG = (int*)shmat(sharedSpaceXG, NULL, 0);

	int sharedSpaceYG = shmget(yGoal, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultYG = (int*)shmat(sharedSpaceYG, NULL, 0);

	int sharedSpaceTG = shmget(tGoal, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultTG = (int*)shmat(sharedSpaceTG, NULL, 0);

	int sharedSpaceJG = shmget(jGoal, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultJG = (int*)shmat(sharedSpaceJG, NULL, 0);

	//share Obs position
	int sharedSpaceOBS1 = shmget(obstacle1, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultobs1 = (int*)shmat(sharedSpaceOBS1, NULL, 0);

	int sharedSpaceOBS2 = shmget(obstacle2, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultobs2 = (int*)shmat(sharedSpaceOBS2, NULL, 0);

	int sharedSpaceOBS3 = shmget(obstacle3, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultobs3 = (int*)shmat(sharedSpaceOBS3, NULL, 0);

	//share ROVISION
	int sharedSpaceROVISION = shmget(ROVISION, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultROVISION = (int*)shmat(sharedSpaceROVISION, NULL, 0);

	//share POSITIONING
	int sharedSpaceXPOSRBT = shmget(XposRBT, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultXPOS = (int*)shmat(sharedSpaceXPOSRBT, NULL, 0);

	int sharedSpaceYPOSRBT = shmget(YposRBT, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultYPOS = (int*)shmat(sharedSpaceYPOSRBT, NULL, 0);

	int sharedSpaceCalc = shmget(EstJ, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
	resultCalcJ = (int*)shmat(sharedSpaceCalc, NULL, 0);
}
void writeBall(int dataXB, int dataYB){
	*resultXB = dataXB;
	*resultYB = dataYB;
}
void writeLine(int dataLE){
	*resultLE = dataLE;
}
void writeGoal(int dataXG, int dataYG, int dataTG,int dataJG){
	*resultXG = dataXG;
	*resultYG = dataYG;
	*resultTG = dataTG;
	*resultJG = dataJG;
}
void writeObs(unsigned int obs1,unsigned int obs2,unsigned int obs3){
	*resultobs1 = obs1;
	*resultobs2 = obs2;
	*resultobs3 = obs3;
}
void writeXPOS(int posx){
	*resultXPOS=posx;
}
void writeYPOS(int posy){
	*resultYPOS=posy;
	fprintf(stderr,"posy=%d\n",posy);
}
void writejarak(int jar){
	*resultCalcJ=jar;
}
void readIPC(int init){
	static int *resultYH, *resultYH1, *resultJB, *resultCP,*resultEV;
	if(init==0){
		int sharedYH = yHead;
		int sharedSpaceYH = shmget(sharedYH, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultYH = (int*)shmat(sharedSpaceYH, NULL, 0);

		int sharedYH1 = yHead1;
		int sharedSpaceYH1 = shmget(sharedYH1, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultYH1 = (int*)shmat(sharedSpaceYH1, NULL, 0);

		int sharedJB = dtJob;
		int sharedSpaceJB = shmget(sharedJB, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultJB = (int*)shmat(sharedSpaceJB, NULL, 0);

		int sharedCP = dtaArahkpl;
		int sharedSpaceCP = shmget(sharedCP, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultCP = (int*) shmat(sharedSpaceCP, NULL, 0);

		int sharedEV = EVISION;
		int sharedSpaceEV = shmget(sharedEV, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultEV = (int*) shmat(sharedSpaceEV, NULL, 0);
	}else{
		dataYH	= *resultYH;
		dataYH1	= *resultYH1;
		dataJB	= *resultJB;
		dataCP 	= *resultCP;
		dataEV  = *resultEV;
	}
}
*/

void importData(){
	ifstream myFile(file);
	string line;
	int j=0, buf=0, data=0;

	if (myFile.is_open()){
		while (myFile.good()){
			char buffer[5] = {};
			getline(myFile,line);
			unsigned int i;
			for(i=0; i<line.length(); i++){
				buffer[j] = line[i];
				if(line[i]==';'){
					j = 0;
					buf = atoi(buffer);
					if(data<3) rMin[data] = buf;
					else if(data<6) rMax[data%3] = buf;
					else if(data<9) rED[data%6] = buf;
					data++;
					continue;
				}
				j++;
			}
		}
		myFile.close();
	}
}

void impDataJ(){
	ifstream buka(filekalib);
	string line;
	int j=0, buf=0, data=0;

	if (buka.is_open()){
		while (buka.good()){
			char buffer[5] = {};
			getline(buka,line);
			for(unsigned int i=0; i<line.length(); i++){
				buffer[j] = line[i];
				if(line[i]==';'){
					j = 0;
					buf = atoi(buffer);
					JF[data] = buf;//printf("JF[%d] : %d\n",data,JF[data]);
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}

bool impDatakpl(){
	char file[]={"/home/eros/param/datakepala.eros"};
	ifstream buka(file);
	string line;
	int j=0, buf=0, data=0;
	if (buka.is_open()){
		while (buka.good()){
			char buffer[5] = {};
			getline(buka,line);
			for(int i=0; i<line.length(); i++){
				buffer[j] = line[i];
				if(line[i]==';'){
					j = 0;
					buf = atoi(buffer);
					switch(data)
					{
						case 0 : sdtblax = buf;   fprintf(stderr,"sdtblax   : %d\n",sdtblax); break;
						case 1 : sdtblay = buf;   fprintf(stderr,"sdtblay   : %d\n",sdtblay); break;
						case 2 : sdtdefy = buf;   fprintf(stderr,"sdtdefy   : %d\n",sdtdefy);  break;
						case 3 : sdtexcute = buf; fprintf(stderr,"sdtexcute : %d\n",sdtexcute); break;
						case 4 : sdtblamx = buf;  fprintf(stderr,"sdtblamx  : %d\n",sdtblamx); break;
						case 5 : sdtblamn = buf;  fprintf(stderr,"sdtblamn  : %d\n",sdtblamn);break;
					}
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
		return 1;
	} else return 0;
}

void initColors(){
	file[al] = '0';
	importData();
	BDMin[H] = rMin[H];	BDMax[H] = rMax[H];	BDed[erSize] = rED[erSize];
	BDMin[S] = rMin[S];	BDMax[S] = rMax[S];	BDed[diSize] = rED[diSize];
	BDMin[V] = rMin[V];	BDMax[V] = rMax[V]; BDed[clSize] = rED[clSize];

	file[al] = '1';
	importData();
	BSMin[H] = rMin[H];	BSMax[H] = rMax[H];	BSed[erSize] = rED[erSize];
	BSMin[S] = rMin[S];	BSMax[S] = rMax[S];	BSed[diSize] = rED[diSize];
	BSMin[V] = rMin[V];	BSMax[V] = rMax[V]; BSed[clSize] = rED[clSize];

	file[al] = '2';
	importData();
	BJMin[H] = rMin[H];	BJMax[H] = rMax[H];	BJed[erSize] = rED[erSize];
	BJMin[S] = rMin[S];	BJMax[S] = rMax[S];	BJed[diSize] = rED[diSize];
	BJMin[V] = rMin[V];	BJMax[V] = rMax[V]; BJed[clSize] = rED[clSize];

	file[al] = '3';
	importData();
	GwMin[H] = rMin[H];	GwMax[H] = rMax[H];	Gwed[erSize] = rED[erSize];
	GwMin[S] = rMin[S];	GwMax[S] = rMax[S];	Gwed[diSize] = rED[diSize];
	GwMin[V] = rMin[V];	GwMax[V] = rMax[V]; Gwed[clSize] = rED[clSize];

	file[al] = '4';
	importData();
	CyMin[H] = rMin[H];	CyMax[H] = rMax[H];	Cyed[erSize] = rED[erSize];
	CyMin[S] = rMin[S];	CyMax[S] = rMax[S];	Cyed[diSize] = rED[diSize];
	CyMin[V] = rMin[V];	CyMax[V] = rMax[V]; Cyed[clSize] = rED[clSize];

	file[al] = '5';
	importData();
	MgMin[H] = rMin[H];	MgMax[H] = rMax[H];	Mged[erSize] = rED[erSize];
	MgMin[S] = rMin[S];	MgMax[S] = rMax[S];	Mged[diSize] = rED[diSize];
	MgMin[V] = rMin[V];	MgMax[V] = rMax[V]; Mged[clSize] = rED[clSize];

	file[al] = '6';
	importData();
	GPMin[H] = rMin[H];	GPMax[H] = rMax[H];	GPed[erSize] = rED[erSize];
	GPMin[S] = rMin[S];	GPMax[S] = rMax[S];	GPed[diSize] = rED[diSize];
	GPMin[V] = rMin[V];	GPMax[V] = rMax[V]; GPed[clSize] = rED[clSize];

	file[al] = '7';
	importData();
	LHMin[H] = rMin[H];	LHMax[H] = rMax[H];	LHed[erSize] = rED[erSize];
	LHMin[S] = rMin[S];	LHMax[S] = rMax[S];	LHed[diSize] = rED[diSize];
	LHMin[V] = rMin[V];	LHMax[V] = rMax[V]; LHed[clSize] = rED[clSize];

	file[al] = '8';
	importData();
	LHMin1[H] = rMin[H];	LHMax1[H] = rMax[H];	LHed1[erSize] = rED[erSize];
	LHMin1[S] = rMin[S];	LHMax1[S] = rMax[S];	LHed1[diSize] = rED[diSize];
	LHMin1[V] = rMin[V];	LHMax1[V] = rMax[V];LHed1[clSize] = rED[clSize];

	file[al] = '9';
	importData();
	ObsMin[H]  = rMin[H];	ObsMax[H] = rMax[H];	ObsED[erSize] = rED[erSize];
	ObsMin[S]  = rMin[S];	ObsMax[S] = rMax[S];	ObsED[diSize] = rED[diSize];
	ObsMin[V]  = rMin[V];	ObsMax[V] = rMax[V]; ObsED[clSize] = rED[clSize];
}

Mat GetThresImage(Mat img,int mode){
	Mat imHSV;
	Mat Thres;
	cvtColor(img,imHSV,COLOR_BGR2HSV);
	if(mode == 1){ //TrackBall
		if(distBall == 1||sobs==true){ //Dekat
			inRange(imHSV,Scalar(BDMin[H],BDMin[S],BDMin[V]), Scalar(BDMax[H],BDMax[S],BDMax[V]),Thres);
		}else if(distBall ==2){ //Sedang
			inRange(imHSV, Scalar(BSMin[H],BSMin[S],BSMin[V]), Scalar(BSMax[H],BSMax[S],BSMax[V]), Thres);
		}else if(distBall == 3){ //Jauh
			inRange(imHSV, Scalar(BJMin[H],BJMin[S],BJMin[V]), Scalar(BJMax[H],BJMax[S],BJMax[V]), Thres);
		}
	}else if(mode == 2){ //TrackGoal
		inRange(imHSV, Scalar(GwMin[H],GwMin[S],GwMin[V]), Scalar(GwMax[H],GwMax[S],GwMax[V]), Thres);
	}else if(mode == 3){ //TrackField
		if(flagLH == 1 ){
			inRange(imHSV, Scalar(LHMin[H],LHMin[S],LHMin[V]), Scalar(LHMax[H],LHMax[S],LHMax[V]), Thres);
		}else{
			inRange(imHSV, Scalar(LHMin1[H],LHMin1[S],LHMin1[V]),Scalar(LHMax1[H],LHMax1[S],LHMax1[V]), Thres);
		}
	}else if(mode == 4){ //TrackLine
		inRange(imHSV, Scalar(GPMin[H],GPMin[S],GPMin[V]), Scalar(GPMax[H],GPMax[S],GPMax[V]), Thres);
	}else if(mode == 5){ //TrackObstacle
		inRange(imHSV, Scalar(ObsMin[H],ObsMin[S],ObsMin[V]), Scalar(ObsMax[H],ObsMax[S],ObsMax[V]), Thres);
		//cvInRangeS(imHSV, cvScalar(CyMin[H],CyMin[S],CyMin[V]), cvScalar(CyMax[H],CyMax[S],CyMax[V]), thres);//Cyan
		//cvInRangeS(imHSV, cvScalar(MgMin[H],MgMin[S],MgMin[V]), cvScalar(MgMax[H],MgMax[S],MgMax[V]), thres);//Magenta
	}
	return Thres;
}

/*****************************Errosion*************************/
Mat Erosion(Mat img,int mode, int tipe){
  	Mat erosion_dst;
  	if(mode==1){ //TrackBall
		if(distBall == 1){ //Dekat
			Mat element = getStructuringElement(tipe, Size(2*BDed[erSize] + 1, 2*BDed[erSize]+1 ), Point(BDed[erSize], BDed[erSize]) );
			erode(img, erosion_dst, element);
		}else if(distBall ==2){ //Sedang
			Mat element = getStructuringElement(tipe, Size(2*BSed[erSize] + 1, 2*BSed[erSize]+1 ), Point(BSed[erSize], BSed[erSize]) );
			erode(img, erosion_dst, element);
		}else if(distBall == 3){ //Jauh
			Mat element = getStructuringElement(tipe, Size(2*BJed[erSize] + 1, 2*BJed[erSize]+1 ), Point(BJed[erSize], BJed[erSize]) );
			erode(img, erosion_dst, element);
		}
	}else if(mode==2){ //TrackGoal
		Mat element = getStructuringElement(tipe, Size(2*Gwed[erSize] + 1, 2*Gwed[erSize]+1 ), Point(Gwed[erSize], Gwed[erSize]) );
		erode(img, erosion_dst, element);
	}else if(mode==3){ //TrackField
		if(distBall == 1){
			Mat element = getStructuringElement(tipe, Size(2*LHed[erSize] + 1, 2*LHed[erSize]+1 ), Point(LHed[erSize], LHed[erSize]) );
			erode(img, erosion_dst, element);
		}else{
			Mat element = getStructuringElement(tipe, Size(2*LHed1[erSize] + 1, 2*LHed1[erSize]+1 ), Point(LHed1[erSize], LHed1[erSize]) );
			erode(img, erosion_dst, element);
		}
	}else if(mode==4){ //TrackLine
		Mat element = getStructuringElement(tipe, Size(2*GPed[erSize] + 1, 2*GPed[erSize]+1 ), Point(GPed[erSize], GPed[erSize]) );
		erode(img, erosion_dst, element);
	}else if(mode==5){ //TrackObstacle
		/*element = cvCreateStructuringElementEx(Mged[erSize]*2+1,
				Mged[erSize]*2+1, Mged[erSize], Mged[erSize], erosion_type, 0 );
		cvErode(img, erosion_dst, element, 1);*/
		Mat element = getStructuringElement(tipe, Size(2*Cyed[erSize] + 1, 2*Cyed[erSize]+1 ), Point(Cyed[erSize], Cyed[erSize]) );
		erode(img, erosion_dst, element);
	}else if(mode==6){ //TrackObstacle
		Mat element = getStructuringElement(tipe, Size(2*2 + 1, 2*2+1 ), Point(2, 2) );
		erode(img, erosion_dst, element);
	}
	return erosion_dst;
}
/****************************Dillation*************************/
Mat Dilation(Mat img,int mode, int tipe){
	Mat dillation_dst;
	if(mode==1){ //TrackBall
		if(distBall == 1){ //Dekat
			Mat element = getStructuringElement(tipe, Size(2*BDed[diSize] + 1, 2*BDed[diSize]+1 ), Point(BDed[diSize], BDed[diSize]) );
			dilate(img, dillation_dst, element);
		}else if(distBall ==2){ //Sedang
			Mat element = getStructuringElement(tipe, Size(2*BSed[diSize] + 1, 2*BSed[diSize]+1 ), Point(BSed[diSize], BSed[diSize]) );
			dilate(img, dillation_dst, element);
		}else if(distBall == 3){ //Jauh
			Mat element = getStructuringElement(tipe, Size(2*BJed[diSize] + 1, 2*BJed[diSize]+1 ), Point(BJed[diSize], BJed[diSize]) );
			dilate(img, dillation_dst, element);
		}
	}else if(mode==2){ //TrackGoal
		Mat element = getStructuringElement(tipe, Size(2*Gwed[diSize] + 1, 2*Gwed[diSize]+1 ), Point(Gwed[diSize], Gwed[diSize]) );
		dilate(img, dillation_dst, element);
	}else if(mode==3){ //TrackField
		if(distBall == 1){
			Mat element = getStructuringElement(tipe, Size(2*LHed[diSize] + 1, 2*LHed[diSize]+1 ), Point(LHed[diSize], LHed[diSize]) );
			dilate(img, dillation_dst, element);
		}else{
			Mat element = getStructuringElement(tipe, Size(2*LHed1[diSize] + 1, 2*LHed1[diSize]+1 ), Point(LHed1[diSize], LHed1[diSize]) );
			dilate(img, dillation_dst, element);
		}
	}else if(mode==4){ //TrackLine
		Mat element = getStructuringElement(tipe, Size(2*GPed[diSize] + 1, 2*GPed[diSize]+1 ), Point(GPed[diSize], GPed[diSize]) );
		dilate(img, dillation_dst, element);
	}else if(mode==5){ //TrackObstacle
		/*element = cvCreateStructuringElementEx(Mged[diSize]*2+1,
				Mged[diSize]*2+1, Mged[diSize], Mged[diSize], erosion_type, 0 );
		cvErode(img, dillation_dst, element, 1);*/
		Mat element = getStructuringElement(tipe, Size(2*Cyed[diSize] + 1, 2*Cyed[diSize]+1 ), Point(Cyed[diSize], Cyed[diSize]) );
		dilate(img, dillation_dst, element);
	}else if(mode==6){ //TrackObstacle
		Mat element = getStructuringElement(tipe, Size(2*2 + 1, 2*2+1 ), Point(2, 2) );
		dilate(img, dillation_dst, element);
	}
	return dillation_dst;
}

Mat Closing(Mat img, int mode, int tipe){
	Mat cls;
	if(mode==1){ //TrackBall
			if(distBall == 1){ //Dekat
				Mat element = getStructuringElement(tipe, Size(2*BDed[clSize] + 1, 2*BDed[clSize]+1 ), Point(BDed[clSize], BDed[clSize]) );
				morphologyEx(img, cls, 3, element);
			}else if(distBall ==2){ //Sedang
				Mat element = getStructuringElement(tipe, Size(2*BSed[clSize] + 1, 2*BSed[clSize]+1 ), Point(BSed[clSize], BSed[clSize]) );
				morphologyEx(img, cls, 3, element);
			}else if(distBall == 3){ //Jauh
				Mat element = getStructuringElement(tipe, Size(2*BJed[clSize] + 1, 2*BJed[clSize]+1 ), Point(BJed[clSize], BJed[clSize]) );
				morphologyEx(img, cls, 3, element);
			}
	}else if(mode==2){ //TrackGoal
		Mat element = getStructuringElement(tipe, Size(2*Gwed[clSize] + 1, 2*Gwed[clSize]+1 ), Point(Gwed[clSize], Gwed[clSize]) );
		morphologyEx(img, cls, 3, element);
	}else if(mode==3){ //TrackField
		if(distBall == 1){
			Mat element = getStructuringElement(tipe, Size(2*LHed[clSize] + 1, 2*LHed[clSize]+1 ), Point(LHed[clSize], LHed[clSize]) );
			morphologyEx(img, cls, 3, element);
		}else{
			Mat element = getStructuringElement(tipe, Size(2*LHed1[clSize] + 1, 2*LHed1[clSize]+1 ), Point(LHed1[clSize], LHed1[clSize]) );
			morphologyEx(img, cls, 3, element);
		}
	}else if(mode==4){ //TrackLine
		Mat element = getStructuringElement(tipe, Size(2*GPed[clSize] + 1, 2*GPed[clSize]+1 ), Point(GPed[clSize], GPed[clSize]) );
		morphologyEx(img, cls, 3, element);
	}else if(mode==5){ //TrackObstacle
		/*element = cvCreateStructuringElementEx(Mged[diSize]*2+1,
				Mged[diSize]*2+1, Mged[diSize], Mged[diSize], erosion_type, 0 );
		cvErode(img, dillation_dst, element, 1);*/
		Mat element = getStructuringElement(tipe, Size(2*Cyed[clSize] + 1, 2*Cyed[clSize]+1 ), Point(Cyed[clSize], Cyed[clSize]) );
		morphologyEx(img, cls, 3, element);
	}else if(mode==6){ //TrackObstacle
		Mat element = getStructuringElement(tipe, Size(2*2 + 1, 2*2+1 ), Point(2, 2) );
		morphologyEx(img, cls, 3, element);
	}
	return cls;
}

void locateBall(){
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat imgBall=frameBall.clone();
	if(show) imshow("Ori", imgBall);

	int result0 = 0, result1 = 0;
	XB = 0; YB = 0;
	int height=0, width=0; //ukuran
	Rect rect = Rect(0, 0, 0, 0);
	int mvi=0;
	int highestcount=0;
	//cropBall(frameBall.rows, frameBall.cols, CV_8UC3);
	/*cropBall.setTo(Scalar(0,0,0));
	frameBall.copyTo(cropBall, mask);
	normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
	if(show)imshow("Ball Crop :*",cropBall);
	//imshow("Field", frameField);
	//imshow("mask", mask);*/
	findContours(imgBall,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );
	for(unsigned int i=0;i<contours.size();i++){
		result0 = (int) fabs(contourArea(contours[i], 0));
		if(result0<result1)continue;
		if(distBall==1){
			if(result0<minBall) continue;
		}
		rect = boundingRect(contours[i]);
		if(distBall==1){
			if(rect.height<40 || rect.width < 40) continue;
		}
		else if(distBall ==  2) {
			if(rect.height<15 || rect.height<15) continue;
			if(rect.height>250||rect.width>250) continue;
		}else if(distBall==3){
			if(rect.height<5 || rect.height<5) continue;
			if(rect.height>130||rect.width>130) continue;
		}
		//if(distBall==1 && rect.y<50)continue;
		//if(distBall==1 || distBall==3) if(rect.y<5) continue;
		if(rect.y<5 || rect.x <10) continue;
		//if(rect.height<10)continue;
		int ukuran,lebarf,tinggif,hasill;
		lebarf=(int)rect.width;
		tinggif=(int)rect.height;
		if(lebarf>tinggif){
			ukuran=lebarf/tinggif;
		}
		else{
			ukuran=tinggif/lebarf;
		}
		if(ukuran>1.2){
			continue;
		}
		ukuran=lebarf*tinggif;
		hasill=ukuran/(float)result0;
		if(hasill>3.0) continue;
		int counter1=0, flag_atas=0, flag_kiri=0, flag_kanan=0;
		//if(distBall != 1){
			if((rect.y-10)>0){ //cek sudut 180
				Scalar scale0 = frameField.at<uchar>(rect.y-10,rect.x+(rect.width/2));
				if(scale0.val[0]==255) counter1++;
				else flag_atas=1;
			}
			if((rect.x+rect.width+10)<lebar_frame){ //cek sudut 90
				Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),(rect.x+rect.width+10));
				if(scale0.val[0]==255) counter1++;
				else flag_kanan=1;
			}
			if((rect.y+rect.height+10)<tinggi_frame){ //cek sudut 0
				Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+10,rect.x+(rect.width/2));
				if(scale0.val[0]==255) counter1++;
			}
			if((rect.x-10)>0){ //cek sudut 270
				Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),rect.x-10);
				if(scale0.val[0]==255) counter1++;
				else flag_kiri=1;
			}
			if((rect.x-9)>0 && (rect.y-9)>0){ //cek sudut 315
				Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x-9);
				if(scale0.val[0]==255) counter1++;
			}
			if((rect.x-9)>0 && (rect.y+rect.height+9)<tinggi_frame){ //cek sudut 225
				Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+9,rect.x-9);
				if(scale0.val[0]==255) counter1++;
			}
			if((rect.x+rect.width+9)<lebar_frame && (rect.y-9)>0){ //cek sudut 45
				Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x+rect.width+9);
				if(scale0.val[0]==255) counter1++;
			}
			if((rect.x+rect.width+9)<lebar_frame && (rect.x+rect.width+9)<tinggi_frame){ //cek sudut 135
				Scalar scale0 = frameField.at<uchar>(rect.x+rect.width+9,rect.x+rect.width+9);
				if(scale0.val[0]==255) counter1++;
			}
			if(flag_atas==1 && flag_kanan==1 && flag_kiri==1) continue;
			if(counter1>=3){
				if(shog) {
					//Code HOG SVM Disini...
					//if(result0>result1){
						vector< float> descriptorsValues;
						vector< Point> locations;
						Mat img=pureframes(rect), img_gray;
						imshow("Bismillah",img);
						resize(img, img, Size(48,48) ); //Size(32*2,16*2)); //Size(80,72) );
						cvtColor(img, img_gray, CV_RGB2GRAY);
						resize(img_gray, img_gray, Size(48,48) );

						d.compute( img_gray, descriptorsValues, Size(0,0), Size(0,0), locations);
						//vector to Mat
						int col=descriptorsValues.size();
						Mat fm = Mat(1,col,CV_32F);
						memcpy( &(fm.data[sizeof(float)]) ,descriptorsValues.data(),col*sizeof(float));
						//Classification whether data is positive or negative
						int result = svm->predict(fm);
						//fprintf("%s - > %d\n", FullFileName, result);

						//Count data
						if(result == 1){
							result1 = result0;
							mvi=i;
							ppp++;
						}
						else
							nnn++;
					//}
					}
					else{
						result0 = (int) fabs(contourArea(contours[i], 0));
						if(result0>result1){
							result1 = result0;
							mvi=i;
							highestcount=counter1;
							//fprintf(stderr,"result : %d\n",result1);
						}
					}
				}
		//}
		/*else{
			result1 = result0;
			mvi = i;
		}*/

	}
	if(result1){
		rect = boundingRect(contours[mvi]);
		//fprintf(stderr,"result : %f\n",result1);//
		XB = rect.x+(rect.width/2);
		YB = rect.y+(rect.height/2);
		height = rect.height;
		width = rect.width;
	}
	else {
		rect=Rect(0,0,0,0);
	}
	if(show){
		rectangle(frames, rect, CV_RGB(0,0,255), 1, 8, 0);
		if(sgrid){
			int p1x=0,p1y=0,p2x=0,p2y=tinggi_frame,Gr=lebar_frame/10;
			for(int kk=0;kk<10;kk++){
				line(frames, Point(p1x, p1y), Point(p2x, p2y), Scalar(255,255,0), 1, CV_AA);
				p1x=p2x=p1x+Gr;
			}
			p1x=0;p2x=lebar_frame;p1y=p2y=tinggi_frame;
			for(int kk=0;kk<6;kk++){
				line(frames, Point(p1x, p1y), Point(p2x, p2y), Scalar(255,255,0), 1, CV_AA);
				p1y=p2y=p1y-Gr;
			}
		}
		imshow("Contours", frames);
		//fprintf(stderr,"Bola Apa Sekarang ? %d\t Data Y Kepalamu Piro ? %d\t YH = %d\n", distBall, dataYH1, dataYH);
		//fprintf(stderr,"XB = %d | YB = %d\tHeight = %d | Width = %d\nr0 = %.2d | r1 = %.2d\n", XB, YB, height, width, result0, result1);
		//imshow("aaaa", frameField);
	}
}

/*void locateBall(){
	vector<Vec3f> circles;
	vector<Vec4i> hierarchy;

	Mat imgBall=frameBall.clone();

	int result0 = 0, result1 = 0;
	XB = 0; YB = 0;
	int height=0, width=0; //ukuran
	Rect rect = Rect(0, 0, 0, 0);
	int mvi=0;
	int highestcount=0;

	HoughCircles( imgBall, circles, CV_HOUGH_GRADIENT, 1, imgBall.rows/8, 200, 100, 0, 0 );
	for(unsigned int i=0;i<circles.size();i++){
		result0 = (int) fabs(contourArea(circles[i], 0));
		if(distBall==1){
			if(result0<minBall) continue;
		}
		rect = boundingRect(circles[i]);
		int counter1=0;
		if((rect.y-10)>0){ //cek sudut 180
			Scalar scale0 = frameField.at<uchar>(rect.y-10,rect.x+(rect.width/2));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+10)<lebar_frame){ //cek sudut 90
			Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),(rect.x+rect.width+10));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.y+rect.height+10)<tinggi_frame){ //cek sudut 0
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+10,rect.x+(rect.width/2));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-10)>0){ //cek sudut 270
			Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),rect.x-10);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-9)>0 && (rect.y-9)>0){ //cek sudut 315
			Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x-9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-9)>0 && (rect.y+rect.height+9)<tinggi_frame){ //cek sudut 225
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+9,rect.x-9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+9)<lebar_frame && (rect.y-9)>0){ //cek sudut 45
			Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x+rect.width+9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+9)<lebar_frame && (rect.x+rect.width+9)<tinggi_frame){ //cek sudut 135
			Scalar scale0 = frameField.at<uchar>(rect.x+rect.width+9,rect.x+rect.width+9);
			if(scale0.val[0]==255) counter1++;
		}
		//if(counter1>4){
			result0 = (int) fabs(contourArea(circles[i], 0));
			//if(result0>result1){
				result1 = result0;
				mvi=i;
				//highestcount=counter1;
				//fprintf(stderr,"result : %d\n",result1);
			//}
		//}
	}
	if(result1){
		rect = boundingRect(circles[mvi]);
		//fprintf(stderr,"result : %f\n",result1);//
		Point center(cvRound(circles[mvi][0]), cvRound(circles[mvi][1]));
		int radius = cvRound(circles[mvi][2]);
		// circle center
		circle( frames, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( frames, center, radius, Scalar(0,0,255), 3, 8, 0 );
		XB = rect.x+(rect.width/2);
		YB = rect.y+(rect.height/2);
		height = rect.height;
		width = rect.width;
	}
	else {
		rect=Rect(0,0,0,0);
	}
	if(show){
		rectangle(frames, rect, CV_RGB(0,0,255), 1, 8, 0);
		imshow("Contours", frames);
		imshow("Bismillah", imgBall);
		fprintf(stderr,"XB = %d | YB = %d", XB, YB);
	}
}*/

/*void locateBall(){
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//Mat imgBall=frameBall.clone();
	//imshow("Ori", imgBall);

	int result0 = 0, result1 = 0;
	XB = 0; YB = 0;
	int height=0, width=0; //ukuran
	Rect rect = Rect(0, 0, 0, 0);
	int mvi=0;
	int highestcount=0;
	//cropBall(frameBall.rows, frameBall.cols, CV_8UC3);
	cropBall.setTo(Scalar(0,0,0));
	frameBall.copyTo(cropBall, mask);
	normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
	if(show)imshow("Ball Crop :*",cropBall);
	//imshow("Field", frameField);
	//imshow("mask", mask);
	findContours(cropBall,contours,hierarchy,RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );
	for(unsigned int i=0;i<contours.size();i++){
		result0 = (int) fabs(contourArea(contours[i], 0));
		if(result0<result1)continue;
		if(distBall==1){
			if(result0<minBall) continue;
		}
		rect = boundingRect(contours[i]);
		if(rect.y<2)continue;
		//if(rect.height<10)continue;
		float ukuran,lebarf,tinggif,hasill;
		lebarf=(float)rect.width;
		tinggif=(float)rect.height;
		if(lebarf>tinggif){
			ukuran=lebarf/tinggif;
		}
		else{
			ukuran=tinggif/lebarf;
		}
		if(ukuran>3.0){
			continue;
		}
		ukuran=lebarf*tinggif;
		hasill=ukuran/(float)result0;
		if(hasill>3.0)continue;
		int counter1=0;
		if((rect.y-10)>0){ //cek sudut 180
			Scalar scale0 = frameField.at<uchar>(rect.y-10,rect.x+(rect.width/2));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+10)<lebar_frame){ //cek sudut 90
			Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),(rect.x+rect.width+10));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.y+rect.height+10)<tinggi_frame){ //cek sudut 0
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+10,rect.x+(rect.width/2));
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-10)>0){ //cek sudut 270
			Scalar scale0 = frameField.at<uchar>(rect.y+(rect.height/2),rect.x-10);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-9)>0 && (rect.y-9)>0){ //cek sudut 315
			Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x-9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x-9)>0 && (rect.y+rect.height+9)<tinggi_frame){ //cek sudut 225
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+9,rect.x-9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+9)<lebar_frame && (rect.y-9)>0){ //cek sudut 45
			Scalar scale0 = frameField.at<uchar>(rect.y-9,rect.x+rect.width+9);
			if(scale0.val[0]==255) counter1++;
		}
		if((rect.x+rect.width+9)<lebar_frame && (rect.x+rect.width+9)<tinggi_frame){ //cek sudut 135
			Scalar scale0 = frameField.at<uchar>(rect.x+rect.width+9,rect.x+rect.width+9);
			if(scale0.val[0]==255) counter1++;
		}
		if(counter1>=4){
			if(shog) {
				//Code HOG SVM Disini...
				//if(result0>result1){
					vector< float> descriptorsValues;
					vector< Point> locations;
					Mat img=pureframes(rect), img_gray;
					imshow("Bismillah",img);
					resize(img, img, Size(48,48) ); //Size(32*2,16*2)); //Size(80,72) );
					cvtColor(img, img_gray, CV_RGB2GRAY);
					resize(img_gray, img_gray, Size(48,48) );

					d.compute( img_gray, descriptorsValues, Size(0,0), Size(0,0), locations);
					//vector to Mat
					int col=descriptorsValues.size();
					Mat fm = Mat(1,col,CV_32F);
					memcpy( &(fm.data[sizeof(float)]) ,descriptorsValues.data(),col*sizeof(float));
					//Classification whether data is positive or negative
					int result = svm->predict(fm);
					//fprintf("%s - > %d\n", FullFileName, result);

					//Count data
					if(result == 1){
						result1 = result0;
						mvi=i;
						ppp++;
					}
					else
						nnn++;
				//}
				}
				else{
					result0 = (int) fabs(contourArea(contours[i], 0));
					if(result0>result1){
						result1 = result0;
						mvi=i;
						highestcount=counter1;
						//fprintf(stderr,"result : %d\n",result1);
					}
				}
			}


	}
	if(result1){
		rect = boundingRect(contours[mvi]);
		//fprintf(stderr,"result : %f\n",result1);//
		XB = rect.x+(rect.width/2);
		YB = rect.y+(rect.height/2);
		height = rect.height;
		width = rect.width;
	}
	else {
		rect=Rect(0,0,0,0);
	}
	if(show){
		rectangle(frames, rect, CV_RGB(0,0,255), 1, 8, 0);
		if(sgrid){
			int p1x=0,p1y=0,p2x=0,p2y=tinggi_frame,Gr=lebar_frame/10;
			for(int kk=0;kk<10;kk++){
				line(frames, Point(p1x, p1y), Point(p2x, p2y), Scalar(255,255,0), 1, CV_AA);
				p1x=p2x=p1x+Gr;
			}
			p1x=0;p2x=lebar_frame;p1y=p2y=tinggi_frame;
			for(int kk=0;kk<6;kk++){
				line(frames, Point(p1x, p1y), Point(p2x, p2y), Scalar(255,255,0), 1, CV_AA);
				p1y=p2y=p1y-Gr;
			}
		}
		imshow("Contours", frames);
		fprintf(stderr,"Bola Apa Sekarang ? %d\t Data Y Kepalamu Piro ? %d\t YH = %d\n", distBall, dataYH1, dataYH);
		//fprintf(stderr,"XB = %d | YB = %d\tHeight = %d | Width = %d\nr0 = %.2d | r1 = %.2d\n", XB, YB, height, width, result0, result1);
		//imshow("aaaa", frameField);
	}
}*/
void locateObs(){
	Mat Obs,Obs2, fieldo, fieldc, fieldf, fieldi;
	fieldc=frameField.clone();
	fieldf=frameField.clone(); //Field Fill
	floodFill(fieldf, cv::Point(0,0), Scalar(255));
	bitwise_not(fieldf, fieldi);
	// Combine the two images to get the foreground.
	fieldo = (fieldc | fieldi);
	//fieldo=field_out.clone();
	if(show){
		rectangle(frames, Point(0, FF1.y), Point(sepertigaframe, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe, FF2.y), Point(sepertigaframe*2, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe*2, FF3.y), Point(lebar_frame, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
	}
	bitwise_or(frameField,frameGoal,Obs);
	bitwise_or(frameBall,Obs,Obs2);
	bitwise_or(frameLine,Obs2,Obs);
	bitwise_not(Obs,Obs2);
	//imshow("Lapangan", fieldo);
	//imshow("Obs2", Obs2);
	Obs=(mask & Obs2);
	rectangle(Obs, Point(0, tinggi_frame), Point(sepersepuluhframe, tinggi_frame-sepertigaframet), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	rectangle(Obs, Point(lebar_frame, tinggi_frame), Point(lebar_frame-sepersepuluhframe, tinggi_frame-sepertigaframet), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	//Obs=Erosion(Obs2,6,MORPH_RECT);
	//Obs=Dilation(Obs,6,MORPH_RECT);
	//if(show)imshow("Ful Uncroped",Obs);
	if(f1!=tinggi_frame){
		rectangle(Obs, Point(0, 0), Point(sepertigaframe, FF1.y), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(Obs, Point(0, 0), Point(sepertigaframe, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);

	if(f2!=tinggi_frame){
		rectangle(Obs, Point(sepertigaframe, 0), Point(sepertigaframe*2, FF2.y), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(Obs, Point(sepertigaframe, 0), Point(sepertigaframe*2, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);

	if(f3!=tinggi_frame){
		rectangle(Obs, Point(sepertigaframe*2, 0), Point(lebar_frame, FF3.y), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(Obs, Point(sepertigaframe*2, 0), Point(lebar_frame, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);

	if(show)imshow("Obs Croped",Obs);
	//if(show)imshow("Obs Croped2",Obs2);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Rect rect1 = Rect(0,0,0,0);
	Rect rect,trect,rect2,rect3;
	rect2=rect3=rect=trect=rect1;
	int result0=0,result1=0,result2=0,result3=0,rtemp=0;
	findContours(Obs,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	for(unsigned int i=0;i<contours.size();i++){
		result0 = (int) fabs(contourArea(contours[i], 0));
		rect = boundingRect(contours[i]);
		if(rect.y > 230) continue;
		if(rect.x<sepertigaframe){
			if((rect.y-FF1.y)>1){
				if(result0<luasnorm)continue;
			}
			else {
				if(result0<luasabnorm)continue;
			}
		}
		else if(rect.x<sepertigaframe*2){
			if((rect.y-FF2.y)>1){
				if(result0<luasnorm)continue;
			}
			else {
				if(result0<luasabnorm)continue;
			}
		}
		else {
			if((rect.y-FF3.y)>1){
				if(result0<luasnorm)continue;
			}
			else {
				if(result0<luasabnorm)continue;
			}
		}
		if(rect.height<macrotinggiframe) continue;
		if((tinggi_frame-(rect.y+rect.height))<2) continue;
		if(result0>result1){
			rtemp=result1;
			result1=result0;
			result0=rtemp;
			trect=rect1;
			rect1=rect;
			rect=trect;
		}
		if(result0>result2){
			rtemp=result2;
			result2=result0;
			result0=rtemp;
			trect=rect2;
			rect2=rect;
			rect=trect;
		}
		if(result0>result3){
			rtemp=result3;
			result3=result0;
			result0=rtemp;
			trect=rect3;
			rect3=rect;
			rect=trect;
		}
	}
	dataObs[0]=dataObs[1]=dataObs[2]=0;
	if(result1){
		dataObs[0]= dataObs[0]|(unsigned int)rect1.x;
		dataObs[0]=(dataObs[0]<<10)|((unsigned int)rect1.x+(unsigned int)rect1.width);
		dataObs[0]=(dataObs[0]<<10)|((unsigned int)rect1.y+(unsigned int)rect1.height); // urutan dari lowbit =>>> titik y bawah, titik x kanan, titik x kiri.
	}
	if(result2){
		dataObs[1]= dataObs[1]|(unsigned int)rect2.x;
		dataObs[1]=(dataObs[1]<<10)|((unsigned int)rect2.x+(unsigned int)rect2.width);
		dataObs[1]=(dataObs[1]<<10)|((unsigned int)rect2.y+(unsigned int)rect2.height); // urutan dari lowbit =>>> titik y bawah, titik x kanan, titik x kiri.
	}
	if(result3){
		dataObs[2]= dataObs[2]|(unsigned int)rect3.x;
		dataObs[2]=(dataObs[2]<<10)|((unsigned int)rect3.x+(unsigned int)rect3.width);
		dataObs[2]=(dataObs[2]<<10)|((unsigned int)rect3.y+(unsigned int)rect3.height); // urutan dari lowbit =>>> titik y bawah, titik x kanan, titik x kiri.
	}
	if(show){
		if(cetak){
			if(result1)fprintf(stderr,"OBS 1 Y : %d Xmax : %d Xmin : %d\n",rect1.y+rect1.height,rect1.x+rect1.width,rect1.x);
			if(result2)fprintf(stderr,"OBS 2 Y : %d Xmax : %d Xmin : %d\n",rect2.y+rect2.height,rect2.x+rect2.width,rect2.x);
			if(result3)fprintf(stderr,"OBS 3 Y : %d Xmax : %d Xmin : %d\n",rect3.y+rect3.height,rect3.x+rect3.width,rect3.x);
		}
		rectangle(frames, rect1, CV_RGB(0,0,255), 4, 8, 0);
		rectangle(frames, rect2, CV_RGB(0,0,255), 4, 8, 0);
		rectangle(frames, rect3, CV_RGB(0,0,255), 4, 8, 0);
		imshow("detected",frames);
	}
}

void locateGoal(){
	Mat imgGoal=frameGoal.clone();
	if(show){
		rectangle(frames, Point(0, FF1.y-40), Point(sepertigaframe, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe, FF2.y-40), Point(sepertigaframe*2, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe*2, FF3.y-40), Point(lebar_frame, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		//imshow("kalib gawang",imgGoal);
	}
	if(FF1.y>5){
		rectangle(imgGoal, Point(0, 0), Point(sepertigaframe, FF1.y-40), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
		rectangle(imgGoal, Point(0, tinggi_frame), Point(sepertigaframe, FF1.y+tembak), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(imgGoal, Point(0, 0), Point(sepertigaframe, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);

	if(FF2.y>5){
		rectangle(imgGoal, Point(sepertigaframe, 0), Point(sepertigaframe*2, FF2.y-40), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
		rectangle(imgGoal, Point(sepertigaframe, tinggi_frame), Point(sepertigaframe*2, FF2.y+tembak), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(imgGoal, Point(sepertigaframe, 0), Point(sepertigaframe*2, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);

	if(FF3.y>5){
		rectangle(imgGoal, Point(sepertigaframe*2, 0), Point(lebar_frame, FF3.y-40), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
		rectangle(imgGoal, Point(sepertigaframe*2, tinggi_frame), Point(lebar_frame, FF3.y+tembak), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	}
	else rectangle(imgGoal, Point(sepertigaframe*2, 0), Point(lebar_frame, tinggi_frame), CV_RGB(0,0,0), CV_FILLED, CV_AA, 0);
	if(show)imshow("kalib gawang croped",imgGoal);

	int result0=0,result1=0,result2=0,temp=0,counter=0;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Rect rect = Rect(0, 0, 0, 0);
	Rect rect1 = Rect(0, 0, 0, 0);
	Rect rect2 = Rect(0, 0, 0, 0);
	Rect trect = Rect(0, 0, 0, 0);
	int count=0;
	findContours(imgGoal,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	for(unsigned int i=0;i<contours.size();i++){
		rect = boundingRect(contours[i]);
		if(rect.height < 20) continue;
		int hasil;
		hasil = rect.width/rect.height;
		if(hasil >= 1) continue;
		if(rect.x<sepertigaframe){
			if((rect.y-FF1.y)>1)continue;
		}
		else if(rect.x<sepertigaframe*2){
			if((rect.y-FF2.y)>1)continue;
		}
		else {
			if((rect.y-FF3.y)>1)continue;
		}
		//if(rect.height<macrotinggiframe)continue;
		if((rect.x-5)>0){
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+5,rect.x-3);
			if(scale0.val[0]==255) count++;
			if((rect.y+rect.height+5)<tinggi_frame){
				scale0 = frameField.at<uchar>(rect.y+rect.height+5,rect.x-3);
				if(scale0.val[0]==255) count++;
			}
		}
		if((rect.x+rect.width+5)<lebar_frame){
			Scalar scale0 = frameField.at<uchar>(rect.y+rect.height+5,rect.x+rect.width+3);
			if(scale0.val[0]==255) count++;
			if((rect.y+rect.height+5)<tinggi_frame){
				scale0 = frameField.at<uchar>(rect.y+rect.height+5,rect.x+rect.width+3);
				if(scale0.val[0]==255) count++;
			}
		}
		if(count<2){
			count=0;
			continue;
		}
		else {
			count=0;
		}
		//fprintf(stderr,"Frame apsdjapsdjap\n");
		result0 = (int) fabs(contourArea(contours[i], 0));
		if(result0>result1){
			temp=result1;
			result1 = result0;
			result0=temp;
			trect=rect1;
			rect1=rect;
			rect=trect;
			//fprintf(stderr,"result : %d\n",result1);
		}
		if(result0>result2&&abs(rect.x-rect1.x)>(sepersepuluhframe*2)){
			result2=result0;
			rect2=rect;
		}
	}
	if(result1!=0)counter++;
	if(result2!=0)counter++;
	if(counter==0){
		XG=YG=TG=0;
	}
	else if(counter==1){
		XG=rect1.x+(rect1.width/2);
		YG=rect1.y+rect1.height;
		TG=1;
	}
	else{
		XG=(rect1.x+(rect1.width/2)+rect2.x+(rect2.width/2))/2;
		YG=(rect1.y+rect1.height+rect2.y+rect2.height)/2;
		TG=2;
	}
	//fprintf(stderr,"result1 : %d\n",rect1.height);
	//fprintf(stderr,"result2 : %d\n",rect2.height);
	if(show){
		rectangle(frames, rect1, CV_RGB(0,0,255), 1, 8, 0);
		rectangle(frames, rect2, CV_RGB(0,0,255), 1, 8, 0);
		rectangle(frames, Point(XG, YG), Point(XG, YG), CV_RGB(0,255,0), 4, 8, 0);
		imshow("goals",frames);
	}
}
void locateLine(){
	Mat imgLine=frameLine.clone();
	Mat cropLine(frameLine.rows, frameLine.cols, CV_8UC3);
	cropLine.setTo(Scalar(0,0,0));
	frameLine.copyTo(cropLine, mask);
	normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
	if(show){ imshow( "Crop", cropLine );  }
	Mat sepertiga;
	Rect rect;
	Rect separuh1 = cvRect(0, 0, lebar_frame, 160);
	Rect separuh2 = cvRect(0, 160, lebar_frame, 160);
	Rect separuh3 = cvRect(0, 320, lebar_frame, 160);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	K1=K2=K3=Rect(0, 0, 0, 0);
	l1=l2=l3=lebar_frame;
	int result0=0,result1=0,result2=0,result3=0,counter=0, hasil=0, kanan=0, kiri=0;
	for(int devide=0;devide<3;devide++){
		if(devide==0){
			sepertiga=cropLine(separuh1);
			//imshow("separuh1",fField);
		}
		else if(devide==1){
			sepertiga=cropLine(separuh2);
			//cvShowImage("separuh2",dilatedField);
			//imshow("separuh2",fField);
		}
		else {
			sepertiga=cropLine(separuh3);
			//cvShowImage("separuh3",dilatedField);
			//imshow("separuh3",fField);
		}

		findContours(sepertiga,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
		result3=result2=result1=result0=0;
		for(unsigned int i=0;i<contours.size();i++){
			result0 = (int) fabs(contourArea(contours[i], 0));
			/*if(result0<luasminlap){
				continue;
			}*/
			if(result0<30) continue;
			rect = boundingRect(contours[i]);
			hasil=rect.width/rect.height;
			//fprintf(stderr,"Frame apsdjapsdjap\n");
			if(rect.width<20 || rect.height <20) continue;
			if(hasil>=4) continue;
			if(rect.x<l1&&devide==0){
				result1=result0;
				l1=rect.x;
				K1=rect;
				if(rect.x<240){
					kiri=1;
					kanan=0;
				}
				else {
					kanan=1;
					kiri=0;
				}
			}
			else if(rect.x<l2&&devide==1){
				result2=result0;
				l2=rect.x;
				K2=rect;
				if(rect.x<240){
					kiri=1;
					kanan=0;
				}
				else {
					kanan=1;
					kiri=0;
				}
			}
			else if(rect.x<l3&&devide==2){
				result3=result0;
				l3=rect.x;
				K3=rect;
				if(rect.x<240){
					kiri=1;
					kanan=0;
				}
				else {
					kanan=1;
					kiri=0;
				}
			}
		}
	}
	if(result1){
		rectangle(frames, K1, CV_RGB(0,0,255), 1, 8, 0);
	}
	if(result2){
		rectangle(frames, K2, CV_RGB(0,0,255), 1, 8, 0);
	}
	if(result3){
		rectangle(frames, K3, CV_RGB(0,0,255), 1, 8, 0);
	}
	if(show) {
		imshow("LINESS",frames);
		fprintf(stderr,"Kanan ? %d | Kiri ? %d\n",kanan, kiri);
	}
	/*vector<Vec2f> lines;
	// detect lines
	HoughLines(cropLine, lines, 1, CV_PI/180, 150, 0, 0 );
	// draw lines
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( frames, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}*/
	/*vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Rect rect = Rect(0, 0, 0, 0);
	Rect rect1 = Rect(0, 0, 0, 0);
	findContours(cropLine,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	for(unsigned int i=0;i<contours.size();i++){
		result0 = (int) fabs(contourArea(contours[i], 0));
		if(result0<luasnorm) continue;
		rect = boundingRect(contours[i]);
		if(rect.width<minlinelong) continue;/*
		if((rect.y-5)>0){
			Scalar scale0 = frameField.at<uchar>(Point(rect.y-5,rect.x+10));
			if(scale0.val[0]==255) counter++;

			scale0 = frameField.at<uchar>(Point(rect.y-5,(rect.x+(rect.width/2)-10)));
			if(scale0.val[0]==255) counter++;
		}
		if((rect.y+3)<tinggi_frame){
			Scalar scale0 = frameField.at<uchar>(Point(rect.y+3,(rect.x+(rect.width/2))));
			if(scale0.val[0]==255) counter++;
		}
		if(counter<1)continue;//
		if(result0>result1){
			result1=result0;
			rect1=rect;
			//fprintf(stderr,"result : %d\n",result1);
		}
	}*/
	/*if(result1){
		LE=1;
		if(show){
			rectangle(frames, rect1, CV_RGB(0,0,255), 1, 8, 0);
			imshow("LINESS",frames);
		}
	}
	else LE=0;*/

}
void calculateField(){
	int skip=0,pemb=0,deff=0, jr=0;
	estjarak=0;
	if(f1!=tinggi_frame){
		if(FF2.y-FF1.y < 15){
			estjarak+=FF1.y;
			pemb++;
		}
	}
	if(f2!=tinggi_frame){
		estjarak+=FF2.y;
		pemb++;
	}
	if(f3!=tinggi_frame){
		if(FF2.y-FF3.y <15){
			estjarak+=FF3.y;
			pemb++;
		}
	}
	if(pemb==3){
		estjarak/=3;
		deff=5;
	}
	else if(pemb==2){
		estjarak/=2;
		deff=5;
	}
	else if(pemb==1){
		deff=1;
	}
	else{
		//fprintf(stderr,"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
		skip=1;
		Mrow=1;
	}
	if(skip!=1){
		estjarak+=JF[0];
		estjarak-=100;
		JG=estjarak;
		jr=estjarak;
		if(estjarak>88)Mrow=1;
		else if(estjarak>48)Mrow=2;
		else if(estjarak>20)Mrow=3;
		else if(estjarak>1)Mrow=4;
		else Mrow=deff;
		//writeYPOS(Mrow);
	}
	//else writeYPOS(Mrow); writejarak(jr);
	if(cetak)fprintf(stderr,"Y POS : %d\t Jarak : %.2f\t Jar = %d\n",Mrow,estjarak,jr);
	if(show){
		line(frames,Point(0, estjarak), Point(lebar_frame, estjarak),CV_RGB(0,0,255), 1, CV_AA);
		rectangle(frames, Point(0, FF1.y), Point(sepertigaframe, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe, FF2.y), Point(sepertigaframe*2, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		rectangle(frames, Point(sepertigaframe*2, FF3.y), Point(lebar_frame, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
		imshow("lapangan",frames);
	}
}
void GTball(){
	if(!sball){
		if(dataYH1<=(suduteksekusi+5)){distBall = 1;} //dekat
		else if(dataYH1>(suduteksekusi+5) && dataYH1<=(sudutcek)){distBall = 2;} //sedang
		else if(dataYH1>(sudutcek)){distBall = 3;} //jauh
		else distBall = 1;/**/
	}
	Mat frameTball = GetThresImage(pureframes, BALL);
	Mat eroded	= Erosion(frameTball, BALL, MORPH_ELLIPSE);
	Mat close = Dilation(eroded, BALL, MORPH_ELLIPSE);
	frameBall	= Closing(close, BALL, MORPH_ELLIPSE);
	ROS_INFO("BAll[%d][%d]",distBall,suduteksekusi);
}

void GTfield(){
	if(!sgoal){
		if(dataYH1<40){flagLH = 1;} //dekat
		else flagLH = 0;/*jauhh*/
	}
	Mat frameTfield = GetThresImage(pureframes, FIELD);
	Mat eroded	= Erosion(frameTfield, FIELD, MORPH_RECT);
	frameField	= Dilation(eroded, FIELD, MORPH_RECT);
	Mat fField=frameField.clone();
	Mat sepertiga;
	Rect rect;
	Rect separuh1 = cvRect(0, 0, sepertigaframe, tinggi_frame);
	Rect separuh2 = cvRect(sepertigaframe, 0, sepertigaframe, tinggi_frame);
	Rect separuh3 = cvRect((sepertigaframe)*2, 0, sepertigaframe, tinggi_frame);

	FF1=FF2=FF3=Rect(0, 0, 0, 0);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int result0=0,result1=0,result2=0,result3=0;


	f1=f2=f3=tinggi_frame;
	for(int devide=0;devide<3;devide++){
		if(devide==0){
			sepertiga=fField(separuh1);
			//imshow("separuh1",fField);
		}
		else if(devide==1){
			sepertiga=fField(separuh2);
			//cvShowImage("separuh2",dilatedField);
			//imshow("separuh2",fField);
		}
		else {
			sepertiga=fField(separuh3);
			//cvShowImage("separuh3",dilatedField);
			//imshow("separuh3",fField);
		}

		findContours(sepertiga,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
		result3=result2=result1=result0=0;
		for(unsigned int i=0;i<contours.size();i++){
			rect = boundingRect(contours[i]);
			//fprintf(stderr,"Frame apsdjapsdjap\n");
			result0 = (int) fabs(contourArea(contours[i], 0));
			if(result0<luasminlap){
				continue;
			}
			if(rect.y<f1&&devide==0){
				result1=result0;
				f1=rect.y;
				FF1=rect;
			}
			else if(rect.y<f2&&devide==1){
				result2=result0;
				f2=rect.y;
				FF2=rect;
			}
			else if(rect.y<f3&&devide==2){
				result3=result0;
				f3=rect.y;
				FF3=rect;
			}
		}
	}
	/** Convex Hull Method **/
	//====== FIELD ======//
	fieldk=frameField.clone();
	fieldfill=frameField.clone(); //Field Fill
	floodFill(fieldfill, cv::Point(0,0), Scalar(255));
	bitwise_not(fieldfill, fieldfill_inv);
	// Combine the two images to get the foreground.
	field_out = (fieldk | fieldfill_inv);

	//==============================//
	findContours( field_out, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	RNG rng;
	/// Draw contours + hull results
	drawing = Mat::zeros( field_out.size(), CV_8UC3 );
	mask = Mat::zeros(drawing.rows, drawing.cols, CV_8UC1);
	Rect kotak = Rect(0, 0, 0, 0);
	vector<vector<Point> >hull( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
		{  convexHull( Mat(contours[i]), hull[i], false ); }

	for( int i = 0; i< contours.size(); i++ )
		{
			kotak = boundingRect(contours[i]);
			if(kotak.width<30||kotak.height<30) continue;
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			//if(show)drawContours( frames, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			drawContours(mask, hull, -1, Scalar(255), CV_FILLED, CV_AA);
		}
}
void GTgoal(){
	Mat frameTgoal = GetThresImage(pureframes, GOAL);
	Mat eroded	= Erosion(frameTgoal, GOAL, MORPH_RECT);
	frameGoal	= Dilation(eroded, GOAL, MORPH_RECT);
}
void GTline(){
	Mat frameTline = GetThresImage(pureframes, LINE);
	Mat eroded	= Erosion(frameTline, LINE, MORPH_RECT);
	frameLine	= Dilation(eroded, LINE, MORPH_RECT);
}

void enableofvision(){
	int temp;
	temp=dataEV;
	temp&=0b1;
	if(temp!=0)findBall=true;
	else findBall=false;
	temp=dataEV;
	temp&=0b10;
	if(temp!=0)findLine=true;
	else findLine=false;
	temp=dataEV;
	temp&=0b100;
	if(temp!=0)findObs=true;
	else findObs=false;
	temp=dataEV;
	temp&=0b1000;
	if(temp!=0)findGoal=true;
	else findGoal=false;
	temp=dataEV;
	temp&=0b10000;
	if(temp!=0)findCalcF=true;
	else findCalcF=false;
	temp=dataEV;
	temp&=0b100000;
	if(temp!=0)findGoal=true;
	else findGoal=false;
	temp=dataEV;
	temp&=64;/*
	if(temp!=0)findPOSY=true;
	else findPOSY=false;
	temp=dataEV;
	temp&=128;
	if(temp!=0)findPOSXka=true;
	else findPOSXka=false;
	temp=dataEV;
	temp&=256;
	if(temp!=0)findPOSXki=true;
	else findPOSXki=false;*/
}
void *capthread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	capture >> preframes;
	//GaussianBlur( preframes, preframes, Size( 7, 7 ), 0, 0 );
	if(preframes.empty()){
		fprintf(stderr,"Frame null!\n");
		fprintf(stderr,"Cek kabel kamera!!\n");
		EXITNOW=true;
	}/*
	gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedcap = %f\n", elapsed);*/
	return 0;
}
void *ballthread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	//locateBall();
	//if( !ball_cascade.load( ball_cascade_name ) ){ printf("--(!)Error loading ball cascade\n");};
	detectAndDisplay(frames);
	//writeBall(XB,YB);
	if(cetak)fprintf(stderr,"XB : %d  YB : %d\n", ball[i].x, ball[i].y);
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *linethread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	locateLine();
	//writeLine(LE);
	if(cetak)fprintf(stderr,"LE : %d\n", LE);
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *goalthread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	locateGoal();
	//writeGoal(XG,YG,TG,JG);
	if(cetak)fprintf(stderr,"XG : %d  YG : %d TG : %d\n", XG, YG, TG);
	/*
	gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	//fprintf(stderr,"elapsedgoal = %f\n", elapsed);*/
	return 0;
}
void *obsthread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	locateObs();
	//writeObs(dataObs[0],dataObs[1],dataObs[2]);
	if(cetak)fprintf(stderr,"OBS1 : %d  OBS2 : %d OBS3 : %d\n", dataObs[0],dataObs[1],dataObs[2]);
	/*
	gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	//fprintf(stderr,"elapsedgoal = %f\n", elapsed);*/
	return 0;
}
void *calcFthread(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	calculateField();
	/*
	gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	//fprintf(stderr,"elapsedgoal = %f\n", elapsed);*/
	return 0;
}
void *getthresball(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	GTball();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *getthresfield(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	GTfield();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *getthresline(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	GTline();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *getthresgoal(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	GTgoal();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *getframes(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	frames=preframes.clone();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}
void *getpureframes(void *args){
	/*struct timeval t1,t2;
	double elapsed;
	gettimeofday(&t1,NULL);*/
	pureframes=preframes.clone();
	//cobacek();
	//fprintf(stderr,"XB : %d  YB : %d\n", XB,YB);/*
	/*gettimeofday(&t2,NULL);
	elapsed=(t2.tv_sec - t1.tv_sec)*1000;
	elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
	fprintf(stderr,"elapsedball = %f\n", elapsed);*/
	return 0;
}

void handler(int s){
	cout << "SIG " << s << endl;;
	EXITNOW=true;
}

int main(int argc,char* args[]) {

	ros::init(argc, args, "vision");
	ros::NodeHandle n;
	ros::Subscriber recv = n.subscribe("it_vision", 100, ItVisionCallback);
	ros::Publisher vision_it = n.advertise<std_msgs::Int32MultiArray>("vision_it", 100);


	if(argc>1){
		for(int k=1;k<argc;k++){
			if(strcmp(args[k], "elapsed")==0){
				selapsed=true;
			}
			else if(strcmp(args[k], "show")==0){
				show=true;debb=true;
			}
			else if(strcmp(args[k], "goal")==0){
				sgoal=true;debb=true;
			}
			else if(strcmp(args[k], "ball")==0){
				sball=true;debb=true;
			}
			else if(strcmp(args[k], "obs")==0){
				sobs=true;debb=true;
			}
			else if(strcmp(args[k], "line")==0){
				sline=true;debb=true;
			}
			else if(strcmp(args[k], "calcf")==0){
				scalcf=true;debb=true;
			}
			else if(strcmp(args[k], "hog")==0){
				shog=true;debb=true;
			}
			else if(strcmp(args[k], "grid")==0){
				sgrid=true;debb=true;
			}
			else if(strcmp(args[k], "cetak")==0){
				cetak=true;
			}
			else {
				fprintf(stderr,"unknown argument:%s maksudmu piye?\n",args[k]);
				return 0;
			}
		}
	}/*
	if(show==true&&sball==false&&sgoal==false&&sobs==false&&sline==false){
		fprintf(stderr,"show what? currently only ball, goal, obs are accepted\n");
		return 1;
	}*/

	// variable penunjang thread
	int ctr,btr;
	void *status;
	pthread_t trGTball,trGTfield,trGTgoal,trGTline;
	pthread_t trBall,trGoal,trObs,trCap,trCalcF,trLine;
	pthread_t copytoframes,copytopureframes;

	// inisialisasi ukuran kamera
	capture.set(CV_CAP_PROP_FRAME_WIDTH,lebar_frame);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, tinggi_frame);
	capture.set(CAP_PROP_FPS, 30);
	// inisialisasi kalibrasi
	initColors();
	impDataJ();
	if(!impDatakpl()){
		fprintf(stderr,"import data kepala GAGAL! cek file\n");
		return 0;
	}
	else {
		fprintf(stderr,"import data kepala berhasil! \n");
	}
	suduteksekusi=SudutBolaY(sdtexcute);
	sudutcek=SudutBolaY(sdtdefy);
	fprintf(stderr,"batas bola dekat %d\n",(suduteksekusi+5));
	fprintf(stderr,"bats bola sedang %d\n",(sudutcek-10));
	// HOG+SVM preparation ---------------------------------------------------------------------------------------
	//if(shog)svm = SVM::load<SVM>("/home/ilmnss/Documents/CobaV/trainedSVM.xml");

	// rasio variabel global thd frame
	tembak=tinggi_frame/7;
	macrotinggiframe=tinggi_frame/70;//33;
	sepertigaframe=lebar_frame/3;sepersepuluhframe=lebar_frame/10;sepertigaframet=tinggi_frame/3;
	luasminlap=(tinggi_frame*lebar_frame)/57;
	luasnorm=(tinggi_frame*lebar_frame)/70;luasabnorm=(tinggi_frame*lebar_frame)/52;
	minlinelong=lebar_frame/2;
	minBall=(tinggi_frame*lebar_frame)/500;
	// pewaktu (checking performance)
	struct timeval t1,t2;
	double elapsed;
	//IPC initialization
	//readIPC(0);
	//initIPC();
	// starting first frame
	ctr=pthread_create(&trCap,NULL,capthread,NULL);
	if(ctr){
		fprintf(stderr,"ERRORRR create ctr= %d\n", ctr);
		return 1;
	}
	ctr=pthread_join(trCap, &status);
	if(ctr){
		fprintf(stderr,"ERRORRR create join ctr= %d\n", ctr);
		return 1;
	}
	if( !jauh_cascade.load( jauh_cascade_name ) ){ printf("--(!)Error loading jauh cascade\n");};
	if( !sedang_cascade.load( sedang_cascade_name ) ){ printf("--(!)Error loading sedang cascade\n");};
	if( !dekat_cascade.load( dekat_cascade_name ) ){ printf("--(!)Error loading dekat cascade\n");};
	while(ros::ok()){
		if(selapsed)gettimeofday(&t1,NULL);
		signal(SIGTERM, handler);
		signal(SIGINT, handler);
		//readIPC(1);
		if(debb){
			dataEV=0;
			if(sball)dataEV|=0b1;
			if(sgoal)dataEV|=0b100000;
			if(scalcf)dataEV|=0b10000;
			if(sobs)dataEV|=0b100;
			if(sline)dataEV|=0b10;
		}/**/
		//dataEV=0;
		enableofvision();
		//===============================copying frames to be processed====================================
		ctr=pthread_create(&copytoframes,NULL,getframes,NULL);
		if(ctr){
			fprintf(stderr,"ERRORRR create ctr= %d\n", ctr);
			return 0;
		}
		ctr=pthread_create(&copytopureframes,NULL,getpureframes,NULL);
		if(ctr){
			fprintf(stderr,"ERRORRR create ctr= %d\n", ctr);
			return 0;
		}
		ctr=pthread_join(copytoframes, &status);
		if(ctr){
			fprintf(stderr,"ERRORRR create join ctr= %d\n", ctr);
			return 0;
		}
		ctr=pthread_join(copytopureframes, &status);
		if(ctr){
			fprintf(stderr,"ERRORRR create join ctr= %d\n", ctr);
			return 0;
		}
		//===============================start thread to take next frame====================================
		ctr=pthread_create(&trCap,NULL,capthread,NULL);
		if(ctr){
			fprintf(stderr,"ERRORRR create ctr= %d\n", ctr);
			return 1;
		}
		//===============================start getting thres image====================================
		if(findBall||findObs||findGoal||findLine||findCalcF){
			btr=pthread_create(&trGTfield,NULL,getthresfield,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findBall||findObs){
			btr=pthread_create(&trGTball,NULL,getthresball,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findGoal||findObs){
			btr=pthread_create(&trGTgoal,NULL,getthresgoal,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findLine||findObs){
			btr=pthread_create(&trGTline,NULL,getthresline,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		//===============================waiting thres image to complete====================================
		if(findBall||findObs||findGoal||findLine||findCalcF){
			btr=pthread_join(trGTfield, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;
			}
		}
		if(findBall||findObs){
			btr=pthread_join(trGTball, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;
			}
		}
		if(findGoal||findObs){
			btr=pthread_join(trGTgoal, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findLine||findObs){
				btr=pthread_join(trGTline,&status);
				if(btr){
					fprintf(stderr,"ERRORRR create btr = %d\n", btr);
					return 1;
				}
			}
		//====================================   ANALYZING    =========================================
		if(findCalcF){
			btr=pthread_create(&trCalcF,NULL,calcFthread,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findBall){
			btr=pthread_create(&trBall,NULL,ballthread,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findGoal){
			btr=pthread_create(&trGoal,NULL,goalthread,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findObs){
			btr=pthread_create(&trObs,NULL,obsthread,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		if(findLine){
			btr=pthread_create(&trLine,NULL,linethread,NULL);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		//===============================  waiting ANALYZER to complete  ====================================
		if(findCalcF){
			btr=pthread_join(trCalcF, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;fprintf(stderr,"stuck 3");
			}
		}
		if(findBall){
			btr=pthread_join(trBall, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;fprintf(stderr,"stuck 3");
			}
		}
		if(findGoal){
			btr=pthread_join(trGoal, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;fprintf(stderr,"stuck 3");
			}
		}
		if(findObs){
			btr=pthread_join(trObs, &status);
			if(btr){
				fprintf(stderr,"ERRORRR create join btr= %d\n", btr);
				return 1;fprintf(stderr,"stuck 3");
			}
		}
		if(findLine){
			btr=pthread_join(trLine,&status);
			if(btr){
				fprintf(stderr,"ERRORRR create btr = %d\n", btr);
				return 1;
			}
		}
		//===============================  waiting grabed next image to complete  ====================================
		ctr=pthread_join(trCap, &status);
		if(ctr){
			fprintf(stderr,"ERRORRR create join ctr= %d\n", ctr);
			return 1;
		}
		//===============================  DEBUGGING variables  ====================================
		if(show&&!sball&&!sgoal)if(waitKey(10)=='q')EXITNOW=true;
		if(show&&sball){
			char c=waitKey(10);
			switch (c)
			{
				case 'q': EXITNOW=true; break;
				case '1':
					distBall=1;
					cout<<"Bola Dekat  "<<distBall<<endl;
					break;
				case '2':
					distBall=2;
					cout<<"Bola sedang  "<<distBall<<endl;
					break;
				case '3':
					distBall=3;
					cout<<"Bola jauh  "<<distBall<<endl;
					break;

			}
		}
		if(show&&sgoal){
			char c=waitKey(10);
			switch (c)
			{
				case 'q': EXITNOW=true; break;
				case '1':
					flagLH=1;
					cout<<"Lapangan dekat  "<<flagLH<<endl;
					break;
				case '2':
					flagLH=2;
					cout<<"lapangan jauh  "<<flagLH<<endl;
					break;
			}
		}
		if(selapsed){
			if(findBall==true)fprintf(stderr,"fb ");
			if(findGoal==true)fprintf(stderr,"fg ");
			if(findObs==true)fprintf(stderr,"fo ");
			if(findLine==true)fprintf(stderr,"fl ");
			if(findCalcF==true)fprintf(stderr,"fc ");
			if(newgoal==true)fprintf(stderr,"fng ");
			gettimeofday(&t2,NULL);
			elapsed=(t2.tv_sec - t1.tv_sec)*1000;
			elapsed+=(t2.tv_usec - t1.tv_usec)/1000;
			fprintf(stderr,"elapsed of vision = %f\n", elapsed);
		}
		if(EXITNOW){
			//fprintf(stderr,"BERHASIL LORISIMO\n");
			break;
		}
        //detectAndDisplay(frames);
		VisionPublish();
		vision_it.publish(dta_vision_it);
		ros::spinOnce();
		/*free(capthread);
		free(ballthread);
		free(linethread);
		free(goalthread);
		free(obsthread);
		free(calcFthread);
		free(getthresball);
		free(getthresfield);
		free(getthresgoal);
		free(getthresline);
		free(getframes);
		free(getpureframes);*/
	}

  return 0;
}
/************* Allahumma yasir walaa tu'assir *************/
