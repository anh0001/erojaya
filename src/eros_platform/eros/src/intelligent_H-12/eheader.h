//============================================================================
// Name        : eheader.h / intelligent
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#ifndef EHEADER_H_
#define EHEADER_H_

#include <string>            // For string
#include <exception>         // For exception class
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <ctime>
#include <time.h>

#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <pthread.h>

#include <stdlib.h>
#include <vector>
#include "ros/ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
/* =============IPC================ */

// //vision
// #define xball 	5001//tampil
// #define sdtTGw 	5002
// #define yball 	5003//tampil
// #define tGwngA 	5004
// #define xgwang	5005//tampil
// #define jgwang	5006
// #define ygwang	5007//tampil
// #define wgwang	5008//tampil

// //head
// #define xkpla	5009//tampil
// #define xkpla1	5010//tampil
// #define ykpla	5011//tampil
// #define ykpla1	5012//tampil
// #define ykpla2	5014//tampil

// //motion
// #define motionR	5013//tampil

// //initial
// #define iGCol 	5016//tampil
// #define iTask	5017
// #define i2nd	5018

// //musuh
// #define xMusuh 	5019 //obstacle
// #define pMusuh 	5020
// #define yMusuh	5021 //obstacle

// //request
// #define rColor 	5022

// //position
// #define xRbt 	5023//tampil
// #define yRbt 	5024//tampil
// #define xBla 	5025//tampil
// #define yBla 	5026//tampil

// #define OwnGoal   	0
// #define EnemyGoal	1

// //compass
// #define dtaCMPS 5030
// #define dtaArah 5031
// #define dtaArahKpl 5032

// //Comm
// #define iTeam 	5015//tampil
// #define dtaJob 	5033
// #define dtaComm	5035
// #define dtapenalty 5036

// //Data Serial
// #define dtaCMPS 5030
// #define flgjthdnlangkahodo 5040
// #define odometryX 5041
// #define odometryZ 5042
// #define odometryH 5043

// #define enablevis 5045 //Xki Xka Y GA P GL O L B
// #define lokasivis 5046
// #define xposRbt 5047
// #define yposRbt 5048

// //Obstacle
// #define obs1 5050
// #define obs2 5051
// #define obs3 5052

// #define xposRbt2 5053 //Hasil diolah
// #define yposRbt2 5054 //Hasil diolah
// #define datapc 5055


// /* =============IPC================ */
 #define sdtblaGBa2 2700//2790//2520 //btas paling atas untuk prediksi tiang depan gawang
 #define sdtblaGBa1 2600//2520 //btas lebih atas untuk prediksi tiang
 #define sdtblaGBa 2550//2320 //btas atas untuk prediksi tiang
 #define sdtblaGBw 1720 //btas bawah untuk prediksi tiang
 #define sdtblaGmx 2650//2700 //search Gw max untuk dirnoleh 2
 //#define sdtblaGmn 2500 //search Gw min untuk dirnoleh 2

 #define COMPASS_Blue 0
 #define COMPASS_Yellow 180
// #define kakidrible 20
// #define ctendangkecil 4


#define KPX 23
#define KDX 7
#define KIX 80

#define KPY 12
#define KDY 20
#define KIY

//PID Without GUI###################################
#define TKPX1 60//70
#define TKDX1 15//17//10//13//20//2
#define TKIX1 70//80

#define TKPX2 90//80//15//20//30//15//7
#define TKDX2 10//17//10//13//20//2
#define TKIX2 150//80//1000//100

#define TKPY 15//15//20//12
#define TKDY 27//23//30//20
#define TKIY
//##################################################

//PID Use GUI#######################################
#define TKPX1_GUI 80//80//15//20//30//15//7
#define TKDX1_GUI 17//17//10//13//20//2
#define TKIX1_GUI 120//80//1000//100

#define TKPX2_GUI 80//80//15//20//30//15//7
#define TKDX2_GUI 17//17//10//13//20//2
#define TKIX2_GUI 120//80//1000//100

