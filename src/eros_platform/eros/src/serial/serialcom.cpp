//============================================================================
// Name        : serialcom.cpp
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <math.h>

#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"
//#include <sys/time.h> //Timelapsed stamp header, uncomment if you want to use this

#define round(a)(int)(a+0.5)
#define MAX_SERIAL_IT 4
#define MAX_IT_SERIAL 4
#define MAX_FROM_SUBCONTROLLER 7
using namespace std;

#define R2D M_PI/180 //Nilai PHI dalam bentuk Derajat (Radian)

int fdGrk = 0;
int dtaToSerial[MAX_IT_SERIAL],dtaReset; //Data matang hasil pembacaan IPC (xHead,yHead,Motion,Mode)
unsigned char Heading,FlagOdo,last_FlagOdo,langkahkaki,Fall,_X1,_X2,_Z1,_Z2,_H1,_H2,_M;	//Data matang yang akan dikirim ke IPC
float XParam,ZParam;
unsigned char serialIn[MAX_FROM_SUBCONTROLLER];											//RAW data from SubController
int OdoX,OdoZ,OdoH,OdoM;
char USBTX[100],system_check[100];
std_msgs::Int32MultiArray dta_serial_it;
std_msgs::Int32MultiArray dta_serial_local;
float theta,last_theta = 0;	
float xpos,ypos,xpos_skr,ypos_skr;
int ct=0;							//Uncomment this for Odometry
//bool flaggettime;													//Uncomment this for Time Lapsed Stamp 

double cosd(double x){
	return cos(R2D*x);
}

double sind(double x){
	return sin(R2D*x);
}

int configure_port(int fd){
	struct termios port_settings;

	cfsetispeed(&port_settings, B57600);    // set baud rates
	cfsetospeed(&port_settings, B57600);

	memset(&port_settings,0,sizeof(port_settings));
	port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
	port_settings.c_cflag &= ~CSTOPB;
	port_settings.c_cflag &= ~CSIZE;
	port_settings.c_cflag |= CS8;

	port_settings.c_cc[4]=0;
	tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port

	return(fd);
}

bool impUSBTX(){
	char file[]={"/home/eros/param/USBTX.eros"};
	ifstream buka(file);
	string line;
	if (buka.is_open()){
		if(buka.good()){
			char buffer[50]={};
			getline(buka,line);
			for(int i=0; i<line.length(); i++){
				buffer[i]=line[i];
			}
			memcpy(USBTX,buffer,sizeof(USBTX));
			printf("iki>>%s--\n\n",USBTX);
		}
		buka.close();
		return 1;
	} else return 0;
}

int kalman(int data){
	static float R, Xp, Pp, K, Q, X, P=1;
	static int lastdata, error;

	R = 0.000016;
	Q = 0.00001;

	Xp = X;
	Pp = P + Q;
	K = (Pp/(Pp + R));
	X = (Xp + (K*(data - Xp)));
	P = (1 - K)*Pp;

	error = data - lastdata;

	if(error < 0)error *=-1;
	if(error > 200)X = data;

	lastdata = data;

	return X;
}
int data[4];
int data_local[4];
void parsing(){
	data[0]=serialIn[0];								//Flag Jatuh
	FlagOdo=serialIn[1];
	OdoM=serialIn[6];								//OdoM
	if(OdoM==19) {OdoX=(serialIn[2]-128)*2;}					//Flag Odometry Kaki	
	else {OdoX=(serialIn[2]-128)*XParam;}						//OdoX
	OdoZ=(serialIn[3]-128)*ZParam/2;							//OdoZ
	OdoH=((-1)*serialIn[4])+serialIn[5];						//OdoH
	data[1]=OdoH; //Heading sementara
	//printf("FJ[%2d]",data[0]);
	//printf("DH[%2d]",data[1]);
	/*printf("FO[%d]",serialIn[2]);
	printf("X[%3d]",OdoX);
	printf("Z[%3d]",OdoZ);
	printf("H[%2d]",OdoH);
	printf("M[%2d]",OdoM);
	printf("c[%2d]",ct);
	printf("XP[%3.2f]",xpos);
	printf("YP[%3.2f]",ypos);
	printf("XP[%4.2f]",xpos_skr);
	printf("YP[%4.2f]\n",ypos_skr);*/
	data[2]=round(xpos_skr);
	data[3]=round(ypos_skr);

}

