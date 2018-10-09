//============================================================================
// Name        : utility.cpp / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================


#include "eheader.h"
using namespace std;

void DebugAI(const char* xx){
	if(debug_mode){
		fprintf(stderr, "%s\n", xx);
	}
}

float KNN_Basic(int x0, int x1, int y0, int y1, int direction){
	int MAX_Data1=960;
	int MAX_Data2=945;
	int MAX_Data3=945;
	int MAX_DATA;
	switch(direction){
		case 1:MAX_DATA=MAX_Data1;break;
		case 2:MAX_DATA=MAX_Data2;break;
		case 3:MAX_DATA=MAX_Data3;break;
	}
	float KNN_Distance[MAX_DATA][2];
	for(int i=0; i<MAX_DATA; i++){
		if (direction==2)
		{
			KNN_Distance[i][1]=sqrt(  	(KNN_Data2[i][0]-x0)*(KNN_Data2[i][0]-x0) + 
										(KNN_Data2[i][1]-x1)*(KNN_Data2[i][1]-x1) + 
										(KNN_Data2[i][2]-y0)*(KNN_Data2[i][2]-y0) + 
										(KNN_Data2[i][3]-y1)*(KNN_Data2[i][3]-y1)  );//distance
			KNN_Distance[i][0]=KNN_Data2[i][4]; //posisi
		}else if(direction==3){
			KNN_Distance[i][1]=sqrt(  	(KNN_Data3[i][0]-x0)*(KNN_Data3[i][0]-x0) + 
										(KNN_Data3[i][1]-x1)*(KNN_Data3[i][1]-x1) + 
										(KNN_Data3[i][2]-y0)*(KNN_Data3[i][2]-y0) + 
										(KNN_Data3[i][3]-y1)*(KNN_Data3[i][3]-y1)  );//distance
			KNN_Distance[i][0]=KNN_Data3[i][4]; //posisi
		}else if(direction==1){
			KNN_Distance[i][1]=sqrt(  	(KNN_Data1[i][0]-x0)*(KNN_Data1[i][0]-x0) + 
										(KNN_Data1[i][1]-x1)*(KNN_Data1[i][1]-x1) + 
										(KNN_Data1[i][2]-y0)*(KNN_Data1[i][2]-y0) + 
										(KNN_Data1[i][3]-y1)*(KNN_Data1[i][3]-y1)  );//distance
			KNN_Distance[i][0]=KNN_Data1[i][4]; //posisi
		}
	}
	int j=0;
	int KNN_DTemp0;//alamat,distance,posisi
	double KNN_DTemp1;
	int KNN_DTemp2;
	//int KNN_cTukar;
	for(int k=0; k<MAX_DATA; k++){
		KNN_DTemp1=0,KNN_DTemp0=j;//KNN_cTukar=0;
		for(int i=j; i<MAX_DATA; i++){
			if(KNN_Distance[i][1]>KNN_DTemp1){
				KNN_DTemp1=KNN_Distance[i][1];
				KNN_DTemp0=i;
				KNN_DTemp2=KNN_Distance[i][0];
				//KNN_cTukar++;
			}
		}
		//if(KNN_cTukar>0){
			KNN_Distance[KNN_DTemp0][0]=KNN_Distance[j][0];
			KNN_Distance[KNN_DTemp0][1]=KNN_Distance[j][1];
			KNN_Distance[j][0]=KNN_DTemp2;
			KNN_Distance[j][1]=KNN_DTemp2;
		//}
		j++;	
	}

	int poscount[6];
	poscount[0]=poscount[1]=poscount[2]=poscount[3]=poscount[4]=poscount[5]=0;
	for (int i = MAX_DATA-1; i >=MAX_DATA-100; --i)
	{
		switch((int)KNN_Distance[i][0]){
			case 1: poscount[1]++; break;
			case 2: poscount[2]++; break;
			case 3: poscount[3]++; break;
			case 4: poscount[4]++; break;
			case 5: poscount[5]++; break;
		}
		//fprintf(stderr, "[%d][%1.2f]\n",(int)KNN_Distance2[i][0],KNN_Distance2[i][1]);
	}
	float fpos;
	fpos=1*poscount[1]+2*poscount[2]+3*poscount[3]+4*poscount[4]+5*poscount[5];
	fpos=fpos/100;
	//fprintf(stderr, "[%d][%d][%d][%d][%d][%1.2f]\n",poscount[1],poscount[2],poscount[3],poscount[4],poscount[5],fpos);
	
	return fpos;
}

/* ====================== Subscriber & Publisher ============================== */

//Variable List: dtaFall,dtaHeading,dtaFlagOdo,dtaOdoX,dtaOdoZ,dtaOdoH,dtaOdoM
//dtaFall = 0::belum serialan , 1::robot jatuh, 2::sudah serialan
void SerialItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromSerial[i]=*it;
      i++;
    }
    dtaFall=dtaFromSerial[0];
    dtaHeading=dtaFromSerial[1];
    dtaXPOS=dtaFromSerial[2];
    dtaYPOS=dtaFromSerial[3];
    //ROS_INFO("I heard Serial: [%d][%d][%d][%d]",dtaFall,dtaHeading,dtaXPOS,dtaYPOS);
}

void VisionLocalCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromField[i]=*it;
      i++;
    }
    if(flagLocalize){
    	int maxLine,IPoint;
    	float grad[3],lastgrad;
    	maxLine=dtaFromField[12];
		if(maxLine>3){
			//fprintf(stderr, "Kelebihan Garis\n");
			getPosition=0;
		}else{
			lastgrad=IPoint=0;
			for(int i=0; i<maxLine; i++){
				grad[i]=(float)(dtaFromField[1+(i*4)]-dtaFromField[3+(i*4)])/(float)(dtaFromField[2+(i*4)]-dtaFromField[0+(i*4)]);
				//90 derajat
				if(posDir==1){
					if(i==0){
						IPoint=0;
					}else if(i==1){
						if(fabs(grad[0])<=fabs(grad[1]))IPoint=0;
						else IPoint=1;
					}else{
						if(fabs(grad[0])<fabs(grad[1]) && fabs(grad[0])<=fabs(grad[2]))IPoint=0;
						else if(fabs(grad[1])<fabs(grad[0]) && fabs(grad[1])<=fabs(grad[2]))IPoint=1;
						else if(fabs(grad[2])<fabs(grad[0]) && fabs(grad[2])<=fabs(grad[1]))IPoint=2;
					}
					HeadingOffset=grad[IPoint]*180;
					//fprintf(stderr,"{%3d,%3d,%3d,%3d,%d[%1.2f]},",dtaFromField[0+(IPoint*4)],dtaFromField[1+(IPoint*4)],dtaFromField[2+(IPoint*4)],dtaFromField[3+(IPoint*4)],IPoint,grad[i]);
					//fprintf(stderr, "\n");
				}else if(posDir==2){
					if(grad[i]>0)continue;
					if(lastgrad==0)IPoint=i;
					if (lastgrad<-2)IPoint=i;
				}else if(posDir==3){
					if(grad[i]<0)continue;
					if(lastgrad==0)IPoint=i;
					if(lastgrad>2)IPoint=i;
				}

				lastgrad=grad[i];
			}
			if(flagGetFieldSampleData){
				fprintf(stderr,"{%d,%d,%d,%d,%d},\n",dtaFromField[0+(IPoint*4)],dtaFromField[1+(IPoint*4)],dtaFromField[2+(IPoint*4)],dtaFromField[3+(IPoint*4)],initpos);
			}
			//fprintf(stderr,"{%3d,%3d,%3d,%3d,%d},[%1.2f]",dtaFromField[0+(IPoint*4)],dtaFromField[1+(IPoint*4)],dtaFromField[2+(IPoint*4)],dtaFromField[3+(IPoint*4)],IPoint,HeadingOffset);
			//fprintf(stderr, "\n");
			//fprintf(stderr,"%d,%d,%d,%d,1\n",dtaFromField[0+(IPoint*4)],dtaFromField[1+(IPoint*4)],dtaFromField[2+(IPoint*4)],dtaFromField[3+(IPoint*4)]);
			getPosition=KNN_Basic(dtaFromField[0+(IPoint*4)],dtaFromField[1+(IPoint*4)],dtaFromField[2+(IPoint*4)],dtaFromField[3+(IPoint*4)],posDir);
		}
    }else{
    	getPosition=0;
    }
    //fprintf(stderr, "Position=%d\n", getPosition);
    //fprintf(stderr,"[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]\n",dtaFromField[0],dtaFromField[1],dtaFromField[2],dtaFromField[3],dtaFromField[4],dtaFromField[5],dtaFromField[6],dtaFromField[7],dtaFromField[8],dtaFromField[9],dtaFromField[10],dtaFromField[11],dtaFromField[12]);
}

void GetFieldSampleData(int xpos){
	flagGetFieldSampleData=true;
	flagLocalize=1;
	if(xpos==1){
		xBall=sdtblax;
		yBall=sdtdefy;
	}else
	if(xpos==2){
		xBall=generatesdtX(90);
		yBall=sdtdefy;
	}else
	if(xpos==3){
		xBall=generatesdtX(-90);
		yBall=sdtdefy;
	}
	posDir=xpos;
	motionAct(xBall,yBall,0,0);
}

void VisionItCallback(const vision_module::vision_outputsConstPtr& msg){
    
	dataXB=msg->ball.center_x_im;
    dataYB=msg->ball.center_y_im;
	//radius=msg->ball.radius_im;
    //ROS_INFO("I heard Vision: [%d][%d]", dataXB,dataYB);
}

void RecItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromRec[i]=*it;
      i++;
    }
	dtComm = dtaFromRec[0];
	dtflagsama = dtaFromRec[1];

    //ROS_INFO("I heard Rec: [%d]",dtaFromRec[0]);
}

void RefereeItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
    int i=0;
    for(std::vector<int>::const_iterator it=msg->data.begin(); it!=msg->data.end(); ++it){
      dtaFromReferee[i]=*it;
      i++;
    }
    myTask = dtaFromReferee[0];
    penalty= dtaFromReferee[1];
    myTask2 = dtaFromReferee[2];
    //ROS_INFO("I heard Referee: [%d][%d][%d]", dtaFromReferee[0],dtaFromReferee[1],dtaFromReferee[2]);
}

void RecPublish(int dtaSudutBola, int dtaState, int dtaPenalty, int dtaArah, int dtaArahKepala){
	dtaPublishREC.data.clear();
	dtaPublishREC.data.push_back(dtaSudutBola);
	dtaPublishREC.data.push_back(dtaState);
	dtaPublishREC.data.push_back(dtaPenalty);
	dtaPublishREC.data.push_back(dtaArah);
	dtaPublishREC.data.push_back(dtaArahKepala);
	dtaPublishREC.data.push_back(dtaXPOS);
	dtaPublishREC.data.push_back(dtaYPOS);
	dtaPublishREC.data.push_back(initialPosition);
	dtaPublishREC.data.push_back(mainx);
}

