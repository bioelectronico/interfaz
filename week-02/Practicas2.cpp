#include <iostream> 
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;


int main (int argc, const char * argv[]) {
	
	VideoCapture cap("video4.avi");     
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);     
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);         
	if (!cap.isOpened())         
		return -1;       
	
	Mat img;     
	HOGDescriptor hog;     
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());       
	namedWindow("video capture", CV_WINDOW_AUTOSIZE);     
	
	while (true){
		cap >> img;
		if (!img.data)
			continue;
		
//Lines Code
		Mat dst, cdst,gray;
		
		cvtColor(img, gray, CV_BGR2GRAY);
		Canny(gray, dst, 1, 1, 3);
		


#if 1
		vector<Vec2f> lines;
		HoughLines(dst, lines, 1, CV_PI / 180, 200, 0, 0);

		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(img, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		}
#else
		vector<Vec4i> lines;
		HoughLinesP(dst, lines, 100, CV_PI / 180, 50, 50, 10);
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
		}
#endif

		//people detector
		vector<Rect> found, found_filtered;
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		size_t i, j;
		for (i = 0; i<found.size(); i++){
			Rect r = found[i];
			for (j = 0; j<found.size(); j++)
				if (j != i && (r & found[j]) == r)
					break;
			if (j == found.size())
				found_filtered.push_back(r); 
		}
		for (i = 0; i<found_filtered.size(); i++){
			Rect r = found_filtered[i];
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.06);
			r.height = cvRound(r.height*0.9);
			rectangle(img, r.tl(), r.br(),
			cv::Scalar(0, 255, 0), 2); 
		} 
		
		imshow("video capture", img);
		if (waitKey(20) >= 0)
			break;     
	}     
	return 0; 
}
