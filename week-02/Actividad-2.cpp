#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  if( argc != 2)
    {
      cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
      return -1;
    }
  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  // Read the file
  if(! image.data )
    // Check for invalid input
    {
      cout << "Could not open or find the image" << std::endl ;
      return -1;
    }
  namedWindow( "Display window1", WINDOW_AUTOSIZE );// Create a window for display.
  namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
  namedWindow( "Display window3", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Display window1", image );
  imshow( "Display window2", image );
  imshow( "Display window3", image );

// Show our image inside it.
  waitKey(0);
  
  return 0;
}