void VisionPublish(int dtaArahKepala, int dtaYKepala, int dtaYSudut, int dtaEField, int dtaEVision){
	dtaPublishVISION.data.clear();
	//dtaPublishVISION.data.push_back(dtaArahKepala);
	dtaPublishVISION.data.push_back(dtaYKepala);
	//dtaPublishVISION.data.push_back(dtaYSudut);
	dtaPublishVISION.data.push_back(dtaEField);
	//dtaPublishVISION.data.push_back(dtaEVision);
}

void SerialPublish(int param1, int param2, int param3, int param4){
	dtaPublishSERIAL.data.clear();
	dtaPublishSERIAL.data.push_back(param1);
	dtaPublishSERIAL.data.push_back(param2);
	dtaPublishSERIAL.data.push_back(param3);
	dtaPublishSERIAL.data.push_back(param4);
}

void DebugPublish(int param1, int param2, int param3, int param4, int param5, int param6){
	dtaPublishDEBUG.data.clear();
	dtaPublishDEBUG.data.push_back(param1);
	dtaPublishDEBUG.data.push_back(param2);
	dtaPublishDEBUG.data.push_back(param3);
	dtaPublishDEBUG.data.push_back(param4);
	dtaPublishDEBUG.data.push_back(param5);
	dtaPublishDEBUG.data.push_back(param6);
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

int motionAct(int xServo, int yServo, int motionN, int Mode) {
	static unsigned char ceksum=0;
	Data *pdata = (struct Data *) malloc(sizeof(*pdata));
	//DataSerial *kdata = (struct DataSerial *) malloc(sizeof(*kdata));
	//unsigned char kdata[1];
	pdata->awal = 77;
	pdata->awal2 = 78;
	pdata->mode = Mode; ceksum+=Mode;
	pdata->x_data = xServo & 0xFF; ceksum+=(xServo & 0xFF);
	pdata->x_bit = xServo >> 8; ceksum+=(xServo >> 8);
	pdata->y_data = yServo & 0xFF; ceksum+=(yServo & 0xFF);
	pdata->y_bit = yServo >> 8; ceksum+=(yServo >> 8);
	pdata->gerak = motionN; ceksum+=motionN;
	//pdata->cek = ~ceksum;
	close(fdGrk);	//Close serial communication, to ensure that serial is closed before we open
	fdGrk = open(USBRX, O_RDWR | O_NOCTTY | O_NONBLOCK);
	configure_port(fdGrk);
	//serialData();
	//if(!write(fdGrk, pdata, 8))printf(".. Cannot write in USBRX Device.\n");
	write(fdGrk, pdata, 8);
	//read(fdGrk, kdata, 1);
	//if(kdata->awal==119 && kdata->awal2==120)
	return 0;
}

/* **************************************************************************** */


/* ============================ Load File ===================================== */
bool impUSBRX(){
	char file[]={"/home/eros/param/USBRX.eros"};
	ifstream buka(file);
	string line;
	if (buka.is_open()){
		if (buka.good()){
			char buffer[50]={};
			getline(buka,line);
			for(int i=0; i<line.length(); i++){
				buffer[i]=line[i];
			}
			memcpy(USBRX,buffer,sizeof(USBRX));
			printf("iki>>%s--\n\n",buffer);
		}
		buka.close();
		return 1;
	} else return 0;
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

bool expData(){
	int j=0, buf=0, data=0;
	char file[]={"/home/eros/param/profile.eros"};
	string line;
	ifstream buka(file);
	if (buka.is_open())
	{
		while ( buka.good() ){
			char tes[5]={};
			getline (buka,line);
			for(int i=0; i<line.length();i++){
				tes[j]=line[i];
				if(line[i]==';'){
					j = 0;
					buf = atoi(tes);
					//printf("nl: %d\n", crut);
					if(data==0){tim = buf;printf("tim: %d\n", tim);}
					else if(data==1){player = buf;printf("player: %d\n", player);}
					data++;
					continue;
				}
				j++;
			}
		}buka.close();
		return 1;
	}
	else {
		cout << "Unable to open file";
		return 0;
	}
}

/* **************************************************************************** */

/* =============================== Filter ===================================== */

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

/* **************************************************************************** */

/* ======================= Searching Ball ============================= */

int generatesdtX(int sudutX){
	int hasilservoX;
	if(sudutX<0)
	{
		hasilservoX = (sudutX*1025/90 * -1) + sdtblax;
	}
	else
	{

		hasilservoX = sdtblax - (sudutX*1025/90);
	}
	return hasilservoX;
}


int generatesdtY(int sudutY){
	int hasilservoY = (sudutY * 1025 / 90) + sdtblay + 5;
	return hasilservoY;
}

int carisudutbolay(int servoY){
	int suduty = (((servoY-sdtblay)*90)/1025);
	jrkbolay = (46*tan(sdtbolay*0.01745329));
	return suduty;
}


int SudutBolaY(){
	sdtbolay = (((yBall-sdtblay)*90)/1025);
	/*if(sdtbolay<=35)
	{
		sdtbolay = (((yBall+((180-dataYB)*2)-sdtblay)*90)/1025);
		if(sdtbolay<1) sdtbolay = 1;
	}*/
	jrkbolay = (46*tan(sdtbolay*0.01745329));
	return sdtbolay;
}

int SudutBolaX(){
	//xBall = kalmansdt(xBall);
	if(xBall >= sdtblax){ //-
		sdtbolax = (-1*(((xBall-sdtblax)*90)/1025));
		//jrkbolax = (jrkbolay*sin(sdtbolax*0.01745329));
	} else if(xBall < sdtblax){ //+
		sdtbolax = (((sdtblax-xBall)*90)/1025);
		//jrkbolax = (jrkbolay*sin(sdtbolax*0.01745329));
	}
	//fprintf(stderr, "xBall=%d | sdtblax=%d | sdtbolax=%d\n",xBall,sdtblax,sdtbolax);
	return sdtbolax;
}

void MotionHeadPrediction(){
	int Konstanta = 0;
	if(motion == 11 && motion == 21)Konstanta = -10;
	else if(motion == 25) Konstanta = -25;
	else if(motion == 12 && motion == 22)Konstanta = 10;
	else if(motion == 26) Konstanta = 25;
	xBall += Konstanta;
}

int GerakServo(int posAwal, int posAkhir){
	int goalPos, selisih;
	selisih = posAkhir - posAwal;
	goalPos = (int) (((float) ((float) ((float) TT / 100)) * selisih) + posAwal);
	TT += sKpl;
	return goalPos;
}

void GerakCariBola(int dataR) {
	sKpl = 1;//3;//1.5;
	int Xmax, Xmin;
	Xmax = sdtblax + dataR;
	Xmin = sdtblax - dataR;
	if (Xmax >= 3500)
		Xmax = 3500;
	else if (Xmin <= 600)
		Xmin = 600;

	if(DelayCari<=0){
		if (arahX == 0) {
			xBall = GerakServo(servoX, GoalXservo);
			if (arahY == 1) {
				yBall = GerakServo(sdtblamn, sdtblamx);
				if (TT >= 100) {
					arahY = 0;
					DelayCari = 60;
				}
			} else if (arahY == 0) {
				yBall = GerakServo(sdtblamx, sdtblamn);
				if (TT >= 100) {
					arahY = 1;
					DelayCari = 15;
				}
			}
			if (TT >= 100) {
				TT = 0;
				servoX = GoalXservo;
				GoalXservo += 196;//262;
				if (GoalXservo >= Xmax || GoalXservo >= 3500) {
					GoalXservo = Xmax;
					arahX = 1;
				}
			}
		} else if(arahX==1){
			xBall = GerakServo(servoX, GoalXservo);
			if (arahY == 1) {
				yBall = GerakServo(sdtblamn, sdtblamx);

				if (TT >= 100) {
					arahY = 0;
					DelayCari = 60;
				}
			} else if (arahY == 0) {
				yBall = GerakServo(sdtblamx, sdtblamn);
				if (TT >= 100) {
					arahY = 1;
					DelayCari = 15;
				}
			}
			if (TT >= 100) {
				TT = 0;
				servoX = GoalXservo;
				GoalXservo -= 196;//262;
				if (GoalXservo <= Xmin || GoalXservo <= 600) {
					GoalXservo = Xmin;
					arahX = 0;
				}
			}
		}
	}else{
		DelayCari--; 
		if(DelayCari <= 0) DelayCari = 0;
	}
}

void GerakCariBola1(){
	sKpl = 1;//2;//4;
	if(DelayCari <= 0){
		if (arahY == 1) {
			yBall = GerakServo(sdtblamn, sdtblamx);
			if (TT >= 100) {
				arahY = 0;
				DelayCari = 40;
			}
		} else if (arahY == 0) {
			yBall = GerakServo(sdtblamx, sdtblamn);
			if (TT >= 100) {
				arahY = 1;
				DelayCari = 15;
			}
		}
		if (TT >= 100) {
			TT = 0;
		}
	} else {
		DelayCari--; if(DelayCari <= 0) DelayCari = 0;
	}
}

int GerakCariBola2(int setdataR, int dataR, int arahXX, int speed){
	int Xmax, Xmin;
	Xmax = setdataR + dataR;
	Xmin = setdataR - dataR;
	sKpl = speed;
	if(DelayCari <= 0){
		if (arahXX == 0) {
			if(stepBCB == 0){
				yBall = sdtblaGBa;
				servoY = yBall;
				DelayCari = 10;
				stepBCB = 1;
			} if(stepBCB == 1){
				xBall = GerakServo(setdataR, Xmax);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 2;
				}
			} else if(stepBCB == 2){
				yBall = GerakServo(sdtblaGBa, sdtblaGBw);
				servoY = yBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 3;
				}
			} else if(stepBCB == 3){
				xBall = GerakServo(Xmax, Xmin);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 4;
				}
			} else if(stepBCB == 4){
				yBall = GerakServo(sdtblaGBw, sdtblaGBa);
				servoY = yBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 5;
				}
			} else if(stepBCB == 5){
				xBall = GerakServo(Xmin, setdataR);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 0;
				}
			}
		} else if (arahXX == 1) {
			if(stepBCB == 0){
				yBall = sdtblaGBa;
				servoY = yBall;
				DelayCari = 10;
				stepBCB = 1;
			} if(stepBCB == 1){
				xBall = GerakServo(setdataR, Xmin);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 2;
				}
			} else if(stepBCB == 2){
				yBall = GerakServo(sdtblaGBa, sdtblaGBw);
				servoY = yBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 3;
				}
			} else if(stepBCB == 3){
				xBall = GerakServo(Xmin, Xmax);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 4;
				}
			} else if(stepBCB == 4){
				yBall = GerakServo(sdtblaGBw, sdtblaGBa);
				servoY = yBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 5;
				}
			} else if(stepBCB == 5){
				xBall = GerakServo(Xmax, setdataR);
				servoX = GoalXservo = xBall;
				if (TT >= 100) {
					TT = 0;
					stepBCB = 0;
				}
			}
		}
	} else { DelayCari--; if(DelayCari <= 0) DelayCari = 0;}

	if (xBall >= 3500)
		xBall = 3500;
	else if (xBall <= 600)
		xBall = 600;

	if (yBall >= sdtblaGmx)
		yBall = sdtblaGmx;
	else if (yBall <= sdtblaGBw)
		yBall = sdtblaGBw;
	return 0;
}

