//============================================================================
// Name        : clientrec.cpp
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

#include <vector>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"

using namespace std;

//Power Status//
#define OnOff   5050

#define SRV_IP "172.17.255.255" //from CORE_iBOT NETWORK
//#define SRV_IP "192.168.255.255"
#define BUFLEN 512
#define NPACK 10

#define PORT1 4050
#define PORT2 4051
#define PORT3 4052
#define PORT4 4053
#define PORT5 4054
#define PORT6 4055

#define MAX 60000
#define jmlEROS 6
#define KEEPER 5 //id player keeper untuk diskup komunikasi

#define MAX_ROS_SUBSCRIBE 	10
#define ROS_PUBLISH_NODE	"rec_it"
#define ROS_SUBSCRIBE_NODE	"it_rec"

//ROS Variable Declaration
int ping;
int dtaSubscribe[MAX_ROS_SUBSCRIBE];
std_msgs::Int32MultiArray dtaPublish;


//Global Variable Declaration
int datacom;

struct paket{
	int dCom;
	int datajob;
	int dataYK;
	int dataarah;
	int dataarahkepala;
	int XPOS;
	int YPOS;
	int initialPOS;
	int mainx;
	bool ball;
};

struct paket e,eros1,eros2,eros3,eros4,eros5;

int dataplayer;
int datapin;
int datapenalty;

int dCom[jmlEROS]			={0,0,0,0,0,0};
int stateEROS[jmlEROS]		={0,0,0,0,0,0};
int sudutbolaEROS[jmlEROS]	={0,0,0,0,0,0};
int dataarah[jmlEROS]		={0,0,0,0,0,0};
int dataarahkepala[jmlEROS]	={0,0,0,0,0,0};
int XPOS[jmlEROS]			={0,0,0,0,0,0};
int YPOS[jmlEROS]			={0,0,0,0,0,0};
int initialPOS[jmlEROS]		={0,0,0,0,0,0};
int mainx[jmlEROS]			={0,0,0,0,0,0};
bool ball[jmlEROS]			={0,0,0,0,0,0};

unsigned char StatusE[5];
unsigned char counter[jmlEROS]= {0,0,0,0,0,0};
bool flagerror;
bool flagonoff[jmlEROS] = {0,1,1,1,1,1};
int s,data;
int d1count,d2count,d3count,d4count,d5count;
int simpand1count,simpand2count,simpand3count,simpand4count,simpand5count;
int broadcastEnable=1;
int counterSerang;
int counterTahan;
int bandingSerang,counterSama;
//int counterserver;
//int counterbanding;

int player,tim;

void diep(char *s){
	perror(s);
	flagerror = true;
	//exit(1);
}

void IntelCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
  	int i=0;
  	for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
  		dtaSubscribe[i]=*it;
  		i++;
  	}
  	sudutbolaEROS[player]=dtaSubscribe[0];
  	stateEROS[player]=dtaSubscribe[1];
	datapenalty=dtaSubscribe[2];
	dataarah[player]=dtaSubscribe[3];
	dataarahkepala[player]=dtaSubscribe[4];
	ball[player]=dtaSubscribe[5];
	XPOS[player]=dtaSubscribe[6];
	YPOS[player]=dtaSubscribe[7];
	initialPOS[player]=dtaSubscribe[8];
	mainx[player]=dtaSubscribe[9];
	
	//ROS_INFO("I heard: [%d][%d][%d][%d][%d]", dtaSubscribe[0],dtaSubscribe[1],dtaSubscribe[2],dtaSubscribe[3],dtaSubscribe[4]);
  	ping=10000;
}

int	abs(int x){
	if(x<0) return -x;
	else return x;
}

