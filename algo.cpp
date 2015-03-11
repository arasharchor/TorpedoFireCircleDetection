#include<opencv2/opencv.hpp>
#include<opencv/highgui.h>
#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
#include<fstream>
#include<cstdlib>
#include <string.h>
#include <time.h>
#include "serial_talk.h"
int my_finish_flag = 0;
int my_start_flag = 0;
ifstream Arduino_Input;
ofstream Arduino_Output;
int main()
{
  //arduino code
  port_initialize("/dev/ttyACM0","9600");
	cout<<"connected"<<endl;//arduino end
  VideoCapture inputVideo("org.avi");
  Mat img, src, src_gray;
  char c;
  int circlenumber=0;//count number of circles
  int radius;
  int tempi=0;//store temporary value of i variable used with circle vector
  int votes = 50;
  int iSliderValue1 =50;
  int iSliderValue2 = 200 ;
  int canny = 200 ;
  int n=1;//frame number
  int unitp=1;//units place of frame number
  int max=0;
  int tempr=0;
  int c1=0; int c2=0;
  double distance=0;
  //measure time
  clock_t time;
  //confi file read
  int i1,j1;
  FILE *fp2 = fopen("circles.txt","w");
  FILE *fp1 = fopen("confi.txt","r");
  fscanf(fp1,"%d",&i1);
  fscanf(fp1,"%d",&j1);
  fclose(fp1);
  //using confi file data
  if(i1!='\0')
  {
    votes = i1;
    iSliderValue1 =i1;
  }
  if(j1!='\0')
  {
    iSliderValue2 = j1 ;
    canny = j1 ;    
  }
  while(1)
  { 
    
    //read frame
    inputVideo.read(img);
    if(!inputVideo.isOpened())
      break;
    time=clock();
    vector<Mat> channels(3);
    // split img
    split(img, channels);
    //contrast streching
    normalize(channels[1],channels[1],255,0,NORM_MINMAX);
    // get the channels (dont forget they follow BGR order in OpenCV)
    //Mat blue = channels[0];
    Mat green = channels[1];
    //Mat red = channels[2];
//start hough    
    /// Convert it to gray
    // cvtColor( img, src_gray, CV_BGR2GRAY );
    /// Reduce the noise so we avoid false circle detection
    GaussianBlur(green, src_gray, Size(9, 9), 2, 2 );
    vector<Vec3f> circles;
    /// Apply the Hough Transform to find the circles    
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, canny, votes, 0, 0 );
    //writing in circle file
    unitp = n%10;
    if(unitp==1)
    fprintf(fp2,"%dst frame\n",n);
    else if(unitp==2)
    fprintf(fp2,"%dnd frame\n",n);
    else if(unitp==3)
    fprintf(fp2,"%drd frame\n",n);
    else fprintf(fp2,"%dth frame\n",n);
    n++;//increase frame number     
    //find out the target circle    
    for( size_t i = 0; i < circles.size(); i++ )
    {
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	radius = cvRound(circles[i][2]);
	if(i==0)
	{
	  max = cvRound(circles[i][0])-cvRound(circles[i][1]);
	}
	if(cvRound(circles[i][0])-cvRound(circles[i][1])>max)
	{
	  max = cvRound(circles[i][0])-cvRound(circles[i][1]);
	  c1=cvRound(circles[i][0]);
	  c2=cvRound(circles[i][1]);
	  tempr=radius;
	}
    }
    //writing the target circle in circle.txt
    fprintf(fp2,"target is [%d",c1);
    fprintf(fp2,",%d]\t ",c2);
    fprintf(fp2,"%d",tempr);
    fprintf(fp2,"\n");
    /// Draw the circles detected    
    for( size_t i = 0; i < circles.size(); i++ )
    {
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	radius = cvRound(circles[i][2]);
	//count the number of circles
	circlenumber++;
	if(cvRound(circles[i][0])-cvRound(circles[i][1])==max)
	{
	  // circle center
	  circle( img, center,3 , Scalar(0,0,255), -1, 8, 0 );
	  //cout << center << "\n" ;//print to terminal
	  // circle outline
	  circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
	  //cout << radius << "\n";//print to terminal
	}
	else
	{
	  // circle center
	  circle( img, center,3 , Scalar(0,255,0), -1, 8, 0 );
	  //cout << center << "\n" ;//print to terminal
	  // circle outline
	  circle( img, center, radius, Scalar(0,255,0), 3, 8, 0 );
	  //cout << radius << "\n";//print to terminal
	}
	fprintf(fp2,"[%d",cvRound(circles[i][0]));
	fprintf(fp2,",%d]\t ",  cvRound(circles[i][1]) );
	fprintf(fp2,"%d",radius);
	fprintf(fp2,"\n");
    }
//hough end
    //drawing aim
    Size sz = img.size();
    int aimHeight = sz.height/2;
    int aimWidth = sz.width/2;
    Point aim(aimWidth,aimHeight);
    circle( img, aim,3 , Scalar(0,0,255), -1, 8, 0 );
    //handling arduino
    distance =sqrt( (c2-aimHeight)^2 + (c1-aimWidth)^2 );
    if (distance<(tempr/2))
    {
      send_via_port((char *)"z","char",1);
      //cout << "fire" << endl;
    }
      else
      {
	if (c2>aimHeight)
	{
	  send_via_port((char *)"s","char",1);
	  //cout << "down" << endl;
	}
	  else if (c2<aimHeight)
	  {
	    send_via_port((char *)"w","char",1);
	    //cout << "up" << endl;
	  }
	if (c1>aimWidth)
	{
	  send_via_port((char *)"d","char",1);
	  //send_via_port((char *)" ","char",1);
	  //cout << "right\n" << endl;
	}
	  else if (c1<aimWidth)
	  {
	    send_via_port((char *)"a","char",1);
	    //send_via_port((char *)" ","char",1);
	    //cout << "left\n" << endl;
	  }
      }
    //all drawing should be before this
    //Show your results
    namedWindow( "circle detect", CV_WINDOW_AUTOSIZE );
    imshow( "circle detect", img );
    createTrackbar("votes", "circle detect", &iSliderValue1, 100);
    votes=iSliderValue1;
    createTrackbar("canny","circle detect",&iSliderValue2,400);
    canny=iSliderValue2;    
    //time measuring
    time=clock()-time;
    fprintf(fp2,"time %ld\n",time);
    
    //write the number of circle detected
    fprintf(fp2,"%d circles detected\n\n\n",circlenumber);
    circlenumber=0;
    //confi file write
    FILE *fp3 = fopen("confi.txt","w");
    fprintf(fp3,"%d\n",votes);
    fprintf(fp3,"%d\n",canny);
    fclose(fp3);//closing confi.txt
    //wait
    c=cvWaitKey(40);
    if(c==27)
      break;
  }
  fclose(fp2);// closing circles.txt
}

