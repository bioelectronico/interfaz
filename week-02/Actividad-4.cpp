#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat image;
double alpha = 1.0;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        Mat gray_image;
        cvtColor( image, gray_image, CV_BGR2GRAY );
        namedWindow( "Gray Image", CV_WINDOW_AUTOSIZE );
        imshow( "Gray Image", gray_image );
    }
    else if  ( event == EVENT_RBUTTONDOWN )
      {
	Mat neg_image = Mat::zeros( image.size(), image.type() );
	Mat sub_mat = Mat::ones(image.size(), image.type())*255;
	subtract(sub_mat, image, neg_image);
        namedWindow( "Neg image", CV_WINDOW_AUTOSIZE );
        imshow( "Neg image", neg_image );   
      }
    else if  ( event == EVENT_MOUSEMOVE )
      {
        double beta;
	Mat src1, src2, dst;
	alpha-= 0.01;

        if (alpha <=0) alpha = 1;

	/// Read image ( same size, same type )                                                                                                                                                                    
	src1 = imread("LinuxLogo.jpg");
	src2 = imread("WindowsLogo.jpg");

	if( !src1.data ) { printf("Error loading src1 \n"); return; }
	if( !src2.data ) { printf("Error loading src2 \n"); return; }

	/// Create Windows                                                                                                                                                                                                                 
	namedWindow("Linear Blend", 1);
	beta = ( 1.0 - alpha );
	addWeighted( src1, alpha, src2, beta, 0.0, dst);
	imshow( "Linear Blend", dst );
      }
}


int main( int argc, char** argv )
{
  char* imageName = argv[1];
  image = imread( imageName, 1 );
  if( argc != 2 || !image.data )
    {
      printf( " No image data \n " );
      return -1;
    }
  namedWindow( "Original", CV_WINDOW_AUTOSIZE );
  imshow( "Original", image );

  //set the callback function for any mouse event
  setMouseCallback("Original", CallBackFunc, NULL);

  waitKey(0);
  return 0;
}
