//============================================================================
// Name        : serialcom_kalib.cpp
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

#include <fstream>
#include <iostream>
#include <math.h>

#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"
//#include <sys/time.h> //Timelapsed stamp header, uncomment if you want to use this

#define MAX_SERIAL_IT 7
#define MAX_IT_SERIAL 4
using namespace std;

#define R2D M_PI/180 //Nilai PHI dalam bentuk Derajat (Radian)

struct Data{
	unsigned char awal;
	unsigned char awal2;
	unsigned char mode;
	unsigned char x_data;
	unsigned char x_bit;
	unsigned char y_data;
	unsigned char y_bit;
	unsigned char gerak;
	unsigned char cek;
};

Data *pdata = (struct Data *) malloc(sizeof(*pdata));
int fdGrk = 0;
int dtaToSerial[MAX_IT_SERIAL],dtaXHead=2048,dtaYHead=2048,dtaMotion=0,dtaSMode=0; //Data matang hasil pembacaan IPC (xHead,yHead,Motion,Mode)
unsigned char serialIn[10];											//RAW data from SubController
std_msgs::Int32MultiArray dta_serial_it;
char USBRX[100],system_check[100];
//float theta,last_theta = 0;										//Uncomment this for Odometry
//bool flaggettime;													//Uncomment this for Time Lapsed Stamp 

double cosd(double x){
	return cos(R2D*x);
}

double sind(double x){
	return sin(R2D*x);
}

bool impUSBRX(){
	char file[]={"/home/eros/param/USBRX.eros"};
	ifstream buka(file);
	string line;
	if (buka.is_open()){
		if(buka.good()){
			char buffer[50]={};
			getline(buka,line);
			for(int i=0; i<line.length(); i++){
				buffer[i]=line[i];
			}
			memcpy(USBRX,buffer,sizeof(USBRX));
			printf("iki>>%s--\n\n",USBRX);
		}
		buka.close();
		return 1;
	} else return 0;
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

void ItSerialCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
  	int i=0;
  	for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
  		dtaToSerial[i]=*it;
  		i++;
  	}
  	dtaXHead=dtaToSerial[0];
  	dtaYHead=dtaToSerial[1];
		dtaMotion=dtaToSerial[2];
		dtaSMode=dtaToSerial[3];
		ROS_INFO("I heard: [%d][%d][%d][%d]", dtaXHead,dtaYHead,dtaMotion,dtaSMode);
}

int motionAct(int xServo, int yServo, int motionN, int Mode) {
	unsigned char ceksum=0;
	pdata->awal = 77;
	pdata->awal2 = 78;
	pdata->mode = Mode; ceksum+=Mode;
	pdata->x_data = xServo & 0xFF; ceksum+=(xServo & 0xFF);
	pdata->x_bit = xServo >> 8; ceksum+=(xServo >> 8);
	pdata->y_data = yServo & 0xFF; ceksum+=(yServo & 0xFF);
	pdata->y_bit = yServo >> 8; ceksum+=(yServo >> 8);
	pdata->gerak = motionN; ceksum+=motionN;
	//pdata->cek = ~ceksum;
	close(fdGrk);
	fdGrk = open(USBRX, O_RDWR | O_NOCTTY | O_NONBLOCK);
	configure_port(fdGrk);
	write(fdGrk, pdata, 8);
	return 0;
}

int main(int argc, char **argv){
	//struct timeval t1, t2;  	//Time Lapsed Stamp Variable, uncomment if you want to use this
	//double elapsedTime;		//Time Lapsed Stamp Variable, uncomment if you want to use this
	cout << "Bismillah" << endl;
	cout << "SerialComm OK" << endl;
	if(impUSBRX() == 1){
		fprintf(stderr,".. USBRX.eros Loaded..\n");
	}
	
	sprintf(system_check,"stty -F %s 57600",USBRX);
	printf("%s",system_check);
	system(system_check);
	system(system_check);
	system(system_check);

	ros::init(argc, argv, "serialcom_kalib");
	ros::NodeHandle n;
	ros::Subscriber recv = n.subscribe("it_serial", 100, ItSerialCallback);
	ros::Publisher serial_it = n.advertise<std_msgs::Int32MultiArray>("serial_it", 100);

  //ros::Rate rate(10);

	close(fdGrk);	//Close serial communication, to ensure that serial is closed before we open
	fdGrk = open(USBRX, O_RDWR | O_NOCTTY | O_NONBLOCK);
	configure_port(fdGrk);
	unsigned char it=0,ready,cM,ceksum,ceksum2;
	unsigned char buff[24],count;
	while(ros::ok()){
		//if(flaggettime == 0) { gettimeofday(&t1, NULL); flaggettime = 1;} //Uncomment this to use Time Lapsed Stamp

    //ROS_INFO("Publishing to Intel.. [%d]", count);
		motionAct(dtaXHead,dtaYHead,dtaMotion,dtaSMode);
		usleep(10000);
		ros::spinOnce();
		//rate.sleep();
		/** Time Lapsed Stamp **********************************************
		 * - uncomment if you want to use this

		gettimeofday(&t2, NULL);
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		//fprintf(stderr," Time : %f \n",elapsedTime);
		flaggettime = 0;
		*/
	}
	return 0;
}
