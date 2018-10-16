//============================================================================
// Name        : skalib.cpp
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

#include <unistd.h>

#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"
//#include <sys/time.h> //Timelapsed stamp header, uncomment if you want to use this

#define MAX_SERIAL_IT 7
using namespace std;

char file[]={"/home/eros/param/datakepala.eros"};
int fdGrk = 0;
int dtaXHead,dtaYHead,dtaMotion,dtaSMode; //Data matang hasil pembacaan IPC (xHead,yHead,Motion,Mode)
int defaultXS = 2100, defaultYS = 1370, sdtdefY = 2350, sdtexcute = 1640, sdtblamx = 2370, sdtblamn = 1480;
int xdirect=0,ydirect=0,headon=0;
int sdtxmin=600,sdtxmax=3500;

struct termios initial_settings,
               new_settings;

int impData(){
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
						case 0 : defaultXS = buf; break;
						case 1 : defaultYS = buf; break;
						case 2 : sdtdefY = buf;   break;
						case 3 : sdtexcute = buf; break;
						case 4 : sdtblamx = buf;  break;
						case 5 : sdtblamn = buf;  break;
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

void captureImage(){
	if(headon==1){
		if(xdirect==0 && ydirect==0){ //awal
			dtaYHead=sdtblamn;
			dtaXHead=defaultXS-1024;
			xdirect=1;
		}else if(xdirect==1){ //nganan
			dtaXHead+=10;
			if(dtaXHead>defaultXS+1024){
				ydirect=1;
				xdirect=2;
			}
		}else if(xdirect==2){ //ngiri
			dtaXHead-=10;
			if(dtaXHead<defaultXS-1024){
				ydirect=1;
				xdirect=1;
			}
		}

		if(ydirect==1){
			if(dtaYHead+200>sdtblamx){
				dtaYHead=sdtblamn;
			}else{
				dtaYHead+=200;
			}
			ydirect=0;
		}
		usleep(10000);
	}
}


int main(int argc, char **argv){
	//struct timeval t1, t2;  	//Time Lapsed Stamp Variable, uncomment if you want to use this
	//double elapsedTime;		//Time Lapsed Stamp Variable, uncomment if you want to use this
	cout << "Bismillah" << endl;
	cout << "Capture Image" << endl;

	int nkey;
	char key,mode;

	if(impData()==1)
			ROS_INFO("Loading kalibkpl.al\t[SUCCESS]");
  	else 	ROS_INFO("Loading kalibkpl.al\t[FAILED]");

  	dtaXHead 	= defaultXS;
  	dtaYHead 	= defaultYS;
  	dtaMotion	= 0;
  	dtaSMode	= 0; 

  	tcgetattr(0,&initial_settings);

	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 0;
	new_settings.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &new_settings);

	ros::init(argc, argv, "skalib");
	ros::NodeHandle n;

	std_msgs::Int32MultiArray dtaPublishSERIAL;
	ros::Publisher  it_serial = n.advertise<std_msgs::Int32MultiArray>("it_serial", 100);

	usleep(100000);
	while(ros::ok()){
		captureImage();
		nkey = getchar();

		if(nkey != EOF){
			key=nkey;

			if(key == 'q' || key=='Q'){
				break;
      		}

			if(key == 32){ //SPACE
				dtaMotion=10;
				usleep(10000);
			}

			if(key == '\n'){ //SPACE
				dtaMotion=0;
				usleep(10000);
			}

			if(key == 'w' || key=='W'){ //SPACE
				dtaMotion=20;
				usleep(10000);
			}

			if(key == 's' || key=='S'){ //SPACE
				dtaMotion=19;
				usleep(10000);
			}

			if(key == 'a' || key=='A'){ //SPACE
				dtaMotion=13;
				usleep(10000);
			}

			if(key == 'D' || key=='d'){ //SPACE
				dtaMotion=14;
				usleep(10000);
			}

			if(key == 'm' || key=='M'){ //SPACE
				headon=1;
				usleep(10000);
			}

			if(key == 'n' || key=='N'){ //SPACE
				headon=0;
				usleep(10000);
			}

			if(key == 'R' || key=='r'){ //SPACE
				dtaXHead=defaultXS;
				dtaYHead=defaultYS;
				usleep(10000);
			}
		}

		dtaPublishSERIAL.data.clear();		    
	    dtaPublishSERIAL.data.push_back(dtaXHead);
	    dtaPublishSERIAL.data.push_back(dtaYHead);
	    dtaPublishSERIAL.data.push_back(dtaMotion);
	    dtaPublishSERIAL.data.push_back(dtaSMode);
	    it_serial.publish(dtaPublishSERIAL);
		ros::spinOnce();
	}
	tcsetattr(0, TCSANOW, &initial_settings);
	return(0);
}
