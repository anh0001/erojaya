//============================================================================
// Name        : referee.cpp
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN
// Description : We Must Win This Competition, No Matter What !!
//============================================================================


#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <time.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <fstream>

#include "ros/ros.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/MultiArrayLayout.h"

#define MAX_REFEREE_IT 3
using namespace std;

// Define Robocup Game Control Data 2017
//#define SRV_IP "192.168.1.255"
#define SRV_IP "192.168.1.255" //from CORE_iBOT Networks

#define PORT_REC 	3838
#define PORT_SEND 	3939

#define iTeam 		5015
#define iGCol 		5016
#define iTask		5017
#define i2nd		5018

#define dtComm		5035
#define dtapenalty 	5036

#define OnOff		5050

#define MAX_NUM_PLAYERS             11
#define SPL_COACH_MESSAGE_SIZE      253


// Humanoid League Define
#define TEAM_CYAN                   0
#define TEAM_MAGENTA                1
#define DROPBALL                    255

#define GAME_ROUNDROBIN             0
#define GAME_PLAYOFF                1
#define GAME_DROPIN                 2

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

#define PENALTY_NONE                        0

#define UNKNOWN                             255
#define NONE                                0
#define SUBSTITUTE                          14
#define MANUAL                              15

#define HL_BALL_MANIPULATION                30
#define HL_PHYSICAL_CONTACT                 31
#define HL_ILLEGAL_ATTACK                   32
#define HL_ILLEGAL_DEFENSE                  33
#define HL_PICKUP_OR_INCAPABLE              34
#define HL_SERVICE                          35


struct RobotInfo
{
  uint8_t penalty;              // penalty state of the player
  uint8_t secsTillUnpenalised;  // estimate of time till unpenalised
  uint8_t yellowCardCount;  // number of yellow cards
  uint8_t redCardCount;  // number of red cards
};

struct TeamInfo
{
  uint8_t teamNumber;           // unique team number
  uint8_t teamColour;           // colour of the team
  uint8_t score;                // team's score
  uint8_t penaltyShot;          // penalty shot counter
  uint16_t singleShots;         // bits represent penalty shot success
  uint8_t coachSequence;        // sequence number of the coach's message
  uint8_t coachMessage[SPL_COACH_MESSAGE_SIZE]; // the coach's message to the team
  RobotInfo coach;
  RobotInfo players[MAX_NUM_PLAYERS]; // the team's players
};

struct RoboCupGameControlData
{
  char header[4];               // header to identify the structure
  uint16_t version;             // version of the data structure
  uint8_t packetNumber;         // number incremented with each packet sent (with wraparound)
  uint8_t playersPerTeam;       // the number of players on a team
  uint8_t gameType;             // type of the game (GAME_ROUNDROBIN, GAME_PLAYOFF, GAME_DROPIN)
  uint8_t state;                // state of the game (STATE_READY, STATE_PLAYING, etc)
  uint8_t firstHalf;            // 1 = game in first half, 0 otherwise
  uint8_t kickOffTeam;          // the team number of the next team to kick off or DROPBALL
  uint8_t secondaryState;       // extra state information - (STATE2_NORMAL, STATE2_PENALTYSHOOT, etc)
  char secondaryStateInfo[4];   // Extra info on the secondary state
  uint8_t dropInTeam;           // number of team that caused last drop in
  uint16_t dropInTime;          // number of seconds passed since the last drop in. -1 (0xffff) before first dropin
  uint16_t secsRemaining;       // estimate of number of seconds remaining in the half
  uint16_t secondaryTime;       // number of seconds shown as secondary time (remaining ready, until free ball, etc)
  TeamInfo teams[2];
};

// data structure header
#define GAMECONTROLLER_RETURN_STRUCT_HEADER      "RGrt"
#define GAMECONTROLLER_RETURN_STRUCT_VERSION     2

#define GAMECONTROLLER_RETURN_MSG_MAN_PENALISE   0
#define GAMECONTROLLER_RETURN_MSG_MAN_UNPENALISE 1
#define GAMECONTROLLER_RETURN_MSG_ALIVE          2