#define TKPY_GUI 15//15//20//12
#define TKDY_GUI 27//23//30//20
#define TKIY_GUI
//##################################################


#define motioncekgw 0

#define KANAN 1
#define KIRI 2

#define ipcimu 5056
#define round(a)(int)(a+0.5)

#define MAX_DATA_TO_SERIAL 4
#define MAX_DATA_TO_REC 5

#define MAX_DATA_FROM_SERIAL  4
#define MAX_DATA_FROM_FIELD 13
#define MAX_DATA_FROM_REFEREE 3
#define MAX_DATA_FROM_REC 2
#define MAX_DATA_FROM_VISION 2


typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

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

struct DataSerial{
	unsigned char awal;
	unsigned char awal2;
	unsigned char id;
	unsigned char dataL;
	unsigned char dataH;
	unsigned char ceksum;
};

struct DataCMPS{
	unsigned char modeCMPS;
};

/********************* Variable *************************/

// Publish & Subscribe Variable
extern int dtaFromField[MAX_DATA_FROM_FIELD];
extern int dtaFromSerial[MAX_DATA_FROM_SERIAL];
extern int dtaFromReferee[MAX_DATA_FROM_REFEREE];
extern int dtaFromRec[MAX_DATA_FROM_REC];
extern int dtaFromVision[MAX_DATA_FROM_VISION];
extern int dtaHeading,dtaXPOS,dtaYPOS,dtaFall; //Variable Serial - Intelligent

/* ============== Data Kalibrasi ============== */
extern int tim,player;
extern char fle[];
extern int Cmps[8],GeserCMPS,PutarCMPS;								// Kalibrasi Kompas
extern int sdtblax, sdtblay, sdtdefy, sdtexcute, sdtblamx, sdtblamn; 	// Kalibrasi Kepala

/* =============== Data Vision ================ */
extern int dataXB,dataYB; 											// Bola
extern int dataXG,dataYG, posTiang, jrkGwang;						// Gawang
extern int xLawan, yLawan; 											// Lawan
extern int dataXbl, dataYbl;										// Mapping
extern unsigned int dataOBS[3];										// Obstacle
extern int dataOBSXmin[3],dataOBSXmax[3],dataOBSY[3];				// Obstacle
extern unsigned char enablevision;
extern int dataLE,dataJar;
extern bool use_gui;

/* ============= Data Communication =========== */
extern int myTask,myTask2; 													// From Referee
extern int dtComm, lastDtComm; 										// From Server
extern int fdGrk,ttySC;												// SerialComm Arm
extern int dtflagsama,catchflagsama;											

/* =============== Data Compass =============== */
extern int myGcol;													// Gawang
extern int dataCMPS, sudutHadap;									// Kompas
extern int arahRobot, arahRobot20, arahLihat, last_arahRobot, init_arahRobot, last_arahLihat;			// Arah
extern bool hit_arahRobot;
extern float HeadingOffset;

/* ======== Variable Searching Bola =========== */
extern bool nBall,flagPEX;														// Ada tidak bola
extern int sKpl;														// Kecepatan Searching
extern int CountLook, CountTrack, DelayCari, CountLost, CountAda, CountDelay, CountUlang,countn,countmundur,count10;		// Counter
extern int FlagLook;													// Flag
extern int lastErrX, lastErrY, lastXball, lastYball, servoX, servoY;	// Perhitungan
extern int GoalXservo;
extern int arahX, lastarahX, arahY, lastarahY;
extern int TT, stepBCB;
extern bool ethreadsearching;
extern int xBallvision, yBallvision, GeserVision;
extern int lastXBall, lastYBall;
extern int lokasirobot;
extern int posrobotX, posrobotY;
extern int simpanposrobotX, simpanposrobotY;

/* ============= Variable Motion ============== */
extern int motion, lastMotion, countgerak;											// Motion

