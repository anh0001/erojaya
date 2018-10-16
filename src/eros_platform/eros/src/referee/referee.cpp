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
#define SRV_IP "172.17.0.10" //KRSBIHUMANOID2018 - UMY
//#define SRV_IP "192.168.1.153" //EROS@BAIM - JESSNOLIMIT
#define PORT_REC 	3838
#define PORT_SEND 	3838

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


struct RoboCupGameControlReturnData e_send;

int datacom;
int player=0, tim=0, warna=0, dataComm=0, stsOnOff=0, kik_of=0;
int pinalty,pinalty2;
int task1, sko=0;
unsigned char StatusE[6];

void diep(char *s){
	perror(s);
  	ROS_INFO("Publish Diep..");
	//writeIPC(0,0,0,0,0,0);
	exit(1);
}

void INThandler(int sig){
	exit(0);
}

void watchdog(int sig){
  	ROS_INFO("Publish Watchdog..");
  	//writeIPC(0,0,0,0,0,0);
  	alarm(3);
  	signal(SIGINT, INThandler);
  	ros::spinOnce();
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

	ros::init(argc, argv, "refsend");
	ros::NodeHandle n;
	
	n.getParam("team",tim);
	n.getParam("player",player);

	// tim=7;
	// player=2;
	ROS_INFO("TIM: %d | PLAYER: %d",tim,player);

	player--;
	ros::Rate rate(10);
	while(ros::ok()){
		kirim();
		ros::spinOnce();
		
		alarm(3);
		signal(SIGALRM, watchdog);
		rate.sleep();
	}
	return 0;
}