int ftBall2(int xBallvision, int yBallvision, int Geser) { //buat kepala cari bola
	int ErrFrameY = 0, ErrFrameX = 0, MM;
	enablevision = 0b00001;
	ethreadsearching = 0;

	if (yBallvision <= 0 || xBallvision <= 0) {
		 sKpl = 2;
		 CountLook --;
		 if(CountLook <= 0){
			 CountLook = 0;
			 CountTrack++;
			 MM = 0;
		 }
		 else if(CountLook > 0)MM = 1;
	} else {
		CountLook++;
		if (CountLook >= 2) { //3
			MM = 1;
			CountTrack = 0;
			if(CountLook >= 30){
				CountLook = 30;
			}
		}

		ErrFrameX = xBallvision - 320 - GeserVision;
		ErrFrameY = yBallvision - 240;
		//ErrFrameY = yBallvision - 180;
		//ErrFrameX = xBallvision - 216 - GeserVision;
		//ErrFrameX = xBallvision - 160 - GeserVision;

		//ErrFrameX = xBallvision - 160 - Geser;
		//ErrFrameY = yBallvision - 120;
		if(ErrFrameX <= 2 && ErrFrameX >= -2) ErrFrameX = 0;
		if(ErrFrameY <= 2 && ErrFrameY >= -2) ErrFrameY = 0;

		if(dtJob >= 3 || CountLook >= 66) //20)
		xBall -= ((ErrFrameX) / TKPX1) + ((ErrFrameX - lastErrX)/TKDX1) + ((ErrFrameX + lastErrX) / TKIX1); //((ErrFrameX) / 10) + ((ErrFrameX - lastErrX) / 14) + ((ErrFrameX + lastErrX) / 80);
		else
		xBall -= ((ErrFrameX) / TKPX2) + ((ErrFrameX - lastErrX)/TKDX2) + ((ErrFrameX + lastErrX) / TKIX2);
		yBall -= ((ErrFrameY) / TKPY) + ((ErrFrameY - lastErrY)/TKDY);//((ErrFrameY) / 12) + ((ErrFrameY - lastErrY) / 20); //((ErrFrameY) / 7) + ((ErrFrameY - lastErrY) / 4);//

		/*if(dtJob >= 3 || CountLook >= 20)
			xBall -= ((ErrFrameX) / 28) + ((ErrFrameX - lastErrX) / 24) + ((ErrFrameX + lastErrX) / 80);
		else{
			xBall -= ((ErrFrameX) / 10) + ((ErrFrameX - lastErrX) / 14) + ((ErrFrameX + lastErrX) / 80);
		}*/

		yBall -= ((ErrFrameY) / 24) + ((ErrFrameY - lastErrY) / 24); //((ErrFrameY) / 7) + ((ErrFrameY - lastErrY) / 4);//((ErrFrameY) / 8) + ((ErrFrameY - lastErrY)/7);

		if (xBall >= 3500)
			xBall = 3500;
		else if (xBall <= 600)
			xBall = 600;
		if (yBall >= sdtblamx+100)
			yBall = sdtblamx+100;
		else if (yBall <= sdtblay+400)
			yBall = sdtblay+400;

		lastXball = servoX = xBall;
		GoalXservo = xBall;
		servoY = yBall;
		lastYball = yBall;
		lastErrX = ErrFrameX;
		lastErrY = ErrFrameY;
		lastMotion = motion;
		MotionHeadPrediction();

		if ((sdtbolax >= 3 && ErrFrameX > -5) || (sdtbolax <= 3 && ErrFrameX > 5)) {
			arahX = lastarahX = 1;
		} else if ((sdtbolax <= 3 && ErrFrameX < 5) || (sdtbolax >= 3 && ErrFrameX < -5))
			arahX = lastarahX = 0;
		else arahX = 0;
		if ((sdtbolay >= 3 && ErrFrameY > -5) || (sdtbolay <= 3 && ErrFrameY > 5)) {
			arahY = lastarahY = 0;
		} else if ((sdtbolay <= 3 && ErrFrameY < 5) || (sdtbolay >= 3 && ErrFrameY < -5))
			arahY = lastarahY = 1;
		else arahY = 0;

		SudutBolaX();
		SudutBolaY();
		lastsdtbolax = sdtbolax;
		lastsdtbolay = sdtbolay;
	}

	if(CountTrack >= 20)
	{
		MM = 0;
		sdtbolax = sdtbolay = 0;
		sKpl = 1;
		//GerakCariBola2(sdtblax, 1000, arahX, 1);
		GerakCariBola2(sdtblax, 1000, lastarahX, 1);
		if(CountTrack < 350){
			motion = 10;
		} else if(CountTrack >= 350){
			if(lastarahX == 1) motion = 12;
			else if(lastarahX == 0) motion = 11;
		}

	}
	if(CountTrack >= 1000)CountTrack=20;
	return MM;
}