/* ============= Variable Kepala ============== */
extern int xBall, yBall, xBall1, yBall1, servoYG, sdtblaGmn;			// Servo
extern int sdtbolax, sdtbolay, lastsdtbolax, lastsdtbolay, lastbolay;	// Sudut Kepala
extern int jrkbolay;													// Jarak bola
extern int servoXG, GoalXservoG, lastskel;

/* ============= Variable Strategi ============ */
extern bool flagStrategi, flagKickOff;
extern int Strategiserang;
extern int dtJob, dataTm;												// State
extern int countKick, hadapGawang;										// Counter
extern int flagserong;													// Flag
extern int mode, ModeKickOFF, play;
extern int serongLost;
extern bool firstHit;

/* ============= Variable Serial ============ */
extern int flagjatuh, dataCOMPAS,langkahodo, last_langkahodo, langkahkaki;
extern float odoX,odoZ,odoH;
extern int flagjatuhdanlangkahodo;
extern int theta, last_theta, xpos, ypos, xpos_skr, ypos_skr;			// odometry
extern int heading_skr, xpos_now, ypos_now;
extern char USBRX[100];
extern int KNN_Data1[960][5],KNN_Data2[945][5],KNN_Data3[945][5],posDir;
extern int stepBuffer;

/* ======================= erostactics.cpp =========================== */

/* ============= Variable Taktik =============== */
//TaktikNggiring
extern bool flagStopDribble;
extern int batasGiringY;
extern int bufferTendang,bufferTendangMax;

//PositionGenerator
extern bool flagcounterpos2;

extern int lostball_arahx;

//8 -----
extern bool flaginposition;
extern int mainx;

//TaktikEksekusi
extern int kakiTendang;

extern int step, laststep,laststep2, nextstep, stepH, stepTB, stepE, stepFD, stepG, stepT,stepK,langkah;			// Step
extern int countGawang, hitungtendangkecil, cariGawangOut,countjalan;	// Counter
extern int cariGawang, countDribb, countDribbChange, countTrackG, countLookG;
extern int CountSafeBall, countGW, countertimeout,countLihatblg,countLihat,counterTerobos,counterNyaduk;// Counter
extern int flagPutar, flagYG, Objectlock, Kick, cekTdgSkaa, okTdg;		// Flag
extern int sdtGWX, sdtkplX, Tiang, lihatGoal, sdtKicking, lastsdtGWX;
extern int setArahRobot, arahGWX2, posRobot, posGR, arahGwng, arahdir;
extern int SafeBall, posCuri, flagCuri, flagRebut, countRebut, countRest, flagCuri2,arahTerobos, count10kebola, flagNyaduk1, flagNyaduk2;
extern int kaki, sudutTendang, sumsdtKick, sdtKick, skelTdg, kakie,lastKick, araheksekusi;
extern int Tbolax, Tbolay;
extern int simpanposisiGW, simpanposisiGW2, sudutxTiang, sudutxTiang2, suduteksekusi, simpansuduteksekusi;
extern int counterpos1,counterpos2,counterobs,countlihatgw, posisiGWX;
extern unsigned char sudahlihatGW;
extern int lastsuduteksekusi, sudahtrackball;
extern int counterobseks;
extern bool flagaktifkanobs;
extern bool flagsudahdekat,flagrotasilostball;
extern unsigned char disampinggw;
extern int sudutTiang[3];
extern int lastposrobot,stepP;
extern int tendangTrue, tendangDelay;
extern int lastmotion;
extern int countLuruskanGawang,arahNggiring;
extern int countGawang;
extern int arhRbt2obs;
extern int counterobs1,counterobs2,counterobs3;
extern int lastdataXB;
extern int flagmundur, counterdef, flaglost;
extern int posisiGWY,selY,selX,majulost,majuloss; 

