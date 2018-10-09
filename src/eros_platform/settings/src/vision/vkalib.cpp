//============================================================================
// Name        : caliberos.cpp
// Author      : Krisna Adji Syahputra
// Version     : 2.1
// Created on  : Jan 10, 2017
// Copyright   : Bismillahirrahmanirrahim
// Description : EROS Juara KRI Regional & Nasional 2017
//============================================================================

/*******************************HEADER***************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>

using namespace std;
using namespace cv;

#define lebar_frame 640
#define tinggi_frame 480
/*************************DEFINE-FUNGSI-VARIABEL****************************/
#define H 	0
#define S	1
#define V	2
#define E	0
#define D	1
#define C	2
#define ul	22

#define RECT	0
#define CROSS	1
#define ELLIPSE	2

char file[] = {"/home/eros/param/vdata0.eros"};
char filekalib[]={"/home/eros/param/vdata.eros"};
int lastevent=0;int yoh=-1,xoh=-1;
int wMin[3]={0,0,0},wMax[3]={255,255,255},ED[3];
int TwMin[3]={0,0,0},TwMax[3]={255,255,255},TED[3];
int JF[4]={0,0,0,0};
float cbarg=0,cbarx,pengx=0.001;
int cbar=0,cbax;
int JFN;
int height, width, step;
int posX=0, posY=0, mode=0;
char c;
double sec,lps;
bool leftclick,mousemoving=false,rectSelected,kotak,kjarak,dyncobs,succ;
bool gawang=false, clicked=false, returnnow=false, kotakdragdrop=false;
string name;
Point initialClickPoint, currentMousePoint;
Rect rectangleROI;
vector<int> H_ROI, S_ROI, V_ROI;
vector<int> H_ROI1, S_ROI1, V_ROI1;
IplImage* frame = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 3);
IplImage* dilated = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 1);
Mat erosion_src, dilation_src, erosion_dst, dilation_dst, closing_src, closing_dst;

/**************************INTERUPT KILL************************/
void handler(int s){
	cout << "SIG " << s << endl;
	returnnow=true;
}