int ftBall3(int xBallvision, int yBallvision, int Geser) { //buat kepala cari bola
	int ErrFrameY = 0, ErrFrameX = 0, MM, limitLook = 20;
	enablevision = 0b00001;
	ethreadsearching = 0;

	 if (yBallvision <= 0 || xBallvision <= 0) {
		 sKpl = 2;//2;//1;//2;
		 CountLook--;
		 if(CountLook <= 0){CountLook = MM = FlagLook = 0;CountTrack++;}
		 else if(CountLook <= limitLook && CountLook > 0){ MM = 1; FlagLook = 2;}
		 else if(CountLook > limitLook){ MM = 1; FlagLook = 1; }

	} else {
		CountLook++;
		if (CountLook >= 3) { //6
			if(CountLook < limitLook) { MM = 1; FlagLook = 2;}
			else if(CountLook >= limitLook) { MM = 1; FlagLook = 1;}
			CountTrack = 0;
			if(CountLook >= 30){
				CountLook = 30;
			}
		}
		ErrFrameX = xBallvision - 328 - Geser;
		ErrFrameY = yBallvision - 240;
		//ErrFrameX = xBallvision - 160 - Geser;
		//ErrFrameY = yBallvision - 120;
		if(ErrFrameX <= 2 && ErrFrameX >= -2) ErrFrameX = 0;
		if(ErrFrameY <= 2 && ErrFrameY >= -2) ErrFrameY = 0;

		if(dtJob >= 3 || CountLook >= 20){
			xBall -= ((ErrFrameX) / 1000) + ((ErrFrameX - lastErrX) / 1000) + ((ErrFrameX + lastErrX) / 4000);
			fprintf(stderr, "iku====");
		}else{
			xBall -= ((ErrFrameX) / KPX) + ((ErrFrameX - lastErrX)/KDX) + ((ErrFrameX + lastErrX) / KIX); //((ErrFrameX) / 10) + ((ErrFrameX - lastErrX) / 14) + ((ErrFrameX + lastErrX) / 80);
			fprintf(stderr, "ini====");
		}

		yBall -= ((ErrFrameY) / KPY) + ((ErrFrameY - lastErrY)/KDY);//((ErrFrameY) / 12) + ((ErrFrameY - lastErrY) / 20); //((ErrFrameY) / 7) + ((ErrFrameY - lastErrY) / 4);//

		if (xBall >= 3500)
			xBall = 3500;
		else if (xBall <= 600)
			xBall = 600;
		if (yBall >= sdtblamx+100)
			yBall = sdtblamx+100;
		else if (yBall <= sdtblamn)
			yBall = sdtblamn;

		lastXball = servoX = xBall;
		GoalXservo = xBall;
		servoY = yBall;
		lastYball = yBall;
		lastErrX = ErrFrameX;
		lastErrY = ErrFrameY;
		lastMotion = motion;
		MotionHeadPrediction();


		if ((sdtbolax >= 3 && ErrFrameX > -5) || (sdtbolax <= 3 && ErrFrameX > 5)) {
			arahX = lastarahX = 1;
		} else if ((sdtbolax <= 3 && ErrFrameX < 5) || (sdtbolax >= 3 && ErrFrameX < -5))
			arahX = lastarahX = 0;
		else arahX = 0;
		if ((sdtbolay >= 3 && ErrFrameY > -5) || (sdtbolay <= 3 && ErrFrameY > 5)) {
			arahY = lastarahY = 0;
		} else if ((sdtbolay <= 3 && ErrFrameY < 5) || (sdtbolay >= 3 && ErrFrameY < -5))
			arahY = lastarahY = 1;
		else arahY = 0;

		SudutBolaX();
		SudutBolaY();
		lastsdtbolax = sdtbolax;
		lastsdtbolay = sdtbolay;
	}
	 if(motion >= 20){
		if(lastsdtbolay > 70 && CountTrack >= 60){
			MM = 0;
			sdtbolax = sdtbolay = 0;
			if (CountTrack >= 350) {
				if(lastarahX == 1) {motion = 12;}
				else if(lastarahX == 0) {motion = 11;}
				xBall = sdtblax;
				GerakCariBola1();
				if (CountTrack >= 1000) {
					CountTrack = 30;
					servoX = GoalXservo = xBall;
					lastsdtbolay = 50;

				}
			} else if (CountTrack < 350) {
				motion = 10;
				GerakCariBola(524);
			}
		} else if(lastsdtbolay > 45 && lastsdtbolay <= 70){
			if (CountTrack >= 50) {
				MM = 0;
				sdtbolax = sdtbolay = 0;
				if (CountTrack >= 520) {
					if(lastarahX == 1) {motion = 12;}
					else if(lastarahX == 0) {motion = 11;}
					xBall = sdtblax;
					GerakCariBola1();

					if (CountTrack >= 1500) {
						CountTrack = 30;
						servoX = GoalXservo = xBall;
						lastsdtbolay = 50;
					}
				} else if (CountTrack < 520) {
					motion = 10;
					GerakCariBola(2000);
				}
			}
		} else if(lastsdtbolay<=45){
			if(CountTrack >= 40 && CountTrack < 100){
				MM = 0;
				sdtbolax = sdtbolay = 0;
				motion = 10;//19;
				GerakCariBola(512);
			} else if (CountTrack >= 100) {
				MM = 0;
				sdtbolax = sdtbolay = 0;
				if(lastarahX == 1) {motion = 12;}
				else if(lastarahX == 0) {motion = 11;}
				xBall = sdtblax;
				GerakCariBola1();
				if (CountTrack >= 1000) {
					CountTrack = 30;
					servoX = GoalXservo = xBall;
					lastsdtbolay = 50;

				}
			}
		}
	} else if(motion < 20){
		if(lastsdtbolay>80 && CountTrack >= 10){
			MM = 0;
			sdtbolax = sdtbolay = 0;
			if (CountTrack >= 350) {
				if(lastarahX == 1) {motion = 12;}
				else if(lastarahX == 0) {motion = 11;}
				xBall = sdtblax;
				GerakCariBola1();
				if (CountTrack >= 1490) {
					CountTrack = 30;
					servoX = GoalXservo = xBall;
					lastsdtbolay = 50;
				}
			} else if (CountTrack < 350) {
				motion = 10;
				GerakCariBola(524);
			}
		} else if(lastsdtbolay>45 && lastsdtbolay<=80){
			if (CountTrack >= 5) {
				MM = 0;
				sdtbolax = sdtbolay = 0;
				if (CountTrack >= 520) {
					if(lastarahX == 1) {motion = 12;}
					else if(lastarahX == 0) {motion = 11;}
					xBall = sdtblax;
					GerakCariBola1();
					if (CountTrack >= 1500) {
						CountTrack = 30;
						servoX = GoalXservo = xBall;
						lastsdtbolay = 50;
					}
				} else if (CountTrack < 520) {
					motion = 10;
					GerakCariBola(2000);
				}
			}
		} else if(lastsdtbolay<=45){
			if(CountTrack >= 1 && CountTrack < 150){
				MM = 0;
				sdtbolax = sdtbolay = 0;
				motion = 10;//19;
				GerakCariBola(512);
			} else if (CountTrack >= 150) {
				MM = 0;
				sdtbolax = sdtbolay = 0;
				if(lastarahX == 1) {motion = 12;}
				else if(lastarahX == 0) {motion = 11;}
				xBall = sdtblax;
				GerakCariBola1();
				if (CountTrack >= 1000) {
					CountTrack = 30;
					servoX = GoalXservo = xBall;
					lastsdtbolay = 50;
				}
			}
		}
	}
	if(CountTrack >= 5000)CountTrack=2000;
	return MM;
}