/* ============= erosstrategy.cpp ============ */
extern int refree, lastrefree, refreeCntrl, sek, dComm,state2nd;
extern int countTask,countTask2,countMM,CountKF,countWait,countLock, countM;
extern int comKickOff,countGeser;
extern int PilihMode, flagMinggir;
extern int BackHome,KickOff;
extern int ktmGwng;
extern int KillProg;
extern int xWaitBall, yWaitBall;
extern int dataIMU, kalibrasiIMU;
extern unsigned char pilihstrategi, piliheksekusi;
extern bool flagPuterKO;
extern int datakompc, batastrackball;
extern int kpx,kix,kdx;
extern int dtComm2;
extern int stkickoff,stserang;
extern int speedX,speedY;

extern unsigned char it,ready,cM,ceksum,ceksum2;

extern unsigned char serialIn[10];
//extern unsigned char kdata[5];
extern int k;
extern unsigned char buff[24],countz;
extern bool flagLocalize,resetOdometry;
extern int initialPosition;
extern float getPosition,relativePosition0,relativePosition1,relativePosition2,vPos;
extern int arahTendang;
extern bool debug_mode,diving_mode;
extern char debug_print[500];
extern int penalty;

/********************* FUNGSI *************************/

/* ======================= erosutility.cpp =========================== */
void DebugAI( const char* xx);
float KNN_Basic(int x0, int x1, int y0, int y1, int direction);
void parsing();
void serialData();
extern std_msgs::Int32MultiArray dtaPublishSERIAL;
extern std_msgs::Int32MultiArray dtaPublishREC;
extern std_msgs::Int32MultiArray dtaPublishVISION;
extern std_msgs::Int32MultiArray dtaPublishDEBUG;

void SerialItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
void VisionLocalCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
void RecItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
void VisionItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
void RefereeItCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
void SerialPublish(int param1, int param2, int param3, int param4);
void DebugPublish(int param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9, int param10);
void RecPublish(int dtaSudutBola, int dtaState, int dtaPenalty, int dtaArah, int dtaArahKepala, bool nBall);
void VisionPublish(int dtaArahKepala, int dtaYKepala, int dtaYSudut, int dtaEField, int dtaEVision);

/* ====================== Import Export ============================== */
void writeIPC(int init, int dataX, int dataY, int dataM, int dataArh);
void readIPCK();
void readIPC(int init);
void readIPCodo(int init);
bool impDatacmps();
bool impDatakpl();
bool impUSBRX();
bool impDatastrategi();
bool expData();
bool impDataimu();
bool impDataSerial();
/* ===================== Serial & Motion ============================== */
int configure_port(int fd);
int bacaMode();
int motionAct(int xServo, int yServo, int motionN, int Mode);

/* =========================== Filter ================================= */
int kalman(int data);
int kalmandtComm(int data);

/* ======================= Searching Ball ============================= */
int generatesdtX(int sudutX);
int generatesdtY(int sudutY);
int carisudutbolay(int servoY);
int SudutBolaY();
int SudutBolaX();
void MotionHeadPrediction();
int GerakServo(int posAwal, int posAkhir);
void GerakCariBola(int dataR);
void GerakCariBola1();
int ftBall2(int xBallvision, int yBallvision, int Geser);
int ftBall3(int xBallvision, int yBallvision, int Geser);
int InvSudut(int hadap);
void aktifkansearching();
void *ftBall4(void *arg);
void *ftBall5(void *arg); //Rhoban mode
void *intelligent(void *arg);

unsigned char checkobstacle(int batasY);

/* =========================== IMU ================================= */
int CariSudut(int Dir, int GoalCol);
short rotasiarah(short arah);
short rotasisudut(short sudut);
short cariarahRobot(int sdtX, int GoalCol);
short CariArahLihat(int sdtX, int GoalCol);
short CariArahGawang(int sdtX, int GoalCol);

/* ========================= Kalibrasi ================================ */
void kalibrasiKPL();
void kalibrasiJLN();
void kalibrasiTendang();
void initplayer();

extern int initpos;
extern bool flagGetFieldSampleData;
void GetFieldSampleData(int xpos);