void odometry()
{
	//if(OdoM == 11 || OdoM == 25) {OdoX = OdoZ = 0; OdoZ*= -1;}
	//if(OdoM == 12 || OdoM == 26) {OdoX = OdoZ = 0;}
	if(OdoM == 13 || OdoM == 15 || OdoM == 14 || OdoM == 16){OdoX = 0;}
	if(OdoM == 23 || OdoM == 24 || OdoM == 33 || OdoM == 34){}
	if(OdoM==10 || OdoM == 20 || OdoM == 30 || 
		 OdoM == 40 || OdoM==50 || OdoM==19 || OdoM==29){ OdoZ = 0;}


	if(FlagOdo == 2 && (last_FlagOdo == 0 || last_FlagOdo == 4)){
		langkahkaki = 1;
	}
	else if(FlagOdo == 4 && (last_FlagOdo == 0 || last_FlagOdo == 2)){
		langkahkaki = 2;
	}
	else langkahkaki = 0;

	if((langkahkaki == 1 || langkahkaki == 2) && OdoM!= 10){
		theta = OdoH;
		ypos = OdoX * cosd(theta) - OdoZ * sind(theta);
		xpos = OdoX * sind(theta) + OdoZ * cosd(theta);

		ypos_skr +=  ypos;
		xpos_skr +=  xpos;
		if(xpos != 0 || ypos != 0){
			ct++;
		}
		//printf("masuk");
		//printf("Langkahodo = %d\n",FlagOdo);
		//printf("OdoX = %f\t OdoZ = %f\t OdoH = %f\n",OdoX,OdoZ,OdoH);
	}

	last_FlagOdo =  FlagOdo;
	
}

void *serialPublisher(void *obj){

	sprintf(system_check,"stty -F %s 57600",USBTX);
	printf("%s",system_check);
	close(fdGrk);
	system(system_check);
	system(system_check);
	system(system_check);	
	
	close(fdGrk);	//Close serial communication, to ensure that serial is closed before we open
	fdGrk = open(USBTX, O_RDWR | O_NOCTTY | O_NONBLOCK);
	configure_port(fdGrk);
	unsigned char it=0,ready,cM,ceksum,ceksum2;
	unsigned char buff[24],count;
	while(1){
		unsigned char kdata[2];
		int k = 0;
		try{
			k= read(fdGrk, kdata, 1);
		}
		catch(exception& e){
			close(fdGrk);
			fdGrk = open(USBTX, O_RDWR | O_NOCTTY | O_NONBLOCK);
			configure_port(fdGrk);
			k= read(fdGrk, kdata, 1);
			printf("There is something error \n");
		}

		/** Read Serial Data from Sub Controller ****************************/
		if (k > 0){
			if(kdata[0]==0xFF && it!=10){
				ready=1; it=0;
			}else if(ready==1){
				if(kdata[0]==0xFC){
					ready=2;it=0;
					ceksum=0;
				}else ready=0;
			}else if(ready==2){
				buff[it]=kdata[0];
				if(it>=MAX_FROM_SUBCONTROLLER){
					ceksum2=~ceksum;
					//if(ceksum2==kdata[0]){
						for(cM=0;cM<it;cM++){
							serialIn[cM]=buff[cM];
						}
						parsing();
						odometry();
						usleep(20000);
						it=ready=ceksum=0;
					//}
				}
				ceksum+=kdata[0];
				it++;
			}else{
				it=ready=0;
			}
		}
	}	
}

void ItSerialCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
  	int i=0;
  	for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
  		dtaToSerial[i]=*it;
  		i++;
  	}
  	dtaReset=dtaToSerial[0];
	//ROS_INFO("I heard: [%d]", dtaReset);
}

void IntelligentPublish(){
	int i;
	dta_serial_it.data.clear();	
	for(i=0; i<MAX_SERIAL_IT;i++){
		dta_serial_it.data.push_back(data[i]);
	}
}

int main(int argc, char **argv){
	//struct timeval t1, t2;  	//Time Lapsed Stamp Variable, uncomment if you want to use this
	//double elapsedTime;		//Time Lapsed Stamp Variable, uncomment if you want to use this
	
	cout << "Bismillah" << endl;
	cout << "SerialComm OK" << endl;
	
	if(impUSBTX() == 1){
		fprintf(stderr,".. USBTX.eros Loaded..\n");
	}

	ros::init(argc, argv, "serialcom");

  	ros::NodeHandle n;
	ros::Publisher serial_it = n.advertise<std_msgs::Int32MultiArray>("serial_it", 100);
	ros::Publisher serial_local = n.advertise<std_msgs::Int32MultiArray>("serial_local", 100);
	ros::Subscriber recv = n.subscribe("it_serial", 100, ItSerialCallback);
	ros::Rate rate(20);
	n.getParam("/serialcom/XParam",XParam);
	n.getParam("/serialcom/ZParam",ZParam);
	pthread_t thread1;
	pthread_create(&thread1,NULL,serialPublisher,NULL);
	while(ros::ok()){
		if(dtaReset==1){
			xpos_skr=0;
			ypos_skr=0;
		}
		IntelligentPublish();
		serial_it.publish(dta_serial_it);
    	ROS_INFO("Publishing to Intel.. [%d][%d][%d][%d]",data[0],data[1],data[2],data[3]);
		rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