unsigned char checkobstacle(int batasY){
	unsigned char posisiobstacle = 0, countobstacle = 0, obsditengah=0, obsdikiri=0, obsdikanan=0;
	//unsigned int detektengahmin = 144, detektengahmax = 288;
	unsigned int detektengahmin = 213, detektengahmax = 426;
	unsigned int select = 1023, temp;
	enablevision |= 0b0000100;
	//ethreadsearching = 0;

	for(int i=0; i<3; i++)
	{
		temp = dataOBS[i];
		dataOBSY[i] = temp & select;
		temp = dataOBS[i];
		dataOBSXmax[i] = (temp>>10) & select;
		temp = dataOBS[i];
		dataOBSXmin[i] = (temp>>20) & select;
		if(dataOBS[i]!=0)
		{
			countobstacle++;

			if(((dataOBSXmin[i] >= detektengahmin && dataOBSXmin[i] <= detektengahmax) || (dataOBSXmax[i] >= detektengahmin && dataOBSXmax[i] <= detektengahmax) || (dataOBSXmin[i]<=detektengahmin && dataOBSXmax[i]>=detektengahmax)) && (dataOBSY[i]>=batasY))
				obsditengah++;

			if(dataOBSXmin[i]<= detektengahmin && (dataOBSY[i]>=batasY))
				obsdikiri++;

			if(dataOBSXmax[i]>= detektengahmax && (dataOBSY[i]>=batasY))
				obsdikanan++;


		}
		//if(dataOBSY[i]!=0)fprintf(stderr,"%d -> (%d %d %d) \n",i,dataOBSY[i],dataOBSXmax[i],dataOBSXmin[i]);
		//fprintf(stderr,"datamentahobs %d = (%d)\n",i,dataOBS[i]);
	}
	//fprintf(stderr,"(%d) (%d) (%d)\n\n",obsdikiri,obsditengah,obsdikanan);

	if(obsdikanan>0) posisiobstacle|= 0b001;
	if(obsditengah>0) posisiobstacle|= 0b010;
	if(obsdikiri>0) posisiobstacle|=0b100;

	return posisiobstacle;
}

