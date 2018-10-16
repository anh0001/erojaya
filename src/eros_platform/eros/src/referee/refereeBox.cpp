//============================================================================
// Name        : refereeBox.cpp
// Author      : EROS#5
// Version     : 1.0
// Copyright   : Bismillah
// Description : Bismillah for everything
//============================================================================

#include "PracticalSocket.h" // For UDPSocket and SocketException
#include <iostream>          // For cout and cerr
#include <cstdlib>           // For atoi()
#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"

#include <fstream>

#include <iostream>
using namespace std;

#define SRV_IP "172.17.0.10" //KRSBIHUMANOID2018 - UMY
// #define SRV_IP "192.168.1.153" //EROS@BAIM - JESSNOLIMIT
#define SOCK_REC 3838

#define BIRU		0
#define MERAH		1
//-------refereeBox Define--------

int player=0, tim=0, warna=0, dataComm=0, stsOnOff=0, kik_of=0;
int pinalty,pinalty2;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

struct RobotInfo {
  uint8_t penalty;              // penalty state of the player
  uint8_t secsTillUnpenalised;  // estimate of time till unpenalised
};

struct TeamInfo{
  uint8_t teamNumber;           // unique team number
  uint8_t teamColour;           // colour of the team
  uint8_t score;                // team's score
  uint8_t penaltyShot;          // penalty shot counter
  uint16_t singleShots;         // bits represent penalty shot success
  uint8_t coachMessage[40]; // the coach's message to the team
  RobotInfo coach;
  RobotInfo players[11]; // the team's players
};

struct RoboCupGameControlData{
  char header[4];               // header to identify the structure
  uint8_t version;              // version of the data structure
  uint8_t packetNumber;         // number incremented with each packet sent (with wraparound)
  uint8_t playersPerTeam;       // The number of players on a team
  uint8_t state;                // state of the game (STATE_READY, STATE_PLAYING, etc)
  uint8_t firstHalf;            // 1 = game in first half, 0 otherwise
  uint8_t kickOffTeam;          // the next team to kick off
  uint8_t secondaryState;       // Extra state information - (STATE2_NORMAL, STATE2_PENALTYSHOOT, etc)
  uint8_t dropInTeam;           // team that caused last drop in
  uint16_t dropInTime;          // number of seconds passed since the last drop in.  -1 before first dropin
  uint16_t secsRemaining;       // estimate of number of seconds remaining in the half
  uint16_t secondaryTime;       // number of seconds shown as secondary time (remaining ready, until free ball, etc)
  TeamInfo teams[2];
};

#define STATE_INITIAL               0
#define STATE_READY                 1
#define STATE_SET                   2
#define STATE_PLAYING               3
#define STATE_FINISHED              4

#define STATE2_NORMAL               0
#define STATE2_PENALTYSHOOT         1
#define STATE2_OVERTIME             2
#define STATE2_TIMEOUT              3
#define STATE2_DIRECT_FREEKICK      4
#define STATE2_INDIRECT_FREEKICK    5
#define STATE2_PENALTYKICK          6

std_msgs::Int32MultiArray dta_referee_it;
ros::Publisher referee_it;

void push_data(int data1, int data2, int data3){
	dta_referee_it.data.clear();
	dta_referee_it.data.push_back(data1);
	dta_referee_it.data.push_back(data2);
	dta_referee_it.data.push_back(data3);
}

void INThandler(int sig){
	exit(0);
}

void watchdog(int sig){
  	push_data(0,0,0);
  	referee_it.publish(dta_referee_it);
  	ROS_INFO("Publish Watchdog..[%d][%d][%d]",dta_referee_it.data[0],dta_referee_it.data[1],dta_referee_it.data[2]);
  	//writeIPC(0,0,0,0,0,0);
  	alarm(3);
  	signal(SIGINT, INThandler);
  	ros::spinOnce();
}

