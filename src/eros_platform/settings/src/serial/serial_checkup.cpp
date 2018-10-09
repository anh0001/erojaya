//============================================================================
// Name        : serial_checkup.cpp
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <fstream>

#define MAX_LOOP 1000
#define MAX_FTDI 3

using namespace std;


struct Data{
	unsigned char ping;
};
Data *pdata = (struct Data *) malloc(sizeof(*pdata));
char USBTX[]={"/home/eros/param/USBTX.eros"};
char USBRX[]={"/home/eros/param/USBRX.eros"};
int fdGrk = 0,count,countRX,countTX,countElse;
char tx_id[256],rx_id[256];
//float theta,last_theta = 0;										//Uncomment this for Odometry
//bool flaggettime;													//Uncomment this for Time Lapsed Stamp 


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

int ftdi_reset(){
	countRX=0;
	countTX=0;
}

int main(int argc, char **argv){
	char system_check[100],system_open[100];
	int loop,current_ftdi=0;
	
	cout << ".. Bismillah" << endl;
	sleep(1);
	cout << ".. Serial Checkup FTDI Receiver Transmitter" << endl;
	sleep(1);
	cout << ".. Make sure that Sub-Controller is [ON]\n" << endl;
	sleep(1);
	cout << ".. Get Ready" << endl;
	sleep(2);
	FILE *ls=popen("ls /dev/ttyUSB*","r");
	char buf[256];
	while(fgets(buf, sizeof(buf), ls) != 0){
		buf[strlen(buf)-1]='\0';
		cout << ".. Catching data from "<< buf << ".." << endl;
		sleep(2);
		sprintf(system_check,"stty -F %s 57600",buf);
		sprintf(system_open,"%s",buf);
		//cout << system_check << endl;
		close(fdGrk);	//Close serial communication, to ensure that serial is closed before we open
		system(system_check);
		system(system_check);
		system(system_check);
		fdGrk = open(system_open, O_RDWR | O_NOCTTY | O_NONBLOCK);
		configure_port(fdGrk);
		pdata->ping=0xFF;
		while(1){
			unsigned char kdata[1];
			int k = 0;
			try{
				write(fdGrk, pdata, 1);
				k= read(fdGrk, kdata, 1);
			}
			catch(exception& e){
				if(count<100){
					close(fdGrk);
					fdGrk = open(system_open, O_RDWR | O_NOCTTY | O_NONBLOCK);
					configure_port(fdGrk);
					count++;
				}else {
					close(fdGrk);
					break;
				}
			}

			if (k > 0){
				//if(kdata[0]==0xFC)printf(">>%d\n",kdata[0]);
				//if(kdata[0]==0xFD)printf(">>%d\n",kdata[0]);
				
				if(kdata[0]==0xFD)countRX++;
				else if(kdata[0]==0xFC)countTX++;
				if(countTX>100 || countRX>100){
					if(countTX>100)memcpy(tx_id,buf,sizeof(tx_id));
					if(countRX>100)memcpy(rx_id,buf,sizeof(rx_id));
					ftdi_reset();
					break;
				}
			}
		}
	}
	pclose(ls);
	printf("\n.. Transmitter :%s\n",tx_id);
	  printf(".. Receiver    :%s\n",rx_id);
	sleep(1);
	printf(".. Writing to USBTX.eros and USBRX.eros\n");
	sleep(1);
	
	ofstream outTX (USBTX);
	if(outTX.is_open()){
		outTX << tx_id << endl;
	}else cout<<".. Unable to load file USBTX.eros"<<endl;
	outTX.close();
	
	ofstream outRX (USBRX);
	if(outRX.is_open()){
		outRX << rx_id << endl;
	}else cout<<".. Unable to load file USBRX.eros"<<endl;
	outRX.close();
	printf(".. serial_checkup finished\n");
	return 0;
}
