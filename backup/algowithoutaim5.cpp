#include<opencv2/opencv.hpp>
#include<opencv/highgui.h>
#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
Mat img, src, src_gray;char c;
int main()
{
  VideoCapture inputVideo("org.avi");
  int votes = 50;
  int iSliderValue1 =50;
  int iSliderValue2 = 200 ;
  int canny = 200 ;
   int n=1;//frame number
  int unitp=1;//units place of frame number
  //confi file read
  int i,j;
  FILE *fp2 = fopen("circles.txt","w");
  FILE *fp1 = fopen("confi.txt","r");
  fscanf(fp1,"%d",&i);
  fscanf(fp1,"%d",&j);
  fclose(fp1);
  if(i!='\0')
  {
    votes = i;
    iSliderValue1 =i;
  }
  if(j!='\0')
  {
    iSliderValue2 = j ;
    canny = j ;    
  }
  
  while(1)
  { 
    //writing in circle file
    unitp = n%10;
    if(unitp==1)
    fprintf(fp2,"\n\n %dst frame\n",n);
    else if(unitp==2)
    fprintf(fp2,"\n\n %dnd frame\n",n);
    else if(unitp==3)
    fprintf(fp2,"\n\n %drd frame\n",n);
    else fprintf(fp2,"\n\n %dth frame\n",n);
    n++;//increase frame number
 
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
	//cout << center << "\n" ;//print to terminal
	// circle outline
	circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
	//cout << radius << "\n";//print to terminal
	fprintf(fp2,"[%d",cvRound(circles[i][0]));
	fprintf(fp2,",%d]\t ",  cvRound(circles[i][1]) );
	fprintf(fp2,"%d",radius);
	fprintf(fp2,"\n");
    }
//hough end
    /// Show your results
    // namedWindow( "cirle detect", CV_WINDOW_AUTOSIZE );
    imshow( "circle detect", img );
    createTrackbar("votes", "circle detect", &iSliderValue1, 100);
    votes=iSliderValue1;
    createTrackbar("canny","circle detect",&iSliderValue2,400);
    canny=iSliderValue2; 
    //confi file write
    FILE *fp3 = fopen("confi.txt","w");
    fprintf(fp3,"%d\n",votes);
    fprintf(fp3,"%d\n",canny);
    fclose(fp3);    
    
    c=cvWaitKey(400);
    if(c==27)
      break;
  }
  fclose(fp2);// closing circles
}