int InvSudut(int hadap) {
	int sudutHadap;
	switch(hadap){
		case 1:sudutHadap=0;break;
		case 2:sudutHadap=18;break;
		case 3:sudutHadap=36;break;
		case 4:sudutHadap=54;break;
		case 5:sudutHadap=72;break;
		case 6:sudutHadap=90;break;
		case 7:sudutHadap=108;break;
		case 8:sudutHadap=126;break;
		case 9:sudutHadap=146;break;
		case 10:sudutHadap=162;break;
		case 11:sudutHadap=179;break;
		case 12:sudutHadap=-162;break;
		case 13:sudutHadap=-146;break;
		case 14:sudutHadap=-126;break;
		case 15:sudutHadap=-108;break;
		case 16:sudutHadap=-90;break;
		case 17:sudutHadap=-72;break;
		case 18:sudutHadap=-54;break;
		case 19:sudutHadap=-36;break;
		case 20:sudutHadap=-18;break;
	}
	//fprintf(stderr, "sudutHadap=%d\n",sudutHadap);
	return sudutHadap;
}

short rotasiarah(short arah){
	short hasilputar;
	if(arah > 20) hasilputar = rotasiarah(arah-20);
	else if(arah < 1) hasilputar = rotasiarah(arah+20);
	else hasilputar = arah;
	return hasilputar;
}