/* ======================= tactics.cpp =========================== */

//KickOff----------------------------------------------------------------
void TaktikTentukanArahKickOff();
void TaktikLuruskanKickOff(int sudutputar);
void TaktikNggiringKickOff();

//Serang-----------------------------------------------------------------
void Taktikcekjarak3();
void TaktikEksekusi3();
void TaktikTrackBall3();

void TaktikLuruskanGW_wide2(int sudutputar);
void Taktikcekjarak2();
void TaktikEksekusi2();
void TaktikTrackBall2();

void TaktikkeBola4(int arahhadap);
void TaktikLuruskanGW_wide4(int sudutputar);

void TaktikkeBola5(int arahhadap);
void TaktikkeBola6(int arahhadap);
void TaktikkeBola7(int arahhadap);
void TaktikkeBola8(int arahhadap);

void lostball_keeping();
void TaktikkeBola_keeping();
void TaktikEksekusi_keeping();
void TaktikBacktoPos_keeping();

void init_positioning();
void Initialize();
void lostball();
void lostball_bertahan();
void TaktikkeBola3(int arahhadap);
void TaktikBertahan();
void TaktikBertahan_off();
void PositionGenerator();
void TaktikNggiring3();
void TaktikEksekusi4();
void TaktikEksekusi5();
void TaktikEksekusiSamping();
void TaktikLuruskanGW_wide();
void TaktikPEksekusi();
void lostball_positioning();

//Bertahan---------------------------------------------------------------
void lostballdef();
void Bertahan();


//void TaktikToBALL();
void TaktikFindGoal3();
void TaktikToBALL_free();
void TaktikToBALL_putar_parabola();
int cekArahExc(int kaki);
void TaktikToExcute();
void TaktikTrackBall();

void TaktikToBALL_putar();
void TaktikMinggir();
int TaktikStayBALL();
int TaktikStayBALLDimas();
void tunggudeff();
int positioningdefense();
void MajuBelakang();
void TaktikKembali();
void trackdefense();
int firstdefense(int pedoman);

void Hit_Ball_SafeBall();
void Hit_Ball_Swing_Target();
void Hit_Ball_Shoot_Target();
void Hit_Ball_Shoot_Target2();
void Hit_Ball_Dribble_Shoot_Target();
void Hit_Ball_Dribble_Target();
void Strategi_Ngiring();
void DirectPass();

void PathPlanning2();

void TaktikkeBola(int arahhadap);
void TaktikkeBola2(int arahhadap);
void TaktikEksekusi();
void TaktikEksekusiwide(short modetendang);
void TaktikEksekusiDribble();
void TaktikNggiringCoba();
void TaktikNggiring();
void TaktikNggiring2(short arah);
void Taktikcekpinggir();
void Taktikcekjarak();
void TaktikcekGw();
void TaktikcekGw2();
void TaktikcekGW_wide();
void TaktikTunggubola();
void TaktikLuruskanGW(int sudutputar);
void TaktikLuruskanGW_wide_rokh();
void lari(int mode);

void lostball_keeping();
void TaktikkeBola_keeping();
void TaktikEksekusi_keeping();
void TaktikBacktoPos_keeping();


/* ======================= strategy.cpp =========================== */
void strategi_kickoff();
void strategi_serang_WIDE();
void pilihstrategieksekusi_wide();
int strategi_Deff();
void waitball();

void strategi_kickoff2();
void strategi_kickoff3();
void strategi_kickoff4R();
void strategi_kickoff4L();
void strategi_kickoff5R();
void strategi_kickoff5L();
void strategi_serang_WIDE2();
void strategi_serang_WIDE3();
void strategi_serang_WIDE4();
void strategi_serang_WIDE5();
void strategi_serang_WIDE6();
void strategi_serang_WIDE7();
void strategi_serang_WIDE8();
void strategi_serang_rokh();
void strategi_bertahan_cpp();
void strategi_bertahan_off();
void strategi_bertahan();

