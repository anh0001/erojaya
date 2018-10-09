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

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"
//#include <sys/time.h> //Timelapsed stamp header, uncomment if you want to use this

#define MAX_SERIAL_IT 7

using namespace cv;
using namespace std;

#define lebar_frame 320 	//lebar frame
#define tinggi_frame 240 	//tinggi frame

//IPC IN
/*#define IPC_XHead			5009 //Nilai Servo X ID:20
#define IPC_YHead			5011 //Nilai Servo Y ID:19
#define IPC_Motion			5013 //Motion yang akan dikirim ke Serial
#define IPC_SMode			5014 //Mode yang dikirim ke serial

//IPC_OUT
#define IPC_Heading			5030 //Heading dari GY25 Serial
#define IPC_FlagOdo			5040 //Flag milik Odometri dari serial
#define IPC_OdoX 			5041 //Odometri sumbu X
#define IPC_OdoZ 			5042 //Odometri sumbu Y
#define IPC_OdoH 			5043 //Sudut yang dibentuk pergerakan kaki pada sumbu X dan Y
#define IPC_OdoM			5044 //Odometri Motion Now
#define IPC_Fall			5045 //Status Jatuh Robot
*/

char file[]={"/home/eros/param/datakepala.eros"};
int fdGrk = 0;
int dtaToSerial[4],dtaXHead,dtaYHead,dtaMotion,dtaSMode; //Data matang hasil pembacaan IPC (xHead,yHead,Motion,Mode)
//int dtaHeading,dtaFlagOdo,dtaOdoX,dtaOdoZ,dtaOdoH,dtaFall,dtaOdoM; 	//Data matang yang akan dikirim ke IPC
int ping=10000;
int defaultXS = 2100, defaultYS = 1370, sdtdefY = 2350, sdtexcute = 1640, sdtblamx = 2370, sdtblamn = 1480;
unsigned char serialIn[10];											//RAW data from SubController
//float theta,last_theta = 0;										//Uncomment this for Odometry
//bool flaggettime;													//Uncomment this for Time Lapsed Stamp 

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

void expData(){
	ofstream output (file);
	if(output.is_open()){
		cout<<"Alhamdulillah Save"<<endl;
		//ROS_INFO("Saving kalibkpl.al\t[SUCCESS]");
		output << defaultXS <<";" << defaultYS << ";" << sdtdefY << ";" << sdtexcute << ";" << sdtblamx << ";" << sdtblamn << ";" <<endl;
	}else cout<<"Gak isok bukak!"<<endl;
	output.close();
}

int SudutBolaY(int yBall){
	int sdtbolay;
	sdtbolay = (int)(((yBall-defaultYS)*90)/1025);
	return sdtbolay;
}

int SudutBolaX(int xBall){
	int sdtbolax;
	if(xBall >= defaultXS){ //-
		sdtbolax = (int)(-1*(((xBall-defaultXS)*90)/1025));
	} else if(xBall < defaultXS){ //+
		sdtbolax = (int)(((defaultXS-xBall)*90)/1025);
	}
	return sdtbolax;
}