void port_initialize(char const *portname, char const *baud)
{
	char tty_settings[200];
	strcpy(tty_settings, "stty -F ");
	strcat(tty_settings, portname);
	strcat(tty_settings,  " cs8 ");
	strcat(tty_settings, baud);
	strcat(tty_settings,  " ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts");

	cout << "BAUD rate: " << baud << endl;

	system(tty_settings);						//Activates the tty connection with the Arduino
	Arduino_Input.open(portname);				//Opens the tty connection as an ifstream
	Arduino_Output.open(portname);				//Opens the tty connection as an ofstream, not used in this example
	double Voltage;								//The Arduino is reading the voltage from A0
	long int Time = time(NULL);
	int i;

	while(time(NULL)-Time < 5){}	//Wait five seconds for the Arduino to start up
}

int send_via_port(void *message, char const *type, int size)
{

//	for(i = 0; i < 100;)
//	{
//		Time = time(NULL);
//		while(time(NULL)-Time < 1){}	//wait one second to get
//good numbers into the Arduino stream
//		while(!Arduino_Input.eof())	//while the eof flage isn't set
//		{
//			Arduino_Input >> Voltage;	//will set the
//error flag if not ready, will get a number from the Arduino stream if ready
//			cout << Voltage << endl;	//Output it to the cout
//stream
//			i++;	//Increament i, it is not known how many numbers
//I'll get at a time
//		}
//		Arduino_Input.clear();	//eof flag won't clear itself
//	}

//	Arduino_Input.close();	//Close the ifstream

	//char msg[] = "ADIOS!!!";
	int bla = 10;
	
	if (strcmp(type, "double") == 0) {
		Arduino_Output << *(double *)message << std::flush;
	}
	else if (strcmp(type, "int") == 0) {
		Arduino_Output << *(int *)message << std::flush;
	}
	else if (strcmp(type, "char") == 0) {
//		Arduino_Output << *(char *)message << std::flush;
		Arduino_Output.write((char const *)message, size);
	}
//	else {
//		Arduino_Output << endl;
//	}

//	Arduino_Output.close();	//Close the ofstream
	return(0);

} 

void *read_from_port(void *message)
{

//	for(i = 0; i < 100;)
//	{
//		Time = time(NULL);
//		while(time(NULL)-Time < 1){}	//wait one second to get
//good numbers into the Arduino stream
//		while(!Arduino_Input.eof())	//while the eof flage isn't set
//		{
//			Arduino_Input >> Voltage;	//will set the
//error flag if not ready, will get a number from the Arduino stream if ready
//			cout << Voltage << endl;	//Output it to the cout
//stream
//			i++;	//Increament i, it is not known how many numbers
//I'll get at a time
//		}
//		Arduino_Input.clear();	//eof flag won't clear itself
//	}

//	Arduino_Input.close();	//Close the ifstream

	//char msg[] = "ADIOS!!!";
	
	int count;
	char bla[15];
	for(count = 0; count < 1; count++)	//while the eof flage isn't set
	{	
		//if (Arduino_Input.eof() == 1) {
			//return 1;
		//}
		Arduino_Input.get(bla, 7);
		//cout << "blallala: " << bla << endl;
		*(double *)message = atof(bla);
		//if (strcmp(type, "double") == 0) {
			////cout << Arduino_Input;
			//Arduino_Input >> *(double *)message;
		//}
		//else if (strcmp(type, "int") == 0) {
			//Arduino_Input >> *(int *)message;
		//}
		//else if (strcmp(type, "char") == 0) {
			//Arduino_Input >> *(char *)message;
		//}
		//else {
			//return 1;
		//}
	}
	my_finish_flag = 1;

//	Arduino_Output.close();	//Close the ofstream
	Arduino_Input.clear();
	pthread_exit(NULL);
	//return(0);

} 

void port_destruct()
{
	Arduino_Input.close();	//Close the ifstream
	Arduino_Output.close();	//Close the ofstream
}