void strategi_keeping1();

void bermain();
void playing();
void init();
void kanankiri();
void ModeKickOff2();

void Modekickoff();
int Tasking();
int Tasking2();
void CheckArahMusuh();
void CheckGawang();
void Reset();
void PilihModeKickOFF();
void PilihModeRobot();
void KepalaDiamAtas();
void KepalaDiamBawah();
void strategidemo();
int positioningdefense();

double cosd(double x);
double sind(double x);
int abs(int x);

/*========================strategi penalty=======================*/
void strategi_penalty_WIDE();
void TaktikEksekusi4_pinalty();
void TaktikkeBola_pinalty(int arahhadap);
void TaktikLuruskanGW_pinalty();

/* ======================= motion.cpp =========================== */
int GerakJalanKompas(int arahRobots, int arahR,int FoRev, int SudutGWX);
int GerakTendang(int xFrame, int yFrame, int kaki, int sdtkaki);
int GerakTendangDribb(int xFrame, int yFrame, int kaki, int direksi);
int GerakTendangDribb2(int xFrame, int yFrame, int kaki, int direksi);
int GerakTendangKecil(int xFrame, int yFrame, int kaki);
int GerakArahBolaLurus(int sdtX, int sdtY);
int GerakNggiring(int sdtX, int sdtY);
int GerakCoverBlokBall(int xGrk, int yGrk);
int GerakCoverLookBall(int sdtX);
int GerakCoverBall(int xGrk, int yGrk);
int GerakCuriBall(int xGrk, int yGrk);
int GerakCoverBall_FW(int xGrk, int yGrk, int hadap);
int GerakCoverBallSlow(int sdtX, int sdtY);
int GerakCuriBallSlow(int sdtX, int sdtY);
int TaktiCariBola();
int GerakCoverBallKanan(int sdtX, int sdtY);
int GerakParabola(int xFrame, int yFrame);
int GerakPutarBola(int xFrame, int sdtY, int geser);
int GerakOfset(int xFrame, int yFrame, int arahRobots, int arahR);
int GerakSync(int arahRobots, int arahR);
int GerakOverShoot(int xFrame, int yFrame, int sdtGeser, int sdthdp );
int GerakOfsetSdt(int xFrame, int yFrame, int sdtGeser, int sdthdp);
int GerakCoverLookBall(int sdtX);
int GerakLurusArah(int sdtX, int sdtY, int arahrobotskr, int arahhadap);
int GerakLurusArahCMPS(int arahrobotskr, int arahhadap);
int GerakHadapBola(int sdtX, int sdtY, int arahrobotskr, int arahhadap);
int GerakHadapBolaSensitive(int sdtX, int sdtY, int arahrobotskr, int arahhadap);
int GerakTerobosBola(int sdtX, int sdtY, int arahrobotskr, int arahhadap);
int GerakTendangdekatfast(int sdtX, int sdtY);
int GerakTendangjauhfast(int sdtX, int sdtY);
int GerakEksekusiwide(int dataX, int dataY, int modeeks);
int GerakEksekusiSampingKananwide(int dataX, int dataY, int modeeks);
int GerakEksekusiSampingKiriwide(int dataX, int dataY, int modeeks);
int GerakEksekusiDribble(int dataX, int dataY);
int Gerakluruswide(int dataX, int dataY);
int GerakPutarArah(int arahrobotskr, int arahhadap);
int Gerakrefkepala(int sdtX,int sdtY, int sdtXmin,int sdtXmax,int sdtYmin,int sdtYmax);
int GerakUncoverBall(int sdtX, int sdtY);
int GerakUncoverBallDimas(int sdtX, int sdtY);
int GerakLurusBola(int sdtX, int sdtY);
int GerakHadapBolaDef(int sdtX, int sdtY, int arahrobotskr, int arahhadap, int lihat);
int GerakAvoidFast2(int dataX, int dataY, int kakix);
#endif /* EHEADER_H_ */