int main(int argc, char **argv){
	//struct timeval t1, t2;  	//Time Lapsed Stamp Variable, uncomment if you want to use this
	//double elapsedTime;		//Time Lapsed Stamp Variable, uncomment if you want to use this
	cout << "Bismillah" << endl;
	cout << "Kalibrasi Kepala OK" << endl;

	int nkey;
	char key,mode;

	if(impData()==1) {
		ROS_INFO("Loading kalibkpl.al\t[SUCCESS]");
		cout << "sdtblax   : "   << defaultXS << endl;
		cout << "sdtblay   : "   << defaultYS << endl;
		cout << "sdtdefY   : "   << sdtdefY   << endl;
		cout << "sdtexcute : " << sdtexcute << endl;
		cout << "sdtblamx  : "  << sdtblamx  << endl;
		cout << "sdtblamn  : "  << sdtblamn  << endl;
  	}else
  	ROS_INFO("Loading kalibkpl.al\t[FAILED]");

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

  	//ros::Rate rate(10);

	usleep(100000);

	unsigned char it=0,ready,cM,ceksum,ceksum2;
	unsigned char buff[24],count;

	VideoCapture capture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,lebar_frame);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, tinggi_frame);

	if(!capture.isOpened()){
		cout << "cannot open camera";
	}

	while(ros::ok()){
		
		unsigned char kdata[5];
		int k = 0;

		//if(flaggettime == 0) { gettimeofday(&t1, NULL); flaggettime = 1;} //Uncomment this to use Time Lapsed Stamp
		Mat frame;
		capture >> frame;

		imshow("cam", frame);
		waitKey((30) >= 0);
		
		nkey = getchar();

		if(nkey != EOF){
			key=nkey;

			if(key == 'q' || key=='Q'){
				break;
      		}

      		if(key == '\n'){
	
			  switch(mode){
			   	   case 1 : defaultXS 	= dtaXHead; break;
			   	   case 2 : defaultYS 	= dtaYHead; break;
			   	   case 3 : sdtdefY 	= dtaYHead; break;
			   	   case 4 : sdtexcute 	= dtaYHead; break;
			   	   case 5 : sdtblamx 	= dtaYHead; break;
			   	   case 6 : sdtblamn 	= dtaYHead; break;
			  }

			  cout<<"Bismillah "<<endl; expData(); usleep(500000);

			}

			if(key == 32){ //SPACE
				cout << "XS : " << dtaXHead << " YS : " << dtaYHead << endl;
				usleep(10000);
			}

			if(key == '1'){
				mode = 1;
				dtaXHead = defaultXS;
				cout<<"Kalibrasi sdtblax..."<< endl;
				usleep(10000);
			}
			if(key == '2'){
				mode = 2;
				dtaYHead = defaultYS;
				cout<<"Kalibrasi sdtblay..."<< endl;
				usleep(10000);
			}
			if(key == '3'){
				mode = 3;
				dtaXHead = defaultXS;
				dtaYHead = sdtdefY;
				cout<<"Kalibrasi sdtdefY..."<< endl;
				usleep(10000);
			}
			if(key == '4'){
				mode = 4;
				dtaXHead = defaultXS;
				dtaYHead = sdtexcute;
				cout<<"Kalibrasi sdtexecute..."<< endl;
				usleep(10000);
			}
			if(key == '5'){
				mode = 5;
				dtaXHead = defaultXS;
				dtaYHead = sdtblamx;
				cout<<"Kalibrasi sdtblamx..."<< endl;
				usleep(10000);
			}
			if(key == '6'){
				mode = 6;
				dtaXHead = defaultXS;
				dtaYHead = sdtblamn;
				cout<<"Kalibrasi sdtblamn..."<< endl;
				usleep(10000);
			}


			if(key == '-'){
				if(mode==1){
				  dtaXHead--;
				}else{
				  dtaYHead--;
				}
				cout << "XS : " << dtaXHead << " YS : " << dtaYHead << " ("<< SudutBolaX(dtaXHead)<<")("<<SudutBolaY(dtaYHead)<<")"<<endl;
				usleep(1000);
			}

			if(key == '='){
			  if(mode==1){
				  dtaXHead++;
			  }else{
				  dtaYHead++;
			  }
			  usleep(1000);
			  cout << "XS : " << dtaXHead << " YS : " << dtaYHead << " ("<< SudutBolaX(dtaXHead)<<")("<<SudutBolaY(dtaYHead)<<")"<<endl;
			}


			if(key == 'p' || key == 'P'){
				cout << "sdtblax   : "  << defaultXS << endl;
				cout << "sdtblay   : "  << defaultYS << endl;
				cout << "sdtdefY   : "  << sdtdefY   << endl;
				cout << "sdtexcute : "  << sdtexcute << endl;
				cout << "sdtblamx  : "  << sdtblamx  << endl;
				cout << "sdtblamn  : "  << sdtblamn  << endl;
				usleep(10000);
      		}

		}

		dtaPublishSERIAL.data.clear();		    
	    dtaPublishSERIAL.data.push_back(dtaXHead);
	    dtaPublishSERIAL.data.push_back(dtaYHead);
	    dtaPublishSERIAL.data.push_back(dtaMotion);
	    dtaPublishSERIAL.data.push_back(dtaSMode);

	    it_serial.publish(dtaPublishSERIAL);
    	//ROS_INFO("Publishing to Intel.. [%d]", count);
		//rate.sleep();

		ros::spinOnce();
		/** Time Lapsed Stamp **********************************************
		 * - uncomment if you want to use this

		gettimeofday(&t2, NULL);
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		//fprintf(stderr," Time : %f \n",elapsedTime);
		flaggettime = 0;
		*/
	}
	tcsetattr(0, TCSANOW, &initial_settings);
	return(0);
}