void *terimaIPC1(void *arg){
	while(1){
		//printf("server 1 bekerja\n");
		struct sockaddr_in si_me1, si_other1;
		int s1, i1, slen1=sizeof(si_other1);
		if ((s1=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	    diep((char *)"socket");
			

		int ret1=setsockopt(s1, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
		memset((char *) &si_me1, 0, sizeof(si_me1));
		si_me1.sin_family = AF_INET;
		si_me1.sin_port = htons(PORT1);
		si_me1.sin_addr.s_addr = htonl(INADDR_ANY);
		//printf("server 1 soket sudah dibuat\n");
	
		if (bind(s1, (struct sockaddr *) &si_me1, sizeof(si_me1))==-1){
			//printf("server 1 binded\n");
			diep((char *)"bind");
			close(s1);
			//return;
		}

		if (recvfrom(s1, (struct sockaddr *) &eros1, sizeof (eros1), 0, (struct sockaddr*) &si_other1, (unsigned int*) &slen1)==-1){
    	   	diep((char *)"recvfrom()");
		}else{
			//printf("server 1 receiving\n");
			dCom[1] = eros1.dCom;
			stateEROS[1] = eros1.datajob;
			sudutbolaEROS[1] = eros1.dataYK;
			dataarah[1] = eros1.dataarah;
			dataarahkepala[1] = eros1.dataarahkepala;
			flagonoff[1] = 1;
			mainx[1] = eros1.mainx;
			ball[1] = eros1.ball;
			ROS_INFO("Receiving UDP1..[%d][%d][%d][%d][%d][%d]",eros1.dCom,eros1.dataYK,eros1.datajob,eros1.dataarah,eros1.dataarahkepala,eros1.mainx);
			d1count++;
		}
		inet_ntoa(si_other1.sin_addr), ntohs(si_other1.sin_port);
		if(d1count>200)d1count=0;
			//return 0;
		usleep(200000);
		close(s1);
	}
}

void *terimaIPC2(void *arg){
	while(1){
		//printf("server 2 bekerja\n");
		struct sockaddr_in si_me2, si_other2;
		int s2, i2, slen2=sizeof(si_other2);

		if ((s2=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep((char *)"socket");
		//printf("server 2 soket sudah dibuat\n");

		int ret2=setsockopt(s2, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
		memset((char *) &si_me2, 0, sizeof(si_me2));
		si_me2.sin_family = AF_INET;
		si_me2.sin_port = htons(PORT2);
		si_me2.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(s2, (struct sockaddr *) &si_me2, sizeof(si_me2))==-1){
			//printf("server 2 binded\n");
			diep((char *)"bind");
			close(s2);
			//return;
		}

		if (recvfrom(s2, (struct sockaddr *) &eros2, sizeof (eros2), 0, (struct sockaddr*) &si_other2, (unsigned int*) &slen2)==-1)
			diep((char *)"recvfrom()");
		else{
			//printf("server 2 receiving\n");
			dCom[2] = eros2.dCom;
			stateEROS[2] = eros2.datajob;
			sudutbolaEROS[2] = eros2.dataYK;
			dataarah[2] = eros2.dataarah;
			dataarahkepala[2] = eros2.dataarahkepala;
			flagonoff[2] = 1;
			mainx[2] = eros2.mainx;
			ball[2] = eros2.ball;
			ROS_INFO("Receiving UDP2..[%d][%d][%d][%d][%d][%d]",eros2.dCom,eros2.dataYK,eros2.datajob,eros2.dataarah,eros2.dataarahkepala,eros2.mainx);
			d2count++;
		}
		inet_ntoa(si_other2.sin_addr), ntohs(si_other2.sin_port);
		if(d2count>200)d2count=0;
		//return 0;
		usleep(200000);
		close(s2);
	}
}

void *terimaIPC3(void *arg){
	while(1){
		//printf("server 3 bekerja\n");
		struct sockaddr_in si_me3, si_other3;
		int s3, i3, slen3=sizeof(si_other3);

		if ((s3=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep((char *)"socket");
		//printf("server 3 soket dibuat\n");

		int ret3=setsockopt(s3, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
		memset((char *) &si_me3, 0, sizeof(si_me3));
		si_me3.sin_family = AF_INET;
		si_me3.sin_port = htons(PORT3);
		si_me3.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(s3, (struct sockaddr *) &si_me3, sizeof(si_me3))==-1){
			//printf("server 3 binded\n");
			diep((char *)"bind");
			close(s3);
			//return;
		}
		if (recvfrom(s3, (struct sockaddr *) &eros3, sizeof (eros3), 0, (struct sockaddr*) &si_other3, (unsigned int*) &slen3)==-1)
			diep((char *)"recvfrom()");
		else{
			dCom[3] = eros3.dCom;
			stateEROS[3] = eros3.datajob;
			sudutbolaEROS[3] = eros3.dataYK;
			dataarah[3] = eros3.dataarah;
			dataarahkepala[3] = eros3.dataarahkepala;
			flagonoff[3] = 1;
			mainx[3] = eros3.mainx;
			ball[3] = eros3.ball;
			ROS_INFO("Receiving UDP3..[%d][%d][%d][%d][%d][%d]",eros3.dCom,eros3.dataYK,eros3.datajob,eros3.dataarah,eros3.dataarahkepala,eros3.mainx);
			d3count++;
		}
		inet_ntoa(si_other3.sin_addr), ntohs(si_other3.sin_port);
		if(d3count>200)d3count=0;
		//return 0;
		usleep(200000);
		close(s3);
	}
}

void *terimaIPC4(void *arg){
	while(1){
		//printf("server 4 bekerja\n");
		struct sockaddr_in si_me4, si_other4;
		int s4, i4, slen4=sizeof(si_other4);

		if ((s4=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep((char *)"socket");
		//printf("server 4 soket dibuat\n");

		int ret4=setsockopt(s4, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
		memset((char *) &si_me4, 0, sizeof(si_me4));
		si_me4.sin_family = AF_INET;
		si_me4.sin_port = htons(PORT4);
		si_me4.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(s4, (struct sockaddr *) &si_me4, sizeof(si_me4))==-1){
			//printf("server 4 binded\n");
			diep((char *)"bind");
			close(s4);
			//return;
		}
		if (recvfrom(s4, (struct sockaddr *) &eros4, sizeof (eros4), 0, (struct sockaddr*) &si_other4, (unsigned int*) &slen4)==-1)
		diep((char *)"recvfrom()");
		else{
			dCom[4] = eros4.dCom;
			stateEROS[4] = eros4.datajob;
			sudutbolaEROS[4] = eros4.dataYK;
			dataarah[4] = eros4.dataarah;
			dataarahkepala[4] = eros4.dataarahkepala;
			flagonoff[4] = 1;
			mainx[4] = eros4.mainx;
			ball[4] = eros4.ball;
			ROS_INFO("Receiving UDP4..[%d][%d][%d][%d][%d][%d]",eros4.dCom,eros4.dataYK,eros4.datajob,eros4.dataarah,eros4.dataarahkepala,eros4.mainx);
			d4count++;
		}
		inet_ntoa(si_other4.sin_addr), ntohs(si_other4.sin_port);
		if(d4count>200)d4count=0;
		//return 0;
		usleep(200000);
		close(s4);
	}
}

void *terimaIPC5(void *arg){
	while(1){
		//printf("server 5 bekerja\n");
		struct sockaddr_in si_me5, si_other5;
		int s5, i5, slen5=sizeof(si_other5);

		if ((s5=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep((char *)"socket");
		//printf("server 5 soket\n");

		int ret5=setsockopt(s5, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
		memset((char *) &si_me5, 0, sizeof(si_me5));
		si_me5.sin_family = AF_INET;
		si_me5.sin_port = htons(PORT5);
		si_me5.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(s5, (struct sockaddr *) &si_me5, sizeof(si_me5))==-1){
			//printf("server 5 binded\n");
			diep((char *)"bind");
			close(s5);
			//return;
		}
		if (recvfrom(s5, (struct sockaddr *) &eros5, sizeof (eros5), 0, (struct sockaddr*) &si_other5, (unsigned int*) &slen5)==-1)
			diep((char *)"recvfrom()");
		else
		{
			dCom[5] = eros5.dCom;
			stateEROS[5] = eros5.datajob;
			sudutbolaEROS[5] = eros5.dataYK;
			dataarah[5] = eros5.dataarah;
			dataarahkepala[5] = eros5.dataarahkepala;
			flagonoff[5] = 1;
			mainx[5] = eros5.mainx;
			ball[5] = eros5.ball;
			ROS_INFO("Receiving UDP5..[%d][%d][%d][%d][%d][%d]",eros5.dCom,eros5.dataYK,eros5.datajob,eros5.dataarah,eros5.dataarahkepala,eros5.mainx);
			d5count++;
		}
		if(d5count>200)d5count=0;
		//return 0;
		usleep(200000);
		close(s5);
	}
}

void *kirimIPC(void *arg){
	while(1){
		int on=1;
		e.dCom 				= dCom[player];
		e.dataYK 			= sudutbolaEROS[player];
		e.datajob 			= stateEROS[player];
		e.dataarah 			= dataarah[player];
		e.dataarahkepala 	= dataarahkepala[player];
		e.XPOS 				= XPOS[player];
		e.YPOS 				= YPOS[player];
		e.initialPOS 		= initialPOS[player];
		e.mainx 			= mainx[player];
		e.ball				= ball[player];

		struct sockaddr_in si_other1;
		int s1, slen1=sizeof(si_other1);
		//int s, i, slen=sizeof(si_other);
		char buf[BUFLEN];
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
		if(player == 1){
			si_other1.sin_port = htons(PORT1);
		}
		else if(player == 2){
			si_other1.sin_port = htons(PORT2);
		}
		else if(player == 3){
			si_other1.sin_port = htons(PORT3);
		}
		else if(player == 4){
			si_other1.sin_port = htons(PORT4);
		}
		else if(player == 5){
			si_other1.sin_port = htons(PORT5);
		}


		if (inet_aton(SRV_IP, &si_other1.sin_addr)==0) {
			fprintf(stderr, "inet_aton() failed\n");
			//exit(1);
		}


		if (sendto(s1, (struct sockaddr *) &e, sizeof (e), 0, (struct sockaddr *) &si_other1, slen1)==-1)
		diep((char *)"sendto()");
		close(s1);

		//ROS_INFO("Sending UDP..[%d][%d][%d][%d][%d]",e.dCom,e.dataYK,e.datajob,e.dataarah,e.dataarahkepala);
		usleep(400000);
	}
}

int prosesbandingkan(){
	int i,k;		   //  1 2 3 4 5
	int datapin[6]=	{0,0,0,0,0,0};
	int penalty;
	int EROSMax=-3; //minimum state that the robot can reach
	int EROSYKMin=100; //maximum degree of pitch neck
	int robotStateMax=0;
	int robotYKMin=0;
	bool flagpenalty;
	bool flagsama;
	//int stateErosmaks=8;
	int dbuff=1;
	//int ykepalamaks;
	//flagonoff(dataplayer)=1;
	penalty = datapenalty;
	//printf("Status penalty : %d",datapin[i]);
	//printf("Penalty \t: ");
	for(i=0;i<5;i++){
		datapin[i+1] = penalty % 2;
		penalty /= 2;
		//printf("%2d, ",datapin[i+1]);
	}
	//printf("\n");
	flagpenalty=false;
	flagsama=false;
	for(i=1;i<5;i++){
		//Koreksi Diri
		if(i==player){
			//hamba kena pinalty?
			if(datapin[i]==1){
				flagpenalty=true;
			}
			continue;
		}

		//Banding Teman Kena Penalty atau Tidak Komunikasi
		if(datapin[i]==1 || flagonoff[i]==0 || i==KEEPER || mainx[i]==0 || stateEROS[i]<=0){
		//semua Teman Kena Penalty atau Tidak Komunikasi
		//if(datapin[i]==1 || flagonoff[i]==0 || i!=3 || mainx[i]==0 || stateEROS[i]<=0){
			continue;
		}

		//Cari state terbesar
		if(stateEROS[i]>=EROSMax){
			EROSMax=stateEROS[i];
			robotStateMax=i;
		}

		//Cari sudut bola terkecil
		if(sudutbolaEROS[i]<=EROSYKMin){
			EROSYKMin=sudutbolaEROS[i];
			robotYKMin=i;
		}
	}

	if(flagpenalty){
		dbuff=9;
	}else{
		if(stateEROS[player]>EROSMax || stateEROS[player]==-9){
			dbuff=1;
		}else if(stateEROS[player]==EROSMax){
			if(EROSMax<2){
				dbuff=1;
			}else{
				if(sudutbolaEROS[player]<EROSYKMin){
					dbuff=1;
				}else{
					dbuff=2;
				}
			}
		}else{
			dbuff=2;
		}
	}

	if(dbuff==1){counterSerang++;counterTahan--;}
	else if(dbuff==2){counterSerang--;counterTahan++;}

	if(counterSerang<=0)counterSerang=0;
	if(counterTahan<=0)counterTahan=0;

	if(counterSerang>75)counterSerang=75;
	if(counterTahan>75)counterTahan=75;

	if(counterSerang-counterTahan>25)bandingSerang=1;
	else if(counterTahan-counterSerang>25)bandingSerang=2;

	if(dbuff==9){bandingSerang=9;counterTahan=0;counterSerang=75;counterSama=0;}
	
	if(bandingSerang==2 && mainx[robotStateMax]==mainx[player]){
		counterSama++;
		if(counterSama>300){
			flagsama=true;
			counterSama=300;
		}
	}else{
		counterSama--;
	}

	if(counterSama<=0){
		counterSama=0;
		flagsama=false;
	}

	datacom=bandingSerang;
	dCom[player]=bandingSerang;
	dtaPublish.data.clear();
	dtaPublish.data.push_back(datacom);
	dtaPublish.data.push_back(flagsama);
	//fprintf(stderr, "cS[%d]cT[%d]mX[%d]\n",counterSerang,counterTahan,mainx[player]);
}

void watchdog(int sig){
  	//printf("Waktu koneksi habis\r\n");
  	if(d1count == simpand1count){
		stateEROS[1] = sudutbolaEROS[1] = dataarah[1] = dataarahkepala[1] = ball[1] = 0;
		flagonoff[1] = 0;
	}
	if(d2count == simpand2count){
		stateEROS[2] = sudutbolaEROS[2] = dataarah[2] = dataarahkepala[2] = ball[2] = 0;
		flagonoff[2] = 0;
	}
	if(d3count == simpand3count){
		stateEROS[3] = sudutbolaEROS[3] = dataarah[3] = dataarahkepala[3] = ball[3] = 0;
		flagonoff[3] = 0;
	}
	if(d4count == simpand4count){
		stateEROS[4] = sudutbolaEROS[4] = dataarah[4] = dataarahkepala[4] = ball[4] = 0;
		flagonoff[4] = 0;
	}
	if(d5count == simpand5count){
		stateEROS[5] = sudutbolaEROS[5] = dataarah[5] = dataarahkepala[5] = ball[5] = 0;
		flagonoff[5] = 0;
	}
	if(d1count>=200)d1count=0;
	if(d2count>=200)d2count=0;
	if(d3count>=200)d3count=0;
	if(d4count>=200)d4count=0;
	if(d5count>=200)d5count=0;
	simpand1count = d1count;
	simpand2count = d2count;
	simpand3count = d3count;
	simpand4count = d4count;
	simpand5count = d5count;
  	alarm(3);
}


int main(int argc, char **argv){

	ros::init(argc, argv, "clientrec");
	ros::NodeHandle n;
	ros::Subscriber recv = n.subscribe(ROS_SUBSCRIBE_NODE, 100, IntelCallback);
	ros::Publisher sendv = n.advertise<std_msgs::Int32MultiArray>(ROS_PUBLISH_NODE, 100);

	pthread_t t1,t2,t3,t4,t5,tsend;	// this is our thread identifier
	int i = 0;

	printf("Bismillah\n");
	//expData(); //Read data from file profile.al

	n.getParam("team",tim);
	n.getParam("player",player);
	bandingSerang=1;

	if(player!=1)pthread_create(&t1,NULL,terimaIPC1,NULL);
	if(player!=2)pthread_create(&t2,NULL,terimaIPC2,NULL);
	if(player!=3)pthread_create(&t3,NULL,terimaIPC3,NULL);
	if(player!=4)pthread_create(&t4,NULL,terimaIPC4,NULL);
	if(player!=5)pthread_create(&t5,NULL,terimaIPC5,NULL);
				 pthread_create(&tsend,NULL,kirimIPC,NULL);

	datacom=0;

	printf("Thread sudah dibuat \n");
	signal(SIGALRM, watchdog);
	alarm(3);

	ros::Rate rate(30);

	while(ros::ok()){
		//system("clear");
		flagerror = false;

		//readIPC(1);
		//printf("dCom \t\t: "); for(i=1;i<=5;i++) printf("%2d, ",dCom[i]); printf("\n");
		//printf("datajob \t: "); for(i=1;i<=5;i++) printf("%2d, ",stateEROS[i]); printf("\n");
		//printf("dataYK \t\t: "); for(i=1;i<=5;i++) printf("%2d, ",sudutbolaEROS[i]); printf("\n");
		//printf("dataarah \t: "); for(i=1;i<=5;i++) printf("%2d, ",dataarah[i]); printf("\n");
		//printf("dataarahkepala \t: "); for(i=1;i<=5;i++) printf("%2d, ",dataarahkepala[i]); printf("\n");
		//printf("OnOff \t\t: "); for(i=1;i<=5;i++) printf("%2d, ",flagonoff[i]); printf("\n");
		prosesbandingkan();
		//writeIPC(1);
		sendv.publish(dtaPublish);
		ros::spinOnce();
		rate.sleep();
		//usleep(100000);
	}
	return 0;
}