/**************************Import Data************************/
void impData(){
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
					if(data<3){
						wMin[data] = buf;printf("wMin : %d\n", wMin[data]);
					}else if(data<6){
						wMax[data%3] = buf;printf("wMax : %d\n", wMax[data%3]);
					}else if(data<9){
						ED[data%6] = buf;printf("EDC : %d\n", ED[data%6]);
					}
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}
void impDataJ(){
	ifstream buka(filekalib);
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
					JF[data] = buf;//printf("JF[%d] : %d\n",data,JF[data]);
					data++;
					continue;
				}
				j++;
			}
		}
		buka.close();
	}
}
void rstvec(){
	H_ROI.clear();
	S_ROI.clear();
	V_ROI.clear();
	H_ROI1.clear();
	S_ROI1.clear();
	V_ROI1.clear();
}
/*********************Record HSV Value*************************/
void recordHSV(cv::Mat frame, cv::Mat hsv_frame){
	if(mousemoving==false&&rectSelected==true){
	//	if (H_ROI.size()>0) H_ROI.clear();
	//	if (S_ROI.size()>0) S_ROI.clear();
	//	if (V_ROI.size()>0 )V_ROI.clear();
		for (int i = rectangleROI.x; i<rectangleROI.x + rectangleROI.width; i++){
			for (int j = rectangleROI.y; j<rectangleROI.y + rectangleROI.height; j++){
				H_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[0]);
				S_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[1]);
				V_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[2]);
			}
		}
	}
	rectSelected=false;
	if (H_ROI.size()>0){
		wMin[H] = *std::min_element(H_ROI.begin(), H_ROI.end());
		wMax[H] = *std::max_element(H_ROI.begin(), H_ROI.end());
	}
	if (S_ROI.size()>0){
		wMin[S] = *std::min_element(S_ROI.begin(), S_ROI.end());
		wMax[S] = *std::max_element(S_ROI.begin(), S_ROI.end());
	}
	if (V_ROI.size()>0){
		wMin[V] = *std::min_element(V_ROI.begin(), V_ROI.end());
		wMax[V] = *std::max_element(V_ROI.begin(), V_ROI.end());
	}
	//H_ROI.clear();
	//S_ROI.clear();
	//V_ROI.clear();
	if (mousemoving == true){
		rectangle(frame, initialClickPoint, cv::Point(currentMousePoint.x, currentMousePoint.y), cv::Scalar(255, 0, 0), 1, 8, 0);
	}
}
void recordHSV2(cv::Mat frame, cv::Mat hsv_frame){
	for (int i = rectangleROI.x; i<rectangleROI.x + rectangleROI.width; i++){
		for (int j = rectangleROI.y; j<rectangleROI.y + rectangleROI.height; j++){
			H_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[0]);
			S_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[1]);
			V_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(j, i)[2]);
		}
	}
	if (H_ROI.size()>0){
		wMin[H] = *std::min_element(H_ROI.begin(), H_ROI.end());
		wMax[H] = *std::max_element(H_ROI.begin(), H_ROI.end());
	}
	if (S_ROI.size()>0){
		wMin[S] = *std::min_element(S_ROI.begin(), S_ROI.end());
		wMax[S] = *std::max_element(S_ROI.begin(), S_ROI.end());
	}
	if (V_ROI.size()>0){
		wMin[V] = *std::min_element(V_ROI.begin(), V_ROI.end());
		wMax[V] = *std::max_element(V_ROI.begin(), V_ROI.end());
	}
}
void recordHSV3(cv::Mat frame, cv::Mat hsv_frame){
	if(yoh>-1&&xoh>-1){
		H_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(yoh, xoh)[0]);
		S_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(yoh, xoh)[1]);
		V_ROI.push_back((int)hsv_frame.at<cv::Vec3b>(yoh, xoh)[2]);
	}
	if (H_ROI.size()>0){
		wMin[H] = *std::min_element(H_ROI.begin(), H_ROI.end());
		wMax[H] = *std::max_element(H_ROI.begin(), H_ROI.end());
	}
	if (S_ROI.size()>0){
		wMin[S] = *std::min_element(S_ROI.begin(), S_ROI.end());
		wMax[S] = *std::max_element(S_ROI.begin(), S_ROI.end());
	}
	if (V_ROI.size()>0){
		wMin[V] = *std::min_element(V_ROI.begin(), V_ROI.end());
		wMax[V] = *std::max_element(V_ROI.begin(), V_ROI.end());
	}
}
/********************************Treshold**********************************
IplImage *GetThresImage(IplImage* img){
	IplImage* imHSV = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 3);
	IplImage* thres = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 1);
	cvCvtColor(img, imHSV, CV_BGR2HSV);
	cvInRangeS(imHSV, cvScalar(wMin[H],wMin[S],wMin[V]), cvScalar(wMax[H],wMax[S],wMax[V]), thres);
	if(rectSelected==true){
		Mat image;
		image = cv::cvarrToMat(img);
		Mat imghsv;
		imghsv = cv::cvarrToMat(imHSV);
		recordHSV2(image,imghsv);
		rectSelected=false;
	}
	cvReleaseImage(&imHSV);

	return thres;
}*/
IplImage *GetThresImage(IplImage* img){
	IplImage* imHSV = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 3);
	IplImage* thres = cvCreateImage(cvSize(lebar_frame,tinggi_frame), 8, 1);
	cvCvtColor(img, imHSV, CV_BGR2HSV);
	cvInRangeS(imHSV, cvScalar(wMin[H],wMin[S],wMin[V]), cvScalar(wMax[H],wMax[S],wMax[V]), thres);
	if(kotakdragdrop){
		if(rectSelected==true){
			Mat image;
			image = cv::cvarrToMat(img);
			Mat imghsv;
			imghsv = cv::cvarrToMat(imHSV);
			recordHSV2(image,imghsv);
			rectSelected=false;
		}
	}
	else {
		if(clicked==true){
			Mat image;
			image = cv::cvarrToMat(img);
			Mat imghsv;
			imghsv = cv::cvarrToMat(imHSV);
			recordHSV3(image,imghsv);
		}
	}
	cvReleaseImage(&imHSV);

	return thres;
}
/****************************Smoothing**************************/
IplImage* GetSmothImage(IplImage* img){
	IplImage* smooth = cvCreateImage(cvSize(432,240), 8, 1);
	cvSmooth(img, smooth, CV_GAUSSIAN, 9, 9);

	return smooth;
}
/*****************************Errosion*************************/
Mat Erosion(Mat img){
	erosion_src = img;

	int erosion_type;
  	if(mode==RECT) erosion_type = MORPH_RECT;
  	else if(mode==CROSS) erosion_type = MORPH_CROSS;
  	else if(mode==ELLIPSE) erosion_type = MORPH_ELLIPSE;

  	Mat element = getStructuringElement(erosion_type, Size(2*ED[E] + 1, 2*ED[E]+1 ), Point(ED[E], ED[E]) );

  	erode(erosion_src, erosion_dst, element);
	return erosion_dst;
}
/****************************Dillation*************************/
Mat Dilation(Mat img){
	dilation_src = img;

	int dilation_type;
  	if(mode==RECT) dilation_type = MORPH_RECT;
  	else if(mode==CROSS) dilation_type = MORPH_CROSS;
  	else if(mode==ELLIPSE) dilation_type = MORPH_ELLIPSE;

  	Mat element = getStructuringElement( dilation_type,
                                       Size( 2*ED[D] + 1, 2*ED[D]+1 ),
                                       Point( ED[D], ED[D] ) );

  	dilate(dilation_src, dilation_dst, element );
	return dilation_dst;
}

