#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
	VideoCapture cap(2);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 480);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 360);
	Mat frame, eucli, thres;

	for(;;)
	{
		cap >> frame;
		threshold( frame, thres, 230, 255, 0 );
		cvtColor(thres, thres, CV_BGR2GRAY);
		distanceTransform(thres, eucli, CV_DIST_L2, 3);
		normalize(eucli, eucli, 0.0, 1.0, NORM_MINMAX);


		double minVal, maxVal;
		Point minPt, maxPt, maxPt2;
		int thickness = -1, lineType = 8, radius = 40;


		minMaxLoc(eucli, &minVal, &maxVal, &minPt, &maxPt);
		circle( eucli, maxPt, radius, Scalar( 0, 0, 0 ), thickness, lineType );
		minMaxLoc(eucli, &minVal, &maxVal, &minPt, &maxPt2);
		circle( eucli, maxPt2, radius, Scalar( 0, 0, 0 ), thickness, lineType );

		radius = 2.0;

		circle( frame, maxPt2, radius, Scalar( 0, 0, 255 ), thickness, lineType );
		circle( frame, maxPt, radius, Scalar( 0, 0, 255 ), thickness, lineType );
		line( frame, maxPt2, maxPt, Scalar( 255, 0, 0 ), 2, lineType );

		imshow("Nodu - reference point", frame);

	if(waitKey(10) >= 0) break;
	}
	return 0;
}