short rotasisudut(short sudut){
	short hasilputar;
	if(sudut > 180) hasilputar = rotasisudut(sudut-360);
	else if(sudut < -180) hasilputar = rotasisudut(sudut+360);
	else hasilputar = sudut;
	//fprintf(stderr, "hasilputar=%d\n",hasilputar);
	return hasilputar;
}

short cariarahRobot(int sdtX, int GoalCol)
{
	short hadap,sdtputar;
	sdtputar = sdtX;

	if(sdtputar >= -9 && sdtputar < 9)				{hadap = 1;}
	else if(sdtputar >= 9 &&  sdtputar < 27)		{hadap = 2;}
	else if(sdtputar >= 27 &&  sdtputar < 45)		{hadap = 3;}
	else if(sdtputar >= 45 &&  sdtputar < 63)		{hadap = 4;}
	else if(sdtputar >= 63 && sdtputar < 81)		{hadap = 5;}
	else if(sdtputar >= 81 && sdtputar < 99)		{hadap = 6;}
	else if(sdtputar >= 99  && sdtputar < 117)		{hadap = 7;}
	else if(sdtputar >= 117 && sdtputar < 135)		{hadap = 8;}
	else if(sdtputar >= 135 && sdtputar < 153)		{hadap = 9;}
	else if(sdtputar >= 153 && sdtputar < 171)		{hadap = 10;}
	else if(sdtputar >= 171 || sdtputar < -171)		{hadap = 11;}
	else if(sdtputar >= -171  && sdtputar < -153)		{hadap = 12;}
	else if(sdtputar >= -153 &&  sdtputar < -135)		{hadap = 13;}
	else if(sdtputar >= -135 &&  sdtputar < -117)		{hadap = 14;}
	else if(sdtputar >= -117 &&  sdtputar < -99)		{hadap = 15;}
	else if(sdtputar >= -99 &&  sdtputar < -81)		{hadap = 16;}
	else if(sdtputar >= -81 &&  sdtputar < -63)		{hadap = 17;}
	else if(sdtputar >= -63 &&  sdtputar < -45)		{hadap = 18;}
	else if(sdtputar >= -45 &&  sdtputar < -27)		{hadap = 19;}
	else if(sdtputar >= -27 &&  sdtputar < -9)		{hadap = 20;}

	return hadap;
}

short CariArahLihat(int sdtX, int GoalCol){
	short sdtlihatkpl,hadap;
	sdtlihatkpl = rotasisudut(sudutHadap + sdtX);

	if(sdtlihatkpl >= -9 && sdtlihatkpl < 9)		{hadap = 1;}
	else if(sdtlihatkpl >= 9 &&  sdtlihatkpl < 27)		{hadap = 2;}
	else if(sdtlihatkpl >= 27 &&  sdtlihatkpl < 45)		{hadap = 3;}
	else if(sdtlihatkpl >= 45 &&  sdtlihatkpl < 63)		{hadap = 4;}
	else if(sdtlihatkpl >= 63 && sdtlihatkpl < 81)		{hadap = 5;}
	else if(sdtlihatkpl >= 81 && sdtlihatkpl < 99)		{hadap = 6;}
	else if(sdtlihatkpl >= 99  && sdtlihatkpl < 117)	{hadap = 7;}
	else if(sdtlihatkpl >= 117 && sdtlihatkpl < 135)	{hadap = 8;}
	else if(sdtlihatkpl >= 135 && sdtlihatkpl < 153)	{hadap = 9;}
	else if(sdtlihatkpl >= 153 && sdtlihatkpl < 171)	{hadap = 10;}
	else if(sdtlihatkpl >= 171 || sdtlihatkpl < -171)	{hadap = 11;}
	else if(sdtlihatkpl >= -171  && sdtlihatkpl < -153)	{hadap = 12;}
	else if(sdtlihatkpl >= -153 &&  sdtlihatkpl < -135)	{hadap = 13;}
	else if(sdtlihatkpl >= -135 &&  sdtlihatkpl < -117)	{hadap = 14;}
	else if(sdtlihatkpl >= -117 &&  sdtlihatkpl < -99)	{hadap = 15;}
	else if(sdtlihatkpl >= -99 &&  sdtlihatkpl < -81)	{hadap = 16;}
	else if(sdtlihatkpl >= -81 &&  sdtlihatkpl < -63)	{hadap = 17;}
	else if(sdtlihatkpl >= -63 &&  sdtlihatkpl < -45)	{hadap = 18;}
	else if(sdtlihatkpl >= -45 &&  sdtlihatkpl < -27)	{hadap = 19;}
	else if(sdtlihatkpl >= -27 &&  sdtlihatkpl < -9)	{hadap = 20;}
	//fprintf(stderr, "sudutHadap=%d\n",sudutHadap);
	//fprintf(stderr, "SLK=%d |h=%d\n",sdtlihatkpl,hadap);

	if(GoalCol == 1){
		if(hadap<11) hadap+=10;
		else hadap-=10;
	}
	return hadap;
}

void initplayer(){
	if(player == 2)
	{
		kpx = 20;//17;
		kix = 13;//20;
		kdx = 100;
	}
	else
	{
		kpx = TKPX1;
		kix = TKDX1;
		kdx = TKIX1;
	}
}
