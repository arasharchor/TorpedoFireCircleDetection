#include<opencv2/opencv.hpp>
#include<opencv/highgui.h>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

Mat img;char c;
Mat src, src_gray;
int main()
{
  VideoCapture inputVideo(0);
  int votes = 10;
  int iSliderValue1 = 50;
  while(1)
  { 
    inputVideo.read(img);
//start hough    
    /// Convert it to gray
    cvtColor( img, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;

    /// Apply the Hough Transform to find the circles
    
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, votes, 0, 0 );

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius = cvRound(circles[i][2]);
	// circle center
	circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
	// circle outline
	circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
//hough end
    /// Show your results
    // namedWindow( "cirle detect", CV_WINDOW_AUTOSIZE );
    imshow( "circle detect", img );
    createTrackbar("votes", "circle detect", &iSliderValue1, 100);
    votes=iSliderValue1;
    
    c=cvWaitKey(20);
    if(c==27)
      break;

  }
}

