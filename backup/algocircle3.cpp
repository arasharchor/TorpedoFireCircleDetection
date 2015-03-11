#include<opencv2/opencv.hpp>
#include<opencv/highgui.h>
#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

Mat img;char c;
Mat src, src_gray;
int main()
{
  VideoCapture inputVideo("org.avi");
  int votes = 50;
  int iSliderValue1 =50;
  int iSliderValue2 = 200 ;
  int canny = 200 ;
  while(1)
  { 
    inputVideo.read(img);
    
      vector<Mat> channels(3);
  // split img:
  split(img, channels);
  //contrast streching
  normalize(channels[1],channels[1],255,0,NORM_MINMAX);
  // get the channels (dont forget they follow BGR order in OpenCV)
  Mat blue = channels[0];
  Mat green = channels[1];
  Mat red = channels[2];
  
  
    
//start hough    
    /// Convert it to gray
   // cvtColor( img, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur(green, src_gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;

    /// Apply the Hough Transform to find the circles
    
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, canny, votes, 0, 0 );

    /// Draw the circles detected
    
    for( size_t i = 0; i < circles.size(); i++ )
    {
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius = cvRound(circles[i][2]);
	// circle center
	circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
	cout << center << "\n" ;
	// circle outline
	circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
	cout << radius << "\n";
    }
//hough end
    /// Show your results
    // namedWindow( "cirle detect", CV_WINDOW_AUTOSIZE );
    imshow( "circle detect", img );
    createTrackbar("votes", "circle detect", &iSliderValue1, 100);
    votes=iSliderValue1;
    createTrackbar("canny","circle detect",&iSliderValue2,400);
    canny=iSliderValue2;
    
    c=cvWaitKey(400);
    if(c==27)
      break;

  }
}