struct RoboCupGameControlReturnData
{
  char header[4];
  uint8_t version;
  uint8_t team;    // team number
  uint8_t player;  // player number starts with 1
  uint8_t message; // one of the three messages defined above

#ifdef __cplusplus
  // constructor
  RoboCupGameControlReturnData() : version(GAMECONTROLLER_RETURN_STRUCT_VERSION)
  {
    const char* init = GAMECONTROLLER_RETURN_STRUCT_HEADER;
    for(unsigned int i = 0; i < sizeof(header); ++i)
      header[i] = init[i];
  }
#endif
};

// End Define Robocup Game Control Data 2017

struct RoboCupGameControlData eros;
struct RoboCupGameControlReturnData e_send;

char fle[]={"/home/eros/param/profile.eros"};
int datacom;
int player=0, tim=0, warna=0, dataComm=0, stsOnOff=0, kik_of=0;
int pinalty,pinalty2;
int task1, sko=0;
unsigned char StatusE[6];
std_msgs::Int32MultiArray dta_referee_it;
ros::Publisher referee_it;

void push_data(int data1, int data2, int data3){
	dta_referee_it.data.clear();
	dta_referee_it.data.push_back(data1);
	dta_referee_it.data.push_back(data2);
	dta_referee_it.data.push_back(data3);
}
/*void writeIPC(int init, int dataTm, int dataTk, int dataGC, int pn, int data2nd){
	static int *resultTm, *resultTk, *resultGC, *resultpenalty;
	static int *resultOO, *result2nd;

	if(init == 0){
		int keyTeam = iTeam;
		int keyTask = iTask;
		int keyGCol = iGCol;
		int keypenalty = dtapenalty;
		int key2nd = i2nd;

		int spaceIdTm = shmget(keyTeam, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultTm = (int*)shmat(spaceIdTm, NULL, 0);

		int spaceId2nd = shmget(key2nd, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		result2nd = (int*)shmat(spaceId2nd, NULL, 0);

		int spaceIdTk = shmget(keyTask, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultTk = (int*)shmat(spaceIdTk, NULL, 0);

		int spaceIdGC = shmget(keyGCol, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultGC = (int*)shmat(spaceIdGC, NULL, 0);

		int spaceIdpenalty = shmget(keypenalty, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultpenalty = (int*)shmat(spaceIdpenalty, NULL, 0);

		int keyOO = OnOff;
		int spaceIdOO = shmget(keyOO, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
		resultOO = (int*)shmat(spaceIdOO, NULL, 0);

	}
	else {
		*result2nd= data2nd;
		*resultTm = dataTm;
		*resultTk = dataTk;
		*resultGC = dataGC;
		*resultpenalty = pn;
		*resultOO = stsOnOff;
	}
	printf("1st: %2d | 2nd: %2d | ",dataTk,data2nd);
	printf("Pn = %2d\n",pn);
}
*/

void diep(char *s){
	perror(s);
	datacom = 0;
	push_data(0,0,0);
  	referee_it.publish(dta_referee_it);
  	ROS_INFO("Publish Diep..[%d][%d][%d]",dta_referee_it.data[0],dta_referee_it.data[1],dta_referee_it.data[2]);
	//writeIPC(0,0,0,0,0,0);
	exit(1);
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

void terima(){
	struct sockaddr_in si_me, si_other;
	int listen_sd, max_sd, new_sd, on=1;
	int s, i, slen = sizeof(si_other);
	signal(SIGALRM, watchdog);
	alarm(5);

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	diep((char *)"socket");

	int broadcastEnable=1;
	int ret=setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT_REC);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1){
		diep((char *)"bind");
		close(s);
		return;
 	}
	if (recvfrom(s,(struct sockaddr *) &eros, sizeof (eros), 0, (struct sockaddr*) &si_other, (unsigned int*) &slen)==-1)
 	diep((char *)"recvfrom()");
	inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port);
	//system("clear");
	//printf("EROS RefereeBox 2017\n");
	//printf("Receiving Data..\n");
	close(s);
}