int main(int argc, char **argv){
	ros::init(argc, argv, "referee");
	ros::NodeHandle n;
	//ros::Subscriber recv = n.subscribe("it_serial", 100, ItSerialCallback);
	referee_it = n.advertise<std_msgs::Int32MultiArray>("referee_it", 100);

	//expData();
	n.getParam("team",tim);
	n.getParam("player",player);

	// tim=7;
	// player=2;
	ROS_INFO("TIM: %d | PLAYER: %d",tim,player);
	int myTim=0, myTask=0, MyColour=0, step=0, gCol=0;

	RoboCupGameControlData* WData;
	RoboCupGameControlData KData;
	int task1, sko=0;

	player = player-1;

	WData = &KData;
	int dataUdp=0;
	UDPSocket sock(SOCK_REC);

	char echoBuffer[255];
	int recvMsgSize;
	string sourceAddress = SRV_IP;
	unsigned short sourcePort;
	sourcePort=3838;
	int myTim_pen;

	unsigned char StatusE[5];
	ros::Rate rate(30);
	while(ros::ok()){
		recvMsgSize = sock.recvFrom(WData, 255, sourceAddress,sourcePort);
		int msk=0;
		if(WData->teams[0].teamNumber==tim){myTim = 0; myTim_pen=0;msk=1;}
		else if(WData->teams[1].teamNumber == tim){myTim = 1; myTim_pen=1;msk=1;}


		//Switching Team for Second Half
		if(WData->firstHalf==0){
			if(myTim==1)myTim=0;
			else myTim=1;
		}



		if(msk==1){
			// system("clear");
			// printf("EROS#: Tim %d\n",myTim);
			// printf("KickOff State: ");
			// if(sko==myTim){
			// 	printf("EROS");
			// }else{
			// 	printf("Enemy");
			// }

			// printf(" | Babak Pinalty: %d \n", WData->secondaryState);
			// printf("Tim 1: %d->%d | Tim 2: %d->%d | Time Remaining:%4d\n", 
			// 		WData->teams[0].teamNumber, WData->teams[0].score, 
			// 		WData->teams[1].teamNumber, WData->teams[1].score, 
			// 		WData->secsRemaining);
			
			stsOnOff=1;
			if(WData->kickOffTeam!=2){sko=WData->kickOffTeam;}

			/* Untuk encode semua yang pinalty atau off */
			pinalty = 0;
			int mul=1;
			for(int i=0;i<5;i++){
				if(WData->teams[myTim_pen].players[i].penalty > 0) pinalty += mul;
				mul*=2;
			}

			/* Untuk decode semua yang pinalty atau off */
			// pinalty2=pinalty;
			// printf("Status: ");
			// for(int i=0;i<5;i++){
			// 	StatusE[i] = pinalty2 % 2;
			// 	pinalty2 /= 2;
			// 	printf("| EROS#%d: %d ",i+1,StatusE[i]);
			// }
			// printf("|\n");

			/* Penalty State */		
			// printf("PState: | EROS#1:%2d | EROS#2:%2d | EROS#3:%2d | EROS#4:%2d | EROS#5:%2d |\n",
			// 	WData->teams[myTim].players[0].penalty,
			// 	WData->teams[myTim].players[1].penalty,
			// 	WData->teams[myTim].players[2].penalty,
			// 	WData->teams[myTim].players[3].penalty,
			// 	WData->teams[myTim].players[4].penalty);

			/* Time Remaining Penalty */
			// printf("Time R: | EROS#1:%2d | EROS#2:%2d | EROS#3:%2d | EROS#4:%2d | EROS#5:%2d |\n",
			// 	WData->teams[myTim].players[0].secsTillUnpenalised,
			// 	WData->teams[myTim].players[1].secsTillUnpenalised,
			// 	WData->teams[myTim].players[2].secsTillUnpenalised,
			// 	WData->teams[myTim].players[3].secsTillUnpenalised,
			// 	WData->teams[myTim].players[4].secsTillUnpenalised);

			//printf("MyColour=%d myTask=%d, gCol=%d, pinalty=%d\n",MyColour,myTask,gCol,pinalty);
			//printf("\n\nState Condition:\n");
			//printf("EROS KickOff: |Initial: 1 |Ready: 5 |Set: 9  |Play: 3 	|Finish: 4 |\n");
			//printf("EnemyKickOff: |Initial: 1 |Ready: 6 |Set: 10 |Play: 2-3 |Finish: 4 |\n");
			
			if(WData->state == STATE_INITIAL){myTask = 1;}
				else if(WData->state == STATE_READY){
					step=0;
					if(WData->secondaryState==STATE2_PENALTYSHOOT){
						if(WData->kickOffTeam == myTim && myTim != WData->firstHalf) myTask = 7;
						else myTask = 8;
						if(WData->secondaryState==myTim) myTask = 7;
					}
					else {
						if(WData->kickOffTeam == myTim) myTask = 5; 
						else myTask = 6;
					}
				}
				else if(WData->state == STATE_SET){
					step=0;
					if(WData->secondaryState==STATE2_PENALTYSHOOT){
						if(WData->kickOffTeam == myTim && myTim != WData->firstHalf) myTask = 7;
						else myTask = 8;
						if(WData->secondaryState==myTim) myTask = 7;
					}
					else {
						if(WData->kickOffTeam == myTim) myTask = 9;
						else myTask = 10;
					}
				}
				else if(WData->state == STATE_PLAYING){
					if(WData->kickOffTeam == myTim) myTask = 3;
					else{
						if(step==0){
							myTask=2;
							if(WData->secondaryTime==0 && WData->secondaryState==0)step=1;
						}
						else if(step==1){
							//jalan....
							myTask=3;
						}
					}
				}
				else if(WData->state == STATE_FINISHED){
					step=0;
					myTask = 4;
				}

				if(WData->teams[myTim].players[player].secsTillUnpenalised!=0){
					myTask=myTask*10;
					stsOnOff=0;
				}
		}

		push_data(myTask,pinalty,0);
		if(myTask!=11){
			ROS_INFO("Publish..[%d][%d][%d]",myTask,pinalty,0);
			referee_it.publish(dta_referee_it);
		}

		ros::spinOnce();
		
		alarm(7);
		signal(SIGALRM, watchdog);
		rate.sleep();
	}
	return 0;
}
