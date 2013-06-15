#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
VideoCapture cap(1); // open the default camera
cap.set(CV_CAP_PROP_FRAME_WIDTH, 480);
cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
//cap.set(CV_CAP_PROP_FPS, 15);
if(!cap.isOpened())  // check if we succeeded
    return -1;

Mat edges;
namedWindow("Center of pupil",1);
Mat canny, src_f, eucli, repla, fil, dila, thres, gthres, src;
for(;;)
{
	cap >> src; // get a new frame from camera
	src.copyTo(src_f);
	cvtColor(src_f, src_f, CV_BGR2GRAY);
	//------THRESHOLD START---------------------------------------------------//
	threshold( src_f, gthres, 20, 255, 0 );
	// 23, 45, 90 -- ani pics
	cvtColor(gthres, thres, CV_GRAY2BGR);
	//------THRESHOLD END; CANNY START----------------------------------------//
	Mat dst, cdst;
	Canny(thres, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);
	//------CANNY END; ERODE START-------------------------------------------*/
	cv::dilate(cdst, dila, cv::Mat(), cv::Point(-1,-1));
	for(int ite = 0; ite < 3; ite++)
		cv::dilate(dila, dila, cv::Mat(), cv::Point(-1,-1));
	//------ERODE END; FILL START--------------------------------------------//
	dila.copyTo(fil);
	CvPoint seed_point = cvPoint(1,1);
	CvScalar newVal = CV_RGB(33,0,46);
	Rect* rect=0;
	Scalar loDiff=Scalar(), upDiff=Scalar();
	int flags=4;
	floodFill(fil, seed_point, newVal, rect, loDiff, upDiff, flags);
	//------FILL END; REPLACE START-------------------------------------------//
	fil.copyTo(repla);
	Mat3b repla2 = repla;
	for (Mat3b::iterator it = repla2.begin(); it != repla2.end(); it++) {
		if (*it == Vec3b(0, 0, 0)) {
			*it = Vec3b(255, 255, 255);
		}
	}
	for (Mat3b::iterator it = repla2.begin(); it != repla2.end(); it++) {
		if (*it == Vec3b(46, 0, 33)) {
			*it = Vec3b(0, 0, 0);
		}
	}
	repla = repla2;
	cvtColor(repla, repla, CV_BGR2GRAY);
	//-----REPLACE END; EUCLIDEAN DIST TRANS START----------------------------//
	distanceTransform(repla, eucli, CV_DIST_L2, 3);
	normalize(eucli, eucli, 0.0, 1.0, NORM_MINMAX);
	//-----EUCLIDEAN DIST TRANS END; CENTER LOC START-------------------------//
	double minVal, maxVal;
	Point minPt, maxPt;
	minMaxLoc(eucli, &minVal, &maxVal, &minPt, &maxPt);
	int thickness = -1;
	int lineType = 8;
	int radius = 2.0;
	cvtColor(src_f, src_f, CV_GRAY2BGR);
	circle( src_f, maxPt, radius, Scalar( 0, 0, 255 ), thickness, lineType );
	cout << "pupil coordinate : " << maxPt << endl;
	//-----CENTER LOC END-----------------------------------------------------//

	imshow("Center of pupil", src_f);
	if(waitKey(30) >= 0) break;
}// the camera will be deinitialized automatically in VideoCapture destructor
return 0;
}