void kirim(){
	int on=1;
	e_send.team 		= tim;
	e_send.player 		= player+1;
	e_send.message 		= GAMECONTROLLER_RETURN_MSG_ALIVE;

	struct sockaddr_in si_other1;
	int s1, slen1=sizeof(si_other1);
	//int s, i, slen=sizeof(si_other);
	//char buf[BUFLEN];
	if ((s1=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	diep((char *)"socket");
	int broadcastEnable=1;
	int ret1=setsockopt(s1, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	ret1 = ioctl(s1, FIONBIO, (char *)&on);
	if (ret1 < 0){
		perror("ioctl() failed");
		close(s1);
		//exit(-1);
		}

	memset((char *) &si_other1, 0, sizeof(si_other1));
	si_other1.sin_family = AF_INET;
	si_other1.sin_port = htons(PORT_SEND);

	if (inet_aton(SRV_IP, &si_other1.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		//exit(1);
		}

		if (sendto(s1, (struct sockaddr *) &e_send, sizeof (e_send), 0, (struct sockaddr *) &si_other1, slen1)==-1)
		//ifsendto(sd, (char *) (send_TargetCellConfiguration_str), sizeof(tTargetCellConfiguration), 0, (struct sockaddr *)&server, slen)
		diep((char *)"sendto()");
		close(s1);

		ROS_INFO("Mengirim Paket..[%s][%d][%d][%d][%d]",e_send.header,e_send.version,e_send.team,e_send.player,e_send.message);
		//printf("Mengirim Package Player : %d>>\n",player);
		//printf("dC : %d, dJ : %d, dataYK : %d, aR : %d, aL : %d\n",e.dCom,e.datajob, e.dataYK, e.dataarah, e.dataarahkepala);

}

int main(int argc, char **argv){

	ros::init(argc, argv, "referee");
	ros::NodeHandle n;
	//ros::Subscriber recv = n.subscribe("it_serial", 100, ItSerialCallback);
	referee_it = n.advertise<std_msgs::Int32MultiArray>("referee_it", 100);

	int msk;
	int myTim=0, myTask=0, MyColour=0, step=0, gCol=0;
	int sCondary=0, myTask2=0;
	//expData();
	n.getParam("team",tim);
	n.getParam("player",player);

	ROS_INFO("TIM: %d | PLAYER: %d",tim,player);

	player--;
	ros::Rate rate(10);
	while(ros::ok()){
		terima();
		kirim();
		if(eros.teams[0].teamNumber==tim){myTim = 0; msk=1;}
		else if(eros.teams[1].teamNumber == tim){myTim = 1; msk=1;}
		else msk=0;
		//printf("1st: %d | 2nd: %d\n",tim,eros.teams[1].teamNumber);
		if(msk==1){
			//system("clear");
			sCondary=(int)eros.secondaryStateInfo[0];
			//printf("1st State: %d | 2nd State: %d | 2nd Info: %d\n",eros.state,eros.secondaryState,sCondary);
			//printf("KickOff: ");
			//if(eros.kickOffTeam==tim)printf("EROS");else printf("Enemy");
			//printf(" | Color: ");
			//if(eros.teams[myTim].teamColour==TEAM_MAGENTA)printf("MAGENTA");else printf("CYAN");
			MyColour=eros.teams[myTim].teamColour;

			//printf("\n");
			/*printf("Tim 1: %d->%d | Tim 2: %d->%d | Time:%4d | Time2:%4d\n", 
					eros.teams[0].teamNumber, eros.teams[0].score, 
					eros.teams[1].teamNumber, eros.teams[1].score, 
					eros.secsRemaining,eros.secondaryTime);
			*/
			stsOnOff=1;
			
			/* Untuk encode semua yang pinalty atau off */
			pinalty = 0;
			int mul=1;
			for(int i=0;i<6;i++){
				if(eros.teams[myTim].players[i].penalty > 0) pinalty += mul;
				mul*=2;
			}

			/* Untuk decode semua yang pinalty atau off */
			pinalty2=pinalty;
			//printf("Status: ");
			for(int i=0;i<6;i++){
				StatusE[i] = pinalty2 % 2;
				pinalty2 /= 2;
				//printf("| #%d: %d ",i+1,StatusE[i]);
			}
			//printf("\n");

			/* Penalty State */		
			/*printf("PState: | #1:%2d | #2:%2d | #3:%2d | #4:%2d | #5:%2d | #6:%2d\n",
				eros.teams[myTim].players[0].penalty,
				eros.teams[myTim].players[1].penalty,
				eros.teams[myTim].players[2].penalty,
				eros.teams[myTim].players[3].penalty,
				eros.teams[myTim].players[4].penalty,
				eros.teams[myTim].players[5].penalty);		
			*/
			/* Time Remaining Penalty */
			/*printf("Time R: | #1:%2d | #2:%2d | #3:%2d | #4:%2d | #5:%2d | #6:%2d\n",
				eros.teams[myTim].players[0].secsTillUnpenalised,
				eros.teams[myTim].players[1].secsTillUnpenalised,
				eros.teams[myTim].players[2].secsTillUnpenalised,
				eros.teams[myTim].players[3].secsTillUnpenalised,
				eros.teams[myTim].players[4].secsTillUnpenalised,
				eros.teams[myTim].players[5].secsTillUnpenalised);
			*/
			//printf("MyColour=%d myTask=%d, gCol=%d, pinalty=%d\n",MyColour,myTask,gCol,pinalty);
			//printf("\n\nState Condition:\n");
			//printf("EROS KO: |I: 1 |R: 5 |S: 9  |P:   3 |F: 4 |\n");
			//printf("EnemyKO: |I: 1 |R: 6 |S: 10 |P: 2-3 |F: 4 |\n");
			
			//printf("__________________________\n");

			/* 2nd State Tasking, 2nd State just for STATE_PLAYING*/
				if(eros.state == STATE_PLAYING && eros.secondaryState != 0){
					if(eros.secondaryState == STATE2_PENALTYKICK && sCondary == tim)myTask2=1;
					else if(eros.secondaryState == STATE2_PENALTYKICK && sCondary != tim)myTask2=2;
					else if(eros.secondaryState == STATE2_DIRECT_FREEKICK && sCondary == tim)myTask2=3;
					else if(eros.secondaryState == STATE2_DIRECT_FREEKICK && sCondary != tim)myTask2=4;
					else if(eros.secondaryState == STATE2_INDIRECT_FREEKICK && sCondary == tim)myTask2=5;
					else if(eros.secondaryState == STATE2_INDIRECT_FREEKICK && sCondary != tim)myTask2=6;
					else myTask2=0;
				}else myTask2=0;

			/* 1st State Tasking */
				if(eros.state == STATE_INITIAL){myTask = 1;}
				else if(eros.state == STATE_READY){
					step=0;
					if(eros.secondaryState==STATE2_PENALTYSHOOT){
						if(eros.kickOffTeam == tim) myTask = 7;
						else myTask = 8;
					}
					else {
						if(eros.kickOffTeam == tim) myTask = 5; 
						else myTask = 6;
					}
				}
				else if(eros.state == STATE_SET){
					step=0;
					if(eros.secondaryState==STATE2_PENALTYSHOOT){
						if(eros.kickOffTeam == tim) myTask = 7;
						else myTask = 8;
					}
					else {
						if(eros.kickOffTeam == tim) myTask = 9;
						else myTask = 10;
					}
				}
				else if(eros.state == STATE_PLAYING){
					if(eros.kickOffTeam == tim) myTask = 3;
					else{
						if(step==0){
							myTask=2;
							myTask2=0;
							//writeIPC(1,0,myTask,0,pinalty,0);
							if(eros.secondaryTime==0 && eros.secondaryState==0)step=1;
						}
						else if(step==1){
							//jalan....
							myTask=3;
						}
					}
				}
				else if(eros.state == STATE_FINISHED){
					step=0;
					myTask = 4;
				}

				if(eros.teams[myTim].players[player].secsTillUnpenalised!=0){
					myTask=myTask*10;
					stsOnOff=0;
				}
		}else{
			myTask=11;
			printf("EROS Not Playing NOW!\n");
		}
		

		ROS_INFO("Publish..[%d][%d][%d]",myTask,pinalty,myTask2);
		push_data(myTask,pinalty,myTask2);
		referee_it.publish(dta_referee_it);
		ros::spinOnce();
		
		alarm(3);
		signal(SIGALRM, watchdog);
		//rate.sleep();
	}
	return 0;
}