Mat Closing(Mat img){
	closing_src = img;

	int closing_type;
	if(mode==RECT) closing_type = MORPH_RECT;
	else if(mode==CROSS) closing_type = MORPH_CROSS;
	else if(mode==ELLIPSE) closing_type = MORPH_ELLIPSE;

	Mat element = getStructuringElement(closing_type,
										Size(2*ED[C] + 1, 2*ED[C]+1),
										Point(ED[C], ED[C]) );
	morphologyEx( closing_src, closing_dst, 3, element );
	return closing_dst;
}

/**************************Hough Circle************************/
/*IplImage* Hough_Circle(IplImage* img){
	IplImage* frame;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* circles = cvHoughCircles(img, storage, CV_HOUGH_GRADIENT, 2, img->height/4, 100, 50, 10,400);
	for(int i=0; i<circles->total; i++){
		float* p=(float*)cvGetSeqElem(circles,i);
		cvCircle(frame, cvPoint(cvRound(p[0]),cvRound(p[1])), 2, CV_RGB(255, 255, 255), -1, 8,0);
		cvCircle(frame, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(0, 255, 0), 2, 8,0);
	}
	cvShowImage("Ball Detected",frame);
	cvReleaseMemStorage(&storage);
	return frame;
}*/
/****************************Clicking Fuction***************************/
void click(int event, int x, int y, int flags, void* param){
	Mat* videoFeed = (Mat*)param;
	if (lastevent==CV_EVENT_LBUTTONDOWN&&event==CV_EVENT_LBUTTONUP&& leftclick==false)
			{
				initialClickPoint = cv::Point(x, y);
				leftclick=true;
			}
	if (lastevent==CV_EVENT_LBUTTONDOWN&&event==CV_EVENT_LBUTTONUP&& leftclick==true)
			{
				currentMousePoint = cv::Point(x, y);
				mousemoving=true;
				kotak=true;
			}
	if (event == CV_EVENT_LBUTTONUP && mousemoving == true){
		rectangleROI = Rect(initialClickPoint, currentMousePoint);
		leftclick=false;
		mousemoving=false;
		rectSelected=true;
	}
	if (event == CV_EVENT_RBUTTONDOWN){
		wMin[H]=wMin[S]=wMin[V]=0;
		wMax[H]=wMax[S]=wMax[V]=255;
		ED[E]=ED[D]=ED[C]=0;
		rstvec();
	}
	lastevent=event;
}
void click2(int event, int x, int y, int flags, void* param){
	Mat* videoFeed = (Mat*)param;
	if (lastevent==CV_EVENT_LBUTTONDOWN&&event==CV_EVENT_LBUTTONUP&& leftclick==false)
		{
			currentMousePoint=initialClickPoint = cv::Point(x, y);
			mousemoving=true;
			leftclick=true;
		}
	else if (lastevent==CV_EVENT_LBUTTONDOWN&&event==CV_EVENT_LBUTTONUP&& leftclick==true){
			rectangleROI = Rect(initialClickPoint, currentMousePoint);
			mousemoving=false;
			leftclick=false;
			rectSelected=true;
		}
	if (event == CV_EVENT_MOUSEMOVE && leftclick == true)
			{
				currentMousePoint = cv::Point(x, y);

			}

	if (event == CV_EVENT_RBUTTONDOWN){
		wMin[H]=wMin[S]=wMin[V]=0;
		wMax[H]=wMax[S]=wMax[V]=255;
		ED[E]=ED[D]=ED[C]=0;
		rstvec();
	}
	lastevent=event;
}
void click3(int event, int x, int y, int flags, void* param){
	Mat* videoFeed = (Mat*)param;
	if(event==CV_EVENT_LBUTTONDOWN&&!clicked){
		clicked=true;
	}
	if(event==CV_EVENT_MOUSEMOVE&&clicked){
		xoh=x;yoh=y;
	}
	if(event==CV_EVENT_LBUTTONUP&&clicked){
		clicked=false;
		xoh=yoh=-1;
	}
	if (event == CV_EVENT_RBUTTONDOWN){
		clicked=false;
		wMin[H]=wMin[S]=wMin[V]=0;
		wMax[H]=wMax[S]=wMax[V]=255;
		ED[E]=ED[D]=ED[C]=0;
		rstvec();
	}
}
/****************************Track Bar*************************/
void trackBar(){/*
	cvNamedWindow("Hasil");
	cvCreateTrackbar("H", "Hasil", &wMin[H], 255, 0);
	cvCreateTrackbar("S", "Hasil", &wMin[S], 255, 0);
	cvCreateTrackbar("V", "Hasil", &wMin[V], 255, 0);
	cvCreateTrackbar("H1", "Hasil", &wMax[H], 255, 0);
	cvCreateTrackbar("S1", "Hasil", &wMax[S], 255, 0);
	cvCreateTrackbar("V1", "Hasil", &wMax[V], 255, 0);
	cvCreateTrackbar("E", "Hasil", &ED[E], 30, 0);
	cvCreateTrackbar("D", "Hasil", &ED[D], 30, 0);*/
	IplImage* pajj = cvCreateImage(cvSize(320,10), 8, 1);
	cvNamedWindow("Hasill");
	cvCreateTrackbar("H", "Hasill", &wMin[H], 255, 0);
	cvCreateTrackbar("S", "Hasill", &wMin[S], 255, 0);
	cvCreateTrackbar("V", "Hasill", &wMin[V], 255, 0);
	cvCreateTrackbar("H1", "Hasill", &wMax[H], 255, 0);
	cvCreateTrackbar("S1", "Hasill", &wMax[S], 255, 0);
	cvCreateTrackbar("V1", "Hasill", &wMax[V], 255, 0);
	cvCreateTrackbar("E", "Hasill", &ED[E], 30, 0);
	cvCreateTrackbar("D", "Hasill", &ED[D], 30, 0);
	cvCreateTrackbar("C", "Hasill", &ED[C], 30, 0);
	cvShowImage("Hasill",pajj);
}

