#include <ball_perceptor/detectball.h>

using namespace std;
using namespace cv;

DetectBall::DetectBall() {
	MIN_COLOR_INTENSITY = 2;
	MIN_BALL_REGION_SIZE = 10;
	BALL_REGION_SIZE_INCR = 20;
	ROBOT_TRUNK_TO_FLOOR_DISTANCE = 0.61f; // TODO: Still correct?
	BALL_CENTER_TO_FLOOR_DISTANCE = 0.095f; // TODO: Still correct?

	jauh_cascade_name = "/home/eros/param/jauh_cascade.xml";
	sedang_cascade_name="/home/eros/param/sedang_cascade.xml";
    dekat_cascade_name = "/home/eros/param/dekat_cascade.xml";


	//Load the cascade
    cout << "Loading the cascade " << jauh_cascade_name << "...";
    cout.flush();
    if (!jauh_cascade.load(jauh_cascade_name)) {
    	cout << endl;
    	cout << "--(!)Error loading CASCADE JAUH: " << jauh_cascade_name << endl;
    	//return EXIT_FAILURE;
    }
    if(!dekat_cascade.load(dekat_cascade_name)){
        cout << endl;
        cout << "--(!)Error loading CASCADE DEKAT: " << dekat_cascade_name << endl;
    }
    if(!sedang_cascade.load(sedang_cascade_name)){
	cout <<  endl;
	cout << "--(!)Error loading CASCADE SEDANG: "<<sedang_cascade_name << endl;
	}
}

void DetectBall::detectBall(Mat image, std::vector<Rect>& detected_balls, int sudutKepala) {
	//Mat resized_image(Size(320, 240), CV_8UC3);  // TODO: Add width and height in config server
	//resize(image, resized_image, resized_image.size());
	//image = resized_image.clone();
    //cout << "DetectBall sK="<< sudutKepala << endl;
    Mat im_gray;
    equalizeHist(im_gray, im_gray);
    cvtColor( image, im_gray, CV_BGR2GRAY );
    if(sudutKepala<=35){
        dekat_cascade.detectMultiScale(im_gray, detected_balls, 1.1, 5, 8, Size(100, 100));    
    }else if(sudutKepala<=50){
        sedang_cascade.detectMultiScale(im_gray, detected_balls, 1.1, 5, 8, Size(24, 24));  
    }else{
        jauh_cascade.detectMultiScale(im_gray, detected_balls, 1.1, 5, 8, Size(10, 10));  
    }
    
}