void trackBarkalib(){
	cvNamedWindow("Kaibrasi jarak",CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Enemy goal", "Kaibrasi jarak", &JF[0], 200, 0);
	cvCreateTrackbar("Right side", "Kaibrasi jarak", &JF[1], 200, 0);
	cvCreateTrackbar("Left side", "Kaibrasi jarak", &JF[2], 200, 0);
	cvCreateTrackbar("Own goal", "Kaibrasi jarak", &JF[3], 200, 0);
}

void SavetoT(){
	TwMin[H]=wMin[H];
	TwMin[S]=wMin[S];
	TwMin[V]=wMin[V];
	TwMax[H]=wMax[H];
	TwMax[S]=wMax[S];
	TwMax[V]=wMax[V];
	TED[E]=ED[E];
	TED[D]=ED[D];
	TED[C]=ED[C];
}
void LoadfromT(){
	wMin[H]=TwMin[H];
	wMin[S]=TwMin[S];
	wMin[V]=TwMin[V];
	wMax[H]=TwMax[H];
	wMax[S]=TwMax[S];
	wMax[V]=TwMax[V];
	ED[E]=TED[E];
	ED[D]=TED[D];
	ED[C]=TED[C];
}


void addtovec(){
	H_ROI.push_back(wMin[H]);
	H_ROI.push_back(wMax[H]);
	S_ROI.push_back(wMin[S]);
	S_ROI.push_back(wMax[S]);
	V_ROI.push_back(wMin[V]);
	V_ROI.push_back(wMax[V]);
}

/**************************Export Data************************/
void expData(int MinH, int MinS, int MinV, int MaxH, int MaxS, int MaxV, int Er, int Dl, int Cl){
	ofstream output (file);
	if(output.is_open()){
		cout<<"Alhamdulillah"<<endl;
		output << MinH<<";"<<MinS<<";"<<MinV<<";"<<endl;
		output << MaxH<<";"<<MaxS<<";"<<MaxV<<";"<<endl;
		output << Er<<";"<<Dl<<";"<<Cl<<";"<<endl;
	}else cout<<"Gak isok bukak!"<<endl;
	output.close();
}
void expDataJ(int EG, int RS, int LS, int OG){
	ofstream output (filekalib);
	if(output.is_open()){
		cout<<"Alhamdulillah"<<endl;
		output << EG<<";"<<RS<<";"<<LS<<";"<<OG<<";"<<endl;
	}else cout<<"Gak isok bukak!"<<endl;
	output.close();
}
/*******************************Help***************************/
static void help(){
	cout<<  "Bismillahirrahmanirrahim"<<endl;
	cout<<	"Program Kalibrasi Warna EROS 2017"<<endl;
	cout<<	"\t Penggunaan Tombol :\n";
	cout<<	"\t 1 	 - kalibrasi bola\n"
		"\t 2    - kalibrasi bola sedang\n"
		"\t 3    - kalibrasi bola jauh\n"
		"\t 4    - kalibrasi gawang\n"
		"\t 5    - kalibrasi obstacle cyan\n"
		"\t 6    - kalibrasi obstacle magenta\n"
		"\t 7    - kalibrasi garis putih\n"
		"\t 8-9  - kalibrasi lapangan\n"
		"\t q    - keluar program\n"
		"\t enter- simpan data kalibrasi"<<endl;

}
/**************************Main Program************************/
int main(int argc,char* args[]){
	if(argc>1){
		if(strcmp(args[1], "kotak")==0){
			kotakdragdrop=true;
		}
		else if(strcmp(args[1], "titik")==0){
			kotakdragdrop=false;
		}
		else {
			fprintf(stderr,"unkown argument %s\n type 'kotak' or 'titik'. default : kotak\n",args[1]);
			return 0;
		}
	}
	else {
		kotakdragdrop=false;
	}
	//time_t start,end;
	/*VideoCapture capture = VideoCapture(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	capture.set(CAP_PROP_FPS, 30);*/
	CvCapture* capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, lebar_frame);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, tinggi_frame);
	int sepertigaframe=lebar_frame/3;
	int luasminlap=(tinggi_frame*lebar_frame)/57;
	int Pos;
	CvSeq* contours0 = NULL;
	CvRect rect = cvRect(0, 0, 0, 0);
	CvRect rect0 = cvRect(0, 0, 0, 0);
	CvRect rects0 = cvRect(0, 0, 0, 0);
	CvRect rects1 = cvRect(0, 0, 0, 0);
	CvRect rects2 = cvRect(0, 0, 0, 0);
	Rect separuh1 = cvRect(0, 0, sepertigaframe, tinggi_frame);
	Rect separuh2 = cvRect(sepertigaframe, 0, sepertigaframe, tinggi_frame);
	Rect separuh3 = cvRect((sepertigaframe)*2, 0, sepertigaframe, tinggi_frame);
	CvRect full = cvRect(0, 0, lebar_frame, tinggi_frame);
	int result0=0,result1=0,result2=0,result3=0;
	int f1=tinggi_frame,f2=tinggi_frame,f3=tinggi_frame,pemb=0,deff=0,skip=0;
	float estjarak=0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	//CvCapture* capture = cvCaptureFromCAM(0);
	//capture = cap;
	impData();
	help();
	Mat dil,err,cls,matt,fdil,reslt,baks;
	while(1){
		skip=0;
		cvSetImageROI(dilated, full);
		signal(SIGTERM, handler);
		signal(SIGINT, handler);
		if(returnnow)return 0;
		if(kjarak==false)trackBar();
		else trackBarkalib();
		//cbarg=(float)cbar;
		//cbarx=(float)cbax;
		//capture.set(CAP_PROP_GAIN,(cbarg*pengx));
		//capture.set(CV_CAP_PROP_EXPOSURE,(cbarx*pengx));
		//if(count==0)	time(&start);
		//double fps=capture.get(CAP_PROP_GAIN);
		//int expos=capture.set(CV_CAP_PROP_EXPOSURE,200);
		//capture >> capp;

		frame=cvQueryFrame(capture);
		Mat blur;
		blur = cv::cvarrToMat(frame);
		GaussianBlur( blur, blur, Size( 7, 7 ), 0, 0 );
		IplImage temp;
		temp=blur;
		cvCopy(&temp,frame);
		//IplImage temp;
		//temp=capp;
		//cvCopy(&temp,frame);
		if(!frame){
			fprintf(stderr,"Frame null!\n");
			fprintf(stderr,"Cek kabel Kamera Bro !\n");
			break;
		}
		IplImage* thresholded1;
		cvNamedWindow("Kamera",CV_WINDOW_AUTOSIZE);
		if(kotakdragdrop)cvSetMouseCallback("Kamera", click2, &frame);
		else cvSetMouseCallback("Kamera", click3, &frame);
		if(mousemoving)cvRectangle(frame, initialClickPoint, currentMousePoint, cv::Scalar(255, 0, 0), 1, 8, 0);
		//IplImage* thresholded  = GetSmothImage(thresholded1);
		//fprintf(stderr,"fpsnya : %f, exposurenya : %f \n",fps);
		if(!dyncobs){
			thresholded1= GetThresImage(frame);
			matt = cvarrToMat(thresholded1);
			if(!gawang){
				err	= Erosion(matt);
				cls = Dilation(err);
				dil = Closing(cls);
			}
			else{
				err	= Dilation(matt);
				cls = Erosion(err);
				dil = Closing(cls);
			}
		}
		else {
			file[ul] = '1';
			if(!succ)impData();
			thresholded1= GetThresImage(frame);
			matt = cvarrToMat(thresholded1);
			err	= Erosion(matt);
			cls = Dilation(err);
			baks = Closing(cls);
			imshow("bolas", baks);
			file[ul] = '8';
			if(!succ)impData();
			thresholded1= GetThresImage(frame);
			matt = cvarrToMat(thresholded1);
			err	= Erosion(matt);
			cls = Dilation(err);
			fdil = Closing(cls);
			file[ul] = '4';
			if(!succ){impData();succ=true;}
			bitwise_or(baks, fdil, reslt);
			bitwise_not(reslt, dil);
			reslt=Erosion(dil);
			cls=Dilation(reslt);
			dil=Closing(cls);
		}
		if(kjarak==true){
			IplImage ipltemp=dil;
			cvCopy(&ipltemp,dilated);
			result0=result1=0;
			f1=f2=f3=tinggi_frame;estjarak=0;pemb=0;
			for(int devide=0;devide<3;devide++){
				if(devide==0){
					cvSetImageROI(dilated, separuh1);
					//cvShowImage("separuh1",dilatedField);
				}
				else if(devide==1){
					cvSetImageROI(dilated, separuh2);
					//cvShowImage("separuh2",dilatedField);
				}
				else {
					cvSetImageROI(dilated, separuh3);
					//cvShowImage("separuh3",dilatedField);
				}

				cvFindContours(dilated, storage, &contours0, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
				result3=result2=result1=result0=0;
				while(contours0){
					rect = cvBoundingRect(contours0, 0);
					result0 = (int) fabs(cvContourArea(contours0, CV_WHOLE_SEQ));
					if(result0<luasminlap){
						contours0 = contours0->h_next;
						continue;
					}
					if(rect.y<f1&&devide==0){
						result1=result0;
						f1=rect.y;
						rects0=rect;
					}
					else if(rect.y<f2&&devide==1){
						result2=result0;
						f2=rect.y;
						rects1=rect;
					}
					else if(rect.y<f3&&devide==2){
						result3=result0;
						f3=rect.y;
						rects2=rect;
					}
					contours0 = contours0->h_next;
				}
			}
			if(f1!=tinggi_frame){
				estjarak+=rects0.y;
				pemb++;
			}
			if(f2!=tinggi_frame){
				estjarak+=rects1.y;
				pemb++;
			}
			if(f3!=tinggi_frame){
				estjarak+=rects2.y;
				pemb++;
			}
			if(pemb==3){
				estjarak/=3;
				deff=5;
			}
			else if(pemb==2){
				estjarak/=2;
				deff=5;
			}
			else if(pemb==1){
				deff=1;
			}
			else{
				//fprintf(stderr,"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
				skip=1;
			}
			if(skip!=1){
				estjarak+=JF[0];
				estjarak-=100;
				if(estjarak>88)Pos=1;
				else if(estjarak>34)Pos=2;
				else if(estjarak>15)Pos=3;
				else if(estjarak>1)Pos=4;
				else Pos=deff;
			}
			else Pos=1;
			cvLine(frame,cvPoint(0, estjarak), cvPoint(lebar_frame, estjarak),CV_RGB(0,255,0), 1, 8, 0);
			cvRectangle(frame, cvPoint(0, rects0.y), cvPoint(sepertigaframe, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
			cvRectangle(frame, cvPoint(sepertigaframe, rects1.y), cvPoint(sepertigaframe*2, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);
			cvRectangle(frame, cvPoint(sepertigaframe*2, rects2.y), cvPoint(lebar_frame, tinggi_frame), CV_RGB(255,0,0), 1, 8, 0);/**/
			fprintf(stderr,"estjarak = %f , POS = %d ,JF  = %d\n",estjarak,Pos,JF[JFN]);
		}
		if(kjarak==true){
			cvDestroyWindow("Hasil");
			imshow("Kaibrasi jarak", dil);
		}
		else {
			cvDestroyWindow("Kaibrasi jarak");
			imshow("Hasil", dil);
		}
		cvShowImage("Kamera", frame);
		c = cvWaitKey(33);
		switch (c)
		{
			case 'q': returnnow=true; break;
			case 32 : cout<<"Simpan data kalibrasi "<<name<<endl; //13
				  if(kjarak==true){
					  expDataJ(JF[0],JF[1],JF[2],JF[3]);
				  }
				  else{
					expData(wMin[H],wMin[S],wMin[V],wMax[H],wMax[S],wMax[V],ED[E],ED[D],ED[C]);
				  }
				  if(dyncobs&&succ)succ=false;
				  break;
			case '1': file[ul] = '0';
				  name = "Bola Dekat"; cout<<name<<endl;
				  dyncobs=kjarak=false;
				  gawang=false;
				  rstvec();
				  SavetoT();
				  impData();
				  addtovec();
				  trackBar();
				  mode = ELLIPSE;
				  break;
			case '2': file[ul] = '1';
				  name = "Bola Sedang";cout<<name<<endl;
				  gawang=false;
				  rstvec();
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = ELLIPSE;
				  break;
			case '3': file[ul] = '2';
				  name = "Bola Jauh";cout<<name<<endl;
				  gawang=false;
				  rstvec();
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = ELLIPSE;
				  break;
			case '4': file[ul] = '3';
				  name = "Gawang";cout<<name<<endl;
				  rstvec();
				  SavetoT();
				  impData();
				  addtovec();
				  gawang=true;
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = RECT;
				  break;
			case '5': file[ul] = '4';
				  name = "Dynamic Obs";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  dyncobs=true;
				  kjarak=false;
				  trackBar();
				  mode = ELLIPSE;
				  break;
			case '6': file[ul] = '5';
				  name = "Obstacle Magenta";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = ELLIPSE;
				  break;
			case '7': file[ul] = '6';
				  name = "Line Putih";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = RECT;
				  break;
			case '8': file[ul] = '7';
				  name = "Lapangan dekat";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = RECT;
				  break;
			case '9': file[ul] = '8';
				  name = "Lapangan Jauh";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = RECT;
				  break;
			case '0': file[ul] = '9';
				  name = "Obstacle (line & Field & Ball)";cout<<name<<endl;
				  rstvec();
				  gawang=false;
				  SavetoT();
				  impData();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  mode = RECT;
				  break;
			case 't':cout<<"Load from previous"<<endl;
				  rstvec();
				  LoadfromT();
				  addtovec();
				  dyncobs=kjarak=false;
				  trackBar();
				  break;
			case 'w':file[ul] = '8';
				  gawang=false;
			      kjarak=true;dyncobs=false;
			      JFN=0;
				  impData();
				  cout<<"Kalibrasi Jarak Enemy goal"<<endl;
				  rstvec();
				  impDataJ();
				  trackBarkalib();
				  mode = RECT;
				  break;
			case 'd':file[ul] = '8';
				  gawang=false;
				  kjarak=true;dyncobs=false;
				  JFN=1;
				  impData();
				  cout<<"Kalibrasi Jarak Right Side"<<endl;
				  rstvec();
				  impDataJ();
				  trackBarkalib();
				  mode = RECT;
				  break;
			case 'a':file[ul] = '8';
				  kjarak=true;dyncobs=false;
				  gawang=false;
				  JFN=2;
				  impData();
				  cout<<"Kalibrasi Jarak Left Side"<<endl;
				  rstvec();
				  impDataJ();
				  trackBarkalib();
				  mode = RECT;
				  break;
			case 's':file[ul] = '8';
				  kjarak=true;dyncobs=false;
				  gawang=false;
				  JFN=3;
				  impData();
				  cout<<"Kalibrasi Jarak own goal"<<endl;
				  rstvec();
				  impDataJ();
				  trackBarkalib();
				  mode = RECT;
				  break;
			case 'u':
				  if (H_ROI.size()>0){
					  H_ROI1.push_back(H_ROI.back());H_ROI.pop_back();
					  S_ROI1.push_back(S_ROI.back());S_ROI.pop_back();
					  V_ROI1.push_back(V_ROI.back());V_ROI.pop_back();
				  }
				  if (H_ROI.size()>0){
					  wMin[H] = *std::min_element(H_ROI.begin(), H_ROI.end());
					  wMax[H] = *std::max_element(H_ROI.begin(), H_ROI.end());
				  }
				  if (S_ROI.size()>0){
					  wMin[S] = *std::min_element(S_ROI.begin(), S_ROI.end());
					  wMax[S] = *std::max_element(S_ROI.begin(), S_ROI.end());
				  }
				  if (V_ROI.size()>0){
					  wMin[V] = *std::min_element(V_ROI.begin(), V_ROI.end());
					  wMax[V] = *std::max_element(V_ROI.begin(), V_ROI.end());
				  }
				  break;
			case 'y':file[ul] = '8';
				  if (H_ROI1.size()>0){
					  H_ROI.push_back(H_ROI1.back());H_ROI1.pop_back();
					  S_ROI.push_back(S_ROI1.back());S_ROI1.pop_back();
					  V_ROI.push_back(V_ROI1.back());V_ROI1.pop_back();
				  }
				  if (H_ROI.size()>0){
					  wMin[H] = *std::min_element(H_ROI.begin(), H_ROI.end());
					  wMax[H] = *std::max_element(H_ROI.begin(), H_ROI.end());
				  }
				  if (S_ROI.size()>0){
					  wMin[S] = *std::min_element(S_ROI.begin(), S_ROI.end());
					  wMax[S] = *std::max_element(S_ROI.begin(), S_ROI.end());
				  }
				  if (V_ROI.size()>0){
					  wMin[V] = *std::min_element(V_ROI.begin(), V_ROI.end());
					  wMax[V] = *std::max_element(V_ROI.begin(), V_ROI.end());
				  }
				  break;
			case 'n':kotakdragdrop=true; break;
			case 'm':kotakdragdrop=false; break;
		}
		//time(&end);
		//sec=difftime(end,start);lps=(double)count/sec;
		//cout<<sec<<"-"<<lps<<endl;;
		cvReleaseImage(&thresholded1);
		/**/
	}
	reslt.release();
	fdil.release();
	err.release();
	dil.release();
	matt.release();
	cls.release();
	cvReleaseImage(&frame);
	//capture.release();;
	return 0;
}
